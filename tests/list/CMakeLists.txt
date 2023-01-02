add_executable(list_ctor "list_ctor.cpp")
target_link_libraries(list_ctor gtest)
add_test(NAME list_ctor_empty_range COMMAND list_ctor)