//
// MinterWallet. 2018
// Eduard Maximovich <edward.vstock@gmail.com>
//

#include <minter/Bip39Mnemonic.h>
#include <minter/HDKeyEncoder.h>
#include <minter/utils.h>
#include "network_minter_mintercore_bip39_NativeHDKeyEncoder.h"

template<size_t N>
void setByteArray(JNIEnv *env, jclass &hdkeyCls, jobject &hdkey, const char *fname, const minter::FixedData<N> &data) {
    jsize sz = static_cast<jsize>(data.size());
    jbyteArray arr = env->NewByteArray(sz);
    env->SetByteArrayRegion(arr, 0, sz, reinterpret_cast<const jbyte *>(data.cdata()));

    env->SetObjectField(hdkey, env->GetFieldID(hdkeyCls, fname, "[B"), arr);
}

void setByteArray(JNIEnv *env, jclass &hdkeyCls, jobject &hdkey, const char *fname, const minter::Data &data) {
    jsize sz = static_cast<jsize>(data.size());
    jbyteArray arr = env->NewByteArray(sz);
    env->SetByteArrayRegion(arr, 0, sz, reinterpret_cast<const jbyte *>(data.cdata()));
    env->SetObjectField(hdkey, env->GetFieldID(hdkeyCls, fname, "[B"), arr);
}

template<size_t N>
minter::FixedData<N> readByteArray(JNIEnv *env, jclass &cls, jobject &obj, const char *fname) {
    jbyteArray tmpArr = readObjectField<jbyteArray >(env, cls, obj, fname, "[B" /*byte[]*/);
    return minter::FixedData<N>(reinterpret_cast<uint8_t *>(env->GetByteArrayElements(tmpArr, 0)));
}

template<typename T>
T readObjectField(JNIEnv *env, jclass &cls, jobject &obj, const char *fname, const char* sig) {
    return (T) env->GetObjectField(obj, env->GetFieldID(cls, fname, sig));
}

jobject Java_network_minter_mintercore_bip39_NativeHDKeyEncoder_encoderMakeBip32RootKey(
    JNIEnv *env, jclass, jobject seed_buffer, jobject net_) {

    minter::Data64 seed(static_cast<const uint8_t *>(env->GetDirectBufferAddress(seed_buffer)), 64);
    minter::BTCNetwork net;

    // Convert java BTCNetwork to native minter::BTCNetwork
    jclass netjClass = env->FindClass("network/minter/mintercore/bip39/BTCNetwork");

    // now we're using only version from instance @TODO
    jintArray ar = readObjectField<jintArray>(env, netjClass, net_, "bip32", "[I" /*int[]*/);

    uint32_t * nets = reinterpret_cast<uint32_t *>(env->GetIntArrayElements(ar, 0));
    net.bip32[0] = nets[0];
    net.bip32[1] = nets[1];

    const minter::HDKey bip32RootKey = minter::HDKeyEncoder::makeBip32RootKey(seed, net);

    // instancing HDKey
    jclass hdkeyjClass = env->FindClass("network/minter/mintercore/bip39/HDKey");
    jobject jkey = env->AllocObject(hdkeyjClass);

    // private key
    setByteArray<32>(env, hdkeyjClass, jkey, "privateKey", bip32RootKey.privateKey);
    // public key
    setByteArray<33>(env, hdkeyjClass, jkey, "publicKey", bip32RootKey.publicKey);
    // chain code
    setByteArray<32>(env, hdkeyjClass, jkey, "chainCode", bip32RootKey.chainCode);
    // ext priv key
    setByteArray<112>(env, hdkeyjClass, jkey, "extPrivateKey", bip32RootKey.extPrivateKey);
    // ext pub key
    setByteArray<112>(env, hdkeyjClass, jkey, "extPublicKey", bip32RootKey.extPublicKey);


    return jkey;
}

jobject Java_network_minter_mintercore_bip39_NativeHDKeyEncoder_encoderMakeExtendedKey(
    JNIEnv *env,
    jclass,
    jobject _rootHdKey,
    jstring _derivation_path) {

    // instancing native HDKey from java HDKey
    minter::HDKey rootHdKey;

    jclass keyCls = env->GetObjectClass(_rootHdKey);

    rootHdKey.publicKey = readByteArray<33>(env, keyCls, _rootHdKey, "publicKey");
    rootHdKey.privateKey = readByteArray<32>(env, keyCls, _rootHdKey, "privateKey");
    rootHdKey.chainCode = readByteArray<32>(env, keyCls, _rootHdKey, "chainCode");
    rootHdKey.extPrivateKey = readByteArray<112>(env, keyCls, _rootHdKey, "extPrivateKey");
    rootHdKey.extPublicKey = readByteArray<112>(env, keyCls, _rootHdKey, "extPublicKey");

    const char *derivation_path = env->GetStringUTFChars(_derivation_path, 0);
    minter::HDKey extHdKey = minter::HDKeyEncoder::makeExtendedKey(rootHdKey, derivation_path);
    env->ReleaseStringUTFChars(_derivation_path, derivation_path);

    // clear root
    rootHdKey.clear();


    // instancing java HDKey from native
    jobject outHDKey = env->AllocObject(keyCls);
    // private key
    setByteArray<32>(env, keyCls, outHDKey, "privateKey", extHdKey.privateKey);
    // public key
    setByteArray<33>(env, keyCls, outHDKey, "publicKey", extHdKey.publicKey);
    // chain code
    setByteArray<32>(env, keyCls, outHDKey, "chainCode", extHdKey.chainCode);
    // ext private key
    setByteArray<112>(env, keyCls, outHDKey, "extPrivateKey", extHdKey.extPrivateKey);
    // ext public key
    setByteArray<112>(env, keyCls, outHDKey, "extPublicKey", extHdKey.extPublicKey);

    return outHDKey;
}
