cmake_minimum_required (VERSION 3.8)

project (ft_containers LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 98)
set (CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

include_directories("include" "googletest/include" "googletest")

include(CTest REQUIRED)
enable_testing()
add_subdirectory("tests")
add_subdirectory("googletest")



