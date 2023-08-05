#pragma once

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jobjectArray JNICALL
Java_com_edwardstock_bip3x_NativeBip39_00024Companion_bip39GetLanguages(JNIEnv*, jobject);

JNIEXPORT jobjectArray JNICALL
Java_com_edwardstock_bip3x_NativeBip39_00024Companion_bip39GetWordsFromLanguage(
    JNIEnv* env, jobject type, jstring language_
);

JNIEXPORT jobject JNICALL Java_com_edwardstock_bip3x_NativeBip39_00024Companion_bip39EncodeBytes(
    JNIEnv* env, jobject type, jobject input, jstring language_, jint entropy
);

JNIEXPORT jboolean JNICALL
Java_com_edwardstock_bip3x_NativeBip39_00024Companion_bip39ValidateMnemonic(
    JNIEnv* env, jobject, jstring mnemonic_, jstring language_
);

JNIEXPORT jbyteArray JNICALL Java_com_edwardstock_bip3x_NativeBip39_00024Companion_bip39WordsToSeed(
    JNIEnv* env, jobject type, jstring mnemonic_
);

JNIEXPORT jobject JNICALL Java_com_edwardstock_bip3x_NativeBip39_00024Companion_bip39Generate(
    JNIEnv* env, jobject type, jstring language_, jint entropy
);

JNIEXPORT jstring JNICALL Java_com_edwardstock_bip3x_NativeBip39_00024Companion_bip39GetEthAddress(
    JNIEnv* env, jobject thiz, jbyteArray private_key
);

JNIEXPORT jbyteArray JNICALL Java_com_edwardstock_bip3x_NativeBip39_00024Companion_bip39SignMessage(
    JNIEnv* env, jobject thiz, jbyteArray private_key, jbyteArray message
);

#ifdef __cplusplus
}
#endif
