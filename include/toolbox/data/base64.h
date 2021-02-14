/*!
 * toolbox.
 * base64.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */
#ifndef TOOLBOX_BASE64_H
#define TOOLBOX_BASE64_H

#include "toolbox/toolbox_config.h"
#include "utils.h"

#include <string>

namespace toolbox {
namespace data {

TOOLBOX_API std::string get_base64_chars();
TOOLBOX_API bool is_base64(unsigned char c);
TOOLBOX_API std::string base64_encode(const std::string& raw_string);
TOOLBOX_API std::string base64_decode(const std::string& encoded_string);
TOOLBOX_API std::vector<uint8_t> base64_encode_bytes(const std::vector<uint8_t>& raw);
TOOLBOX_API std::vector<uint8_t> base64_decode_bytes(const std::vector<uint8_t>& encoded);
TOOLBOX_API std::vector<uint8_t> base64_encode_chars(const std::vector<char>& raw);
TOOLBOX_API std::vector<uint8_t> base64_decode_chars(const std::vector<char>& encoded);

} // namespace data
} // namespace toolbox

#endif // TOOLBOX_BASE64_H
