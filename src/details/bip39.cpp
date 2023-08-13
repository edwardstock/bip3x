#include "bip39.h"

#include "bip3x/crypto/sha256.h"
#include "mnemonic.h"
#include "wordlist.h"

#include <bip3x/crypto/hmac.h>
#include <bip3x/crypto/pbkdf2.hpp>
#include <cstring>

/* Maximum length including up to 2 bytes for checksum */
#define BIP39_ENTROPY_LEN_MAX (BIP39_ENTROPY_LEN_320 + sizeof(unsigned char) * 2)

static const struct {
    const char name[4];
    const struct words* words;
} lookup[] = {
    {"en", &en_words},
    {"es", &es_words},
    {"fr",  &fr_words},
    {"it",  &it_words},
    {"jp",  &jp_words},
    {"zhs", &zhs_words},
    {"zht", &zht_words},
    /* FIXME: Should 'zh' map to traditional or simplified? */
};

int bip39_get_languages(std::vector<std::string> &output) {
    auto len = (size_t) bip39_get_languages_size();
    for (size_t i = 0; i < len; i++) {
        output[i] = std::string(lookup[i].name);
    }

    return len;
}

int bip39_get_languages_size() {
    return sizeof(lookup) / sizeof(lookup[0]);
}

int bip39_get_wordlist(const char *lang, struct words **output) {
    size_t i;

    if (!output)
        return BIP3X_EINVAL;

    *output = (struct words *) &en_words; /* Fallback to English if not found */

    if (lang)
        for (i = 0; i < (size_t) bip39_get_languages_size(); ++i)
            if (!strcmp(lang, lookup[i].name)) {
                *output = (struct words *) lookup[i].words;
                break;
            }
    return BIP3X_OK;
}

int bip39_get_word(const struct words *w, size_t idx, std::string &output) {
    const char *word;

    w = w ? w : &en_words;

    if (!(word = wordlist_lookup_index(w, idx)))
        return BIP3X_EINVAL;

    output.assign(word);
    return output.length() ? BIP3X_OK : BIP3X_ENOMEM;
}

/* Convert an input entropy length to a mask for checksum bits. As it
 * returns 0 for bad lengths, it serves as a validation function too.
 */
static size_t len_to_mask(size_t len) {
    switch (len) {
        case BIP39_ENTROPY_LEN_128: return 0xf0;
        case BIP39_ENTROPY_LEN_160: return 0xf8;
        case BIP39_ENTROPY_LEN_192: return 0xfc;
        case BIP39_ENTROPY_LEN_224: return 0xfe;
        case BIP39_ENTROPY_LEN_256: return 0xff;
        case BIP39_ENTROPY_LEN_288: return 0x80ff;
        case BIP39_ENTROPY_LEN_320: return 0xC0ff;
        default:return 0;
    }
}

size_t bip39_checksum(const uint8_t *bytes, size_t bytes_len, size_t mask) {

    uint8_t out[CSHA256::OUTPUT_SIZE];
    trezor::SHA256_CTX ctx;
    sha256_Init(&ctx);
    sha256_Update(&ctx, bytes, bytes_len);
    sha256_Final(&ctx, out);

    size_t ret = out[0] | (out[1] << 8u);
    return ret & mask;
}

int bip39_mnemonic_from_bytes(const struct words *w,
                              const unsigned char *bytes, size_t bytes_len,
                              char **output) {
    unsigned char tmp_bytes[BIP39_ENTROPY_LEN_MAX];
    size_t checksum, mask;

    if (output)
        *output = nullptr;

    if (!bytes || !bytes_len || !output)
        return BIP3X_EINVAL;

    w = w ? w : &en_words;

    if (w->bits != 11u || !(mask = len_to_mask(bytes_len)))
        return BIP3X_EINVAL;

    memcpy(tmp_bytes, bytes, bytes_len);
    checksum = bip39_checksum(bytes, bytes_len, mask);
    tmp_bytes[bytes_len] = (uint8_t) (checksum & 0xFFu);
    if (mask > 0xFFu) {
        tmp_bytes[++bytes_len] = (uint8_t) ((checksum >> 8u) & 0xFFu);
    }

    *output = mnemonic_from_bytes(w, tmp_bytes, bytes_len + 1);
    bzero(tmp_bytes, sizeof(tmp_bytes));
    return *output ? BIP3X_OK : BIP3X_ENOMEM;
}

