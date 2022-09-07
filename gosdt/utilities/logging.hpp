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

/*
 * All the Logging facilities make use of std::cout and expect that all the
 * types passed to it implement the << operator.
 */

#include <ostream>
#include <iostream>
#include <cassert>

//#define _DEBUG_VERBOSE
#define _DEBUG


#ifdef _DEBUG_VERBOSE
    #ifndef _DEBUG
        #define _DEBUG
    #endif
#endif


#ifdef _DEBUG
    #define DOUT std::cout
#else
    #define DOUT if(0) std::cerr
#endif

#ifdef _DEBUG_VERBOSE
#define DVOUT std::cout
#else
#define DVOUT if(0) std::cerr
#endif

#ifdef _DEBUG
#define DASSERT(X) assert(X)
#else
#define DASSERT(X)
#endif
