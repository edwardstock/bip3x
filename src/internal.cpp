#include <cstring>
#include "internal.h"
#include "pbkdf2.hpp"
#include "crypto/sha256.h"
#include "crypto/sha512.h"
#include "ripemd160.h"

char *wally_strdup(const char *str) {
    size_t len = strlen(str) + 1;
    auto newStr = (char *) malloc(len);
    if (newStr) {
        memcpy(newStr, str, len); /* Copies terminating nul */
    }

    return newStr;
}
