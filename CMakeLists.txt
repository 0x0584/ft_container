cmake_minimum_required (VERSION 3.8)

project ("ft_containers")

set (CMAKE_CXX_STANDARD 98)
set (CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_CXX_STANDARD_REQUIRED ON)


if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

add_subdirectory(tests)