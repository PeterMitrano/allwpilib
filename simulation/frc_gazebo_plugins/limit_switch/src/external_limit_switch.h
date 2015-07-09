#pragma once

#include "switch.h"

#include <gazebo/sensors/sensors.hh>
#include <boost/pointer_cast.hpp>
#include <gazebo/gazebo.hh>

using namespace gazebo;

class ExternalLimitSwitch : public Switch {
public: 
  ExternalLimitSwitch(sdf::ElementPtr sdf);
  ~ExternalLimitSwitch();
  
  /// \brief Returns true when the switch is triggered.
  virtual bool Get();

private:
  sensors::ContactSensorPtr sensor;
};
