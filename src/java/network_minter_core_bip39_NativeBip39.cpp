#include <jni.h>
#include <minter/Bip39Mnemonic.h>
#include "network_minter_core_bip39_NativeBip39.h"

/**
 * bip39. 2018
 * NativeBip39.cpp
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */

jobjectArray Java_network_minter_core_bip39_NativeBip39_bip39GetLanguages(JNIEnv *env, jclass) {
    const auto langs = minter::Bip39Mnemonic::getLanguages();

    jobjectArray
        langArr = env->NewObjectArray(static_cast<jsize>(langs.size()),
                                      env->FindClass("java/lang/String"),
                                      nullptr);
    for (jsize i = 0; i < langs.size(); i++) {
        env->SetObjectArrayElement(langArr, i, env->NewStringUTF(langs[i].c_str()));
    }

    return langArr;
}

jobjectArray Java_network_minter_core_bip39_NativeBip39_bip39GetWordsFromLanguage(
    JNIEnv *env, jclass, jstring language_) {
    const char *language = env->GetStringUTFChars(language_, 0);

    const std::vector<std::string> words = minter::Bip39Mnemonic::getWordsFromLanguage(language);

    jobjectArray wordsArr = env->NewObjectArray(static_cast<jsize>(words.size()),
                                                env->FindClass("java/lang/String"),nullptr);
    for (size_t i = 0; i < words.size(); i++) {
        jstring s = env->NewStringUTF(words[i].c_str());
        env->SetObjectArrayElement(wordsArr, static_cast<jsize>(i), s);
        env->DeleteLocalRef(s);
    }

    env->ReleaseStringUTFChars(language_, language);
    return wordsArr;
}
jboolean Java_network_minter_core_bip39_NativeBip39_bip39ValidateMnemonic(
    JNIEnv *env,
    jclass,
    jstring mnemonic_,
    jstring language_) {
    const char *mnemonic = env->GetStringUTFChars(mnemonic_, 0);
    const char *language = env->GetStringUTFChars(language_, 0);

    bool res = minter::Bip39Mnemonic::validateWords(language, mnemonic);

    env->ReleaseStringUTFChars(mnemonic_, mnemonic);
    env->ReleaseStringUTFChars(language_, language);
    return static_cast<jboolean>(res ? 1 : 0);
}

jobject Java_network_minter_core_bip39_NativeBip39_bip39EncodeBytes(
    JNIEnv *env, jclass, jobject input, jstring language_, jint entropy) {

    uint8_t *buffer = (uint8_t *) env->GetDirectBufferAddress(input);
    const char *language = env->GetStringUTFChars(language_, 0);

    const minter::Bip39Mnemonic::MnemonicResult out = minter::Bip39Mnemonic::encodeBytes(
        buffer, language, static_cast<size_t>(entropy)
    );
    env->ReleaseStringUTFChars(language_, language);

    jclass mrClass = env->FindClass("network/minter/core/bip39/MnemonicResult");
    jobject mrObj = env->AllocObject(mrClass);
    env->SetIntField(mrObj, env->GetFieldID(mrClass, "status", "I"), out.status);
    env->SetIntField(mrObj, env->GetFieldID(mrClass, "len", "I"), static_cast<jint>(out.len));
    env->SetObjectField(
        mrObj,
        env->GetFieldID(mrClass, "words", "Ljava/lang/String;"),
        env->NewStringUTF(out.raw.c_str())
    );

    return mrObj;
}

jobject Java_network_minter_core_bip39_NativeBip39_bip39Generate(
    JNIEnv *env,
    jclass type,
    jstring language_,
    jint entropy) {
    const char *language = env->GetStringUTFChars(language_, 0);
    const minter::Bip39Mnemonic::MnemonicResult
        out = minter::Bip39Mnemonic::generate(language, static_cast<size_t>(entropy));

    env->ReleaseStringUTFChars(language_, language);

    jclass mrClass = env->FindClass("network/minter/core/bip39/MnemonicResult");
    jobject mrObj = env->AllocObject(mrClass);
    env->SetIntField(mrObj, env->GetFieldID(mrClass, "status", "I"), out.status);
    env->SetIntField(mrObj, env->GetFieldID(mrClass, "len", "I"), static_cast<jint>(out.len));
    env->SetObjectField(
        mrObj,
        env->GetFieldID(mrClass, "words", "Ljava/lang/String;"),
        env->NewStringUTF(out.raw.c_str())
    );

    return mrObj;
}

jbyteArray Java_network_minter_core_bip39_NativeBip39_bip39WordsToSeed(
    JNIEnv *env, jclass, jstring mnemonic_) {
    const char *mnemonic = env->GetStringUTFChars(mnemonic_, 0);

    minter::Data tmp(64);
    size_t written;
    minter::Bip39Mnemonic::wordsToSeed(mnemonic, tmp.data(), &written);

    env->ReleaseStringUTFChars(mnemonic_, mnemonic);

    jbyteArray out = env->NewByteArray(64);
    env->SetByteArrayRegion(out, 0, 64, reinterpret_cast<const jbyte *>(tmp.cdata()));

    return out;
}

