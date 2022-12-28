
include(CTest)
enable_testing()

add_executable(vector_test vector.cpp)
add_test(NAME vector_test COMMAND vector_test)