

#ifndef _SIM_ENCODER_H
#define _SIM_ENCODER_H

#ifdef _WIN32
  // Ensure that Winsock2.h is included before Windows.h, which can get
  // pulled in by anybody (e.g., Boost).
  #include <Winsock2.h>
#endif


#include "msgs/msgs.h"
#include <gazebo/transport/transport.hh>
#include <gazebo/common/Time.hh>

using namespace gazebo;

class SimEncoder {
public:
	SimEncoder(std::string topic);

	void Reset();
	void Start();
	void Stop();
	double GetPosition();
	double GetVelocity();

private:
	void sendCommand(std::string cmd);

	double position, velocity;
	transport::SubscriberPtr posSub, velSub;
	transport::PublisherPtr commandPub;
	void positionCallback(const msgs::ConstFloat64Ptr &msg);
	void velocityCallback(const msgs::ConstFloat64Ptr &msg);
};

#endif
