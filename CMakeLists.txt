cmake_minimum_required (VERSION 3.8)
project (ft_containers LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 98)
set (CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("include")
if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

include(CTest REQUIRED)

#include(FetchContent)
#FetchContent_Declare(
#  googletest
#  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
#)
# For Windows: Prevent overriding the parent project's compiler/linker settings
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(googletest)

# Now simply link against gtest or gtest_main as needed. Eg
#add_executable(example example.cpp)
#target_link_libraries(example gtest_main)
#add_test(NAME example_test COMMAND example)

enable_testing()

add_subdirectory("tests")

