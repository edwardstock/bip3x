#pragma once

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jobjectArray JNICALL
Java_com_edwardstock_bip3x_BIP3X_00024Companion_bip3xGetLanguages(JNIEnv*, jobject);

JNIEXPORT jobjectArray JNICALL
Java_com_edwardstock_bip3x_BIP3X_00024Companion_bip3xGetWordsFromLanguage(
    JNIEnv* env, jobject type, jstring language_
);

JNIEXPORT jobject JNICALL Java_com_edwardstock_bip3x_BIP3X_00024Companion_bip3xEncodeBytes(
    JNIEnv* env, jobject type, jobject input, jstring language_, jint entropy
);

JNIEXPORT jboolean JNICALL Java_com_edwardstock_bip3x_BIP3X_00024Companion_bip3xValidateMnemonic(
    JNIEnv* env, jobject, jstring mnemonic_, jstring language_
);

JNIEXPORT jbyteArray JNICALL Java_com_edwardstock_bip3x_BIP3X_00024Companion_bip3xWordsToSeed(
    JNIEnv* env, jobject type, jstring mnemonic_
);

JNIEXPORT jobject JNICALL Java_com_edwardstock_bip3x_BIP3X_00024Companion_bip3xGenerate(
    JNIEnv* env, jobject type, jstring language_, jint entropy
);

JNIEXPORT jstring JNICALL Java_com_edwardstock_bip3x_BIP3X_00024Companion_bip3xGetEthAddress(
    JNIEnv* env, jobject thiz, jbyteArray private_key
);

JNIEXPORT jbyteArray JNICALL Java_com_edwardstock_bip3x_BIP3X_00024Companion_bip3xSignMessage(
    JNIEnv* env, jobject thiz, jbyteArray private_key, jbyteArray message
);

#ifdef __cplusplus
}
#endif
