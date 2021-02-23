/*!
 * bip39.
 * minter_mnemonic_test.cpp
 *
 * \date 2018
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include <bip3x/HDKeyEncoder.h>
#include <bip3x/Bip39Mnemonic.h>
#include <bip3x/utils.h>
#include <gtest/gtest.h>
#include <iostream>

using namespace bip3x;

HDKey makeRootKey(const bytes_64& seed) {
    return HDKeyEncoder::makeBip32RootKey(seed, bip3x::MainNet);
}

void makeExtKey(HDKey& rootHdKey) {
    HDKeyEncoder::makeExtendedKey(rootHdKey, "m/44'/60'/0'/0/0");
}

TEST(BIP39, PrivateKeyFromMnemonic) {
    bytes_64 seed;
    size_t written;
    Bip39Mnemonic::wordsToSeed("lock silly satisfy version solution bleak rain candy phone loan powder dose",
                               seed.data(),
                               &written);
    HDKey key = makeRootKey(seed);
    makeExtKey(key);

    const char* expectedPrivateKey = "fd90261f5bd702ffbe7483c3b5aa7b76b1f40c1582cc6a598120b16067d3cb9a";

    ASSERT_STREQ(expectedPrivateKey, key.privateKey.to_hex().c_str());
}


TEST(BIP39, GenerateMnemonics) {
    bip3x::Bip39Mnemonic::MnemonicResult m1 = Bip39Mnemonic::generate();
    bip3x::Bip39Mnemonic::MnemonicResult m2 = Bip39Mnemonic::generate();
    bip3x::Bip39Mnemonic::MnemonicResult m3 = Bip39Mnemonic::generate();

    std::cout << m1.raw << std::endl;
    std::cout << m2.raw << std::endl;
    std::cout << m3.raw << std::endl;

    ASSERT_STRNE(m1.raw.c_str(), m2.raw.c_str());
    ASSERT_STRNE(m1.raw.c_str(), m3.raw.c_str());
    ASSERT_STRNE(m2.raw.c_str(), m3.raw.c_str());
}
