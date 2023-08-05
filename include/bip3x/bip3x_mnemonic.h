#pragma once

#include "bip3x/bip3x_config.h"
#include "bip3x/details/utils.h"

#include <iostream>
#include <random>
#include <vector>

namespace bip3x {

class BIP3X_CORE_API bip3x_mnemonic {
public:
    enum mnemonic_status { ok, unsupported_entropy, unknown_error };

    struct mnemonic_result {
        mnemonic_status status;
        std::vector<std::string> words;
        std::string raw;
        size_t len;
    };

    static std::vector<std::string> get_supported_languages();
    static std::vector<std::string> get_words_for_language(const char* lang);
    static mnemonic_result generate(const char* lang = "en", size_t entropy = BIP3X_ENTROPY_LEN_128);
    static mnemonic_result
    encode_bytes(const uint8_t* src, const char* lang = "en", size_t entropy = BIP3X_ENTROPY_LEN_128);
    static bytes_data decode_mnemonic(const char* mnemonic, const char* lang = "en", size_t entropy = BIP3X_ENTROPY_LEN_128);
    static void words_to_seed(const char* mnemonic, uint8_t* out64, size_t* writtenSz);
    static bool validate_words(const char* lang, const char* mnemonic);

private:
    static bool validate_entropy(size_t entropy);
};

} // namespace bip3x
