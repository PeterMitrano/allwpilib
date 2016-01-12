/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2016. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "HAL/Notifier.hpp"
#include "ChipObject.h"
#include "HAL/HAL.hpp"
#include "HAL/cpp/priority_mutex.h"
#include "HAL/SafeThread.h"
#include <atomic>
#include <cstdlib>
#include <mutex>

static const uint32_t kTimerInterruptNumber = 28;

static priority_mutex notifierInterruptMutex;
static priority_recursive_mutex notifierMutex;
static tAlarm *notifierAlarm = nullptr;
static tInterruptManager *notifierManager = nullptr;
static uint64_t closestTrigger = UINT64_MAX;
struct Notifier {
	Notifier *prev, *next;
	void *param;
	void (*process)(uint64_t, void*);
	uint64_t triggerTime = UINT64_MAX;
	uint8_t threaded = 0;
};
static Notifier *notifiers = nullptr;
static std::atomic_flag notifierAtexitRegistered = ATOMIC_FLAG_INIT;
static std::atomic_int notifierRefCount{0};

static void alarmCallback(uint32_t, void*)
{
	std::unique_lock<priority_recursive_mutex> sync(notifierMutex);

	int32_t status = 0;
	uint64_t currentTime = 0;

	// the hardware disables itself after each alarm
	closestTrigger = UINT64_MAX;

	// process all notifiers
	Notifier *notifier = notifiers;
	while (notifier) {
		if (notifier->triggerTime != UINT64_MAX) {
			if (currentTime == 0)
				currentTime = getFPGATime(&status);
			if (notifier->triggerTime < currentTime) {
				notifier->triggerTime = UINT64_MAX;
				auto process = notifier->process;
				auto param = notifier->param;
				sync.unlock();
				process(currentTime, param);
				sync.lock();
			} else if (notifier->triggerTime < closestTrigger) {
				updateNotifierAlarm(notifier, notifier->triggerTime, &status);
			}
		}
		notifier = notifier->next;
	}
}

static void cleanupNotifierAtExit() {
	notifierAlarm = nullptr;
	notifierManager = nullptr;
}

class NotifierThread : public SafeThread {
 public:
  void Main();

  bool m_notify = false;
  void* m_param = nullptr;
  void (*process)(uint64_t, void*);
  uint8_t (*threadInit)(void*);
  void (*threadEnd)(void*);
  uint64_t m_currentTime;
};

class ThreadedNotifier : public SafeThreadOwner<NotifierThread> {
 public:
  void SetFunc(void (*process)(uint64_t, void*), uint8_t (*threadInit)(void*), void (*threadEnd)(void*), void* param);
  void* GetParam(void);

  void Notify(uint64_t currentTime) {
    auto thr = GetThread();
    if (!thr) return;
    thr->m_currentTime = currentTime;
    thr->m_notify = true;
    thr->m_cond.notify_one();
  }
};

void ThreadedNotifier::SetFunc(void (*process)(uint64_t, void*), uint8_t (*threadInit)(void*), void (*threadEnd)(void*), void* param) {
  auto thr = GetThread();
  if (!thr) return;
  thr->m_param = param;
  thr->process = process;
  thr->threadInit = threadInit;
  thr->threadEnd = threadEnd;
}

void* ThreadedNotifier::GetParam(void)
{
  auto thr = GetThread();
  if (!thr) return nullptr;
  return thr->m_param;
}

void NotifierThread::Main() {
	
  if(threadInit)
  {
	  if (threadInit(m_param)) return;
  }

  std::unique_lock<std::mutex> lock(m_mutex);
  while (m_active) {
    m_cond.wait(lock, [&] { return !m_active || m_notify; });
    if (!m_active) break;
    m_notify = false;
	if (!process) return;
    uint64_t currentTime = m_currentTime;
    lock.unlock();  // don't hold mutex during callback execution
	process(currentTime, m_param);
    lock.lock();
  }
  if (threadEnd) threadEnd(m_param);
}

void notifierHandler(uint64_t currentTimeInt, void* param) {
  ((ThreadedNotifier*)param)->Notify(currentTimeInt);
}

