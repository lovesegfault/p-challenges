cmake_minimum_required(VERSION 3.5)
project(05_enc_dec C)

set(CMAKE_C_STANDARD 99)

if(${CMAKE_C_COMPILER_ID} MATCHES "Clang")
    set(SANITIZE "-fsanitize=address,undefined,leak,cfi -flto -fvisibility=hidden")
    set(DEBUG_OPT "-O1")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(SANITIZE "-fsanitize=address,undefined,leak")
    set(DEBUG_OPT "-Og")
endif()

set(WARNINGS "-Wall -Wextra -Wformat-security -Wmissing-include-dirs -Wuninitialized -Wno-unused-parameter -Werror=delete-non-virtual-dtor -Werror=return-type -Werror=implicit")
set(TUNE "-mtune=native -march=native")

set(CMAKE_C_FLAGS_DEBUG "-std=c99 -g ${DEBUG_OPT} ${SANITIZE} ${WARNINGS}")
set(CMAKE_C_FLAGS_RELEASE "-std=c99 -O3 ${TUNE} ${WARNINGS}")

# Get ncurses, we need it for the stdin mode
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# Jemalloc for the linked lists, pthreads for threading
find_library(jemalloc_lib jemalloc)
find_library(pthread_lib pthread)
set(jemalloc_libs ${jemalloc_lib} ${pthread_lib})

# FIFO
add_library(fifo STATIC src/fifo.c)
target_link_libraries(fifo ${jemalloc_libs})

# Base64 Encoder / Decoder
add_library(base64 STATIC src/base64.c)

# Input
add_library(input STATIC src/input.c)
target_link_libraries(input ${CURSES_LIBRARIES})

# Output
add_library(output STATIC src/output.c)

# CLI
add_executable(main src/main.c)
set_target_properties(main PROPERTIES COMPILE_FLAGS "-DTESTS_DISABLED")
target_link_libraries(main pthread fifo input base64 output)


# Tests
add_library(testlib STATIC ../tests/test.c)
add_executable(tests src/tests.c)
target_link_libraries(tests testlib fifo)

