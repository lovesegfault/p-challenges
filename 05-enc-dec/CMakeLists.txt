cmake_minimum_required(VERSION 3.9)
project(05_enc_dec C)

# Get ncurses, we need it for the stdin mode
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# Jemalloc for the linked lists, pthreads for threading
find_library(jemalloc_lib jemalloc)
find_library(pthread_lib pthread)
set(jemalloc_libs ${jemalloc_lib} ${pthread_lib})

set(CMAKE_C_FLAGS_DEBUG  "-std=c99 -Wall -Wextra -Wformat-security -Wmissing-include-dirs -Wuninitialized -Wno-unused-parameter -Werror=delete-non-virtual-dtor -Werror=return-type -Werror=implicit -g")
set(CMAKE_C_FLAGS_RELEASE "-std=c99 -O3 -mtune=native -march=native -Wall -Wextra -Wformat-security -Wmissing-include-dirs -Wuninitialized  -Wno-unused-parameter -Werror=delete-non-virtual-dtor -Werror=return-type -Werror=implicit")

# FIFO
add_library(fifo src/fifo.h src/fifo.c)
target_link_libraries(fifo ${jemalloc_libs})

# Base64 Encoder / Decoder
add_library(base64 src/base64.h src/base64.c)

# Input
add_library(input src/input.h src/input.c)
target_link_libraries(input ${CURSES_LIBRARIES})

# Output
add_library(output src/output.h src/output.c)

# CLI
add_executable(main src/main.c)
set_target_properties(main PROPERTIES COMPILE_FLAGS "-DTESTS_DISABLED")
target_link_libraries(main pthread fifo input)

# Tests
add_executable(tests src/tests.c)
target_link_libraries(tests fifo)

