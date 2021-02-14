/*!
 * toolbox.
 * time_profiler.h
 *
 * \date 11/30/2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef TOOLBOXPP_TIME_PROFILER_H
#define TOOLBOXPP_TIME_PROFILER_H

#include "toolbox_config.h"

#include <chrono>
#include <mutex>
#include <string>
#include <unordered_map>

namespace toolbox {

class TOOLBOX_API time_profiler {
public:
    using hires_clock = std::chrono::high_resolution_clock;
    using hires_time_t = std::chrono::high_resolution_clock::time_point;
    using mutex_t = std::mutex;

    static time_profiler& get() {
        static time_profiler p;
        return p;
    }

    time_profiler(const time_profiler&) = delete;
    time_profiler(time_profiler&&) = delete;
    ~time_profiler();

    void clear();
    size_t size() const;
    void begin(std::string tag);
    void end(std::string tag, double* result = nullptr);

    std::unordered_map<std::string, hires_time_t> timings;
    mutable mutex_t lock;
    time_profiler() = default;
};

} // namespace toolbox

#endif // TOOLBOXPP_TIME_PROFILER_H
