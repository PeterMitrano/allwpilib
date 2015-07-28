#include <jni.h>
#include <assert.h>
#include "Log.hpp"

#include "edu_wpi_first_wpilibj_communication_FRCNetworkCommunicationsLibrary.h"
#include "HAL/HAL.hpp"

// set the logging level
TLogLevel netCommLogLevel = logWARNING;

#define NETCOMM_LOG(level) \
    if (level > netCommLogLevel) ; \
    else Log().Get(level)

//
// field ids
//
bool initializeComplete = false;
jclass dataClass;
jfieldID packetIndexFieldID;
jfieldID controlFieldID;
jfieldID teamIDFieldID;
jfieldID dsID_AllianceFieldID;
jfieldID dsID_PositionFieldID;
jfieldID stick0AxesFieldID;
jfieldID stick0ButtonsFieldID;
jfieldID stick1AxesFieldID;
jfieldID stick1ButtonsFieldID;
jfieldID stick2AxesFieldID;
jfieldID stick2ButtonsFieldID;
jfieldID stick3AxesFieldID;
jfieldID stick3ButtonsFieldID;
jfieldID versionDataFieldID;

/*
 * Class:     edu_wpi_first_wpilibj_communication_FRCNetworkCommunicationsLibrary
 * Method:    setNewDataSem
 * Signature: ([B)V
 */
JNIEXPORT void JNICALL Java_edu_wpi_first_wpilibj_communication_FRCNetworkCommunicationsLibrary_setNewDataSem
  (JNIEnv * env, jclass, jobject id )
{
	MULTIWAIT_ID javaId = (MULTIWAIT_ID)env->GetDirectBufferAddress(id);
	NETCOMM_LOG(logDEBUG) << "Mutex Ptr = " << javaId;
	HALSetNewDataSem(javaId->native_handle());
}

/*
 * Class:     edu_wpi_first_wpilibj_communication_FRC_NetworkCommunicationsLibrary
 * Method:    FRC_NetworkCommunication_observeUserProgramDisabled
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_edu_wpi_first_wpilibj_communication_FRCNetworkCommunicationsLibrary_FRCNetworkCommunicationObserveUserProgramDisabled
  (JNIEnv *, jclass)
{
	HALNetworkCommunicationObserveUserProgramDisabled();
}

/*
 * Class:     edu_wpi_first_wpilibj_communication_FRC_NetworkCommunicationsLibrary
 * Method:    FRC_NetworkCommunication_observeUserProgramAutonomous
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_edu_wpi_first_wpilibj_communication_FRCNetworkCommunicationsLibrary_FRCNetworkCommunicationObserveUserProgramAutonomous
  (JNIEnv *, jclass)
{
	HALNetworkCommunicationObserveUserProgramAutonomous();
}

/*
 * Class:     edu_wpi_first_wpilibj_communication_FRC_NetworkCommunicationsLibrary
 * Method:    FRC_NetworkCommunication_observeUserProgramTeleop
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_edu_wpi_first_wpilibj_communication_FRCNetworkCommunicationsLibrary_FRCNetworkCommunicationObserveUserProgramTeleop
  (JNIEnv *, jclass)
{
	HALNetworkCommunicationObserveUserProgramTeleop();
}

/*
 * Class:     edu_wpi_first_wpilibj_communication_FRC_NetworkCommunicationsLibrary
 * Method:    FRC_NetworkCommunication_observeUserProgramTest
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_edu_wpi_first_wpilibj_communication_FRCNetworkCommunicationsLibrary_FRCNetworkCommunicationObserveUserProgramTest
  (JNIEnv *, jclass)
{
	HALNetworkCommunicationObserveUserProgramTest();
}

/*
 * Class:     edu_wpi_first_wpilibj_communication_FRCNetworkCommunicationsLibrary
 * Method:    HALGetJoystickAxes
 * Signature: (B)[S
 */
JNIEXPORT jshortArray JNICALL Java_edu_wpi_first_wpilibj_communication_FRCNetworkCommunicationsLibrary_HALGetJoystickAxes
  (JNIEnv * env, jclass, jbyte joystickNum)
{
	NETCOMM_LOG(logDEBUG) << "Calling HALJoystickAxes";
    HALJoystickAxes axes;
    HALGetJoystickAxes(joystickNum, &axes);

    jshortArray axesArray = env->NewShortArray(axes.count);
    env->SetShortArrayRegion(axesArray, 0, axes.count, axes.axes);

    return axesArray;
}

/*
 * Class:     edu_wpi_first_wpilibj_communication_FRCNetworkCommunicationsLibrary
 * Method:    HALGetJoystickPOVs
 * Signature: (B)[S
 */
JNIEXPORT jshortArray JNICALL Java_edu_wpi_first_wpilibj_communication_FRCNetworkCommunicationsLibrary_HALGetJoystickPOVs
  (JNIEnv * env, jclass, jbyte joystickNum)
{
	NETCOMM_LOG(logDEBUG) << "Calling HALJoystickPOVs";
    HALJoystickPOVs povs;
    HALGetJoystickPOVs(joystickNum, &povs);

    jshortArray povsArray = env->NewShortArray(povs.count);
    env->SetShortArrayRegion(povsArray, 0, povs.count, povs.povs);

    return povsArray;
}

