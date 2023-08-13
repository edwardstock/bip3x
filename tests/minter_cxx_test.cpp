#include <bip3x/bip3x_crypto.h>
#include <bip3x/bip3x_hdkey_encoder.h>
#include <bip3x/bip3x_mnemonic.h>
#include <bip3x/details/utils.h>
#include <gtest/gtest.h>
#include <iostream>

using namespace bip3x;

hdkey make_root_key(const bytes_64& seed) {
    return bip3x_hdkey_encoder::make_bip32_root_key(seed, bip3x::mainnet);
}

void extend_key(hdkey& rootHdKey) {
    bip3x_hdkey_encoder::extend_key(rootHdKey, "m/44'/60'/0'/0/0");
}

TEST(Mnemonic, PrivateKeyFromMnemonic) {
    bytes_64 seed;
    size_t written;
    bip3x_mnemonic::words_to_seed(
        "lock silly satisfy version solution bleak rain candy phone loan powder dose",
        seed.data(),
        &written
    );
    hdkey key = make_root_key(seed);
    extend_key(key);

    const char* expected_pk = "fd90261f5bd702ffbe7483c3b5aa7b76b1f40c1582cc6a598120b16067d3cb9a";

    ASSERT_STREQ(expected_pk, key.private_key.to_hex().c_str());
}

TEST(Mnemonic, Check3GeneratedMnemonicsAreDifferent) {
    bip3x::bip3x_mnemonic::mnemonic_result m1 = bip3x_mnemonic::generate();
    bip3x::bip3x_mnemonic::mnemonic_result m2 = bip3x_mnemonic::generate();
    bip3x::bip3x_mnemonic::mnemonic_result m3 = bip3x_mnemonic::generate();

    std::cout << m1.raw << std::endl;
    std::cout << m2.raw << std::endl;
    std::cout << m3.raw << std::endl;

    ASSERT_STRNE(m1.raw.c_str(), m2.raw.c_str());
    ASSERT_STRNE(m1.raw.c_str(), m3.raw.c_str());
    ASSERT_STRNE(m2.raw.c_str(), m3.raw.c_str());
}

TEST(Crypto, SigningMessage) {
    bytes_32 private_key("fae45a8d43fbea23bc6450c832f3f1ad20f9f3022b4c534e1edcfbb44fc439a3");
    bytes_data message = bytes_data::from_string_raw("Hello, Hyperlens!");

    auto result = bip3x::sign_message(message, private_key);

    ASSERT_STREQ(
        "0ae77a1bcaaf0e28519a06221bc3d74a27ffd7fb1ffca6760b84b579184f0b45",
        result.r.to_hex().c_str()
    );
    ASSERT_STREQ(
        "552c5491a59d18b81fa4882725c019a0351d0f3e03bd70d23c25390daf13e6eb",
        result.s.to_hex().c_str()
    );
    ASSERT_STREQ("1b", result.v.to_hex().c_str());
}

TEST(Crypto, GetEthAddress) {
    std::string mnemonic =
        "vague soft expose improve gaze kitten pass point select access battle wish";

    auto seed = bip3x_hdkey_encoder::make_bip39_seed(mnemonic);
    hdkey root_key = bip3x::bip3x_hdkey_encoder::make_bip32_root_key(seed);
    hdkey ext_key = bip3x::bip3x_hdkey_encoder::make_extended_key(root_key, "m/44'/60'/0'/0/0");

    auto private_key = ext_key.private_key;

    auto address = bip3x::get_eth_address(private_key);
    ASSERT_STREQ("0xa425ce86fe337ba87429f733ae3ad2606efcae20", address.c_str());
}
