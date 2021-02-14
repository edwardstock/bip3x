/*!
 * toolbox.
 * transformers.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */
#ifndef TOOLBOX_TRANSFORMERS_H
#define TOOLBOX_TRANSFORMERS_H

#include "base64.h"
#include "bytes_data.h"
#include "utils.h"

#include <string>

namespace toolbox {
namespace data {

static std::vector<uint8_t> to_base_64(const std::vector<uint8_t>& source) {
    return base64_encode_bytes(source);
}

static std::vector<uint8_t> from_base_64(const std::vector<uint8_t>& source) {
    return base64_decode_bytes(source);
}

template<>
template<>
struct basic_data<uint8_t>::converter<std::string> {
    std::string operator()(const basic_data<uint8_t>& source) {
        return std::string(source.begin(), source.end());
    }
};
using bytes_to_string = basic_data<uint8_t>::converter<std::string>;

template<>
template<>
struct basic_data<uint8_t>::converter<basic_data<char>> {
    basic_data<char> operator()(const basic_data<uint8_t>& source) {
        return std::vector<char>(source.begin(), source.end());
    }
};
using bytes_to_chars = basic_data<uint8_t>::converter<basic_data<char>>;

} // namespace data
} // namespace toolbox

#endif // TOOLBOX_TRANSFORMERS_H
