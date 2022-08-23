cmake_minimum_required(VERSION 3.20)

# Project `Name` and `Language`
project(GOSDT-Rewrite)

# Set the language standard to `c++20`
set(CMAKE_CXX_STANDARD 20)

# Set the compiler flags
set(CMAKE_C_FLAGS                   "${CMAKE_C_FLAGS} -Wall -Wextra")
set(CMAKE_C_FLAGS_DEBUG             "${CMAKE_C_FLAGS_DEBUG} -O0 -DDEBUG -g")
set(CMAKE_C_FLAGS_RELEASE           "${CMAKE_C_FLAGS_RELEASE}")

set(CMAKE_CXX_FLAGS                 "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG           "${CMAKE_CXX_FLAGS_DEBUG} -O0 -DDEBUG -g")
set(CMAKE_CXX_FLAGS_RELEASE         "${CMAKE_CXX_FLAGS_RELEASE}")

#
# MARK: - Targets
#

# Target Definition
set(TARGET "gosdt")
set(TARGET_TESTS "${TARGET}_tests")

# Target: GOSDT
# Define the list of source files used to compile the target
file(GLOB_RECURSE SOURCE_FILES gosdt/*.cpp)
add_executable(${TARGET} ${SOURCE_FILES})
target_include_directories(${TARGET} PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Target: GOSDT Tests
set(SOURCE_FILES_TESTS ${SOURCE_FILES} gosdt_test/test.cpp)
list(REMOVE_ITEM SOURCE_FILES_TESTS "${CMAKE_SOURCE_DIR}/gosdt/main.cpp")
list(INSERT SOURCE_FILES_TESTS 0 "${CMAKE_SOURCE_DIR}/gosdt_test/test.cpp")
add_executable(${TARGET_TESTS} ${SOURCE_FILES_TESTS})
target_include_directories(${TARGET_TESTS} PUBLIC ${CMAKE_SOURCE_DIR}/include)

message(STATUS "SRC MAIN: ${SOURCE_FILES}")
message(STATUS "SRC TEST: ${SOURCE_FILES_TESTS}")

#
# MARK: - Dependencies
#

# Dependencies: GMP
find_package(PkgConfig REQUIRED)
pkg_check_modules(GMP REQUIRED IMPORTED_TARGET gmp)
target_link_libraries(${TARGET} PRIVATE PkgConfig::GMP)
target_include_directories(${TARGET} PRIVATE ${GMP_INCLUDE_DIRS})
target_link_libraries(${TARGET_TESTS} PRIVATE PkgConfig::GMP)
target_include_directories(${TARGET_TESTS} PRIVATE ${GMP_INCLUDE_DIRS})

# Dependencies: rapidcsv
include(FetchContent)
FetchContent_Declare(
        rapidcsv
        GIT_REPOSITORY "https://github.com/d99kris/rapidcsv.git"
        GIT_TAG        "v8.50"
)
FetchContent_MakeAvailable(rapidcsv)
target_link_libraries(${TARGET} PUBLIC rapidcsv)
target_link_libraries(${TARGET_TEST} rapidcsv)
