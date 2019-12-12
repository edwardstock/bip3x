/*! 
 * bip39. 2018
 * 
 * \author Eduard Maximovich <edward.vstock@gmail.com>
 * \link https://github.com/edwardstock
 */

#include "bip3x/HDKeyEncoder.h"

#include "bip3x/crypto/ripemd160.h"
#include "bip3x/uint256_t.hpp"

#include <toolbox/data/bytes_data.h>
#include <toolbox/strings.hpp>

bip3x::bytes_64 bip3x::HDKeyEncoder::makeBip39Seed(const std::string& mnemonicWords) {
    size_t n;
    bytes_64 out;
    Bip39Mnemonic::wordsToSeed(mnemonicWords.c_str(), out.data(), &n);
    return out;
}
bip3x::bytes_64 bip3x::HDKeyEncoder::makeBip39Seed(const std::vector<std::string>& mnemonicWords) {
    return makeBip39Seed(toolbox::strings::glue(" ", mnemonicWords));
}

bip3x::HDKey bip3x::HDKeyEncoder::makeBip32RootKey(const char* mnemonic, bip3x::BTCNetwork net) {
    return makeBip32RootKey(makeBip39Seed(mnemonic), net);
}

bip3x::HDKey bip3x::HDKeyEncoder::makeBip32RootKey(const bip3x::bytes_64& seed, bip3x::BTCNetwork net) {
    HDKey out;

    out = fromSeed(seed);
    out.net = std::move(net);

    serialize(out, 0x00000000, out.net.bip32[1], false);
    fillPublicKey(out);
    serialize(out, 0x00000000, out.net.bip32[0], true);

    return out;
}

void bip3x::HDKeyEncoder::makeExtendedKey(bip3x::HDKey& rootKey, const Derivation& derivation) {
    derivePath(rootKey, derivation.path, true);

    serialize(rootKey, rootKey.fingerprint, rootKey.net.bip32[1], false);

    fillPublicKey(rootKey);
    serialize(rootKey, rootKey.fingerprint, rootKey.net.bip32[0], true);
    fillPublicKey(rootKey);
}

void bip3x::HDKeyEncoder::derive(bip3x::HDKey& key, uint32_t index) {
    CONFIDENTIAL bytes_data buff(37);
    CONFIDENTIAL uint256_t a, b;
    CONFIDENTIAL bytes_64 I;

    // fetching parent fingerprint
    key.fingerprint = fetchFingerprint(key);

    if (index & 0x80000000) { // private derivation
        buff.write(0, (uint8_t) 0x00);
        buff.write(1, key.privateKey.cdata(), 32);
    } else { // public derivation
        if (!key.curve->params) {
            return;
        }
        fillPublicKey(key);
        buff.write(0, key.publicKey.cdata(), 33);
    }
    buff.write(33, index);

    a = key.privateKey;

    CONFIDENTIAL hmac_sha512(key.chainCode.cdata(), 32, buff.cdata(), buff.size(), I.data());

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
                bn_write_be(&b.val(), key.privateKey.data());
                break;
            }

            buff.write(0, (uint8_t) 1);
            buff.write(1, I.take_last(32));
            hmac_sha512(key.chainCode.cdata(), 32, buff.cdata(), buff.size(), I.data());
        }
    } else {
        key.privateKey = I.take_first(32);
    }

    key.chainCode = I.take_last(32);
    key.depth++;
    key.index = index;
    key.publicKey.clear();

    // making sure to wipe our memory
    a.clear();
    b.clear();
    I.clear();
    buff.clear();
}

