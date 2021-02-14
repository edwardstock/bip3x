//
// Created by Edward on 30.11.2019.
//

#ifndef TOOLBOXPP_UTILS_H
#define TOOLBOXPP_UTILS_H

#include "toolbox/toolbox_config.h"

#include <algorithm>
#include <cassert>
#include <string>
#include <type_traits>
#include <vector>

namespace toolbox {
namespace data {

/// \brief Convert hex string to unsigned byte vector
/// \param hex
/// \return
TOOLBOX_API std::vector<uint8_t> hex_to_bytes(const std::string& hex);

/// \brief Convert byte array to hex string
/// \param data
/// \param len
/// \return output len will be len*2, as 1 byte in hex is a 2 chars
TOOLBOX_API std::string bytes_to_hex(const uint8_t* data, size_t len);

/// \brief Convert any integral type to byte array
/// \tparam NumT any integral type, that can be asserted with std::is_integral
/// \param num value
/// \param out output vector ref
template<typename U, typename NumT = typename std::enable_if<std::is_integral<U>::value>::type>
void num_to_bytes(const NumT num, std::vector<uint8_t>& out) {
    static_assert(std::is_integral<NumT>::value, "Only integral types can be passed");

    size_t sz = sizeof(num);
    for (size_t i = 0; i < sz; i++) {
        out[(out.size() - 1) - i] = (num >> (i * 8));
    }
}

template<typename NumT>
std::vector<uint8_t> num_to_bytes(NumT num) {
    std::vector<uint8_t> out(sizeof(num));
    num_to_bytes<NumT>(num, out);
    return out;
}

template<typename NumT>
std::vector<uint8_t> num_to_bytes(NumT num, size_t outSize) {
    assert(outSize >= sizeof(num));
    std::vector<uint8_t> out(outSize);
    num_to_bytes<NumT>(num, out);
    return out;
}

TOOLBOX_API std::string to_ascii(const std::vector<char>& input);
TOOLBOX_API std::string to_ascii(const std::vector<uint8_t>& input);

TOOLBOX_API std::vector<char> to_chars(const std::vector<uint8_t>& input);
TOOLBOX_API std::vector<uint8_t> to_bytes(const std::vector<char>& input);
TOOLBOX_API std::vector<uint8_t> to_bytes(const char* data, size_t len);
TOOLBOX_API std::vector<uint8_t> to_bytes(std::string input);

} // namespace data
} // namespace toolbox

#endif // TOOLBOXPP_UTILS_H
