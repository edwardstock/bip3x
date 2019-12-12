//
// bip3x. 2018
// Eduard Maximovich <edward.vstock@gmail.com>
//

#include "com_edwardstock_bip3x_NativeHDKeyEncoder.h"

#include "nobject.h"

#include <bip3x/HDKeyEncoder.h>
#include <bip3x/utils.h>

JNIEXPORT jobject JNICALL
Java_com_edwardstock_bip3x_NativeHDKeyEncoder_encoderMakeBip32RootKey(
    JNIEnv* env, jclass, jobject seed_buffer, jobject net_) {

    bip3x::bytes_64 seed(static_cast<const uint8_t*>(env->GetDirectBufferAddress(seed_buffer)), 64);
    bip3x::BTCNetwork net;

    // Convert java BTCNetwork to native minter::BTCNetwork
    bip3x::nobject nnet(env, net_);
    // now we're using only version from instance @TODO
    jintArray ar = nnet.getFieldObject<jintArray>("bip32", "[I" /*int[]*/);

    uint32_t* nets = reinterpret_cast<uint32_t*>(env->GetIntArrayElements(ar, 0));
    net.bip32[0] = nets[0];
    net.bip32[1] = nets[1];

    const bip3x::HDKey bip32RootKey = bip3x::HDKeyEncoder::makeBip32RootKey(seed, net);

    // instancing HDKey
    bip3x::nobject out(env, "com/edwardstock/bip3x/HDKey");

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
Java_com_edwardstock_bip3x_NativeHDKeyEncoder_encoderMakeExtendedKey(
    JNIEnv* env,
    jclass,
    jobject _rootHdKey,
    jobject net_,
    jstring _derivation_path) {

    bip3x::BTCNetwork net;
    // Convert java BTCNetwork to native minter::BTCNetwork
    bip3x::nobject nnet(env, net_);
    // now we're using only version from instance @TODO
    jintArray ar = nnet.getFieldObject<jintArray>("bip32", "[I" /*int[]*/);

    uint32_t* nets = reinterpret_cast<uint32_t*>(env->GetIntArrayElements(ar, 0));
    net.bip32[0] = nets[0];
    net.bip32[1] = nets[1];

    // instancing native HDKey from java HDKey
    bip3x::HDKey hdkey;
    bip3x::nobject inkey(env, _rootHdKey);

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
    bip3x::HDKeyEncoder::makeExtendedKey(hdkey, derivation_path);
    env->ReleaseStringUTFChars(_derivation_path, derivation_path);

    // instancing java HDKey from native
    bip3x::nobject out(env, env->GetObjectClass(_rootHdKey));

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
