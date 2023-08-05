#ifndef GOLANG_BIP39_H
#define GOLANG_BIP39_H

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#include "cbip3x_config.h"

#define BIP39_ENTROPY_LEN_128 16
#define BIP39_ENTROPY_LEN_160 20
#define BIP39_ENTROPY_LEN_192 24
#define BIP39_ENTROPY_LEN_224 28
#define BIP39_ENTROPY_LEN_256 32
#define BIP39_ENTROPY_LEN_288 36
#define BIP39_ENTROPY_LEN_320 40

typedef enum bip3x_mnemonic_status {
    Ok,
    UnsupportedEntropy,
    UnknownError
} minter_mnemonic_status;

typedef struct bip3x_mnemonic_result {
    char** words;
    size_t len;
    char* raw;
    enum bip3x_mnemonic_status status;
} minter_mnemonic_result;

BIP3X_CORE_API char** bip3x_get_languages(size_t* num_written);
BIP3X_CORE_API void bip3x_free_string_array(char** arr, size_t len);
BIP3X_CORE_API char** bip3x_get_words_from_language(const char* lang, size_t* num_written);
/// \brief
/// \param lang
/// \param entropy use by default: BIP39_ENTROPY_LEN_128
/// \return
BIP3X_CORE_API bip3x_mnemonic_result* bip3x_generate_mnemonic(const char* lang, size_t entropy);
BIP3X_CORE_API void bip3x_free_mnemonic(bip3x_mnemonic_result* mnemonic);
BIP3X_CORE_API bip3x_mnemonic_result*
minter_encode_bytes(const uint8_t* src, const char* lang, size_t entropy);
BIP3X_CORE_API void bip3x_words_to_seed(const char* words, uint8_t* out64, size_t* num_written);
BIP3X_CORE_API bool bip3x_validate_words(const char* lang, const char* mnemonic);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // GOLANG_BIP39_H
