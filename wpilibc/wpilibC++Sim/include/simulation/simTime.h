#pragma once

#ifdef _WIN32
  // Ensure that Winsock2.h is included before Windows.h, which can get
  // pulled in by anybody (e.g., Boost).
  #include <Winsock2.h>
#endif

#include "simulation/SimFloatInput.h"

namespace wpilib { namespace internal {
    extern double simTime;
	extern MULTIWAIT_ID time_wait;
	extern MUTEX_ID time_wait_mutex;
	extern void time_callback(const msgs::ConstFloat64Ptr &msg);
    // transport::SubscriberPtr time_sub;
}}
