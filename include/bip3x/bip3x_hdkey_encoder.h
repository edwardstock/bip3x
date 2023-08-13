#pragma once

#include "bip3x/crypto/secp256k1.h"
#include "bip3x_config.h"
#include "bip3x_mnemonic.h"
#include "details/utils.h"

namespace bip3x {

struct BIP3X_CORE_API btc_network {
    std::string name;
    std::string bech32;
    uint32_t bip32[2];
    uint8_t pubKeyHash;
    uint8_t scriptHash;
    uint8_t wif;
};

static const btc_network mainnet = {
    "bitcoin", "bc", {0x0488b21e, 0x0488ade4},
      0x00, 0x05, 0x80
};

static const btc_network testnet = {
    "testnet", "tb", {0x043587cf, 0x04358394},
      0x6f, 0xc4, 0xef
};

static const std::string MASTER_SECRET = "Bitcoin seed";

class BIP3X_CORE_API derivation_path {
public:
    derivation_path(const std::string& path);
    derivation_path(std::string&& path);
    derivation_path(const char* path);

    std::string path;
};

class BIP3X_CORE_API hdkey {
public:
    hdkey();
    ~hdkey();
    hdkey(const hdkey& other);
    hdkey(hdkey&& other) noexcept;
    hdkey& operator=(bip3x::hdkey other);

    std::string get_address() const;

    void clear();

    bytes_array<33> public_key;
    bytes_array<32> private_key;
    bytes_array<32> chain_code;
    bip32_key ext_private_key;
    bip32_key ext_public_key;
    btc_network net;
    uint8_t depth;
    uint32_t index;
    uint32_t fingerprint;
    const curve_info* curve;
};

class BIP3X_CORE_API bip3x_hdkey_encoder {
public:
    static bytes_64 make_bip39_seed(const std::string& mnemonic_words);
    static bytes_64 make_bip39_seed(const std::vector<std::string>& mnemonic_words);
    static hdkey make_bip32_root_key(const char* mnemonic, btc_network net = bip3x::mainnet);
    static hdkey make_bip32_root_key(const bytes_64& seed, btc_network net = bip3x::mainnet);
    static hdkey make_extended_key(const hdkey& root_key, const derivation_path& derivation);
    static void extend_key(hdkey& root_key, const derivation_path& derivation);

private:
    static void derive(hdkey& key, uint32_t index);
    static hdkey from_seed(const bytes_data& seed);
    static hdkey from_seed(const char* seed);
    static hdkey from_seed(const uint8_t* seed, size_t seedLength);
    static void derive_path(hdkey& key, const std::string& path, bool priv = true);
    static uint32_t fetch_fingerprint(hdkey& key);
    static void fill_public_key(hdkey& key);
    static void serialize(hdkey& key, uint32_t fingerprint, uint32_t version, bool publicKey = false);
};
} // namespace bip3x
