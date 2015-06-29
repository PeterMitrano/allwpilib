/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.							  */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/

#include "OSAL/Synchronized.h"
#include <stdlib.h>


pthread_mutexattr_t mta;
bool hasInit = false;


NTSynchronized::NTSynchronized(NTReentrantSemaphore& semaphore):m_semaphore(semaphore)
{
	m_semaphore.take();
}

NTSynchronized::~NTSynchronized()
{
	m_semaphore.give();
}
