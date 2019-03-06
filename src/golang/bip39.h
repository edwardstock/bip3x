#ifndef GOLANG_BIP39_H
#define GOLANG_BIP39_H

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "bip39_config.h"

#define BIP39_ENTROPY_LEN_128 16
#define BIP39_ENTROPY_LEN_160 20
#define BIP39_ENTROPY_LEN_192 24
#define BIP39_ENTROPY_LEN_224 28
#define BIP39_ENTROPY_LEN_256 32
#define BIP39_ENTROPY_LEN_288 36
#define BIP39_ENTROPY_LEN_320 40
typedef
enum minter_mnemonic_status {
  Ok,
  UnsupportedEntropy,
  UnknownError
} minter_mnemonic_status;

typedef
struct minter_mnemonic_result {
  char **words;
  size_t len;
  char *raw;
  enum minter_mnemonic_status status;
} minter_mnemonic_result;

GOEXPORT char **minter_get_languages(size_t *num_written);
GOEXPORT void minter_free_string_array(char **langs, size_t len);
GOEXPORT char **minter_get_words_from_language(const char *lang, size_t *num_written);
/// \brief
/// \param lang
/// \param entropy use by default: BIP39_ENTROPY_LEN_128
/// \return
GOEXPORT minter_mnemonic_result *minter_generate_mnemonic(const char *lang, size_t entropy);
GOEXPORT void minter_free_mnemonic(minter_mnemonic_result *mnemonic);
GOEXPORT minter_mnemonic_result *minter_encode_bytes(const uint8_t *src, const char *lang, size_t entropy);
GOEXPORT void minter_words_to_seed(const char *words, uint8_t *out64, size_t *num_written);
GOEXPORT bool minter_validate_words(const char *lang, const char *mnemonic);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // GOLANG_BIP39_H