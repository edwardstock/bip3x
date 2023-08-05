#include "bip3x/bip3x_mnemonic.h"

#include "bip3x/bip3x_config.h"

#ifdef USE_OPENSSL_RANDOM
#include <openssl/rand.h>
#else
#include "bip3x/details/PCGRand.hpp"
#endif

#include "../details/bip39.h"
#include "../details/wordlist.h"

#include <chrono>
#include <toolbox/strings.hpp>

std::vector<std::string> bip3x::bip3x_mnemonic::get_supported_languages() {
    int sz = bip39_get_languages_size();
    if (sz <= 0) {
        return std::vector<std::string>(0);
    }

    std::vector<std::string> languages(static_cast<size_t>(sz));
    bip39_get_languages(languages);

    return languages;
}
std::vector<std::string> bip3x::bip3x_mnemonic::get_words_for_language(const char* lang) {
    words* wl[1];
    bip39_get_wordlist(lang, wl);
    if (!wl[0]) {
        return {};
    }

    std::vector<std::string> wordsList(wl[0]->len);
    for (size_t i = 0; i < wordsList.size(); i++) {
        wordsList[i] = std::string(wl[0]->indices[i]);
    }

    return wordsList;
}

bip3x::bip3x_mnemonic::mnemonic_result
bip3x::bip3x_mnemonic::generate(const char* lang, size_t entropy) {
#ifdef USE_OPENSSL_RANDOM
    bytes_data bts(entropy);
    RAND_bytes(bts.data(), (int) entropy);

    return encodeBytes(bts.cdata(), lang, entropy);
#else
    std::uniform_int_distribution<> udist(0, 255);

#ifdef __MINGW32__
    auto duration = std::chrono::high_resolution_clock::now().time_since_epoch();
    static std::mt19937 rand(std::chrono::duration_cast<std::chrono::microseconds>(duration).count()
    );
#else
    static std::random_device dev;
    static PCGRand rand(dev);
#endif

    bytes_data bts(entropy);
    for (size_t i = 0; i < entropy; ++i) {
        bts.write(i, (uint8_t) udist(rand));
    }

    return encode_bytes(bts.cdata(), lang, entropy);

#endif
}

bip3x::bip3x_mnemonic::mnemonic_result
bip3x::bip3x_mnemonic::encode_bytes(const uint8_t* src, const char* lang, size_t entropy) {
    mnemonic_result result{ok, std::vector<std::string>(0), "", 0};

    if (!validate_entropy(entropy)) {
        result.status = unsupported_entropy;
        return result;
    }

    struct words* wordList[1];
    bip39_get_wordlist(lang, wordList);

    char* output[1];
    bool encRes = bip39_mnemonic_from_bytes(wordList[0], src, entropy, output) == BIP3X_OK;

    if (!encRes) {
        result.status = unknown_error;
        return result;
    }

    result.words = toolbox::strings::split(output[0], " ");
    result.len = result.words.size();
    result.raw = std::string(output[0]);

    free(output[0]);

    return result;
}
bip3x::bytes_data
bip3x::bip3x_mnemonic::decode_mnemonic(const char* mnemonic, const char* lang, size_t entropy) {
    struct words* wordList[1];
    bip39_get_wordlist(lang, wordList);

    if (!validate_entropy(entropy)) {
        return nullptr;
    }

    bytes_data data;
    data.resize(entropy);

    size_t written = 0;

    bool decRes =
        bip39_mnemonic_to_bytes(wordList[0], mnemonic, data.data(), entropy, &written) == BIP3X_OK;
    if (decRes) {
        return data;
    }

    return nullptr;
}
void bip3x::bip3x_mnemonic::words_to_seed(const char* mnemonic, uint8_t* out64, size_t* writtenSz) {
    bip39_mnemonic_to_seed(mnemonic, nullptr, out64, 64, writtenSz);
}
bool bip3x::bip3x_mnemonic::validate_entropy(size_t entropy) {
    switch (entropy) {
        case BIP39_ENTROPY_LEN_128:
        case BIP39_ENTROPY_LEN_160:
        case BIP39_ENTROPY_LEN_192:
        case BIP39_ENTROPY_LEN_224:
        case BIP39_ENTROPY_LEN_256:
        case BIP39_ENTROPY_LEN_288:
        case BIP39_ENTROPY_LEN_320: return true;
        default: break;
    }

    return false;
}
bool bip3x::bip3x_mnemonic::validate_words(const char* lang, const char* mnemonic) {
    words* wl[1];
    bip39_get_wordlist(lang, wl);
    if (!wl[0]) {
        return false;
    }

    return bip39_mnemonic_validate(wl[0], mnemonic) == BIP3X_OK;
}
