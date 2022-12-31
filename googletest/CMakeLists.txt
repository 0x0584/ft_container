add_library(gtest STATIC)
target_sources(gtest PRIVATE 
 "src/gtest.cc"
 "src/gtest_main.cc"
 "src/gtest-all.cc"
 "src/gtest-assertion-result.cc"
 "src/gtest-death-test.cc"
 "src/gtest-filepath.cc"
 "src/gtest-matchers.cc"
 "src/gtest-port.cc"
 "src/gtest-printers.cc"
 "src/gtest-test-part.cc"
 "src/gtest-typed-test.cc"
 "src/gtest-internal-inl.h"
)
