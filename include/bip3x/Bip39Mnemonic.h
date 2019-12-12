/*! 
 * bip39. 2018
 * 
 * \author Eduard Maximovich <edward.vstock@gmail.com>
 * \link https://github.com/edwardstock
 */

#ifndef BIP39_BIP32MNEMONIC_H
#define BIP39_BIP32MNEMONIC_H

#include "bip39.h"
#include "bip39_core.h"
#include "utils.h"
#include "wordlist.h"

#include <iostream>
#include <random>
#include <vector>

namespace bip3x {

class BIP39_CORE_API Bip39Mnemonic {
public:
    enum MnemonicStatus {
        Ok,
        UnsupportedEntropy,
        UnknownError
    };

    struct MnemonicResult {
        MnemonicStatus status;
        std::vector<std::string> words;
        std::string raw;
        size_t len;
    };

    static std::vector<std::string> getLanguages();
    static std::vector<std::string> getWordsFromLanguage(const char* lang);
    static MnemonicResult generate(const char* lang = "en", size_t entropy = BIP39_ENTROPY_LEN_128);
    static MnemonicResult encodeBytes(const uint8_t* src,
                                      const char* lang = "en",
                                      size_t entropy = BIP39_ENTROPY_LEN_128);
    static bytes_data decodeMnemonic(const char* mnemonic, const char* lang, size_t entropy = BIP39_ENTROPY_LEN_128);
    static void wordsToSeed(const char* words, uint8_t* out64, size_t* writtenSz);
    static bool validateWords(const char* lang, const char* mnemonic);

private:
    static bool validateEntropy(size_t entropy);
};

} // namespace minter

#endif //BIP39_BIP32MNEMONIC_H
