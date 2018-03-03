cmake_minimum_required(VERSION 3.9)
project(05_enc_dec C)

find_library(jemalloc_lib jemalloc)
find_library(pthread_lib pthread)
set(jemalloc_libs ${jemalloc_lib} ${pthread_lib})

set(CMAKE_C_FLAGS "-std=c99 -O2 -mtune=native -march=native -Wall -Wextra -Wformat-security -Wmissing-include-dirs -Wuninitialized -Wtrampolines -Wlogical-op -Wno-unused-parameter -Werror=delete-non-virtual-dtor -Werror=return-type -Werror=implicit -g")

add_library(fifo src/fifo.h src/fifo.c)
target_link_libraries(fifo ${jemalloc_libs})

add_library(base64 src/base64.h src/base64.c)

add_library(input src/input.h src/input.c)

add_executable(main src/main.c)
set_target_properties(main PROPERTIES COMPILE_FLAGS "-DTESTS_DISABLED")
target_link_libraries(main fifo base64 input)

add_executable(tests src/tests.c)
target_link_libraries(tests fifo base64 input)

