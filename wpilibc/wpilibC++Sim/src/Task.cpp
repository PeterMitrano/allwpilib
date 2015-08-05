/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.
 */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/

#include "Task.h"

//#include "NetworkCommunication/UsageReporting.h"
#include "WPIErrors.h"
#include <errno.h>
#include <string.h>
#include <stdio.h>

#ifndef OK
#define OK 0
#endif /* OK */
#ifndef ERROR
#define ERROR (-1)
#endif /* ERROR */

Task& Task::operator=(Task&& task) {
  m_thread.swap(task.m_thread);
  m_taskName = std::move(task.m_taskName);

  return *this;
}

Task::~Task() {
  if (m_thread.joinable()) {
    std::cout << "[HAL] Exited task " << m_taskName << std::endl;
  }
}

void Task::join() {
  m_thread.join();
}
