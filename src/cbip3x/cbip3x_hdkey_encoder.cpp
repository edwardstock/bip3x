#include "cbip3x/cbip3x_hdkey_encoder.h"

#include "bip3x/bip3x_hdkey_encoder.h"

static void copy_hdkey(const bip3x::hdkey& in, minter_hdkey* out) {
    memcpy(out->public_key.data, in.public_key.cdata(), in.public_key.size());
    memcpy(out->private_key.data, in.private_key.cdata(), in.private_key.size());
    memcpy(out->chain_code.data, in.chain_code.cdata(), in.chain_code.size());
    memcpy(out->ext_private_key.data, in.ext_private_key.cdata(), in.ext_private_key.size());
    memcpy(out->ext_public_key.data, in.ext_public_key.cdata(), in.ext_public_key.size());
    out->depth = in.depth;
    out->index = in.index;
    out->fingerprint = in.fingerprint;
}

static void minter_memset_s(uint8_t* dst, uint8_t val, size_t n) {
    if (dst == NULL)
        return;
    volatile unsigned char* p = dst;
    while (n--) {
        *p++ = val;
    }
}

void encoder_make_bip32_root_key(const struct bip3x_data64* seed, minter_hdkey* hdkey) {
    copy_hdkey(
        bip3x::bip3x_hdkey_encoder::make_bip32_root_key(
            bip3x::bytes_64(seed->data, 64), bip3x::mainnet
        ),
        hdkey
    );
}

void encoder_extend_key(struct minter_hdkey* hdkey, const char* derivation_path) {
    bip3x::hdkey in;
    in.net = bip3x::mainnet;
    in.public_key.get().assign(hdkey->public_key.data, hdkey->public_key.data + 33);
    in.private_key.get().assign(hdkey->private_key.data, hdkey->private_key.data + 32);
    in.chain_code.get().assign(hdkey->chain_code.data, hdkey->chain_code.data + 32);
    in.ext_public_key.get().assign(hdkey->ext_public_key.data, hdkey->ext_public_key.data + 112);
    in.ext_private_key.get().assign(hdkey->ext_private_key.data, hdkey->ext_private_key.data + 112);

    in.index = hdkey->index;
    in.depth = hdkey->depth;
    in.fingerprint = hdkey->fingerprint;
    bip3x::bip3x_hdkey_encoder::extend_key(in, derivation_path);

    copy_hdkey(in, hdkey);
}

void free_hdkey(struct minter_hdkey* key) {
    if (key == NULL)
        return;

    minter_memset_s(key->public_key.data, 0, 33);
    minter_memset_s(key->private_key.data, 0, 32);
    minter_memset_s(key->chain_code.data, 0, 32);
    minter_memset_s(key->ext_private_key.data, 0, 112);
    minter_memset_s(key->ext_public_key.data, 0, 112);
}
