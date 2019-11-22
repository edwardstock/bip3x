#include <cstring>
#include <cstdlib>

char *wally_strdup(const char *str) {
    size_t len = strlen(str) + 1;
    auto newStr = (char *) malloc(len);
    if (newStr) {
        memcpy(newStr, str, len); /* Copies terminating nul */
    }

    return newStr;
}
