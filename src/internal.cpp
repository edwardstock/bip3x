#include <cstring>
#include <trezor-crypto/pbkdf2.hpp>
#include <trezor-crypto/ripemd160.h>
#include "bip39/internal.h"
#include "bip39/crypto/sha256.h"
#include "bip39/crypto/sha512.h"

char *wally_strdup(const char *str) {
    size_t len = strlen(str) + 1;
    auto newStr = (char *) malloc(len);
    if (newStr) {
        memcpy(newStr, str, len); /* Copies terminating nul */
    }

    return newStr;
}
