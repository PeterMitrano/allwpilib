#ifndef Elevator_H
#define Elevator_H

#include "Commands/PIDSubsystem.h"
#include "WPILib.h"

/**
 * The elevator subsystem uses PID to go to a given height. Unfortunately, in it's current
 * state PID values for simulation are different than in the real world do to minor differences.
 */
class Elevator : public PIDSubsystem {
private:
    SpeedController* motor;
    Potentiometer* pot;

    static const double kP_real, kI_real ,
            			kP_simulation, kI_simulation;

public:
    Elevator();
    void InitDefaultCommand() {}

	/**
	 * The log method puts interesting information to the SmartDashboard.
	 */
    void Log();

    /**
     * Use the potentiometer as the PID sensor. This method is automatically
     * called by the subsystem.
     */
    double ReturnPIDInput() const;


    /**
     * Use the motor as the PID output. This method is automatically called by
     * the subsystem.
     */
    void UsePIDOutput(double d);
};

#endif
