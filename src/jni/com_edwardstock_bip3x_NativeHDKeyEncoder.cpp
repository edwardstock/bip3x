#include "com_edwardstock_bip3x_NativeHDKeyEncoder.h"

#include "nobject.h"

#include <bip3x/bip3x_hdkey_encoder.h>
#include <bip3x/details/utils.h>

static jbyteArray create_byte_array(JNIEnv* env, const toolbox::data::bytes_data& data) {
    auto sz = static_cast<jsize>(data.size());
    jbyteArray arr = env->NewByteArray(sz);
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        return nullptr;
    }
    env->SetByteArrayRegion(arr, 0, (jsize) data.size(), (jbyte*) data.data());
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        return nullptr;
    }
    return arr;
}

JNIEXPORT jobject JNICALL Java_com_edwardstock_bip3x_NativeHDKeyEncoder_encoderMakeBip32RootKey(
    JNIEnv* env, jobject, jobject seed_buffer, jobject net_
) {

    if (env->ExceptionCheck()) {
        env->Throw(env->ExceptionOccurred());
        env->ExceptionDescribe();
        return nullptr;
    }

    bip3x::bytes_64 seed(static_cast<const uint8_t*>(env->GetDirectBufferAddress(seed_buffer)), 64);
    bip3x::btc_network net;

    if (env->ExceptionCheck()) {
        env->Throw(env->ExceptionOccurred());
        env->ExceptionDescribe();
        return nullptr;
    }

    // Convert java BTCNetwork to native bip3x::btc_network
    bip3x::nobject nnet(env, net_);
    if (env->ExceptionCheck()) {
        env->Throw(env->ExceptionOccurred());
        env->ExceptionDescribe();
        return nullptr;
    }
    // now we're using only version from instance @TODO
    auto ar = nnet.getFieldObject<jintArray>("bip32", "[I" /*int[]*/);
    if (ar == nullptr) {
        env->ThrowNew(env->FindClass("java/lang/NullPointerException"), "bip32 is null");
        return nullptr;
    }

    jint* nets = env->GetIntArrayElements(ar, nullptr);
    if (env->ExceptionCheck()) {
        env->Throw(env->ExceptionOccurred());
        env->ExceptionDescribe();
        return nullptr;
    }
    if (nets == nullptr) {
        env->ThrowNew(
            env->FindClass("java/lang/NullPointerException"), "Failed to get int array elements"
        );
        return nullptr;
    }

    net.bip32[0] = nets[0];
    net.bip32[1] = nets[1];

    const bip3x::hdkey bip32RootKey = bip3x::bip3x_hdkey_encoder::make_bip32_root_key(seed, net);
    if (env->ExceptionCheck()) {
        env->Throw(env->ExceptionOccurred());
        env->ExceptionDescribe();
        return nullptr;
    }

    // instancing HDKey
    jclass hdkey_clazz = env->FindClass("com/edwardstock/bip3x/HDKey");
    if (env->ExceptionCheck()) {
        env->Throw(env->ExceptionOccurred());
        env->ExceptionDescribe();
        return nullptr;
    }
    jmethodID hdkey_init = env->GetMethodID(hdkey_clazz, "<init>", "(BII[B[B[B[B[B)V");
    if (env->ExceptionCheck()) {
        env->Throw(env->ExceptionOccurred());
        env->ExceptionDescribe();
        return nullptr;
    }

    jvalue args[9];
    args[0].b = (jbyte) bip32RootKey.depth;
    args[1].i = (jint) bip32RootKey.index;
    args[2].i = (jint) bip32RootKey.fingerprint;
    args[3].l = create_byte_array(env, bip32RootKey.public_key);
    args[4].l = create_byte_array(env, bip32RootKey.private_key);
    args[5].l = create_byte_array(env, bip32RootKey.chain_code);
    args[6].l = create_byte_array(env, bip32RootKey.ext_public_key);
    args[7].l = create_byte_array(env, bip32RootKey.ext_private_key);

    return env->NewObjectA(hdkey_clazz, hdkey_init, args);
}

JNIEXPORT jobject JNICALL Java_com_edwardstock_bip3x_NativeHDKeyEncoder_encoderMakeExtendedKey(
    JNIEnv* env, jobject, jobject _rootHdKey, jobject net_, jstring _derivation_path
) {

    bip3x::btc_network net;
    // Convert java BTCNetwork to native bip3x::BTCNetwork
    bip3x::nobject nnet(env, net_);
    // now we're using only version from instance @TODO
    jintArray ar = nnet.getFieldObject<jintArray>("bip32", "[I" /*int[]*/);

    uint32_t* nets = reinterpret_cast<uint32_t*>(env->GetIntArrayElements(ar, 0));
    net.bip32[0] = nets[0];
    net.bip32[1] = nets[1];

    // instancing native HDKey from java HDKey
    bip3x::hdkey hdkey;
    bip3x::nobject inkey(env, _rootHdKey);

    hdkey.net = net;
    hdkey.public_key = inkey.getFieldUint8Array<33>("publicKeyBytes");
    hdkey.private_key = inkey.getFieldUint8Array<32>("privateKeyBytes");
    hdkey.chain_code = inkey.getFieldUint8Array<32>("chainCodeBytes");
    hdkey.ext_private_key = inkey.getFieldUint8Array<112>("extPrivateKeyBytes");
    hdkey.ext_public_key = inkey.getFieldUint8Array<112>("extPublicKeyBytes");
    hdkey.depth = inkey.getFieldUint8("depth");
    hdkey.index = inkey.getFieldUint32("index");
    hdkey.fingerprint = inkey.getFieldUint32("fingerprint");

    const char* derivation_path = env->GetStringUTFChars(_derivation_path, 0);
    bip3x::bip3x_hdkey_encoder::make_extended_key(hdkey, derivation_path);
    env->ReleaseStringUTFChars(_derivation_path, derivation_path);

    // instancing java HDKey from native
    jclass hdkey_clazz = env->FindClass("com/edwardstock/bip3x/HDKey");
    jmethodID hdkey_init = env->GetMethodID(hdkey_clazz, "<init>", "(BII[B[B[B[B[B)V");

    jvalue args[9];
    args[0].b = (jbyte) hdkey.depth;
    args[1].i = (jint) hdkey.index;
    args[2].i = (jint) hdkey.fingerprint;
    args[3].l = create_byte_array(env, hdkey.public_key);
    args[4].l = create_byte_array(env, hdkey.private_key);
    args[5].l = create_byte_array(env, hdkey.chain_code);
    args[6].l = create_byte_array(env, hdkey.ext_public_key);
    args[7].l = create_byte_array(env, hdkey.ext_private_key);

    hdkey.clear();

    return env->NewObjectA(hdkey_clazz, hdkey_init, args);
}
