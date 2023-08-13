#include "bip3x/bip3x_crypto.h"

#include "bip3x/bip3x_hdkey_encoder.h"

bip3x::ecdsa_recoverable_signature
bip3x::sign_message(const bytes_data& msg, const bytes_32& priv_key) {
    return sign_message(msg.data(), msg.size(), priv_key.cdata());
}

bip3x::ecdsa_recoverable_signature
bip3x::sign_message(const uint8_t* msg, size_t msg_len, const uint8_t* priv_key) {

    bytes_array<65> sig_result;
    uint8_t pby_result;

    ecdsa_sign(
        &secp256k1, HASHER_SHA3K, priv_key, msg, msg_len, sig_result.data(), &pby_result, nullptr
    );
    sig_result[64] = ((uint8_t) pby_result) + (uint8_t) 27;

    ecdsa_recoverable_signature result;
    result.r = sig_result.take_first(32);
    result.s = sig_result.take_range(32, 64);
    result.v = std::vector<uint8_t>{sig_result[64]};

    return result;
}

std::string bip3x::get_eth_address(const bip3x::bytes_32& private_key) {
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

bip3x::bytes_32 bip3x::mnemonic_to_private_key(
    const std::string& mnemonic_words, const bip3x::derivation_path& derivation
) {
    auto seed = bip3x_hdkey_encoder::make_bip39_seed(mnemonic_words);
    hdkey bip32_root_key = bip3x::bip3x_hdkey_encoder::make_bip32_root_key(seed);
    hdkey bip32_ext_key = bip32_root_key;
    bip3x::bip3x_hdkey_encoder::make_extended_key(bip32_ext_key, derivation);

    return bip32_ext_key.private_key;
}
