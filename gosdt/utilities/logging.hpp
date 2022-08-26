#pragma once

/*
 * All the Logging facilities make use of std::cout and expect that all the
 * types passed to it implement the << operator.
 */

#include <ostream>
#include <cassert>

// We want to define the _DEBUG flag if it's not defined
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
