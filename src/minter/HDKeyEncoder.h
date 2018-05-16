/*! 
 * bip39. 2018
 * 
 * \author Eduard Maximovich <edward.vstock@gmail.com>
 * \link https://github.com/edwardstock
 */

#ifndef BIP39_KEY_ENCODER_H
#define BIP39_KEY_ENCODER_H

#include "utils.h"
#include "Bip39Mnemonic.h"
#include <secp256k1.h>
#include <base58.h>
#include <ecdsa.h>
#include <hmac.h>
#include <curves.h>

namespace minter {

struct BTCNetwork {
  std::string name;
  std::string bech32;
  uint32_t bip32[2];
  uint8_t pubKeyHash;
  uint8_t scriptHash;
  uint8_t wif;
};

class Derivation {
 public:
    Derivation(const std::string &path) : path(path) { }
    Derivation(std::string &&path) : path(std::move(path)) { }
    Derivation(const char* path): path(std::string(path)) { }

    std::string path;

};

class HDKey {
 public:
    HDKey() :
        curve(&secp256k1_info),
        depth(0),
        index(0),
        fingerprint(0) { };
    ~HDKey() {
        clear();
    }

    void clear() {
        publicKey.clear();
        privateKey.clear();
        chainCode.clear();
        extPrivateKey.clear();
        extPublicKey.clear();
    }

    FixedData<33> publicKey;
    FixedData<32> privateKey;
    FixedData<32> chainCode;
    Bip32Key extPrivateKey;
    Bip32Key extPublicKey;
    BTCNetwork net;
    uint8_t depth;
    uint32_t index;
    uint32_t fingerprint;
    const curve_info *curve;
};

class HDKeyEncoder {
 private:
    static const std::string masterSecret;
    static const BTCNetwork networks[2];
    static const Derivation EthereumAccountDerivation;

 public:
    static const BTCNetwork MainNet;
    static const BTCNetwork TestNet;

    static Data64 makeBip39Seed(const std::string &mnemonicWords);
    static Data64 makeBip39Seed(const std::vector<std::string> &mnemonicWords);
    static HDKey makeBip32RootKey(const char* mnemonic, BTCNetwork net = MainNet);
    static HDKey makeBip32RootKey(const Data64 &seed, BTCNetwork net = MainNet);
    static HDKey makeExtendedKey(const HDKey &rootKey, const Derivation &derivation);

    static std::string getAddress(const HDKey &key);
 private:
    static void derive(HDKey &key, uint32_t index);
    static HDKey fromSeed(const Data &seed);
    static HDKey fromSeed(const char *seed);
    static HDKey fromSeed(const uint8_t *seed, size_t seedLength);
    static void derivePath(HDKey &key, const std::string &path, bool priv = true);
    static uint32_t fetchFingerprint(HDKey &key);
    static void fillPublicKey(HDKey &key);
    static void serialize(HDKey &key, uint32_t fingerprint, uint32_t version, bool publicKey = false);
};
}

#endif //BIP39_KEY_ENCODER_H
