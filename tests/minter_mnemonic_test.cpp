/*!
 * bip39.
 * minter_mnemonic_test.cpp
 *
 * \date 2018
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include <iostream>
#include "gtest/gtest.h"
#include "minter/utils.h"
#include "minter/HDKeyEncoder.h"

using namespace minter;

HDKey makeRootKey(const Data64 &seed) {
    return minter::HDKeyEncoder::makeBip32RootKey(seed, minter::HDKeyEncoder::MainNet);
}

HDKey makeExtKey(const HDKey &rootHdKey) {
    return minter::HDKeyEncoder::makeExtendedKey(rootHdKey, "m/44'/60'/0'/0/0");
}

TEST(Minter, PrivateKeyFromMnemonic) {

    minter::Data64 seed;
    size_t written;
    minter::Bip39Mnemonic::wordsToSeed("lock silly satisfy version solution bleak rain candy phone loan powder dose",
                                       seed.data(),
                                       &written);
    HDKey rootKey = makeRootKey(seed);
    HDKey extKey = makeExtKey(rootKey);
    Data32 privateKey = extKey.privateKey;

    const char *expectedPrivateKey = "fd90261f5bd702ffbe7483c3b5aa7b76b1f40c1582cc6a598120b16067d3cb9a";

    ASSERT_STREQ(expectedPrivateKey, privateKey.toHex().c_str());
}

TEST(Minter, StringCopyToCharPtr) {
    std::string ascii = "hello world";
    char* asciiPtr = new char[ascii.size()+1];
    ascii.copy(asciiPtr, ascii.size());
    asciiPtr[ascii.size()] = '\0';

    std::string unicoded = "константная юникодная строка";
    char* unicodePtr = new char[unicoded.size()];
    unicoded.copy(unicodePtr, unicoded.size());
    unicodePtr[unicoded.size()] = '\0';

    ASSERT_STREQ("hello world", asciiPtr);
    ASSERT_STREQ("константная юникодная строка", unicodePtr);


}

 