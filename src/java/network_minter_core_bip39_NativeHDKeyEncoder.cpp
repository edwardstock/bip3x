//
// MinterWallet. 2018
// Eduard Maximovich <edward.vstock@gmail.com>
//

#include <minter/bip39/HDKeyEncoder.h>
#include <minter/bip39/utils.h>
#include "network_minter_core_bip39_NativeHDKeyEncoder.h"
#include "nobject.h"

JNIEXPORT jobject JNICALL
Java_network_minter_core_bip39_NativeHDKeyEncoder_encoderMakeBip32RootKey(
    JNIEnv *env, jclass, jobject seed_buffer, jobject net_) {

    minter::Data64 seed(static_cast<const uint8_t *>(env->GetDirectBufferAddress(seed_buffer)), 64);
    minter::BTCNetwork net;

    // Convert java BTCNetwork to native minter::BTCNetwork
    minter::nobject nnet(env, net_);
    // now we're using only version from instance @TODO
    jintArray ar = nnet.getFieldObject<jintArray>("bip32", "[I" /*int[]*/);

    uint32_t *nets = reinterpret_cast<uint32_t *>(env->GetIntArrayElements(ar, 0));
    net.bip32[0] = nets[0];
    net.bip32[1] = nets[1];

    const minter::HDKey bip32RootKey = minter::HDKeyEncoder::makeBip32RootKey(seed, net);

    // instancing HDKey
    minter::nobject out(env, "network/minter/core/bip39/HDKey");

    // private key
    out.setFieldUint8Array<32>("privateKey", bip32RootKey.privateKey);
    // public key
    out.setFieldUint8Array<33>("publicKey", bip32RootKey.publicKey);
    // chain code
    out.setFieldUint8Array<32>("chainCode", bip32RootKey.chainCode);
    // ext priv key
    out.setFieldUint8Array<112>("extPrivateKey", bip32RootKey.extPrivateKey);
    // ext pub key
    out.setFieldUint8Array<112>("extPublicKey", bip32RootKey.extPublicKey);

    out.setField("depth", bip32RootKey.depth);
    out.setField("index", bip32RootKey.index);
    out.setField("fingerprint", bip32RootKey.fingerprint);

    return out.getObject();
}

JNIEXPORT jobject JNICALL
Java_network_minter_core_bip39_NativeHDKeyEncoder_encoderMakeExtendedKey(
    JNIEnv *env,
    jclass,
    jobject _rootHdKey,
    jobject net_,
    jstring _derivation_path) {

    minter::BTCNetwork net;
    // Convert java BTCNetwork to native minter::BTCNetwork
    minter::nobject nnet(env, net_);
    // now we're using only version from instance @TODO
    jintArray ar = nnet.getFieldObject<jintArray>("bip32", "[I" /*int[]*/);

    uint32_t *nets = reinterpret_cast<uint32_t *>(env->GetIntArrayElements(ar, 0));
    net.bip32[0] = nets[0];
    net.bip32[1] = nets[1];

    // instancing native HDKey from java HDKey
    minter::HDKey hdkey;
    minter::nobject inkey(env, _rootHdKey);

    hdkey.net = net;
    hdkey.publicKey = inkey.getFieldUint8Array<33>("publicKey");
    hdkey.privateKey = inkey.getFieldUint8Array<32>("privateKey");
    hdkey.chainCode = inkey.getFieldUint8Array<32>("chainCode");
    hdkey.extPrivateKey = inkey.getFieldUint8Array<112>("extPrivateKey");
    hdkey.extPublicKey = inkey.getFieldUint8Array<112>("extPublicKey");
    hdkey.depth = inkey.getFieldUint8("depth");
    hdkey.index = inkey.getFieldUint32("index");
    hdkey.fingerprint = inkey.getFieldUint32("fingerprint");

    const char *derivation_path = env->GetStringUTFChars(_derivation_path, 0);
    minter::HDKeyEncoder::makeExtendedKey(hdkey, derivation_path);
    env->ReleaseStringUTFChars(_derivation_path, derivation_path);


    // instancing java HDKey from native
    minter::nobject out(env, env->GetObjectClass(_rootHdKey));

    // private key
    out.setFieldUint8Array<32>("privateKey", hdkey.privateKey);
    // public key
    out.setFieldUint8Array<33>("publicKey", hdkey.publicKey);
    // chain code
    out.setFieldUint8Array<32>("chainCode", hdkey.chainCode);
    // ext private key
    out.setFieldUint8Array<112>("extPrivateKey", hdkey.extPrivateKey);
    // ext public key
    out.setFieldUint8Array<112>("extPublicKey", hdkey.extPublicKey);

    out.setField("depth", hdkey.depth);
    out.setField("index", hdkey.index);
    out.setField("fingerprint", hdkey.fingerprint);

    // clear root
    hdkey.clear();
    return out.getObject();
}
