add_executable(vector_ctor "vector_ctor.cpp")
target_link_libraries(vector_ctor gtest)
add_test(NAME vector_ctor COMMAND vector_ctor)