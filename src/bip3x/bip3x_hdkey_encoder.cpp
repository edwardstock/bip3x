#include "bip3x/bip3x_hdkey_encoder.h"

#include "bip3x/crypto/base58.h"
#include "bip3x/crypto/ecdsa.h"
#include "bip3x/crypto/hmac.h"
#include "bip3x/crypto/ripemd160.h"
#include "bip3x/crypto/secp256k1.h"
#include "bip3x/details/uint256_t.hpp"

#include <toolbox/data/bytes_data.h>
#include <toolbox/strings.hpp>

bip3x::bytes_64 bip3x::bip3x_hdkey_encoder::make_bip39_seed(const std::string& mnemonic_words) {
    size_t n;
    bytes_64 out;
    bip3x_mnemonic::words_to_seed(mnemonic_words.c_str(), out.data(), &n);
    return out;
}
bip3x::bytes_64
bip3x::bip3x_hdkey_encoder::make_bip39_seed(const std::vector<std::string>& mnemonic_words) {
    return make_bip39_seed(toolbox::strings::glue(" ", mnemonic_words));
}

bip3x::hdkey
bip3x::bip3x_hdkey_encoder::make_bip32_root_key(const char* mnemonic, btc_network net) {
    return make_bip32_root_key(make_bip39_seed(mnemonic), net);
}

bip3x::hdkey
bip3x::bip3x_hdkey_encoder::make_bip32_root_key(const bytes_64& seed, btc_network net) {
    hdkey out;

    out = from_seed(seed);
    out.net = std::move(net);

    serialize(out, 0x00000000, out.net.bip32[1], false);
    fill_public_key(out);
    serialize(out, 0x00000000, out.net.bip32[0], true);

    return out;
}

bip3x::hdkey bip3x::bip3x_hdkey_encoder::make_extended_key(
    const bip3x::hdkey& root_key, const bip3x::derivation_path& derivation
) {
    hdkey out = root_key;
    extend_key(out, derivation.path);
    return out;
}

void bip3x::bip3x_hdkey_encoder::extend_key(hdkey& root_key, const derivation_path& derivation) {
    derive_path(root_key, derivation.path, true);

    serialize(root_key, root_key.fingerprint, root_key.net.bip32[1], false);

    fill_public_key(root_key);
    serialize(root_key, root_key.fingerprint, root_key.net.bip32[0], true);
    fill_public_key(root_key);
}

void bip3x::bip3x_hdkey_encoder::derive(bip3x::hdkey& key, uint32_t index) {
    CONFIDENTIAL bytes_data buff(37);
    CONFIDENTIAL uint256_t a, b;
    CONFIDENTIAL bytes_64 I;

    // fetching parent fingerprint
    key.fingerprint = fetch_fingerprint(key);

    if (index & 0x80000000) { // private derivation
        buff.write(0, (uint8_t) 0x00);
        buff.write(1, key.private_key.cdata(), 32);
    } else { // public derivation
        if (!key.curve->params) {
            return;
        }
        fill_public_key(key);
        buff.write(0, key.public_key.cdata(), 33);
    }
    buff.write(33, index);

    a = key.private_key;

    CONFIDENTIAL hmac_sha512(key.chain_code.cdata(), 32, buff.cdata(), buff.size(), I.data());

    if (key.curve->params) {
        while (true) {
            bool failed = false;
            bn_read_be(I.cdata(), &b.val());

            if (!bn_is_less(&b.val(), &key.curve->params->order)) {
                failed = true;
            } else {
                bn_add(&b.val(), &a.val());
                bn_mod(&b.val(), &key.curve->params->order);
                if (b == 0) {
                    failed = true;
                }
            }

            if (!failed) {
                bn_write_be(&b.val(), key.private_key.data());
                break;
            }

            buff.write(0, (uint8_t) 1);
            buff.write(1, I.take_last(32));
            hmac_sha512(key.chain_code.cdata(), 32, buff.cdata(), buff.size(), I.data());
        }
    } else {
        key.private_key = I.take_first(32);
    }

    key.chain_code = I.take_last(32);
    key.depth++;
    key.index = index;
    key.public_key.clear();

    // making sure to wipe our memory
    a.clear();
    b.clear();
    I.clear();
    buff.clear();
}

void bip3x::bip3x_hdkey_encoder::derive_path(hdkey& key, const std::string& path, bool priv) {
    std::vector<std::string> pathBits = toolbox::strings::split(path, "/");
    for (const auto& bit : pathBits) {
        if (bit == "m" || bit == "'") {
            continue;
        }

        bool hardened = bit.length() > 1 && bit[bit.length() - 1] == '\'';
        uint32_t index;
        if (hardened) {
            const std::string tmp(bit.begin(), bit.end() - 1);
            if (tmp.length() == 1 && tmp[0] == 'm') {
                throw std::invalid_argument("Derivation path has wrong format: only digit segments "
                                            "can be hardened; remove apostrophe from m'/");
            }
            index = str_to_uint32_t(tmp);
        } else {
            index = str_to_uint32_t(bit);
        }

        bool isPrivateKey = priv;
        bool invalidDerivationPath = hardened && !isPrivateKey;

        if (invalidDerivationPath) {
            return;
        }

        if (hardened) {
            derive(key, index + 0x80000000);
        } else {
            if (isPrivateKey) {
                derive(key, index);
            } else {
                //@TODO
                //                hdnode_public_ckd(node, index);
            }
        }
    }
}

