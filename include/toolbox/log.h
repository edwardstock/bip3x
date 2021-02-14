/*!
 * toolbox.
 * log.h
 *
 * \date 11/30/2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef TOOLBOXPP_LOG_H
#define TOOLBOXPP_LOG_H

#include "toolbox/toolbox_config.h"

#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <unordered_map>

namespace toolbox {

using level_t = uint16_t;
constexpr const static level_t LEVEL_DEBUG = (1u << 0u);
constexpr const static level_t LEVEL_WARNING = (1u << 1u);
constexpr const static level_t LEVEL_INFO = (1u << 2u);
constexpr const static level_t LEVEL_ERROR = (1u << 3u);
constexpr const static level_t LEVEL_CRITICAL = (1u << 4u);
constexpr const static level_t LEVEL_ALL = LEVEL_DEBUG | LEVEL_WARNING | LEVEL_INFO | LEVEL_ERROR | LEVEL_CRITICAL;

constexpr const static level_t VERBOSITY_0 = LEVEL_CRITICAL | LEVEL_ERROR;
constexpr const static level_t VERBOSITY_1 = VERBOSITY_0 | LEVEL_INFO;
constexpr const static level_t VERBOSITY_2 = LEVEL_ALL;

class TOOLBOX_API tlog {
public:
    tlog(const tlog& copy) = delete;
    tlog(tlog&& copy) = delete;
    tlog& operator=(const tlog& copy) = delete;
    tlog& operator=(tlog&& copy) = delete;

private:
    typedef std::recursive_mutex mutex_t;

    level_t m_level = LEVEL_ALL;
    std::size_t m_buffer_limit = 0;
    bool m_print_datetime = true;

    std::ostream* out_stream;
    std::ostream* err_stream;
    mutex_t log_mutex;

    std::unordered_map<level_t, std::queue<std::string>> logs;
    std::unordered_map<level_t, std::string> levelMap = {
        {LEVEL_DEBUG, "debug"},
        {LEVEL_WARNING, "warning"},
        {LEVEL_INFO, "info"},
        {LEVEL_ERROR, "error"},
        {LEVEL_CRITICAL, "critical"},
    };

    tlog();
    ~tlog() = default;

    std::string level_to_string(level_t level);
    level_t string_to_level(const std::string& level);
    bool can_log(level_t level);

public:
    static tlog& get() {
        static tlog logger;
        return logger;
    }

    void set_out_stream(std::ostream* out);
    void set_err_stream(std::ostream* out);
    void set_level(level_t level);
    void set_level(const std::string& level_s);
    void set_verbosity(level_t verbosity);
    void set_datetime_enable(bool enabled);

    /**
     * @param limit 0 means infinite
     */
    void set_buffer_limit(std::size_t limit);
    void clear();
    void flush();

    void log(level_t level, const char* tag, const char* message);
    void log(level_t level, const std::string& tag, const std::string& message);
    void log(level_t level, const char* file, int line, const char* tag, const char* message);
    void log(level_t level, const std::string& file, int line, const std::string& tag, const std::string& message);

    // ERROR
    void e(const char* file, int line, const char* tag, const char* message);
    void e(const std::string& file, int line, const std::string& tag, const std::string& message);
    void e(const std::string& tag, const std::string& message);
    void e(const char* tag, const char* message);

    // CRITICAL
    void crit(const char* file, int line, const char* tag, const char* message);
    void crit(const std::string& file, int line, const std::string& tag, const std::string& message);
    void crit(const std::string& tag, const std::string& message);
    void crit(const char* tag, const char* message);

    // WARNING
    void w(const char* file, int line, const char* tag, const char* message);
    void w(const std::string& file, int line, const std::string& tag, const std::string& message);
    void w(const std::string& tag, const std::string& message);
    void w(const char* tag, const char* message);

    // INFO
    void i(const char* file, int line, const char* tag, const char* message);
    void i(const std::string& file, int line, const std::string& tag, const std::string& message);
    void i(const std::string& tag, const std::string& message);
    void i(const char* tag, const char* message);

    // DEBUG
    void d(const char* file, int line, const char* tag, const char* message);
    void d(const std::string& file, int line, const std::string& tag, const std::string& message);
    void d(const std::string& tag, const std::string& message);
    void d(const char* tag, const char* message);
};

} // namespace toolbox

#define L_LEVEL(level) toolbox::tlog::get().set_level(level)

#define L_DEBUG(tag, message) toolbox::tlog::get().d(__FILE__, __LINE__, tag, message);
#define L_DEBUG_F(tag, message, ...)                                 \
    {                                                                \
        char __tmp_buff[512];                                        \
        sprintf(__tmp_buff, message, __VA_ARGS__);                   \
        toolbox::tlog::get().d(__FILE__, __LINE__, tag, __tmp_buff); \
    }

#define L_INFO(tag, message) toolbox::tlog::get().i(__FILE__, __LINE__, tag, message);
#define L_INFO_F(tag, message, ...)                                  \
    {                                                                \
        char __tmp_buff[512];                                        \
        sprintf(__tmp_buff, message, __VA_ARGS__);                   \
        toolbox::tlog::get().i(__FILE__, __LINE__, tag, __tmp_buff); \
    }

#define L_WARN(tag, message) toolbox::tlog::get().w(__FILE__, __LINE__, tag, message);
#define L_WARN_F(tag, message, ...)                                  \
    {                                                                \
        char __tmp_buff[512];                                        \
        sprintf(__tmp_buff, message, __VA_ARGS__);                   \
        toolbox::tlog::get().w(__FILE__, __LINE__, tag, __tmp_buff); \
    }
#define L_ERR(tag, message) toolbox::tlog::get().e(__FILE__, __LINE__, tag, message);
#define L_ERR_F(tag, message, ...)                                   \
    {                                                                \
        char __tmp_buff[512];                                        \
        sprintf(__tmp_buff, message, __VA_ARGS__);                   \
        toolbox::tlog::get().e(__FILE__, __LINE__, tag, __tmp_buff); \
    }
#define L_CRIT(tag, message) toolbox::tlog::get().crit(__FILE__, __LINE__, tag, message);
#define L_CRIT_F(tag, message, ...)                                     \
    {                                                                   \
        char __tmp_buff[512];                                           \
        sprintf(__tmp_buff, message, __VA_ARGS__);                      \
        toolbox::tlog::get().crit(__FILE__, __LINE__, tag, __tmp_buff); \
    }

#ifdef _MSC_VER
#define L_TRAP(tag, message) \
    L_CRIT(tag, message)     \
    __debugbreak()
#else
#include <csignal>
#define L_TRAP(tag, message) \
    L_CRIT(tag, message);    \
    raise(SIGABRT)
#endif

#define L_FLUSH() toolbox::tlog::get().flush()

#endif // TOOLBOXPP_LOG_H
