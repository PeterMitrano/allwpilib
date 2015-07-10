

#ifndef _SIM_FLOAT_INPUT_H
#define _SIM_FLOAT_INPUT_H

#include "msgs/msgs.h"

#ifdef _WIN32
  // Ensure that Winsock2.h is included before Windows.h, which can get
  // pulled in by anybody (e.g., Boost).
  #include <Winsock2.h>
#endif

#include <gazebo/transport/transport.hh>

using namespace gazebo;

class SimFloatInput {
public:
	SimFloatInput(std::string topic);

	/**
	 * @return The value of the potentiometer.
	 */
	double Get();
  
private:
	double value;
    transport::SubscriberPtr sub;
    void callback(const msgs::ConstFloat64Ptr &msg);
};

#endif
