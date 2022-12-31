add_executable(vector_ctor "vector_ctor.cpp")
target_link_libraries(vector_ctor gtest)
add_test(NAME vector_ctor_empty_range COMMAND vector_ctor 0 0)
add_test(NAME vector_ctor_size_capacity COMMAND vector_ctor 0 10)

#add_executable(testfile "test.cpp")
#add_test(NAME testfile COMMAND testfile)
#target_link_libraries(vector_ctor PRIVATE ft)