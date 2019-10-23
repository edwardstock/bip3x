/*!
 * bip39.
 * hdkey_encoder.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include <minter/bip39/HDKeyEncoder.h>
#include "cbip39_hdkey_encoder.h"

minter_hdkey *copy_hdkey(minter::HDKey &&res) {
    auto *out = new minter_hdkey;

    memcpy(out->public_key.data, res.publicKey.data(), res.publicKey.size());
    memcpy(out->private_key.data, res.privateKey.data(), res.privateKey.size());
    memcpy(out->chain_code.data, res.chainCode.data(), res.chainCode.size());
    memcpy(out->ext_private_key.data, res.extPrivateKey.data(), res.extPrivateKey.size());
    memcpy(out->ext_public_key.data, res.extPublicKey.data(), res.extPublicKey.size());
    out->depth = res.depth;
    out->index = res.index;
    out->fingerprint = res.fingerprint;

    return out;
}

minter_hdkey *encoder_make_bip32_root_key(const struct minter_data64 *seed) {
    return copy_hdkey(minter::HDKeyEncoder::makeBip32RootKey(minter::Data64(seed->data),
                                                             minter::HDKeyEncoder::MainNet));
}
minter_hdkey *encoder_make_ext_key(const struct minter_hdkey *root_key, const char *derivation_path) {
    minter::HDKey in;
    in.net = minter::HDKeyEncoder::MainNet;
    in.publicKey.get().assign(root_key->public_key.data, root_key->public_key.data + 33);
    in.privateKey.get().assign(root_key->private_key.data, root_key->private_key.data + 32);
    in.chainCode.get().assign(root_key->chain_code.data, root_key->chain_code.data + 32);
    in.extPublicKey.get().assign(root_key->ext_public_key.data, root_key->ext_public_key.data + 112);
    in.extPrivateKey.get().assign(root_key->ext_private_key.data, root_key->ext_private_key.data + 112);

    in.index = root_key->index;
    in.depth = root_key->depth;
    in.fingerprint = root_key->fingerprint;

    return copy_hdkey(minter::HDKeyEncoder::makeExtendedKey(in, derivation_path));
}

void free_hdkey(struct minter_hdkey *key) {
    if (!key) return;
    memset(key->public_key.data, 0, 33);
    memset(key->private_key.data, 0, 32);
    memset(key->chain_code.data, 0, 32);
    memset(key->ext_private_key.data, 0, 112);
    memset(key->ext_public_key.data, 0, 112);

    delete key;
}

