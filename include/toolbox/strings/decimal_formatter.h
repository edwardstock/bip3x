/*!
 * toolbox.
 * decimal_formatter.h
 *
 * \date 11/30/2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef TOOLBOXPP_STRINGS_DECIMAL_FORMATTER_H
#define TOOLBOXPP_STRINGS_DECIMAL_FORMATTER_H
#include "toolbox/toolbox_config.h"

#include <ostream>
#include <string>
namespace toolbox {
namespace strings {

class TOOLBOX_API decimal_formatter {
public:
    decimal_formatter() = default;
    explicit decimal_formatter(int num);
    explicit decimal_formatter(double num);
    explicit decimal_formatter(float num);
    explicit decimal_formatter(std::string num);
    explicit decimal_formatter(const char* num);

    std::string operator()(const std::string& num);

    decimal_formatter& set_delimiter(const std::string& delimiter);
    decimal_formatter& set_delimiter(char delimiter);
    decimal_formatter& set_decimals_group(size_t num);
    decimal_formatter& set_max_fractions(size_t max_fractions);
    decimal_formatter& set_min_fractions(size_t min_fractions);

    std::string format() const;
    friend std::ostream& operator<<(std::ostream& os, const decimal_formatter& formatter);

private:
    std::string m_num;
    size_t m_decimals = 3;
    size_t m_max_fractions = 18;
    size_t m_min_fractions = 4;
    std::string m_delimiter = " ";
};

} // namespace strings
} // namespace toolbox

TOOLBOX_API std::ostream& operator<<(std::ostream& os, const toolbox::strings::decimal_formatter& formatter);

#endif // TOOLBOXPP_STRINGS_DECIMAL_FORMATTER_H
