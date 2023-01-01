add_library(gtest STATIC)
target_sources(gtest PRIVATE 
 "src/gtest_main.cc"
 "src/gtest-all.cc"
 "src/gtest-internal-inl.h"
)
