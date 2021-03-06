/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008-2016. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include "Base.h"
#include "Controller.h"
#include "HAL/cpp/priority_mutex.h"
#include "LiveWindow/LiveWindow.h"
#include "Notifier.h"
#include "PIDInterface.h"
#include "PIDSource.h"
#include "Timer.h"

#include <memory>

#include <atomic>
#include <queue>

class PIDOutput;

/**
 * Class implements a PID Control Loop.
 *
 * Creates a separate thread which reads the given PIDSource and takes
 * care of the integral calculations, as well as writing the given PIDOutput.
 *
 * This feedback controller runs in discrete time, so time deltas are not used
 * in the integral and derivative calculations. Therefore, the sample rate
 * affects the controller's behavior for a given set of PID constants.
 */
class PIDController : public LiveWindowSendable,
                      public PIDInterface,
                      public ITableListener {
 public:
  PIDController(float p, float i, float d, PIDSource* source, PIDOutput* output,
                float period = 0.05);
  PIDController(float p, float i, float d, float f, PIDSource* source,
                PIDOutput* output, float period = 0.05);
  virtual ~PIDController();

  PIDController(const PIDController&) = delete;
  PIDController& operator=(const PIDController) = delete;

  virtual float Get() const;
  virtual void SetContinuous(bool continuous = true);
  virtual void SetInputRange(float minimumInput, float maximumInput);
  virtual void SetOutputRange(float minimumOutput, float maximumOutput);
  void SetPID(double p, double i, double d) override;
  virtual void SetPID(double p, double i, double d, double f);
  double GetP() const override;
  double GetI() const override;
  double GetD() const override;
  virtual double GetF() const;

  void SetSetpoint(float setpoint) override;
  double GetSetpoint() const override;
  double GetDeltaSetpoint() const;

  virtual float GetError() const;
  virtual float GetAvgError() const;

  virtual void SetPIDSourceType(PIDSourceType pidSource);
  virtual PIDSourceType GetPIDSourceType() const;

  virtual void SetTolerance(float percent);
  virtual void SetAbsoluteTolerance(float absValue);
  virtual void SetPercentTolerance(float percentValue);
  virtual void SetToleranceBuffer(unsigned buf = 1);
  virtual bool OnTarget() const;

  void Enable() override;
  void Disable() override;
  bool IsEnabled() const override;

  void Reset() override;

  void InitTable(std::shared_ptr<ITable> subtable) override;

 protected:
  PIDSource* m_pidInput;
  PIDOutput* m_pidOutput;

  std::shared_ptr<ITable> m_table;
  virtual void Calculate();
  virtual double CalculateFeedForward();
  double GetContinuousError(double error) const;

 private:
  // factor for "proportional" control
  float m_P;
  // factor for "integral" control
  float m_I;
  // factor for "derivative" control
  float m_D;
  // factor for "feed forward" control
  float m_F;
  // |maximum output|
  float m_maximumOutput = 1.0;
  // |minimum output|
  float m_minimumOutput = -1.0;
  // maximum input - limit setpoint to this
  float m_maximumInput = 0;
  // minimum input - limit setpoint to this
  float m_minimumInput = 0;
  // do the endpoints wrap around? eg. Absolute encoder
  bool m_continuous = false;
  // is the pid controller enabled
  bool m_enabled = false;
  // the prior error (used to compute velocity)
  float m_prevError = 0;
  // the sum of the errors for use in the integral calc
  double m_totalError = 0;
  enum {
    kAbsoluteTolerance,
    kPercentTolerance,
    kNoTolerance
  } m_toleranceType = kNoTolerance;

  // the percetage or absolute error that is considered on target.
  float m_tolerance = 0.05;
  float m_setpoint = 0;
  float m_prevSetpoint = 0;
  float m_error = 0;
  float m_result = 0;
  float m_period;

  // Length of buffer for averaging for tolerances.
  std::atomic<unsigned> m_bufLength{1};
  std::queue<double> m_buf;
  double m_bufTotal = 0;

  mutable priority_recursive_mutex m_mutex;

  std::unique_ptr<Notifier> m_controlLoop;
  Timer m_setpointTimer;

  std::shared_ptr<ITable> GetTable() const override;
  std::string GetSmartDashboardType() const override;
  void ValueChanged(ITable* source, llvm::StringRef key,
                    std::shared_ptr<nt::Value> value, bool isNew) override;
  void UpdateTable() override;
  void StartLiveWindowMode() override;
  void StopLiveWindowMode() override;
};
