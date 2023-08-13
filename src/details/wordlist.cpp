#include "wordlist.h"

#include <cstdlib>
#include <cstring>
#include <vector>

static char* wally_strdup(const char* str) {
    size_t len = strlen(str) + 1;
    auto newStr = (char*) malloc(len);
    if (newStr) {
        memcpy(newStr, str, len); /* Copies terminating nul */
    }

    return newStr;
}

/* https://graphics.stanford.edu/~seander/bithacks.html#IntegerLogObvious */
static int get_bits(size_t n) {
    size_t bits = 0;
    while (n >>= 1u) {
        ++bits;
    }
    return bits;
}

/* Allocate a new words structure */
static struct words *wordlist_alloc(const char *words, size_t len) {
    auto w = (struct words *) malloc(sizeof(struct words));
    if (w) {
        w->str = wally_strdup(words);
        if (w->str) {
            w->str_len = strlen(w->str);
            w->len = len;
            w->bits = (size_t) get_bits(len);
            w->indices = (const char **) (malloc(len * sizeof(const char *)));
            if (w->indices)
                return w;
        }
        w->str_len = 0;
        wordlist_free(w);
    }
    return nullptr;
}

static size_t wordlist_count(const char *words) {
    size_t len = 1u; /* Always 1 less separator than words, so start from 1 */
    while (*words)
        len += *words++ == ' '; /* FIXME: utf-8 sep */
    return len;
}

words *wordlist_init(const char *words) {
    size_t i, len = wordlist_count(words);
    struct words *w = wordlist_alloc(words, len);

    if (w) {
        /* Tokenise the strings into w->indices */
        const char *p = w->str;
        for (len = 0; len < w->len; ++len) {
            w->indices[len] = p;
            while (*p && *p != ' ') /* FIXME: utf-8 sep */
                ++p;
            *((char *) p) = '\0';
            ++p;
        }

        w->sorted = true;
        for (i = 1; i < len && w->sorted; ++i)
            if (strcmp(w->indices[i - 1], w->indices[i]) > 0)
                w->sorted = false;
    }
    return w;
}

int binary_search_find_index(std::vector<std::string> v, const std::string &data) {
    #ifdef _MSC_VER
    auto it = std::lower_bound(v.begin(), v.end(), data, std::less<>());
    #else
    auto it = std::lower_bound(v.begin(), v.end(), data);
    #endif

    if (it == v.end() || *it != data) {
        return -1;
    } else {
        std::size_t index = std::distance(v.begin(), it);
        return index;
    }
}

size_t wordlist_lookup_word(const struct words *w, const std::string &word) {
    const char **found = nullptr;

    if (w->sorted) {
        const std::vector<std::string> words(&w->indices[0], &w->indices[w->len - 1]);
        int idx = binary_search_find_index(words, word);
        if (idx != -1) {
            found = w->indices + idx;
        }
    }

    if (!w->sorted || !found) {
        size_t i;
        for (i = 0; i < w->len && !found; ++i)
            if (!strcmp(word.c_str(), w->indices[i]))
                found = w->indices + i;
    }
    return found ? found - w->indices + 1u : 0u;
}

const char *wordlist_lookup_index(const struct words *w, size_t idx) {
    if (idx >= w->len) {
        return nullptr;
    }

    return w->indices[idx];
}

void wordlist_free(struct words *w) {
    if (w && w->str_len) {
        if (w->str) {
            bzero((void *) w->str, w->str_len);
            free((void *) w->str);
        }
        if (w->indices)
            free((void *) w->indices); /* No need to clear */
        bzero(w, sizeof(*w));
        free(w);
    }
}
