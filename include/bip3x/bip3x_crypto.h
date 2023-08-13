#pragma once

#include "bip3x/crypto/secp256k1.h"
#include "bip3x/bip3x_hdkey_encoder.h"
#include "bip3x/details/utils.h"
#include "bip3x_config.h"

namespace bip3x {

struct BIP3X_CORE_API ecdsa_recoverable_signature {
    bytes_32 r, s;
    bytes_array<1> v;
};

BIP3X_CORE_API
ecdsa_recoverable_signature
sign_message(const bytes_data& msg, const bytes_32& priv_key);

BIP3X_CORE_API
ecdsa_recoverable_signature
sign_message(const uint8_t* msg, size_t msg_len, const uint8_t* priv_key);

//BIP3X_CORE_API bool verify_message();

/// \brief Derives eth-like address from hdkey private key
/// \return address in hex format with prefix: 0xAAABBBCCC...
BIP3X_CORE_API
std::string
get_eth_address(const bytes_32& private_key);

BIP3X_CORE_API
bytes_32
mnemonic_to_private_key(const std::string& mnemonic_words, const bip3x::derivation_path& derivation = "m/44'/0'/0'/0/0");

} // namespace bip3x
