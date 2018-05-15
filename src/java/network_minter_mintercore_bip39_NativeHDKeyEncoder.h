//
// MinterWallet. 2018
// Eduard Maximovich <edward.vstock@gmail.com>
//

#include <jni.h>

#ifndef MINTERWALLET_NETWORK_MINTER_MINTERCORE_BIP39_NATIVEHDKEYENCODER_H
#define MINTERWALLET_NETWORK_MINTER_MINTERCORE_BIP39_NATIVEHDKEYENCODER_H

template<size_t N>
void setByteArray(JNIEnv *env, jclass &hdkeyCls, jobject &hdkey, const char* fname, const minter::FixedData<N> &data);
void setByteArray(JNIEnv *env, jclass &hdkeyCls, jobject &hdkey, const char* fname, const minter::Data &data);
template<size_t N>
minter::FixedData<N> readByteArray(JNIEnv *env, jclass &cls, jobject &obj, const char* fname);

template <typename T>
T readObjectField(JNIEnv *env, jclass &cls, jobject &obj, const char* fname, const char* sig);

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
    jstring _derivation_path
);


#ifdef __cplusplus
}
#endif

#endif //MINTERWALLET_NETWORK_MINTER_MINTERCORE_BIP39_NATIVEHDKEYENCODER_H
