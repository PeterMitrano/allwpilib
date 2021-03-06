/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008-2016. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package edu.wpi.first.wpilibj;

import edu.wpi.first.wpilibj.hal.FRCNetComm.tResourceType;
import edu.wpi.first.wpilibj.hal.HAL;
import edu.wpi.first.wpilibj.hal.SolenoidJNI;
import edu.wpi.first.wpilibj.livewindow.LiveWindow;
import edu.wpi.first.wpilibj.livewindow.LiveWindowSendable;
import edu.wpi.first.wpilibj.tables.ITable;
import edu.wpi.first.wpilibj.tables.ITableListener;
import edu.wpi.first.wpilibj.util.AllocationException;
import edu.wpi.first.wpilibj.util.CheckedAllocationException;

/**
 * DoubleSolenoid class for running 2 channels of high voltage Digital Output.
 *
 * <p>The DoubleSolenoid class is typically used for pneumatics solenoids that have two positions
 * controlled by two separate channels.
 */
public class DoubleSolenoid extends SolenoidBase implements LiveWindowSendable {

  /**
   * Possible values for a DoubleSolenoid.
   */
  public enum Value {
    kOff,
    kForward,
    kReverse
  }

  private int m_forwardChannel; // /< The forward channel on the module to control.
  private int m_reverseChannel; // /< The reverse channel on the module to control.
  private byte m_forwardMask; // /< The mask for the forward channel.
  private byte m_reverseMask; // /< The mask for the reverse channel.
  private int m_forwardHandle = 0;
  private int m_reverseHandle = 0;

  /**
   * Constructor. Uses the default PCM ID of 0.
   *
   * @param forwardChannel The forward channel number on the PCM (0..7).
   * @param reverseChannel The reverse channel number on the PCM (0..7).
   */
  public DoubleSolenoid(final int forwardChannel, final int reverseChannel) {
    this(getDefaultSolenoidModule(), forwardChannel, reverseChannel);
  }

  /**
   * Constructor.
   *
   * @param moduleNumber   The module number of the solenoid module to use.
   * @param forwardChannel The forward channel on the module to control (0..7).
   * @param reverseChannel The reverse channel on the module to control (0..7).
   */
  public DoubleSolenoid(final int moduleNumber, final int forwardChannel,
                        final int reverseChannel) {
    super(moduleNumber);
    m_forwardChannel = forwardChannel;
    m_reverseChannel = reverseChannel;

    checkSolenoidModule(m_moduleNumber);
    checkSolenoidChannel(m_forwardChannel);
    checkSolenoidChannel(m_reverseChannel);

    int portHandle = SolenoidJNI.getPortWithModule((byte) m_moduleNumber, (byte) m_forwardChannel);
    m_forwardHandle = SolenoidJNI.initializeSolenoidPort(portHandle);

    try {
      portHandle = SolenoidJNI.getPortWithModule((byte) m_moduleNumber, (byte) m_reverseChannel);
      m_reverseHandle = SolenoidJNI.initializeSolenoidPort(portHandle);
    } catch (RuntimeException ex) {
      // free the forward handle on exception, then rethrow
      SolenoidJNI.freeSolenoidPort(m_forwardHandle);
      m_forwardHandle = 0;
      m_reverseHandle = 0;
      throw ex;
    }

    m_forwardMask = (byte) (1 << m_forwardChannel);
    m_reverseMask = (byte) (1 << m_reverseChannel);

    HAL.report(tResourceType.kResourceType_Solenoid, m_forwardChannel,
                                   m_moduleNumber);
    HAL.report(tResourceType.kResourceType_Solenoid, m_reverseChannel,
                                   m_moduleNumber);
    LiveWindow.addActuator("DoubleSolenoid", m_moduleNumber, m_forwardChannel, this);
  }

  /**
   * Destructor.
   */
  public synchronized void free() {
    SolenoidJNI.freeSolenoidPort(m_forwardHandle);
    SolenoidJNI.freeSolenoidPort(m_reverseChannel);
    super.free();
  }

  /**
   * Set the value of a solenoid.
   *
   * @param value The value to set (Off, Forward, Reverse)
   */
  public void set(final Value value) {
    boolean forward = false;
    boolean reverse = false;

    switch (value) {
      case kOff:
        forward = false;
        reverse = false;
        break;
      case kForward:
        forward = true;
        reverse = false;
        break;
      case kReverse:
        forward = false;
        reverse = true;
        break;
      default:
        throw new AssertionError("Illegal value: " + value);

    }

    SolenoidJNI.setSolenoid(m_forwardHandle, forward);
    SolenoidJNI.setSolenoid(m_reverseHandle, reverse);
  }

  /**
   * Read the current value of the solenoid.
   *
   * @return The current value of the solenoid.
   */
  public Value get() {
    boolean valueForward = SolenoidJNI.getSolenoid(m_forwardHandle);
    boolean valueReverse = SolenoidJNI.getSolenoid(m_reverseHandle);

    if (valueForward) {
      return Value.kForward;
    }
    if (valueReverse) {
      return Value.kReverse;
    }
    return Value.kOff;
  }

  /**
   * Check if the forward solenoid is blacklisted. If a solenoid is shorted, it is added to the
   * blacklist and disabled until power cycle, or until faults are cleared.
   *
   * @return If solenoid is disabled due to short.
   * @see #clearAllPCMStickyFaults()
   */
  public boolean isFwdSolenoidBlackListed() {
    int blackList = getPCMSolenoidBlackList();
    return ((blackList & m_forwardMask) != 0);
  }

  /**
   * Check if the reverse solenoid is blacklisted. If a solenoid is shorted, it is added to the
   * blacklist and disabled until power cycle, or until faults are cleared.
   *
   * @return If solenoid is disabled due to short.
   * @see #clearAllPCMStickyFaults()
   */
  public boolean isRevSolenoidBlackListed() {
    int blackList = getPCMSolenoidBlackList();
    return ((blackList & m_reverseMask) != 0);
  }

  /*
   * Live Window code, only does anything if live window is activated.
   */
  public String getSmartDashboardType() {
    return "Double Solenoid";
  }

  private ITable m_table;
  private ITableListener m_tableListener;

  @Override
  public void initTable(ITable subtable) {
    m_table = subtable;
    updateTable();
  }

  @Override
  public ITable getTable() {
    return m_table;
  }

  @Override
  public void updateTable() {
    if (m_table != null) {
      // TODO: this is bad
      m_table.putString("Value", (get() == Value.kForward ? "Forward"
          : (get() == Value.kReverse ? "Reverse" : "Off")));
    }
  }

  @Override
  public void startLiveWindowMode() {
    set(Value.kOff); // Stop for safety
    m_tableListener = new ITableListener() {
      public void valueChanged(ITable itable, String key, Object value, boolean bln) {
        // TODO: this is bad also
        if (value.toString().equals("Reverse")) {
          set(Value.kReverse);
        } else if (value.toString().equals("Forward")) {
          set(Value.kForward);
        } else {
          set(Value.kOff);
        }
      }
    };
    m_table.addTableListener("Value", m_tableListener, true);
  }

  @Override
  public void stopLiveWindowMode() {
    set(Value.kOff); // Stop for safety
    // TODO: Broken, should only remove the listener from "Value" only.
    m_table.removeTableListener(m_tableListener);
  }
}
