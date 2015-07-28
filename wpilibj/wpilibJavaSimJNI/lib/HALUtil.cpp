#include <jni.h>
#include <assert.h>
#include "Log.hpp"
#include "edu_wpi_first_wpilibj_hal_HALUtil.h"
#include "HAL/HAL.hpp"
#include "errno.h"
#include <string.h>

// set the logging level
TLogLevel halUtilLogLevel = logWARNING;

#define HALUTIL_LOG(level) \
    if (level > halUtilLogLevel) ; \
    else Log().Get(level)


//
// indicate JNI version support desired
//
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
	// set our logging level
	Log::ReportingLevel() = logDEBUG;
	return JNI_VERSION_1_6;
}

/*
 * Class:     edu_wpi_first_wpilibj_hal_HALUtil
 * Method:    initializeMutex
 * Signature: (I)Ljava/nio/ByteBuffer;
 */
JNIEXPORT jobject JNICALL Java_edu_wpi_first_wpilibj_hal_HALUtil_initializeMutexNormal
(JNIEnv * env, jclass)
{
	HALUTIL_LOG(logDEBUG) << "Calling HALUtil initializeMutex";
	MUTEX_ID mutexPtr = (MUTEX_ID)new unsigned char[sizeof(MUTEX_ID)];
	mutexPtr = initializeMutexNormal();
	HALUTIL_LOG(logDEBUG) << "Mutex Ptr = " << mutexPtr;
	return env->NewDirectByteBuffer(mutexPtr, sizeof(MUTEX_ID));
}

/*
 * Class:     edu_wpi_first_wpilibj_hal_HALUtil
 * Method:    initializeMultiWait
 * Signature: ()Ljava/nio/ByteBuffer;
 */
JNIEXPORT jobject JNICALL Java_edu_wpi_first_wpilibj_hal_HALUtil_initializeMultiWait
(JNIEnv * env, jclass)
{
	HALUTIL_LOG(logDEBUG) << "Calling HALUtil initializeMultiWait";
	MULTIWAIT_ID multiWaitPtr = (MULTIWAIT_ID)new unsigned char[4];
	multiWaitPtr = initializeMultiWait();
	HALUTIL_LOG(logDEBUG) << "MultiWait Ptr = " << multiWaitPtr;
	return env->NewDirectByteBuffer( multiWaitPtr, 4);
}

/*
 * Class:     edu_wpi_first_wpilibj_hal_HALUtil
 * Method:    takeMultiWait
 * Signature: (Ljava/nio/ByteBuffer;Ljava/nio/ByteBuffer;I)B
 */
JNIEXPORT jbyte JNICALL Java_edu_wpi_first_wpilibj_hal_HALUtil_takeMultiWait
  (JNIEnv * env, jclass, jobject multiWaitId, jobject mutexId, jint timeout)
{
 	MULTIWAIT_ID javaMultiWaitId = (MULTIWAIT_ID)env->GetDirectBufferAddress(multiWaitId);
	MUTEX_ID javaMutexId = (MUTEX_ID)env->GetDirectBufferAddress(mutexId);
	takeMultiWait(javaMultiWaitId, javaMutexId, (int32_t) timeout);
	return 0;
}

/*
 * Class:     edu_wpi_first_wpilibj_hal_HALUtil
 * Method:    getHALErrorMessage
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_edu_wpi_first_wpilibj_hal_HALUtil_getHALErrorMessage
  (JNIEnv * paramEnv, jclass, jint paramId)
{
	const char * msg = getHALErrorMessage(paramId);
	HALUTIL_LOG(logDEBUG) << "Calling HALUtil getHALErrorMessage id=" << paramId << " msg=" << msg;
	return paramEnv->NewStringUTF(msg);
}

JNIEXPORT jint JNICALL
    Java_edu_wpi_first_wpilibj_hal_HALUtil_pointerSize(JNIEnv*, jclass) {
  return sizeof(void*);
}
