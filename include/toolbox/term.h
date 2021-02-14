/*!
 * toolbox.
 * term.h
 *
 * \date 11/30/2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef TOOLBOXPP_TERM_H
#define TOOLBOXPP_TERM_H

#include "toolbox/toolbox_config.h"

#include <iostream>
#include <string>

// LCOV_EXCL_START
namespace toolbox {
namespace term {

#if defined(HAVE_TERMIOS_H) && defined(HAVE_UNISTD_H)
TOOLBOX_API ssize_t _password(const std::string& message, char** pw, size_t sz, int mask = 0, FILE* fp = stdin);
TOOLBOX_API std::string prompt_password(const std::string& message, size_t passSize = 32);
#endif

TOOLBOX_API bool confirm(std::istream& in, std::ostream& out, const std::string& message, bool defValue = false);
TOOLBOX_API bool confirm(const std::string& message, bool defValue = false);
TOOLBOX_API std::string prompt(std::istream& in, std::ostream& out, const std::string& message, bool required = false,
                               const std::string& defValue = "");
TOOLBOX_API std::string prompt(const std::string& message, bool required = false, const std::string& defValue = "");

} // namespace term
} // namespace toolbox
// LCOV_EXCL_STOP

#endif // TOOLBOXPP_TERM_H
