/*!
 * bip39.
 * bip39.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include <minter/Bip39Mnemonic.h>
#include <cstdlib>
#include "bip39.h"

char **minter_get_languages(size_t *num_written) {
    std::vector<char *> tmp = minter::Bip39Mnemonic::getLanguages();
    *num_written = tmp.size();
    char **out = new char *[tmp.size()];
    for (size_t i = 0; i < tmp.size(); i++) {
        out[i] = new char[sizeof(tmp[i])];
        strcpy(out[i], tmp.at(i));
    }

    return out;
}
void minter_free_string_array(char **langs, size_t len) {
    for (size_t i = 0; i < len; i++) {
        memset(langs[i], 0, sizeof(char *) * strlen(langs[i]));
//        delete[] langs[i];
    }

    delete[] langs;
}

char **minter_get_words_from_language(const char *lang, size_t *num_written) {
    std::vector<const char *> tmp = minter::Bip39Mnemonic::getWordsFromLanguage(lang);
    *num_written = tmp.size();
    char **out = new char *[tmp.size()];
    for (size_t i = 0; i < tmp.size(); i++) {
        out[i] = new char[sizeof(tmp[i])];
        strcpy(out[i], tmp.at(i));
    }

    return out;
}

minter_mnemonic_result *copy_mnemonic(minter::Bip39Mnemonic::MnemonicResult &&res) {
    auto *out = new minter_mnemonic_result();
    out->len = res.len;
    out->words = new char *[res.len];
    for (size_t i = 0; i < res.len; i++) {
        out->words[i] = new char[sizeof(res.words[i]).c_str()];
        memcpy(out->words[i], res.words[i].c_str(), res.words[i].size());
    }

    switch (res.status) {
        case minter::Bip39Mnemonic::MnemonicStatus::Ok:out->status = minter_mnemonic_status::Ok;
            break;
        case minter::Bip39Mnemonic::MnemonicStatus::UnknownError:out->status = minter_mnemonic_status::UnknownError;
            break;
        case minter::Bip39Mnemonic::MnemonicStatus::UnsupportedEntropy:
            out->status = minter_mnemonic_status::UnsupportedEntropy;
            break;
    }
    out->raw = new char[res.raw.size()];
    memcpy(out->raw, res.raw.c_str(), res.raw.size());

    return out;
}

minter_mnemonic_result *minter_generate_mnemonic(const char *lang, size_t entropy) {
    return copy_mnemonic(minter::Bip39Mnemonic::generate(lang, entropy));
}

void minter_free_mnemonic(minter_mnemonic_result *mnemonic) {
    if (!mnemonic) return;

    for (size_t i = 0; i < mnemonic->len; i++) {
        delete[] mnemonic->words[i];
    }
    delete[] mnemonic->words;
    delete[] mnemonic->raw;
}
minter_mnemonic_result *minter_encode_bytes(const uint8_t *src, const char *lang, size_t entropy) {
    return copy_mnemonic(minter::Bip39Mnemonic::encodeBytes(src, lang, entropy));
}
void minter_words_to_seed(const char *words, uint8_t *out64, size_t *num_written) {
    minter::Bip39Mnemonic::wordsToSeed(words, out64, num_written);
}
bool minter_validate_words(const char *lang, const char *mnemonic) {
    return minter::Bip39Mnemonic::validateWords(lang, mnemonic);
}