/*
 * Class:     edu_wpi_first_wpilibj_communication_FRCNetworkCommunicationsLibrary
 * Method:    HALGetJoystickButtons
 * Signature: (B)S
 */
JNIEXPORT jint JNICALL Java_edu_wpi_first_wpilibj_communication_FRCNetworkCommunicationsLibrary_HALGetJoystickButtons
  (JNIEnv * env, jclass, jbyte joystickNum, jobject count)
{
	NETCOMM_LOG(logDEBUG) << "Calling HALJoystickButtons";
    HALJoystickButtons joystickButtons;
    HALGetJoystickButtons(joystickNum, &joystickButtons);
	jbyte *countPtr = (jbyte*)env->GetDirectBufferAddress(count);
	NETCOMM_LOG(logDEBUG) << "Buttons = " << joystickButtons.buttons;
	NETCOMM_LOG(logDEBUG) << "Count = " << (jint)joystickButtons.count;
	*countPtr = joystickButtons.count;
	NETCOMM_LOG(logDEBUG) << "CountBuffer = " << (jint)*countPtr;
    return joystickButtons.buttons;
}

/*
 * Class:     edu_wpi_first_wpilibj_communication_FRCNetworkCommunicationsLibrary
 * Method:    HALSetJoystickOutputs
 * Signature: (BISS)I
 */
JNIEXPORT jint JNICALL Java_edu_wpi_first_wpilibj_communication_FRCNetworkCommunicationsLibrary_HALSetJoystickOutputs
  (JNIEnv *, jclass, jbyte port, jint outputs, jshort leftRumble, jshort rightRumble)
{
	NETCOMM_LOG(logDEBUG) << "Calling HALSetJoystickOutputs on port " << port;
	NETCOMM_LOG(logDEBUG) << "Outputs: " << outputs;
	NETCOMM_LOG(logDEBUG) << "Left Rumble: " << leftRumble << " Right Rumble: " << rightRumble;
	return HALSetJoystickOutputs(port, outputs, leftRumble, rightRumble);
}

/*
 * Class:     edu_wpi_first_wpilibj_communication_FRCNetworkCommunicationsLibrary
 * Method:    HALGetJoystickIsXbox
 * Signature: (B)I
 */
JNIEXPORT jint JNICALL Java_edu_wpi_first_wpilibj_communication_FRCNetworkCommunicationsLibrary_HALGetJoystickIsXbox
  (JNIEnv *, jclass, jbyte port)
  {
	NETCOMM_LOG(logDEBUG) << "Calling HALGetJoystickIsXbox";
	return HALGetJoystickIsXbox(port);
  }

/*
 * Class:     edu_wpi_first_wpilibj_communication_FRCNetworkCommunicationsLibrary
 * Method:    HALGetJoystickType
 * Signature: (B)I
 */
JNIEXPORT jint JNICALL Java_edu_wpi_first_wpilibj_communication_FRCNetworkCommunicationsLibrary_HALGetJoystickType
  (JNIEnv *, jclass, jbyte port)
  {
	NETCOMM_LOG(logDEBUG) << "Calling HALGetJoystickType";
	return HALGetJoystickType(port);
  }

/*
 * Class:     edu_wpi_first_wpilibj_communication_FRCNetworkCommunicationsLibrary
 * Method:    HALGetJoystickName
 * Signature: (B)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_edu_wpi_first_wpilibj_communication_FRCNetworkCommunicationsLibrary_HALGetJoystickName
  (JNIEnv * env, jclass, jbyte port)
 {
	NETCOMM_LOG(logDEBUG) << "Calling HALGetJoystickName";
	return env->NewStringUTF(HALGetJoystickName(port));
  }

/*
 * Class:     edu_wpi_first_wpilibj_communication_FRCNetworkCommunicationsLibrary
 * Method:    NativeHALGetControlWord
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_edu_wpi_first_wpilibj_communication_FRCNetworkCommunicationsLibrary_NativeHALGetControlWord
  (JNIEnv *, jclass)
{
	NETCOMM_LOG(logDEBUG) << "Calling HAL Control Word";
    jint controlWord;
    HALGetControlWord((HALControlWord*)&controlWord);
    return controlWord;
}

/*
 * Class:     edu_wpi_first_wpilibj_communication_FRCNetworkCommunicationsLibrary

 * Method:    HALGetMatchTime
 * Signature: ()F
 */
JNIEXPORT jfloat JNICALL Java_edu_wpi_first_wpilibj_communication_FRCNetworkCommunicationsLibrary_HALGetMatchTime
  (JNIEnv * env, jclass)
{
	jfloat matchTime;
	HALGetMatchTime((float*)&matchTime);
	return matchTime;
}

/*
 * Class:     edu_wpi_first_wpilibj_communication_FRCNetworkCommunicationsLibrary
 * Method:    NativeHALGetAllianceStation
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_edu_wpi_first_wpilibj_communication_FRCNetworkCommunicationsLibrary_NativeHALGetAllianceStation
  (JNIEnv *, jclass)
{
	NETCOMM_LOG(logDEBUG) << "Calling HAL Alliance Station";
    jint allianceStation;
    HALGetAllianceStation((HALAllianceStationID*)&allianceStation);
    return allianceStation;
}
