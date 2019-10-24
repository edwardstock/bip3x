#ifndef BIP39_CORE_BIP39_H
#define BIP39_CORE_BIP39_H

#include "bip39_core.h"
#include "wordlist.h"

#ifdef __cplusplus
extern "C" {
#endif

//struct words;

/** Valid entropy lengths */
#define BIP39_ENTROPY_LEN_128 16
#define BIP39_ENTROPY_LEN_160 20
#define BIP39_ENTROPY_LEN_192 24
#define BIP39_ENTROPY_LEN_224 28
#define BIP39_ENTROPY_LEN_256 32
#define BIP39_ENTROPY_LEN_288 36
#define BIP39_ENTROPY_LEN_320 40

/** The required size of the output buffer for `bip39_mnemonic_to_seed` */
#define BIP39_SEED_LEN_512 64

/** The number of words in a BIP39 compliant wordlist */
#define BIP39_WORDLIST_LEN 2048

/**
 * Get the list of default supported languages.
 *
 * ..note:: The string returned should be freed using `wally_free_string`.
 */
BIP39_CORE_API int bip39_get_languages(std::vector<std::string> &output);

/**
 * Get the default word list for a language.
 *
 * :param lang: Language to use. Pass NULL to use the default English list.
 * :param output: Destination for the resulting word list.
 *
 * .. note:: The returned structure should not be freed or modified.
 */
BIP39_CORE_API int bip39_get_wordlist(
    const char *lang,
    struct words **output);

/**
 * \brief Get length o languages
 * \return
 */
BIP39_CORE_API int bip39_get_languages_size();

/**
 * Get the 'index'th word from a word list.
 *
 * :param w: Word list to use. Pass NULL to use the default English list.
 * :param index: The 0-based index of the word in ``w``.
 * :param output: Destination for the resulting word.
 *
 * The string returned should be freed using `wally_free_string`.
 */
BIP39_CORE_API int bip39_get_word(
    const struct words *w,
    size_t index,
    std::string &output);

/**
 * Generate a mnemonic sentence from the entropy in ``bytes``.
 *
 * :param w: Word list to use. Pass NULL to use the default English list.
 * :param bytes: Entropy to convert.
 * :param bytes_len: The length of ``bytes`` in bytes.
 * :param output: Destination for the resulting mnemonic sentence.
 *
 * .. note:: The string returned should be freed using `wally_free_string`.
 */
BIP39_CORE_API int bip39_mnemonic_from_bytes(
    const struct words *w,
    const unsigned char *bytes,
    size_t bytes_len,
    char **output);

size_t bip39_checksum(const unsigned char *bytes, size_t bytes_len, size_t mask);

/**
 * Convert a mnemonic sentence into entropy at ``bytes_out``.
 *
 * :param w: Word list to use. Pass NULL to use the default English list.
 * :param mnemonic: Mnemonic to convert.
 * :param bytes_out: Where to store the resulting entropy.
 * :param len: The length of ``bytes_out`` in bytes.
 * :param written: Destination for the number of bytes written to ``bytes_out``.
 */
BIP39_CORE_API int bip39_mnemonic_to_bytes(
    const struct words *w,
    const char *mnemonic,
    unsigned char *bytes_out,
    size_t len,
    size_t *written);

/**
 * Validate the checksum embedded in a mnemonic sentence.
 *
 * :param w: Word list to use. Pass NULL to use the default English list.
 * :param mnemonic: Mnemonic to validate.
 */
BIP39_CORE_API int bip39_mnemonic_validate(
    const struct words *w,
    const char *mnemonic);

/**
 * Convert a mnemonic into a binary seed.
 *
 * :param mnemonic: Mnemonic to convert.
 * :param password: Mnemonic password or NULL if no password is needed.
 * :param bytes_out: The destination for the binary seed.
 * :param len: The length of ``bytes_out`` in bytes. Currently This must
 *|      be ``BIP39_SEED_LEN_512``.
 * :param written: Destination for the number of bytes written to ``bytes_out``.
 */
BIP39_CORE_API int bip39_mnemonic_to_seed(
    const char *mnemonic,
    const char *password,
    unsigned char *bytes_out,
    size_t len,
    size_t *written);

#ifdef __cplusplus
}
#endif

#endif /* BIP39_CORE_BIP39_H */
