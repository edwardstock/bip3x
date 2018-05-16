//
// MinterWallet. 2018
// Eduard Maximovich <edward.vstock@gmail.com>
//

#include <minter/Bip39Mnemonic.h>
#include <minter/HDKeyEncoder.h>
#include <minter/utils.h>
#include <type_traits>
#include "network_minter_mintercore_bip39_NativeHDKeyEncoder.h"
#include "nobject.h"

jobject Java_network_minter_mintercore_bip39_NativeHDKeyEncoder_encoderMakeBip32RootKey(
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
    minter::nobject out(env, "network/minter/mintercore/bip39/HDKey");

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

jobject Java_network_minter_mintercore_bip39_NativeHDKeyEncoder_encoderMakeExtendedKey(
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
    minter::HDKey rootHdKey;
    minter::nobject rk(env, _rootHdKey);

    rootHdKey.net = net;
    rootHdKey.publicKey = rk.getFieldUint8Array<33>("publicKey");
    rootHdKey.privateKey = rk.getFieldUint8Array<32>("privateKey");
    rootHdKey.chainCode = rk.getFieldUint8Array<32>("chainCode");
    rootHdKey.extPrivateKey = rk.getFieldUint8Array<112>("extPrivateKey");
    rootHdKey.extPublicKey = rk.getFieldUint8Array<112>("extPublicKey");
    rootHdKey.depth = rk.getFieldUint8("depth");
    rootHdKey.index = rk.getFieldUint32("index");
    rootHdKey.fingerprint = rk.getFieldUint32("fingerprint");


    const char *derivation_path = env->GetStringUTFChars(_derivation_path, 0);
    minter::HDKey extHdKey = minter::HDKeyEncoder::makeExtendedKey(rootHdKey, derivation_path);
    env->ReleaseStringUTFChars(_derivation_path, derivation_path);


    // instancing java HDKey from native
    minter::nobject out(env, env->GetObjectClass(_rootHdKey));
//    jobject outHDKey = env->AllocObject(keyCls);

    // private key
    out.setFieldUint8Array<32>("privateKey", extHdKey.privateKey);
    // public key
    out.setFieldUint8Array<33>("publicKey", extHdKey.publicKey);
    // chain code
    out.setFieldUint8Array<32>("chainCode", extHdKey.chainCode);
    // ext private key
    out.setFieldUint8Array<112>("extPrivateKey", extHdKey.extPrivateKey);
    // ext public key
    out.setFieldUint8Array<112>("extPublicKey", extHdKey.extPublicKey);

    out.setField("depth", extHdKey.depth);
    out.setField("index", extHdKey.index);
    out.setField("fingerprint", extHdKey.fingerprint);

    // clear root
    rootHdKey.clear();
    return out.getObject();
}