void bip3x::HDKeyEncoder::derivePath(HDKey& key, const std::string& path, bool priv) {
    std::vector<std::string> pathBits = toolbox::strings::split(path, "/");
    for (const auto& bit : pathBits) {
        if (bit == "m" || bit == "'") {
            continue;
        }

        bool hardened = bit.length() > 1 && bit[bit.length() - 1] == '\'';
        uint32_t index;
        if (hardened) {
            const std::string tmp(bit.begin(), bit.end() - 1);
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

uint32_t bip3x::HDKeyEncoder::fetchFingerprint(bip3x::HDKey& key) {
    bytes_32 digest;
    uint32_t fingerprint;
    fillPublicKey(key);

    trezor::SHA256_CTX ctx;
    sha256_Init(&ctx);
    sha256_Update(&ctx, key.publicKey.cdata(), 33);
    sha256_Final(&ctx, digest.data());

    ripemd160(digest.data(), 32, digest.data());
    fingerprint = digest.to_num<uint32_t>();
    digest.clear();

    return fingerprint;
}
void bip3x::HDKeyEncoder::fillPublicKey(bip3x::HDKey& key) {
    if (key.curve->params) {
        if (key.publicKey.size() == 0) {
            key.publicKey.resize(33);
        }
        ecdsa_get_public_key33(key.curve->params, key.privateKey.cdata(), key.publicKey.data());
    }
}
void bip3x::HDKeyEncoder::serialize(bip3x::HDKey& key, uint32_t fingerprint, uint32_t version, bool publicKey) {
    bytes_data data(78);
    data.write(0, version);
    data.write(4, key.depth);
    data.write(5, fingerprint);
    data.write(9, key.index);
    data.write(13, key.chainCode);

    bip32_key* outKey;
    if (publicKey) {
        data.write(45, key.publicKey.cdata(), 33);
        outKey = &key.extPublicKey;
    } else {
        data.write(45, (uint8_t) 0);
        data.write(46, key.privateKey.cdata(), 32);
        outKey = &key.extPrivateKey;
    }

    outKey->clear();

    base58_encode_check(
        data.cdata(), static_cast<int>(data.size()), // input
        key.curve->hasher_base58, // hasher
        reinterpret_cast<char *>(outKey->data()), static_cast<int>(outKey->size()) // output
    );

    data.clear();
}

bip3x::HDKey bip3x::HDKeyEncoder::fromSeed(const char* seed) {
    return fromSeed(bytes_data(reinterpret_cast<const uint8_t*>(seed), strlen(seed)));
}

bip3x::HDKey bip3x::HDKeyEncoder::fromSeed(const uint8_t* seed, size_t seedLength) {
    return fromSeed(bytes_data(seed, seedLength));
}
bip3x::HDKey bip3x::HDKeyEncoder::fromSeed(const bip3x::bytes_data& seed) {
    HDKey out;
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

    out.privateKey = I.take_first(32);
    out.chainCode = I.take_last(32);
    out.publicKey.clear();
    I.clear();

    return out;
}

bip3x::HDKey::HDKey()
    : publicKey(),
      privateKey(),
      chainCode(),
      extPrivateKey(),
      extPublicKey(),
      net(MainNet),
      depth(0),
      index(0),
      fingerprint(0),
      curve(&secp256k1_info) { }

bip3x::HDKey::HDKey(const bip3x::HDKey& other)
    : publicKey(other.publicKey),
      privateKey(other.privateKey),
      chainCode(other.chainCode),
      extPrivateKey(other.extPrivateKey),
      extPublicKey(other.extPublicKey),
      net(other.net),
      depth(other.depth),
      index(other.index),
      fingerprint(other.fingerprint),
      curve(&secp256k1_info) {

}

bip3x::HDKey::HDKey(bip3x::HDKey&& other) noexcept
    : publicKey(std::move(other.publicKey)),
      privateKey(std::move(other.privateKey)),
      chainCode(std::move(other.chainCode)),
      extPrivateKey(std::move(other.extPrivateKey)),
      extPublicKey(std::move(other.extPublicKey)),
      net(std::move(other.net)),
      depth(other.depth),
      index(other.index),
      fingerprint(other.fingerprint),
      curve(&secp256k1_info) {

}

bip3x::HDKey& bip3x::HDKey::operator=(bip3x::HDKey other) {
    std::swap(publicKey, other.publicKey);
    std::swap(privateKey, other.privateKey);
    std::swap(chainCode, other.chainCode);
    std::swap(extPrivateKey, other.extPrivateKey);
    std::swap(extPublicKey, other.extPublicKey);
    std::swap(net, other.net);
    std::swap(depth, other.depth);
    std::swap(index, other.index);
    std::swap(fingerprint, other.fingerprint);
    curve = &secp256k1_info;
    return *this;
}

bip3x::HDKey::~HDKey() {
    clear();
}

void bip3x::HDKey::clear() {
    publicKey.clear();
    privateKey.clear();
    chainCode.clear();
    extPrivateKey.clear();
    extPublicKey.clear();
}

bip3x::Derivation::Derivation(const std::string& path)
    : path(path) {
}
bip3x::Derivation::Derivation(std::string&& path)
    : path(std::move(path)) {
}
bip3x::Derivation::Derivation(const char* path)
    : path(std::string(path)) {
}