static bool checksum_ok(const uint8_t *bytes, size_t idx, size_t mask) {
    /* The checksum is stored after the data to sum */
    size_t calculated = bip39_checksum(bytes, idx, mask);
    size_t stored = bytes[idx];
    if (mask > 0xffu)
        stored |= (bytes[idx + 1u] << 8u);
    return (stored & mask) == calculated;
}

int bip39_mnemonic_to_bytes(const struct words *w, const char *mnemonic,
                            uint8_t *bytes_out, size_t len,
                            size_t *written) {

    std::vector<uint8_t> tmp_bytes(BIP39_ENTROPY_LEN_MAX);
    size_t mask, tmp_len;
    int ret;

    /* Ideally we would infer the wordlist here. Unfortunately this cannot
     * work reliably because the default word lists overlap. In combination
     * with being sorted lexographically, this means the default lists
     * were poorly chosen. But we are stuck with them now.
     *
     * If the caller doesn't know which word list to use, they should iterate
     * over the available ones and try any resulting list that the mnemonic
     * validates against.
     */
    w = w ? w : &en_words;

    if (written) {
        *written = 0;
    }

    if (w->bits != 11u || !mnemonic || !bytes_out) {
        return BIP3X_EINVAL;
    }

    ret = mnemonic_to_bytes(w, mnemonic, &tmp_bytes[0], tmp_bytes.size(), &tmp_len);

    if (!ret) {
        /* Remove checksum bytes from the output length */
        --tmp_len;
        if (tmp_len > BIP39_ENTROPY_LEN_256) {
            --tmp_len; /* Second byte required */
        }

        if (tmp_len > tmp_bytes.size()) {
            ret = BIP3X_EINVAL; /* Too big for biggest supported entropy */
        } else {
            if (tmp_len <= len) {
                if (!(mask = len_to_mask(tmp_len)) ||
                    !checksum_ok(&tmp_bytes[0], tmp_len, mask)) {
                    tmp_len = 0;
                    ret = BIP3X_EINVAL; /* Bad checksum */
                } else {
                    memcpy(bytes_out, &tmp_bytes[0], tmp_len);
                }
            }
        }
    }

    std::fill(tmp_bytes.begin(), tmp_bytes.end(), 0);
    if (!ret && written) {
        *written = tmp_len;
    }

    return ret;
}

int bip39_mnemonic_validate(const struct words *w, const char *mnemonic) {
    unsigned char buf[BIP39_ENTROPY_LEN_MAX];
    size_t len;
    int ret = bip39_mnemonic_to_bytes(w, mnemonic, buf, sizeof(buf), &len);
    bzero(buf, sizeof(buf));
    return ret;
}

int bip39_mnemonic_to_seed(const char *mnemonic, const char *password,
                           unsigned char *bytes_out, size_t len,
                           size_t *written) {

    const size_t bip9_cost = 2048u;
    const char *prefix = "mnemonic";
    const size_t prefix_len = strlen(prefix);
    const size_t password_len = password ? strlen(password) : 0;
    const size_t salt_len = prefix_len + password_len;
    unsigned char *salt;

    if (written) {
        *written = 0;
    }

    if (!mnemonic || !bytes_out || len != BIP39_SEED_LEN_512)
        return BIP3X_EINVAL;

    salt = (uint8_t *) malloc(salt_len);
    if (!salt)
        return BIP3X_ENOMEM;

    memcpy(salt, prefix, prefix_len);
    if (password_len) {
        memcpy(salt + prefix_len, password, password_len);
    }

    pbkdf2_hmac_sha512(reinterpret_cast<const uint8_t *>(mnemonic),
                       strlen(mnemonic),
                       salt,
                       salt_len,
                       bip9_cost,
                       bytes_out);

    if (written)
        *written = BIP39_SEED_LEN_512; /* Succeeded */

    bzero(salt, salt_len);
    free(salt);

    return 1;
}
