/*!
 * bip39.
 * minter_go_mnemonic_test.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include <gtest/gtest.h>
#include <cbip39.h>
#include <cbip39_hdkey_encoder.h>
#include <minter/bip39/utils.h>

void print_hdkey_data(minter_hdkey *key, std::string name) {
    std::cout << "=== Key " << name << " ===" << std::endl;
    std::cout << " pubkey: " << toolboxpp::data::bytesToHex(key->public_key.data, 33) << std::endl;
    std::cout << "privkey: " << toolboxpp::data::bytesToHex(key->private_key.data, 32) << std::endl;
    std::cout << "chain_code: " << toolboxpp::data::bytesToHex(key->chain_code.data, 32) << std::endl;
    std::cout << "ext_pub_key: " << toolboxpp::data::bytesToHex(key->ext_public_key.data, 112) << std::endl;
    std::cout << "ext_priv_key: " << toolboxpp::data::bytesToHex(key->ext_private_key.data, 112) << std::endl;

    std::cout << "=== END KEY " << name << " ===" << std::endl;
}

TEST(CMinter, PrivateKeyFromMnemonic) {

    minter_data64 seed;
    size_t written;
    minter_words_to_seed("lock silly satisfy version solution bleak rain candy phone loan powder dose",
                         seed.data,
                         &written);

    std::cout << "SEED: " << toolboxpp::data::bytesToHex(seed.data, 64) << std::endl;

    minter_hdkey hdkey;
    encoder_make_bip32_root_key(&seed, &hdkey);
    print_hdkey_data(&hdkey, "ROOT");

    encoder_make_ext_key(&hdkey, "m/44'/60'/0'/0/0");
    print_hdkey_data(&hdkey, "EXT KEY");

    minter_data32 privateKey = hdkey.private_key;

    const char *expectedPrivateKey = "fd90261f5bd702ffbe7483c3b5aa7b76b1f40c1582cc6a598120b16067d3cb9a";
    uint8_t givenPrivate[32];
    memcpy(givenPrivate, privateKey.data, 32);

    ASSERT_STREQ(expectedPrivateKey, toolboxpp::data::bytesToHex(givenPrivate, 32).c_str());

    free_hdkey(&hdkey);
}

TEST(CMinter, GetLanguages) {
    size_t n;
    char **langs = minter_get_languages(&n);

    ASSERT_EQ(n, 7);
    ASSERT_STREQ(langs[0], "en");
    ASSERT_STREQ(langs[1], "es");
    ASSERT_STREQ(langs[2], "fr");
    ASSERT_STREQ(langs[3], "it");
    ASSERT_STREQ(langs[4], "jp");
    ASSERT_STREQ(langs[5], "zhs");
    ASSERT_STREQ(langs[6], "zht");

    minter_free_string_array(langs, n);
}

TEST(CMinter, GetLanguageWords) {
    size_t n;
    char **words = minter_get_words_from_language("en", &n);

    ASSERT_EQ(2048, n);
    std::cout << "First word: " << words[0] << std::endl;
    std::cout << "Last word:  " << words[n-1] << std::endl;
    ASSERT_STREQ(words[0], "abandon");
    ASSERT_STREQ(words[n - 1], "zoo");

    minter_free_string_array(words, n);
}

TEST(CMinter, ValidateWords) {
    const char *seed = "lock silly satisfy version solution bleak rain candy phone loan powder dose";
    bool res1 = minter_validate_words("en", seed);
    ASSERT_TRUE(res1);

    bool res2 = minter_validate_words("en", "wtf wtf wtf wtf wtf wtf wtf wtf wtf wtf wtf wtf");
    ASSERT_FALSE(res2);
}
 