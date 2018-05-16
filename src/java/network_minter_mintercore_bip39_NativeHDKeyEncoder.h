//
// MinterWallet. 2018
// Eduard Maximovich <edward.vstock@gmail.com>
//

#include <jni.h>

#ifndef MINTERWALLET_NETWORK_MINTER_MINTERCORE_BIP39_NATIVEHDKEYENCODER_H
#define MINTERWALLET_NETWORK_MINTER_MINTERCORE_BIP39_NATIVEHDKEYENCODER_H

#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jobject JNICALL
Java_network_minter_mintercore_bip39_NativeHDKeyEncoder_encoderMakeBip32RootKey(
    JNIEnv *env,
    jclass type,
    jobject seed_buffer,
    jobject net);


JNIEXPORT jobject JNICALL
Java_network_minter_mintercore_bip39_NativeHDKeyEncoder_encoderMakeExtendedKey(
    JNIEnv *env,
    jclass type,
    jobject _rootHdKey,
    jobject net_,
    jstring _derivation_path
);


#ifdef __cplusplus
}
#endif

#endif //MINTERWALLET_NETWORK_MINTER_MINTERCORE_BIP39_NATIVEHDKEYENCODER_H
