#include "bip3x/bip3x_crypto.h"
#include "bip3x/bip3x_hdkey_encoder.h"
#include "bip3x/bip3x_mnemonic.h"
#include "bip3x/crypto/secp256k1.h"
#include "bip3x/crypto/sha3.h"
#include "bip3x/details/utils.h"

#include <iomanip>
#include <iostream>
using namespace bip3x;

int main() {
    //vague soft expose improve gaze kitten pass point select access battle wish
    bytes_64 entropy("f0b9c942b9060af6a82d3ac340284d7e");
    bip3x_mnemonic::mnemonic_result encoded_mnemonic =
        bip3x_mnemonic::encode_bytes(entropy.data(), "en", BIP3X_ENTROPY_LEN_128);

    // Generate
    // bip3x::bip3x_mnemonic::mnemonic_result encoded_mnemonic = bip3x::bip3x_mnemonic::generate();

    // Or set manually
    /*
    bip3x::bip3x_mnemonic::mnemonic_result encoded_mnemonic;
    encoded_mnemonic.raw = "crystal enable mask thought ask upgrade custom broccoli green zero wrestle cloth";
    encoded_mnemonic.words = {"crystal", "enable", "mask", "thought","ask", "upgrade", "custom", "broccoli",
    "green","zero", "wrestle", "cloth"}; encoded_mnemonic.len = 12; encoded_mnemonic.status =
    bip3x::bip3x_mnemonic::mnemonic_status::ok;
     */

    // create seed from mnemonic phrase vector
    bytes_64 seed = bip3x_hdkey_encoder::make_bip39_seed(encoded_mnemonic.words);

    // or from raw string
    /*
    bytes_64 seed = bip3x_hdkey_encoder::make_bip39_seed("vague soft expose improve gaze kitten pass point select access
    battle wish");
     */

    // make root key from seed, where seed is a 64 bytes array, not a mnemonic phrase
    hdkey bip32_root_key = bip3x::bip3x_hdkey_encoder::make_bip32_root_key(seed);

    // copy root key
    hdkey bip32_ext_key = bip32_root_key;
    // modify it
    bip3x::bip3x_hdkey_encoder::extend_key(bip32_ext_key, "m/44'/60'/0'/0/0");

    // copy root key
    hdkey bip44_ext_key = bip32_root_key;
    // make it bip44
    bip3x_hdkey_encoder::make_extended_key(bip44_ext_key, "m/44'/60'/0'");

    std::cout << "Mnemonic words count:    " << encoded_mnemonic.len << std::endl;
    std::cout << "Mnemonic words:          " << encoded_mnemonic.raw << std::endl;
    std::cout << "Seed:                    " << seed.to_hex() << std::endl;
    std::cout << "Bip32 root key:          " << bip32_root_key.ext_private_key.to_string() << std::endl;
    std::cout << std::endl;
    std::cout << "Bip32 extended priv key: " << bip32_ext_key.ext_private_key.to_string() << std::endl;
    std::cout << "Bip32 extended pub key:  " << bip32_ext_key.ext_public_key.to_string() << std::endl;
    std::cout << "Bip32 priv key:          " << bip32_ext_key.private_key.to_hex() << std::endl;
    std::cout << "Bip32 pub key:           " << bip32_ext_key.public_key.to_hex() << std::endl;
    std::cout << "ETH-like address:        " << bip3x::get_eth_address(bip32_ext_key.private_key) << std::endl;
    std::cout << std::endl;
    std::cout << "Bip44 priv key:          " << bip44_ext_key.private_key.to_hex() << std::endl;
    std::cout << "Bip44 pub key:           " << bip44_ext_key.public_key.to_hex() << std::endl;
    std::cout << "Bip44 extended priv key: " << bip44_ext_key.ext_private_key.to_string() << std::endl;
    std::cout << "Bip44 extended pub key:  " << bip44_ext_key.ext_public_key.to_string() << std::endl;


    bytes_32 secret("fae45a8d43fbea23bc6450c832f3f1ad20f9f3022b4c534e1edcfbb44fc439a3");
    bytes_data message = bytes_data::from_string_raw("Hello, Hyperlens!");

    auto sig_result = bip3x::sign_message(message.data(), message.size(), secret.data());

    // r: 0ae77a1bcaaf0e28519a06221bc3d74a27ffd7fb1ffca6760b84b579184f0b45
    // s: 552c5491a59d18b81fa4882725c019a0351d0f3e03bd70d23c25390daf13e6eb
    // v: 1b
    std::cout << "r: " << sig_result.r.to_hex() << std::endl;
    std::cout << "s: " << sig_result.s.to_hex() << std::endl;
    std::cout << "v: " << sig_result.v.to_hex() << std::endl;

    bip32_root_key.clear();
    bip32_ext_key.clear();
    bip44_ext_key.clear();

    return 0;
}
