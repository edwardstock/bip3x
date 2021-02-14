/*!
 * toolbox.
 * regex.h
 *
 * \date 11/30/2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef TOOLBOXPP_STRINGS_REGEX_H
#define TOOLBOXPP_STRINGS_REGEX_H

#include "toolbox/toolbox_config.h"

#ifdef HAVE_REGEX_H

#if defined(HAVE_STD_REGEX)
#include <regex>
namespace rxns = std;
#elif defined(HAVE_BOOST_REGEX)
#include <boost/regex.hpp>
namespace rxns = boost;
#endif //#if defined(HAVE_STD_REGEX)

#include <string>
#include <vector>

namespace toolbox {
namespace strings {

using match_all_res_t = std::vector<std::vector<std::string>>;

/**
 * Match regex pattern
 * @param pattern
 * @param source
 * @return
 */
TOOLBOX_API bool matches_pattern(const rxns::regex& pattern, const std::string& source);
/**
 * Match regex pattern.
 * @param pattern Just string. By default: search with flag
 * std::regex_constants::icase
 * @param source
 * @return
 */
TOOLBOX_API bool matches_pattern(const std::string& pattern, const std::string& source);
/**
 * Returns all found groups in source with pattern
 * @param rx custom regex object
 * @param s
 * @return vector of vectors of strings
 */
TOOLBOX_API match_all_res_t find_all_pattern(const rxns::regex& pattern, const std::string& source);
/**
 * Returns all found groups in source with pattern
 * @param pattern
 * @param source
 * @return vector of vectors of strings
 */
TOOLBOX_API match_all_res_t find_all_pattern(const std::string& pattern, const std::string& source);
/**
 * Take first regex match and return it
 * @param pattern
 * @param source
 * @return
 */
TOOLBOX_API std::string find_pattern_first(const rxns::regex& pattern, const std::string& source);
/**
 * Take first regex match and return it (with custom regex object)
 * @param pattern
 * @param source
 * @return
 */
TOOLBOX_API std::string find_pattern_first(const std::string& pattern, const std::string& source);

/**
 * Less boilerplate for std::regex
 * Attention! Don't use this method to check if regex pattern has in string, use
 * {@code hasRegex()} instead, because different compilers compiles different
 * results
 * @param rxPattern
 * @param source
 * @return
 */
TOOLBOX_API std::vector<std::string> find_pattern(const rxns::regex& rxPattern, const std::string& source);

/**
 * Less boilerplate for std::regex
 * Attention! Don't use this method to check if regex pattern has in string, use
 * {@code hasRegex()} instead, because different compilers compiles different
 * results
 * @param pattern
 * @param source
 * @return
 */
TOOLBOX_API std::vector<std::string> find_pattern(const std::string& pattern, const std::string& source);

/// \brief Check input string has a valid integer value (including exponential values)
/// \param input
/// \return
TOOLBOX_API bool num_is_integer(const std::string& input);

/// \brief Check input string is a real value
/// \param input
/// \return
TOOLBOX_API bool num_is_real(const std::string& input);

} // namespace strings
} // namespace toolbox

#endif //#ifdef HAVE_REGEX_H

#endif // TOOLBOXPP_STRINGS_REGEX_H
