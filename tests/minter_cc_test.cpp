#include <bip3x/details/utils.h>
#include <cbip3x/cbip3x.h>
#include <cbip3x/cbip3x_hdkey_encoder.h>
#include <gtest/gtest.h>

void print_hdkey_data(minter_hdkey* key, std::string name) {
    std::cout << "=== Key " << name << " ===" << std::endl;
    std::cout << " pubkey: " << toolbox::data::bytes_to_hex(key->public_key.data, 33) << std::endl;
    std::cout << "privkey: " << toolbox::data::bytes_to_hex(key->private_key.data, 32) << std::endl;
    std::cout << "chain_code: " << toolbox::data::bytes_to_hex(key->chain_code.data, 32)
              << std::endl;
    std::cout << "ext_pub_key: " << toolbox::data::bytes_to_hex(key->ext_public_key.data, 112)
              << std::endl;
    std::cout << "ext_priv_key: " << toolbox::data::bytes_to_hex(key->ext_private_key.data, 112)
              << std::endl;

    std::cout << "=== END KEY " << name << " ===" << std::endl;
}

TEST(CMnemonic, PrivateKeyFromMnemonic) {

    bip3x_data64 seed;
    size_t written;
    bip3x_words_to_seed(
        "lock silly satisfy version solution bleak rain candy phone loan powder dose",
        seed.data,
        &written
    );

    std::cout << "SEED: " << toolbox::data::bytes_to_hex(seed.data, 64) << std::endl;

    minter_hdkey root_key;
    encoder_make_bip32_root_key(&seed, &root_key);
    print_hdkey_data(&root_key, "ROOT");

    encoder_extend_key(&root_key, "m/44'/60'/0'/0/0");
    print_hdkey_data(&root_key, "EXT KEY");

    bip3x_data32 privateKey = root_key.private_key;

    const char* expectedPrivateKey =
        "fd90261f5bd702ffbe7483c3b5aa7b76b1f40c1582cc6a598120b16067d3cb9a";
    uint8_t givenPrivate[32];
    memcpy(givenPrivate, privateKey.data, 32);

    ASSERT_STREQ(expectedPrivateKey, toolbox::data::bytes_to_hex(givenPrivate, 32).c_str());

    free_hdkey(&root_key);
}

TEST(CMnemonic, GetLanguages) {
    size_t n;
    char** langs = bip3x_get_languages(&n);

    ASSERT_EQ(n, 7);
    ASSERT_STREQ(langs[0], "en");
    ASSERT_STREQ(langs[1], "es");
    ASSERT_STREQ(langs[2], "fr");
    ASSERT_STREQ(langs[3], "it");
    ASSERT_STREQ(langs[4], "jp");
    ASSERT_STREQ(langs[5], "zhs");
    ASSERT_STREQ(langs[6], "zht");

    bip3x_free_string_array(langs, n);
}

TEST(CMnemonic, GetLanguageWords) {
    size_t n;
    char** words = bip3x_get_words_from_language("en", &n);

    ASSERT_EQ(2048, n);
    std::cout << "First word: " << words[0] << std::endl;
    std::cout << "Last word:  " << words[n - 1] << std::endl;
    ASSERT_STREQ(words[0], "abandon");
    ASSERT_STREQ(words[n - 1], "zoo");

    bip3x_free_string_array(words, n);
}

TEST(CMnemonic, ValidateWords) {
    const char* seed =
        "lock silly satisfy version solution bleak rain candy phone loan powder dose";
    bool res1 = bip3x_validate_words("en", seed);
    ASSERT_TRUE(res1);

    bool res2 = bip3x_validate_words("en", "wtf wtf wtf wtf wtf wtf wtf wtf wtf wtf wtf wtf");
    ASSERT_FALSE(res2);
}
