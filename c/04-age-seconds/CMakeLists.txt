cmake_minimum_required(VERSION 3.9)
project(04_age_seconds C)

set(CMAKE_C_FLAGS "-std=c99 -mtune=native -march=native -Wall -Wextra -Wformat-security -Wmissing-include-dirs -Wuninitialized -Wtrampolines -Wlogical-op -Wno-unused-parameter -Werror=delete-non-virtual-dtor -Werror=return-type -Werror=implicit -g")
set(CMAKE_C_STANDARD 99)

add_executable(04_age_seconds main.c)
