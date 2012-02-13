/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2011 Alexander Barker.  All Rights Received.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "NativeErrors.h"
#include "NativeHelpers.h"
#include "NativeThread.h"
#include "org_jnativehook_GlobalScreen.h"

JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getAutoRepeatRate(JNIEnv * env, jobject UNUSED(obj)) {
	long rate = GetAutoRepeatRate();

	if (rate >= 0) {
		#ifdef DEBUG
		fprintf(stdout, "Java_org_jnativehook_GlobalScreen_getAutoRepeatRate(): successful. (rate: %li)\n", rate);
		#endif
	}
	else {
		#ifdef DEBUG
		fprintf(stderr, "Java_org_jnativehook_GlobalScreen_getAutoRepeatRate(): failure!\n");
		#endif

		ThrowException(env, NATIVE_KEY_EXCEPTION, "Could not determine the keyboard auto repeat rate.");
		rate = JNI_ERR;
	}

	return (jlong) rate;
}

JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getAutoRepeatDelay(JNIEnv * env, jobject UNUSED(obj)) {
	long delay = GetAutoRepeatDelay();

	if (delay >= 0) {
		#ifdef DEBUG
		fprintf(stdout, "Java_org_jnativehook_GlobalScreen_getAutoRepeatDelay(): successful. (delay: %li)\n", delay);
		#endif
	}
	else {
		#ifdef DEBUG
		fprintf(stderr, "Java_org_jnativehook_GlobalScreen_getAutoRepeatDelay(): failure!\n");
		#endif

		ThrowException(env, NATIVE_KEY_EXCEPTION, "Could not determine the keyboard auto repeat delay.");
		delay = JNI_ERR;
	}

	return (jlong) delay;
}

// 0-Threshold X, 1-Threshold Y and 2-Speed
JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getPointerAccelerationMultiplier(JNIEnv * env, jobject UNUSED(obj)) {
	long multiplier = GetPointerAccelerationMultiplier();

	if (multiplier >= 0) {
		#ifdef DEBUG
		fprintf(stdout, "Java_org_jnativehook_GlobalScreen_getPointerAccelerationMultiplier(): successful. (multiplier: %li)\n", multiplier);
		#endif
	}
	else {
		#ifdef DEBUG
		fprintf(stdout, "Java_org_jnativehook_GlobalScreen_getPointerAccelerationMultiplier(): failure!\n");
		#endif

		ThrowException(env, NATIVE_MOUSE_EXCEPTION, "Could not determine the pointer acceleration multiplier.");
		multiplier = JNI_ERR;
	}

	return (jlong) multiplier;
}

// 0-Threshold X, 1-Threshold Y and 2-Speed
JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getPointerAccelerationThreshold(JNIEnv * env, jobject UNUSED(obj)) {
	long threshold = GetPointerAccelerationThreshold();

	if (threshold >= 0) {
		#ifdef DEBUG
		fprintf(stdout, "Java_org_jnativehook_GlobalScreen_getPointerAccelerationThreshold(): successful. (threshold: %li)\n", threshold);
		#endif
	}
	else {
		#ifdef DEBUG
		fprintf(stdout, "Java_org_jnativehook_GlobalScreen_getPointerAccelerationThreshold(): failure!\n");
		#endif

		ThrowException(env, NATIVE_MOUSE_EXCEPTION, "Could not determine the pointer acceleration threshold.");
		threshold = JNI_ERR;
	}

	return (jlong) threshold;
}


JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getPointerSensitivity(JNIEnv * env, jobject UNUSED(obj)) {
	long sensitivity = GetPointerSensitivity();

	if (sensitivity >= 0) {
		#ifdef DEBUG
		fprintf(stdout, "Java_org_jnativehook_GlobalScreen_getPointerSensitivity(): successful. (sensitivity: %li)\n", sensitivity);
		#endif
	}
	else {
		#ifdef DEBUG
		fprintf(stdout, "Java_org_jnativehook_GlobalScreen_getPointerSensitivity(): failure!\n");
		#endif

		ThrowException(env, NATIVE_MOUSE_EXCEPTION, "Could not determine the pointer sensitivity.");
		sensitivity = JNI_ERR;
	}

	return (jlong) sensitivity;
}


JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getDoubleClickTime(JNIEnv * env, jobject UNUSED(obj)) {
	long clicktime = GetDoubleClickTime();

	if (clicktime >= 0) {
		#ifdef DEBUG
		fprintf(stdout, "Java_org_jnativehook_GlobalScreen_getDoubleClickTime(): successful. (time: %li)\n", clicktime);
		#endif
	}
	else {
		#ifdef DEBUG
		fprintf(stderr, "Java_org_jnativehook_GlobalScreen_getDoubleClickTime(): failure!\n");
		#endif

		ThrowException(env, NATIVE_KEY_EXCEPTION, "Could not determine the mouse double click time.");
		clicktime = JNI_ERR; //Naturally exit so jni exception is thrown.
	}

	return (jlong) clicktime;
}



JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_registerNativeHook(JNIEnv * env, jobject UNUSED(obj)) {
	if (StartNativeThread() != EXIT_SUCCESS) {
		ThrowException(env, NATIVE_HOOK_EXCEPTION, "Could not register the native hook.");
	}
}


JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_unregisterNativeHook(JNIEnv * env, jobject UNUSED(obj)) {
	if (StopNativeThread() != EXIT_SUCCESS) {
		ThrowException(env, NATIVE_HOOK_EXCEPTION, "Could not unregister the native hook.");
	}
}

JNIEXPORT jboolean JNICALL Java_org_jnativehook_GlobalScreen_isNativeHookRegistered(JNIEnv * UNUSED(env), jobject UNUSED(obj)) {
	return (jboolean) IsNativeThreadRunning();
}



//This is where java attaches to the native machine.  Its kind of like the java + native constructor.
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM * vm, void * UNUSED(reserved)) {
	//Grab the currently running virtual machine so we can attach to it in
	//functions that are not called from java. ( I.E. ThreadLoop )
	jvm = vm;
	JNIEnv * env = 0;

	jint jni_version = JNI_VERSION_1_4;
	if ((*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL) == JNI_OK) {
		#ifdef DEBUG
		fprintf(stdout, "JNI_OnUnload(): AttachCurrentThread() successful.\n");
		#endif

		//Run platform specific load items.
		OnLibraryLoad(env);
	}
	else {
		#ifdef DEBUG
		fprintf(stderr, "JNI_OnUnload(): AttachCurrentThread() failed!\n");
		#endif
	}

    return jni_version;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM * UNUSED(vm), void * UNUSED(reserved)) {
	//Stop the native thread if its running.
	if (IsNativeThreadRunning()) {
		StopNativeThread();
	}

	//Run platform specific unload items.
	OnLibraryUnload();

	//Detach the current thread to the JVM.
	if ((*jvm)->DetachCurrentThread(jvm) == JNI_OK) {
		#ifdef DEBUG
		fprintf(stdout, "JNI_OnUnload(): DetachCurrentThread() successful.\n");
		#endif
	}
	else {
		#ifdef DEBUG
		fprintf(stderr, "JNI_OnUnload(): DetachCurrentThread() failed!\n");
		#endif
	}

	#ifdef DEBUG
	fprintf(stdout, "JNI Unloaded.\n");
	#endif
}