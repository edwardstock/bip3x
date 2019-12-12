/*!
 * bip39.
 * hdkey_encoder.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include "cbip39_hdkey_encoder.h"

#include <bip3x/HDKeyEncoder.h>

static void copy_hdkey(const bip3x::HDKey& res, minter_hdkey* out) {
    memcpy(out->public_key.data, res.publicKey.cdata(), res.publicKey.size());
    memcpy(out->private_key.data, res.privateKey.cdata(), res.privateKey.size());
    memcpy(out->chain_code.data, res.chainCode.cdata(), res.chainCode.size());
    memcpy(out->ext_private_key.data, res.extPrivateKey.cdata(), res.extPrivateKey.size());
    memcpy(out->ext_public_key.data, res.extPublicKey.cdata(), res.extPublicKey.size());
    out->depth = res.depth;
    out->index = res.index;
    out->fingerprint = res.fingerprint;
}

static void minter_memset_s(uint8_t *dst, uint8_t val, size_t n) {
    if (dst == NULL) return;
    volatile unsigned char *p = dst;
    while (n--) {
        *p++ = val;
    }
}

void encoder_make_bip32_root_key(const struct bip3x_data64* seed, minter_hdkey* hdkey) {
    copy_hdkey(
        bip3x::HDKeyEncoder::makeBip32RootKey(bip3x::bytes_64(seed->data, 64), bip3x::MainNet),
        hdkey);
}

void encoder_make_ext_key(struct minter_hdkey *hdkey, const char *derivation_path) {
    bip3x::HDKey in;
    in.net = bip3x::MainNet;
    in.publicKey.get().assign(hdkey->public_key.data, hdkey->public_key.data + 33);
    in.privateKey.get().assign(hdkey->private_key.data, hdkey->private_key.data + 32);
    in.chainCode.get().assign(hdkey->chain_code.data, hdkey->chain_code.data + 32);
    in.extPublicKey.get().assign(hdkey->ext_public_key.data, hdkey->ext_public_key.data + 112);
    in.extPrivateKey.get().assign(hdkey->ext_private_key.data, hdkey->ext_private_key.data + 112);

    in.index = hdkey->index;
    in.depth = hdkey->depth;
    in.fingerprint = hdkey->fingerprint;
    bip3x::HDKeyEncoder::makeExtendedKey(in, derivation_path);

    copy_hdkey(in, hdkey);
}

void free_hdkey(struct minter_hdkey *key) {
    if (key == NULL) return;

    minter_memset_s(key->public_key.data, 0, 33);
    minter_memset_s(key->private_key.data, 0, 32);
    minter_memset_s(key->chain_code.data, 0, 32);
    minter_memset_s(key->ext_private_key.data, 0, 112);
    minter_memset_s(key->ext_public_key.data, 0, 112);
}