uint32_t bip3x::bip3x_hdkey_encoder::fetch_fingerprint(hdkey& key) {
    bytes_32 digest;
    uint32_t fingerprint;
    fill_public_key(key);

    trezor::SHA256_CTX ctx;
    sha256_Init(&ctx);
    sha256_Update(&ctx, key.public_key.cdata(), 33);
    sha256_Final(&ctx, digest.data());

    ripemd160(digest.data(), 32, digest.data());
    fingerprint = digest.to_num<uint32_t>();
    digest.clear();

    return fingerprint;
}
void bip3x::bip3x_hdkey_encoder::fill_public_key(hdkey& key) {
    if (key.curve->params) {
        if (key.public_key.empty()) {
            key.public_key.resize(33);
        }
        ecdsa_get_public_key33(key.curve->params, key.private_key.cdata(), key.public_key.data());
    }
}
void bip3x::bip3x_hdkey_encoder::serialize(
    bip3x::hdkey& key, uint32_t fingerprint, uint32_t version, bool publicKey
) {
    bytes_data data(78);
    data.write(0, version);
    data.write(4, key.depth);
    data.write(5, fingerprint);
    data.write(9, key.index);
    data.write(13, key.chain_code);

    bip32_key* outKey;
    if (publicKey) {
        data.write(45, key.public_key.cdata(), 33);
        outKey = &key.ext_public_key;
    } else {
        data.write(45, (uint8_t) 0);
        data.write(46, key.private_key.cdata(), 32);
        outKey = &key.ext_private_key;
    }

    outKey->clear();

    base58_encode_check(
        data.cdata(),
        static_cast<int>(data.size()), // input
        key.curve->hasher_base58,      // hasher
        reinterpret_cast<char*>(outKey->data()),
        static_cast<int>(outKey->size()) // output
    );

    data.clear();
}

bip3x::hdkey bip3x::bip3x_hdkey_encoder::from_seed(const char* seed) {
    return from_seed(bytes_data(reinterpret_cast<const uint8_t*>(seed), strlen(seed)));
}

bip3x::hdkey bip3x::bip3x_hdkey_encoder::from_seed(const uint8_t* seed, size_t seedLength) {
    return from_seed(bytes_data(seed, seedLength));
}
bip3x::hdkey bip3x::bip3x_hdkey_encoder::from_seed(const bytes_data& seed) {
    hdkey out;
    bytes_64 I;
    out.depth = 0;
    out.index = 0;
    if (out.curve == nullptr) {
        return out; // @TODO error handling
    }

    I = seed.switch_map_c(to_hmac_sha512(out.curve->bip32_name));

    if (out.curve->params) {
        uint256_t a;
        while (true) {
            a = I;

            if (a != 0 && a < out.curve->params->order) {
                break;
            }

            I.switch_map(to_hmac_sha512(out.curve->bip32_name));
        }
        a.clear();
    }

    out.private_key = I.take_first(32);
    out.chain_code = I.take_last(32);
    out.public_key.clear();
    I.clear();

    return out;
}

bip3x::hdkey::hdkey()
    : public_key()
    , private_key()
    , chain_code()
    , ext_private_key()
    , ext_public_key()
    , net(mainnet)
    , depth(0)
    , index(0)
    , fingerprint(0)
    , curve(&secp256k1_info) {}

bip3x::hdkey::hdkey(const bip3x::hdkey& other)
    : public_key(other.public_key)
    , private_key(other.private_key)
    , chain_code(other.chain_code)
    , ext_private_key(other.ext_private_key)
    , ext_public_key(other.ext_public_key)
    , net(other.net)
    , depth(other.depth)
    , index(other.index)
    , fingerprint(other.fingerprint)
    , curve(&secp256k1_info) {}

bip3x::hdkey::hdkey(bip3x::hdkey&& other) noexcept
    : public_key(std::move(other.public_key))
    , private_key(std::move(other.private_key))
    , chain_code(std::move(other.chain_code))
    , ext_private_key(std::move(other.ext_private_key))
    , ext_public_key(std::move(other.ext_public_key))
    , net(std::move(other.net))
    , depth(other.depth)
    , index(other.index)
    , fingerprint(other.fingerprint)
    , curve(&secp256k1_info) {}

bip3x::hdkey& bip3x::hdkey::operator=(bip3x::hdkey other) {
    std::swap(public_key, other.public_key);
    std::swap(private_key, other.private_key);
    std::swap(chain_code, other.chain_code);
    std::swap(ext_private_key, other.ext_private_key);
    std::swap(ext_public_key, other.ext_public_key);
    std::swap(net, other.net);
    std::swap(depth, other.depth);
    std::swap(index, other.index);
    std::swap(fingerprint, other.fingerprint);
    curve = &secp256k1_info;
    return *this;
}

bip3x::hdkey::~hdkey() {
    clear();
}

std::string bip3x::hdkey::get_address() const {
    bytes_array<65> pub_key_uncompressed;
    ecdsa_get_public_key65(&secp256k1, private_key.cdata(), pub_key_uncompressed.data());

    bytes_32 sha3_result;
    {
        SHA3_CTX sha3ctx;
        keccak_256_Init(&sha3ctx);
        keccak_256(pub_key_uncompressed.take_last_c(64).cdata(), 64, sha3_result.data());
    }

    bytes_array<20> address = sha3_result.take_last_c(20);
    return "0x" + address.to_hex();
}

void bip3x::hdkey::clear() {
    public_key.clear();
    private_key.clear();
    chain_code.clear();
    ext_private_key.clear();
    ext_public_key.clear();
}

bip3x::derivation_path::derivation_path(const std::string& path)
    : path(path) {}
bip3x::derivation_path::derivation_path(std::string&& path)
    : path(std::move(path)) {}
bip3x::derivation_path::derivation_path(const char* path)
    : path(std::string(path)) {}
