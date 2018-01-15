cmake_minimum_required(VERSION 3.9)
project(01_heads_tails C)

set(CMAKE_C_FLAGS "-std=c99 -Wall -Wextra -Wformat-security -Wmissing-include-dirs -Wuninitialized -Wtrampolines -Wlogical-op -Wno-unused-parameter -Werror=delete-non-virtual-dtor -Werror=return-type -Werror=implicit -g")
set(CMAKE_C_STANDARD 99)

add_executable(01_heads_tails main.c heads-tails.c heads-tails.h)