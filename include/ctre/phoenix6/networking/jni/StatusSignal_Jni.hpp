/*
 * Copyright (C) Cross The Road Electronics.  All rights reserved.
 * License information can be found in CTRE_LICENSE.txt
 * For support and suggestions contact support@ctr-electronics.com or file
 * an issue tracker at https://github.com/CrossTheRoadElec/Phoenix-Releases
 */
/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_ctre_phoenix6_jni_StatusSignalJNI */

#ifndef _Included_com_ctre_phoenix6_jni_StatusSignalJNI
#define _Included_com_ctre_phoenix6_jni_StatusSignalJNI
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_ctre_phoenix6_jni_StatusSignalJNI
 * Method:    JNI_GetUnits
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_ctre_phoenix6_jni_StatusSignalJNI_JNI_1GetUnits
  (JNIEnv *, jobject);

/*
 * Class:     com_ctre_phoenix6_jni_StatusSignalJNI
 * Method:    JNI_RefreshSignal
 * Signature: (D)I
 */
JNIEXPORT jint JNICALL Java_com_ctre_phoenix6_jni_StatusSignalJNI_JNI_1RefreshSignal
  (JNIEnv *, jobject, jdouble);

/*
 * Class:     com_ctre_phoenix6_jni_StatusSignalJNI
 * Method:    JNI_WaitForSignal
 * Signature: (D)I
 */
JNIEXPORT jint JNICALL Java_com_ctre_phoenix6_jni_StatusSignalJNI_JNI_1WaitForSignal
  (JNIEnv *, jobject, jdouble);

/*
 * Class:     com_ctre_phoenix6_jni_StatusSignalJNI
 * Method:    JNI_WaitForAll
 * Signature: (Ljava/lang/String;D[Lcom/ctre/phoenix6/jni/StatusSignalJNI;)I
 */
JNIEXPORT jint JNICALL Java_com_ctre_phoenix6_jni_StatusSignalJNI_JNI_1WaitForAll
  (JNIEnv *, jclass, jstring, jdouble, jobjectArray);

/*
 * Class:     com_ctre_phoenix6_jni_StatusSignalJNI
 * Method:    JNI_SetUpdateFrequency
 * Signature: (DD)I
 */
JNIEXPORT jint JNICALL Java_com_ctre_phoenix6_jni_StatusSignalJNI_JNI_1SetUpdateFrequency
  (JNIEnv *, jobject, jdouble, jdouble);

/*
 * Class:     com_ctre_phoenix6_jni_StatusSignalJNI
 * Method:    JNI_SetUpdateFrequencyForAll
 * Signature: (D[Lcom/ctre/phoenix6/jni/StatusSignalJNI;D)I
 */
JNIEXPORT jint JNICALL Java_com_ctre_phoenix6_jni_StatusSignalJNI_JNI_1SetUpdateFrequencyForAll
  (JNIEnv *, jclass, jdouble, jobjectArray, jdouble);

/*
 * Class:     com_ctre_phoenix6_jni_StatusSignalJNI
 * Method:    JNI_GetAppliedUpdateFrequency
 * Signature: ()D
 */
JNIEXPORT jdouble JNICALL Java_com_ctre_phoenix6_jni_StatusSignalJNI_JNI_1GetAppliedUpdateFrequency
  (JNIEnv *, jobject);

/*
 * Class:     com_ctre_phoenix6_jni_StatusSignalJNI
 * Method:    JNI_OptimizeUpdateFrequencies
 * Signature: (Ljava/lang/String;IDD)I
 */
JNIEXPORT jint JNICALL Java_com_ctre_phoenix6_jni_StatusSignalJNI_JNI_1OptimizeUpdateFrequencies
  (JNIEnv *, jclass, jstring, jint, jdouble, jdouble);

#ifdef __cplusplus
}
#endif
#endif