extern "C" {

void* initializeNotifier(void (*process)(uint64_t, void*), void *param, int32_t *status)
{
	if (!process) {
		*status = NULL_PARAMETER;
		return nullptr;
	}
	if (!notifierAtexitRegistered.test_and_set())
		std::atexit(cleanupNotifierAtExit);
	if (notifierRefCount.fetch_add(1) == 0) {
		std::lock_guard<priority_mutex> sync(notifierInterruptMutex);
		// create manager and alarm if not already created
		if (!notifierManager) {
			notifierManager = new tInterruptManager(1 << kTimerInterruptNumber, false, status);
			notifierManager->registerHandler(alarmCallback, NULL, status);
			notifierManager->enable(status);
		}
		if (!notifierAlarm) notifierAlarm = tAlarm::create(status);
	}

	std::lock_guard<priority_recursive_mutex> sync(notifierMutex);
	// create notifier structure and add to list
	Notifier* notifier = new Notifier();
	notifier->prev = nullptr;
	notifier->next = notifiers;
	if (notifier->next) notifier->next->prev = notifier;
	notifier->param = param;
	notifier->process = process;
	notifier->threaded = 0;
	notifiers = notifier;
	return notifier;
}

void* initializeNotifierThreaded(NotifierHandlerFunction process, uint8_t (*threadInit)(void*), 
								 void (*threadEnd)(void*), void* param, int32_t *status)
{
	ThreadedNotifier* notify = new ThreadedNotifier;
	notify->Start();
	notify->SetFunc(process, threadInit, threadEnd, param);
	
	void *notifierPtr = initializeNotifier(notifierHandler, notify, status);
	
	if (!notifierPtr || *status != 0)
	{
		delete notify;
	}
	((Notifier*)notifierPtr)->threaded = 1;
	
	return notifierPtr;
}

void cleanNotifier(void* notifier_pointer, int32_t *status)
{
	Notifier* notifier = (Notifier*)notifier_pointer;
	ThreadedNotifier* notify = NULL;
	if (notifier->threaded)
		notify = (ThreadedNotifier*)notifier->param;
	
	{
		std::lock_guard<priority_recursive_mutex> sync(notifierMutex);

		// remove from list and delete
		if (notifier->prev) notifier->prev->next = notifier->next;
		if (notifier->next) notifier->next->prev = notifier->prev;
		if (notifiers == notifier) notifiers = notifier->next;
		delete notifier;
	}

	if (notifierRefCount.fetch_sub(1) == 1) {
		std::lock_guard<priority_mutex> sync(notifierInterruptMutex);
		// if this was the last notifier, clean up alarm and manager
		if (notifierAlarm) {
			notifierAlarm->writeEnable(false, status);
			delete notifierAlarm;
			notifierAlarm = nullptr;
		}
		if (notifierManager) {
			notifierManager->disable(status);
			delete notifierManager;
			notifierManager = nullptr;
		}
	}
	
	if (notify) delete notify;
}

void* getNotifierParam(void* notifier_pointer, int32_t *status)
{
	return ((Notifier*)notifier_pointer)->param;
}

void* getThreadedNotifierParam(void* notifier_pointer, int32_t *status)
{
	void* notifier = ((Notifier*)notifier_pointer)->param;
	if (notifier)
	{
		return ((ThreadedNotifier*)notifier)->GetParam();
	}
	return nullptr;
}

void updateNotifierAlarm(void* notifier_pointer, uint64_t triggerTime, int32_t *status)
{
	std::lock_guard<priority_recursive_mutex> sync(notifierMutex);

	Notifier* notifier = (Notifier*)notifier_pointer;
	notifier->triggerTime = triggerTime;
	bool wasActive = (closestTrigger != UINT64_MAX);

	if (!notifierInterruptMutex.try_lock() || notifierRefCount == 0 ||
			!notifierAlarm)
		return;

	// Update alarm time if closer than current.
	if (triggerTime < closestTrigger) {
		closestTrigger = triggerTime;
		// Simply truncate the hardware trigger time to 32-bit.
		notifierAlarm->writeTriggerTime((uint32_t)triggerTime, status);
	}
	// Enable the alarm.  The hardware disables itself after each alarm.
	if (!wasActive) notifierAlarm->writeEnable(true, status);

	notifierInterruptMutex.unlock();
}

void stopNotifierAlarm(void* notifier_pointer, int32_t *status)
{
	std::lock_guard<priority_recursive_mutex> sync(notifierMutex);
	Notifier* notifier = (Notifier*)notifier_pointer;
	notifier->triggerTime = UINT64_MAX;
}

}  // extern "C"
