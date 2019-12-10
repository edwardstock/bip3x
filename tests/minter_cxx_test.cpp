/*!
 * bip39.
 * minter_mnemonic_test.cpp
 *
 * \date 2018
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include <iostream>
#include <gtest/gtest.h>
#include <minter/bip39/utils.h>
#include <minter/bip39/HDKeyEncoder.h>

using namespace minter;

HDKey makeRootKey(const Data64& seed) {
    return minter::HDKeyEncoder::makeBip32RootKey(seed, minter::MainNet);
}

void makeExtKey(HDKey& rootHdKey) {
    minter::HDKeyEncoder::makeExtendedKey(rootHdKey, "m/44'/60'/0'/0/0");
}

TEST(BIP39, PrivateKeyFromMnemonic) {
    minter::Data64 seed;
    size_t written;
    minter::Bip39Mnemonic::wordsToSeed("lock silly satisfy version solution bleak rain candy phone loan powder dose",
                                       seed.data(),
                                       &written);
    HDKey key = makeRootKey(seed);
    makeExtKey(key);

    const char* expectedPrivateKey = "fd90261f5bd702ffbe7483c3b5aa7b76b1f40c1582cc6a598120b16067d3cb9a";

    ASSERT_STREQ(expectedPrivateKey, key.privateKey.to_hex().c_str());
}

 