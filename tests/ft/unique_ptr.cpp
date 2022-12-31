#include "ft.hpp"
#include "gtest/gtest.h"

TEST(UniquePtrCtor, UniquePtrCtorDefault) {
  using value_type = int;
  using unique_ptr = ft::unique_ptr<value_type>;

  unique_ptr::pointer p = new value_type{};
  unique_ptr ptr(p);
}
