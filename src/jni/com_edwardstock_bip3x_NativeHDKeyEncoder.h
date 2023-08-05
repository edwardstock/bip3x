#pragma once

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jobject JNICALL Java_com_edwardstock_bip3x_NativeHDKeyEncoder_encoderMakeBip32RootKey(
    JNIEnv* env, jobject type, jobject seed_buffer, jobject net
);

JNIEXPORT jobject JNICALL Java_com_edwardstock_bip3x_NativeHDKeyEncoder_encoderMakeExtendedKey(
    JNIEnv* env, jobject type, jobject _rootHdKey, jobject net_, jstring _derivation_path
);

#ifdef __cplusplus
}
#endif
