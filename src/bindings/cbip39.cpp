/*!
 * bip39.
 * bip39.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include "cbip39.h"

#include <bip3x/Bip39Mnemonic.h>
#include <cstdlib>

char** bip3x_get_languages(size_t* num_written) {
    std::vector<std::string> tmp = bip3x::Bip39Mnemonic::getLanguages();
    *num_written = tmp.size();
    char** out = new char*[tmp.size()];
    for (size_t i = 0; i < tmp.size(); i++) {
        const size_t sz = tmp[i].size();
        out[i] = new char[sz + 1];
        tmp.at(i).copy(out[i], sz);
        out[i][sz] = '\0';
    }

    return out;
}
void bip3x_free_string_array(char** arr, size_t len) {
    if (!arr) {
        //nullptr
        return;
    }

    for (size_t i = 0; i < len; i++) {
        if (arr[i]) {
            memset(arr[i], 0, sizeof(char) * strlen(arr[i]));
            delete[] arr[i];
        }
    }

    delete[] arr;
}

char** bip3x_get_words_from_language(const char* lang, size_t* num_written) {
    std::vector<std::string> tmp = bip3x::Bip39Mnemonic::getWordsFromLanguage(lang);
    *num_written = tmp.size();
    char** out = new char*[tmp.size()];
    for (size_t i = 0; i < tmp.size(); i++) {
        const size_t sz = tmp[i].size();
        out[i] = new char[sz + 1];
        tmp.at(i).copy(out[i], sz);
        out[i][sz] = '\0';
    }

    return out;
}

bip3x_mnemonic_result* copy_mnemonic(bip3x::Bip39Mnemonic::MnemonicResult&& res) {
    auto* out = new bip3x_mnemonic_result();
    out->len = res.len;
    out->words = new char*[res.len];
    for (size_t i = 0; i < res.len; i++) {
        out->words[i] = new char[res.words[i].size() + 1];
        res.words[i].copy(out->words[i], res.words[i].size());
        out->words[i][res.words[i].size()] = '\0';
    }

    switch (res.status) {
    case bip3x::Bip39Mnemonic::MnemonicStatus::Ok:
        out->status = bip3x_mnemonic_status::Ok;
        break;
    case bip3x::Bip39Mnemonic::MnemonicStatus::UnknownError:
        out->status = bip3x_mnemonic_status::UnknownError;
        break;
    case bip3x::Bip39Mnemonic::MnemonicStatus::UnsupportedEntropy:
        out->status = bip3x_mnemonic_status::UnsupportedEntropy;
        break;
    }

    out->raw = new char[res.raw.size()+1];
    res.raw.copy(out->raw, res.raw.size());
    out->raw[res.raw.size()] = '\0';

    return out;
}

bip3x_mnemonic_result* bip3x_generate_mnemonic(const char* lang, size_t entropy) {
    return copy_mnemonic(bip3x::Bip39Mnemonic::generate(lang, entropy));
}

void bip3x_free_mnemonic(bip3x_mnemonic_result* mnemonic) {
    if (!mnemonic)
        return;

    for (size_t i = 0; i < mnemonic->len; i++) {
        delete[] mnemonic->words[i];
    }
    delete[] mnemonic->words;
    delete[] mnemonic->raw;
}
bip3x_mnemonic_result* minter_encode_bytes(const uint8_t* src, const char* lang, size_t entropy) {
    return copy_mnemonic(bip3x::Bip39Mnemonic::encodeBytes(src, lang, entropy));
}
void bip3x_words_to_seed(const char* words, uint8_t* out64, size_t* num_written) {
    bip3x::Bip39Mnemonic::wordsToSeed(words, out64, num_written);
}
bool bip3x_validate_words(const char* lang, const char* mnemonic) {
    return bip3x::Bip39Mnemonic::validateWords(lang, mnemonic);
}
