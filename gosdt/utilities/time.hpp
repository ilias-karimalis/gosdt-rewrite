#pragma once

// Defines useful time facilities so that we need not look at std::chrono
#include <chrono>

#include "numeric_types.hpp"
#include "optional.hpp"

// Time types
namespace ch = std::chrono;

template<class Clock, class Duration>
inline bool
time_limit_exceeded(
        ch::time_point<Clock> start_time,
        opt<u64> time_limit)
{
    if (!time_limit.has_value()) return false;

    auto duration_sc = ch::steady_clock::now() - start_time;
    auto duration = ch::duration_cast<Duration>(duration_sc);
    return (u64) duration.count() > time_limit.value();
}

template<class Clock, class Duration>
inline u64
duration(
        ch::time_point<Clock> start_time,
        std::chrono::time_point<Clock> end_time)
{
    auto duration_sc = end_time - start_time;
    auto duration = ch::duration_cast<Duration>(duration_sc);
    return duration.count();
}
