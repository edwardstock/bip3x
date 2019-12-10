/**
 * bip39. 2018
 * utils.cpp
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */

#include "minter/bip39/utils.h"

#include <limits>
#include <sstream>
#include <stdexcept>
#include <vector>

uint32_t minter::str_to_uint32_t(const std::string& str, size_t* idx, int base) {
    unsigned long result = std::stoul(str, idx, base);
    if (result > std::numeric_limits<uint32_t>::max()) {
        throw std::out_of_range("str_to_uint32_t");
    }

    return static_cast<uint32_t>(result);
}
