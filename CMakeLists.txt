cmake_minimum_required(VERSION 3.20)

project(gosdt-REWRITE)

set(CMAKE_CXX_STANDARD 17)

# Set the compiler flags
set(CMAKE_C_FLAGS                   "${CMAKE_C_FLAGS} -Wall -Wextra")
set(CMAKE_C_FLAGS_DEBUG             "${CMAKE_C_FLAGS_DEBUG} -O0 -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE           "${CMAKE_C_FLAGS_RELEASE}")


# Set the compiler flags
set(CMAKE_CXX_FLAGS                 "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG           "${CMAKE_CXX_FLAGS_DEBUG} -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE         "${CMAKE_CXX_FLAGS_RELEASE}")

# Target definition
set(TARGET "gosdt")

# Target: GOSDT
# Define the list of source files used to compile the target
file(GLOB_RECURSE SOURCE_FILES gosdt/*.cpp)
add_executable(${TARGET} ${SOURCE_FILES} include/json/json.hpp include/matrix/matrix.hpp)
target_include_directories(${TARGET} PUBLIC ${CMAKE_SOURCE_DIR}/include)

## Dependencies: Intel TBB
#find_package(TBB REQUIRED)
#target_link_libraries(${TARGET} PRIVATE TBB::tbb)
#target_link_libraries(${TARGET} PRIVATE TBB::tbbmalloc)
#target_link_libraries(${TARGET_TESTS} PRIVATE TBB::tbb)
#target_link_libraries(${TARGET_TESTS} PRIVATE TBB::tbbmalloc)
#
## Dependencies: Boost
#set(Boost_USE_STATIC_LIBS OFF)
#set(Boost_USE_MULTITHREADED ON)
#set(Boost_USE_STATIC_RUNTIME OFF)
#find_package(Boost 1.45.0 COMPONENTS numeric)
#
#if(Boost_FOUND)
#    target_include_directories(${TARGET} PRIVATE ${Boost_INCLUDE_DIRS})
#    target_link_libraries(${TARGET} PRIVATE ${Boost_LIBRARIES})
#    target_include_directories(${TARGET_TESTS} PRIVATE ${Boost_INCLUDE_DIRS})
#    target_link_libraries(${TARGET_TESTS} PRIVATE ${Boost_LIBRARIES})
#    message("Found the boost library.")
#endif()

# Dependencies: GMP
find_package(PkgConfig REQUIRED)
pkg_check_modules(GMP REQUIRED IMPORTED_TARGET gmp)
target_link_libraries(${TARGET} PRIVATE PkgConfig::GMP)
target_include_directories(${TARGET} PRIVATE ${GMP_INCLUDE_DIRS})

# Dependencies: rapidcsv
include(FetchContent)
FetchContent_Declare(
        rapidcsv
        GIT_REPOSITORY "https://github.com/d99kris/rapidcsv.git"
        GIT_TAG        "v8.50"
)
FetchContent_MakeAvailable(rapidcsv)
target_link_libraries(${TARGET} PUBLIC rapidcsv)



