/*!
 * toolbox.
 * bytes_buffer.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef TOOLBOXPP_BYTES_BUFFER_H
#define TOOLBOXPP_BYTES_BUFFER_H

#include "bytes_data.h"
#include "toolbox/toolbox_config.h"

namespace toolbox {
namespace data {

class TOOLBOX_API bytes_buffer : public bytes_data {
public:
    bytes_buffer();
    bytes_buffer(const basic_data& other);
    bytes_buffer(basic_data&& other);
    explicit bytes_buffer(std::size_t size);
    bytes_buffer(std::initializer_list<uint8_t> ilist);
    bytes_buffer(std::initializer_list<std::vector<uint8_t>> ilist);
    bytes_buffer(std::vector<uint8_t> data);
    bytes_buffer(const uint8_t* data, size_t len);
    bytes_buffer(const char* hexString);
    bytes_buffer(const std::string& hexString);

    size_t pop_front_to(size_t outPosition, basic_data<uint8_t>& out);
    size_t pop_front_to(size_t readLength, size_t position, basic_data<uint8_t>& out);
    size_t pop_back_to(basic_data<uint8_t>& out);
    size_t pop_back_to(size_t readLength, basic_data<uint8_t>& out);
    size_t pop_front_to(size_t readLength, iterator position, basic_data<uint8_t>& out);
};

} // namespace data
} // namespace toolbox

#endif //TOOLBOXPP_BYTES_BUFFER_H
