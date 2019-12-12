//
// bip3x. 2018
// Eduard Maximovich <edward.vstock@gmail.com>
//

#include <jni.h>

#ifndef MINTERWALLET_com_edwardstock_bip3x_NATIVEHDKEYENCODER_H
#define MINTERWALLET_com_edwardstock_bip3x_NATIVEHDKEYENCODER_H

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jobject JNICALL
Java_com_edwardstock_bip3x_NativeHDKeyEncoder_encoderMakeBip32RootKey(
    JNIEnv* env,
    jclass type,
    jobject seed_buffer,
    jobject net);


JNIEXPORT jobject JNICALL
Java_com_edwardstock_bip3x_NativeHDKeyEncoder_encoderMakeExtendedKey(
    JNIEnv* env,
    jclass type,
    jobject _rootHdKey,
    jobject net_,
    jstring _derivation_path);


#ifdef __cplusplus
}
#endif

#endif //MINTERWALLET_com_edwardstock_bip3x_NATIVEHDKEYENCODER_H
