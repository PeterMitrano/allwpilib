

#ifndef _SIM_GYRO_H
#define _SIM_GYRO_H

#include "msgs/msgs.h"

#ifdef _WIN32
  // Ensure that Winsock2.h is included before Windows.h, which can get
  // pulled in by anybody (e.g., Boost).
  #include <Winsock2.h>
#endif

#include <gazebo/transport/transport.hh>

using namespace gazebo;

class SimGyro {
public:
	SimGyro(std::string topic);

    void Reset();
    double GetAngle();
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
