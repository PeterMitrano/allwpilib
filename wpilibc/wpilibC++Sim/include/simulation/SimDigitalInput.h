

#ifndef _SIM_DIGITAL_INPUT_H
#define _SIM_DIGITAL_INPUT_H

#ifdef _WIN32
  // Ensure that Winsock2.h is included before Windows.h, which can get
  // pulled in by anybody (e.g., Boost).
  #include <Winsock2.h>
#endif

#include "msgs/msgs.h"
#include <gazebo/transport/transport.hh>

using namespace gazebo;

class SimDigitalInput {
public:
	SimDigitalInput(std::string topic);

	/**
	 * @return The value of the potentiometer.
	 */
	bool Get();
  
private:
	bool value;
    transport::SubscriberPtr sub;
    void callback(const msgs::ConstBoolPtr &msg);
};

#endif
