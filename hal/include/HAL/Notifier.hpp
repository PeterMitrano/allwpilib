#pragma once

#include <stdint.h>

extern "C"
{
	typedef void (*NotifierHandlerFunction)(uint64_t currentTime, void *param);
	
	void* initializeNotifier(NotifierHandlerFunction, void* param, int32_t *status);
	void* initializeNotifierThreaded(NotifierHandlerFunction, void* callbackParam, 
									 uint8_t (*threadInit)(void*), void* initParam, 
									 void (*threadEnd)(void*), void* endParam, 
									 int32_t *status);
	void cleanNotifier(void* notifier_pointer, int32_t *status);
	void* getNotifierParam(void* notifier_pointer, int32_t *status);
	void updateNotifierAlarm(void* notifier_pointer, uint64_t triggerTime, int32_t *status);
	void stopNotifierAlarm(void* notifier_pointer, int32_t *status);
}
