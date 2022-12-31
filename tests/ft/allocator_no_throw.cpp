#include "ft.hpp"
#include "gtest/gtest.h"
TEST(AllocatorNoThrow, AllocatorNoThrowDefault) {
  using value_type = int;
  ft::allocator_no_throw<value_type> alloc; 
}
