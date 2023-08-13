#include "bip3x/details/utils.h"

#include <limits>
#include <sstream>
#include <stdexcept>
#include <vector>

uint32_t bip3x::str_to_uint32_t(const std::string& str, size_t* idx, int base) {
    unsigned long result = std::stoul(str, idx, base);
    if (result > std::numeric_limits<uint32_t>::max()) {
        throw std::out_of_range("str_to_uint32_t");
    }

    return static_cast<uint32_t>(result);
}
