/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2016. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <jni.h>
#include <assert.h>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
#include <stdio.h>
#include "Log.hpp"
#include "edu_wpi_first_wpilibj_hal_NotifierJNI.h"
#include "HAL/Notifier.hpp"
#include "HALUtil.h"
#include <iostream>

// set the logging level
TLogLevel notifierJNILogLevel = logERROR;

#define NOTIFIERJNI_LOG(level) \
    if (level > notifierJNILogLevel) ; \
    else Log().Get(level)

// Thread where callbacks are actually performed.
//
// JNI's AttachCurrentThread() creates a Java Thread object on every
// invocation, which is both time inefficient and causes issues with Eclipse
// (which tries to keep a thread list up-to-date and thus gets swamped).
//
// Instead, this class attaches just once.  When a hardware notification
// occurs, a condition variable wakes up this thread and this thread actually
// makes the call into Java.
//
// We don't want to use a FIFO here. If the user code takes too long to
// process, we will just ignore the redundant wakeup.



extern "C" {

struct JVMData
{
  jobject m_func;
  jmethodID m_mid;
  JNIEnv *env;
  
};

uint8_t ThreadInit(void *param)
{
  std::cout << "Thread Init" << std::endl;
  JVMData* data = (JVMData*)param;
  JavaVMAttachArgs args;
  args.version = JNI_VERSION_1_2;
  args.name = const_cast<char*>("Notifier");
  args.group = nullptr;
  jint rs = jvm->AttachCurrentThreadAsDaemon((void**)&(data->env), &args);
  if (rs != JNI_OK) return 1;
  return 0;
  
}

void ThreadProcess(uint64_t currentTime, void *param)
{
	std::cout << "Thread Process" << std::endl;
	JVMData* data = (JVMData*)param;
	if (!data->m_func) return;
	jobject func = data->m_func;
    jmethodID mid = data->m_mid;
	data->env->CallVoidMethod(func, mid, (jlong)currentTime);
    if (data->env->ExceptionCheck()) {
      data->env->ExceptionDescribe();
      data->env->ExceptionClear();
    }
}

void ThreadEnd(void *param)
{
	std::cout << "Thread End" << std::endl;
  JVMData* data = (JVMData*)param;
	// free global reference
  if (data->m_func) data->env->DeleteGlobalRef(data->m_func);

  jvm->DetachCurrentThread();
}
/*
 * Class:     edu_wpi_first_wpilibj_hal_NotifierJNI
 * Method:    initializeNotifier
 * Signature: (Ljava/lang/Runnable;)J
 */
JNIEXPORT jlong JNICALL Java_edu_wpi_first_wpilibj_hal_NotifierJNI_initializeNotifier
  (JNIEnv *env, jclass, jobject func)
{
  NOTIFIERJNI_LOG(logDEBUG) << "Calling NOTIFIERJNI initializeNotifier";

  std::cout << "initializeNotifier" << std::endl;
  jclass cls = env->GetObjectClass(func);
  if (cls == 0) {
    NOTIFIERJNI_LOG(logERROR) << "Error getting java class";
    assert(false);
    return 0;
  }
  jmethodID mid = env->GetMethodID(cls, "apply", "(J)V");
  if (mid == 0) {
    NOTIFIERJNI_LOG(logERROR) << "Error getting java method ID";
    assert(false);
    return 0;
  }

  // each notifier runs in its own thread; this is so if one takes too long
  // to execute, it doesn't keep the others from running
  /*
  NotifierJNI* notify = new NotifierJNI;
  notify->Start();
  notify->SetFunc(env, func, mid);
  */
  std::cout << "Setting Data Pointer" << std::endl;
  JVMData* data = new JVMData;
  
  
  
  int32_t status = 0;
  std::cout << "Starting Notifier" << std::endl;
  void *notifierPtr = initializeNotifierThreaded(ThreadProcess, ThreadInit, ThreadEnd, data, &status);
  
  data->m_func = env->NewGlobalRef(func);
  data->m_mid = mid;

  NOTIFIERJNI_LOG(logDEBUG) << "Notifier Ptr = " << notifierPtr;
  NOTIFIERJNI_LOG(logDEBUG) << "Status = " << status;

  if (!notifierPtr || !CheckStatus(env, status)) {
    // something went wrong in HAL, clean up
    delete data;
  }

  return (jlong)notifierPtr;
}

/*
 * Class:     edu_wpi_first_wpilibj_hal_NotifierJNI
 * Method:    cleanNotifier
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_edu_wpi_first_wpilibj_hal_NotifierJNI_cleanNotifier
  (JNIEnv *env, jclass, jlong notifierPtr)
{
  NOTIFIERJNI_LOG(logDEBUG) << "Calling NOTIFIERJNI cleanNotifier";

  NOTIFIERJNI_LOG(logDEBUG) << "Notifier Ptr = " << (void *)notifierPtr;

  int32_t status = 0;
  JVMData* data = (JVMData*)getThreadedNotifierParam((void*)notifierPtr, &status);
  cleanNotifier((void*)notifierPtr, &status);
  NOTIFIERJNI_LOG(logDEBUG) << "Status = " << status;
  CheckStatus(env, status);
  
  delete data;
}

/*
 * Class:     edu_wpi_first_wpilibj_hal_NotifierJNI
 * Method:    updateNotifierAlarm
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_edu_wpi_first_wpilibj_hal_NotifierJNI_updateNotifierAlarm
  (JNIEnv *env, jclass cls, jlong notifierPtr, jlong triggerTime)
{
  NOTIFIERJNI_LOG(logDEBUG) << "Calling NOTIFIERJNI updateNotifierAlarm";

  NOTIFIERJNI_LOG(logDEBUG) << "Notifier Ptr = " << (void *)notifierPtr;

  NOTIFIERJNI_LOG(logDEBUG) << "triggerTime = " << triggerTime;

  int32_t status = 0;
  updateNotifierAlarm((void*)notifierPtr, (uint64_t)triggerTime, &status);
  NOTIFIERJNI_LOG(logDEBUG) << "Status = " << status;
  CheckStatus(env, status);
}

/*
 * Class:     edu_wpi_first_wpilibj_hal_NotifierJNI
 * Method:    stopNotifierAlarm
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_edu_wpi_first_wpilibj_hal_NotifierJNI_stopNotifierAlarm
  (JNIEnv *env, jclass cls, jlong notifierPtr)
{
  NOTIFIERJNI_LOG(logDEBUG) << "Calling NOTIFIERJNI stopNotifierAlarm";

  NOTIFIERJNI_LOG(logDEBUG) << "Notifier Ptr = " << (void *)notifierPtr;

  int32_t status = 0;
  stopNotifierAlarm((void*)notifierPtr, &status);
  NOTIFIERJNI_LOG(logDEBUG) << "Status = " << status;
  CheckStatus(env, status);
}

}  // extern "C"
