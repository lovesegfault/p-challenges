cmake_minimum_required(VERSION 3.5)
project(01_name_generator C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS_DEBUG  "-std=c99 -fsanitize=address,undefined -Wall -Wextra -Wformat-security -Wmissing-include-dirs -Wuninitialized -Wno-unused-parameter -Werror=delete-non-virtual-dtor -Werror=return-type -Werror=implicit -g")
set(CMAKE_C_FLAGS_RELEASE "-std=c99 -O3 -mtune=native -march=native -Wall -Wextra -Wformat-security -Wmissing-include-dirs -Wuninitialized  -Wno-unused-parameter -Werror=delete-non-virtual-dtor -Werror=return-type -Werror=implicit")


add_library(person_generator src/person-generator.c)


add_executable(main src/main.c)
set_target_properties(main PROPERTIES COMPILE_FLAGS "-DTESTS_DISABLED")
target_link_libraries(main person_generator)