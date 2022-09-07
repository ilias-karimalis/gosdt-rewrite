// GOSDT algorithm
//
//
// MIT License
//
// Copyright (c) 2022 Systopia Lab, Computer Science, University of British Columbia
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

// Defines useful time facilities so that we need not work directlyw with std::chrono
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
