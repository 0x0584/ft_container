add_executable(allocator allocator.cpp)
target_link_libraries(allocator ft)
add_test(NAME allocator_unit_test COMMAND allocator)

add_executable(allocator_no_throw "allocator_no_throw.cpp")
target_link_libraries(allocator_no_throw ft)
add_test(NAME allocator_no_throw_unit_test COMMAND allocator_no_throw)

add_executable(unique_ptr unique_ptr.cpp)
target_link_libraries(unique_ptr ft)
add_test(NAME unique_ptr_unit_test COMMAND unique_ptr)