#include "com_edwardstock_bip3x_NativeBip39.h"

#include <bip3x/bip3x_crypto.h>
#include <bip3x/bip3x_mnemonic.h>
#include <jni.h>

JNIEXPORT jobjectArray JNICALL
Java_com_edwardstock_bip3x_NativeBip39_00024Companion_bip39GetLanguages(JNIEnv* env, jobject) {
    const auto langs = bip3x::bip3x_mnemonic::get_supported_languages();

    jobjectArray langArr = env->NewObjectArray(
        static_cast<jsize>(langs.size()), env->FindClass("java/lang/String"), nullptr
    );
    for (size_t i = 0; i < langs.size(); i++) {
        env->SetObjectArrayElement(langArr, i, env->NewStringUTF(langs[i].c_str()));
    }

    return langArr;
}

JNIEXPORT jobjectArray JNICALL
Java_com_edwardstock_bip3x_NativeBip39_00024Companion_bip39GetWordsFromLanguage(
    JNIEnv* env, jobject, jstring language_
) {
    const char* language = env->GetStringUTFChars(language_, 0);

    const std::vector<std::string> words = bip3x::bip3x_mnemonic::get_words_for_language(language);

    jobjectArray wordsArr = env->NewObjectArray(
        static_cast<jsize>(words.size()), env->FindClass("java/lang/String"), nullptr
    );
    for (size_t i = 0; i < words.size(); i++) {
        jstring s = env->NewStringUTF(words[i].c_str());
        env->SetObjectArrayElement(wordsArr, static_cast<jsize>(i), s);
        env->DeleteLocalRef(s);
    }

    env->ReleaseStringUTFChars(language_, language);
    return wordsArr;
}

JNIEXPORT jboolean JNICALL
Java_com_edwardstock_bip3x_NativeBip39_00024Companion_bip39ValidateMnemonic(
    JNIEnv* env, jobject, jstring mnemonic_, jstring language_
) {
    const char* mnemonic = env->GetStringUTFChars(mnemonic_, 0);
    const char* language = env->GetStringUTFChars(language_, 0);

    bool res = bip3x::bip3x_mnemonic::validate_words(language, mnemonic);

    env->ReleaseStringUTFChars(mnemonic_, mnemonic);
    env->ReleaseStringUTFChars(language_, language);
    return static_cast<jboolean>(res ? 1 : 0);
}

JNIEXPORT jobject JNICALL Java_com_edwardstock_bip3x_NativeBip39_00024Companion_bip39EncodeBytes(
    JNIEnv* env, jobject, jobject input, jstring language_, jint entropy
) {

    auto* buffer = (uint8_t*) env->GetDirectBufferAddress(input);
    const char* language = env->GetStringUTFChars(language_, JNI_FALSE);

    const bip3x::bip3x_mnemonic::mnemonic_result out =
        bip3x::bip3x_mnemonic::encode_bytes(buffer, language, static_cast<size_t>(entropy));

    env->ReleaseStringUTFChars(language_, language);

    jclass mrClass = env->FindClass("com/edwardstock/bip3x/MnemonicResult");
    jobject mrObj = env->AllocObject(mrClass);
    env->SetIntField(mrObj, env->GetFieldID(mrClass, "status", "I"), static_cast<jint>(out.status));
    env->SetIntField(mrObj, env->GetFieldID(mrClass, "len", "I"), static_cast<jint>(out.len));
    env->SetObjectField(
        mrObj,
        env->GetFieldID(mrClass, "mnemonic", "Ljava/lang/String;"),
        env->NewStringUTF(out.raw.c_str())
    );

    return mrObj;
}

JNIEXPORT jobject JNICALL Java_com_edwardstock_bip3x_NativeBip39_00024Companion_bip39Generate(
    JNIEnv* env, jobject, jstring language_, jint entropy
) {
    const char* language = env->GetStringUTFChars(language_, 0);
    const bip3x::bip3x_mnemonic::mnemonic_result out =
        bip3x::bip3x_mnemonic::generate(language, static_cast<size_t>(entropy));

    const jint statusInt = static_cast<jint>(out.status);

    env->ReleaseStringUTFChars(language_, language);

    jclass mrClass = env->FindClass("com/edwardstock/bip3x/MnemonicResult");
    jmethodID mrCtor = env->GetMethodID(mrClass, "<init>", "(Ljava/lang/String;I)V");
    jobject mrObj = env->NewObject(mrClass, mrCtor, env->NewStringUTF(out.raw.c_str()), statusInt);

    return mrObj;
}

JNIEXPORT jbyteArray JNICALL Java_com_edwardstock_bip3x_NativeBip39_00024Companion_bip39WordsToSeed(
    JNIEnv* env, jobject, jstring mnemonic_
) {
    const char* mnemonic = env->GetStringUTFChars(mnemonic_, JNI_FALSE);

    bip3x::bytes_data tmp(64);
    size_t written;
    bip3x::bip3x_mnemonic::words_to_seed(mnemonic, tmp.data(), &written);

    env->ReleaseStringUTFChars(mnemonic_, mnemonic);

    jbyteArray out = env->NewByteArray(64);
    env->SetByteArrayRegion(out, 0, 64, (jbyte*) (tmp.cdata()));

    return out;
}

JNIEXPORT jstring JNICALL Java_com_edwardstock_bip3x_NativeBip39_00024Companion_bip39GetEthAddress(
    JNIEnv* env, jobject, jbyteArray private_key_
) {
    jsize private_key_len = env->GetArrayLength(private_key_);
    bip3x::bytes_32 private_key;

    env->GetByteArrayRegion(private_key_, 0, private_key_len, (jbyte*) private_key.data());

    auto address = bip3x::get_eth_address(private_key);

    return env->NewStringUTF(address.c_str());
}

JNIEXPORT jbyteArray JNICALL Java_com_edwardstock_bip3x_NativeBip39_00024Companion_bip39SignMessage(
    JNIEnv* env, jobject, jbyteArray private_key_, jbyteArray message_
) {

    jsize private_key_len = env->GetArrayLength(private_key_);
    jsize message_len = env->GetArrayLength(message_);

    bip3x::bytes_32 private_key;
    env->GetByteArrayRegion(private_key_, 0, private_key_len, (jbyte*) private_key.data());

    bip3x::bytes_data message(message_len);
    env->GetByteArrayRegion(message_, 0, message_len, (jbyte*) message.data());

    auto result = bip3x::sign_message(message, private_key);

    jbyteArray result_array = env->NewByteArray((jsize) 65);
    env->SetByteArrayRegion(result_array, 0, (jsize) 32, (const jbyte*) result.r.cdata());
    env->SetByteArrayRegion(result_array, 32, (jsize) 32, (const jbyte*) result.s.cdata());
    env->SetByteArrayRegion(result_array, 64, (jsize) 1, (const jbyte*) result.v.cdata());

    return result_array;
}
