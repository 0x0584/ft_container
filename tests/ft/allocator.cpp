#include "ft.hpp"
#include "gtest/gtest.h"

using value_type = int;
using allocator = ft::allocator<value_type>;

TEST(AllocatorCtor, Default) {
  const allocator::size_type pre_cond = 0;
  //
  allocator alloc;

  ASSERT_EQ(alloc.available(), pre_cond);
  ASSERT_EQ(alloc.max_size(), pre_cond);
  ASSERT_EQ(alloc.num_allocations(), pre_cond);
  ASSERT_EQ(alloc.num_frees(), pre_cond);
}

TEST(AllocatorCtor, WithSize) {
  const allocator::value_type pre_cond = 1024;
  const allocator::size_type post_cond = 0;
  //
  allocator alloc(pre_cond);

  ASSERT_EQ(alloc.available(), pre_cond);
  ASSERT_EQ(alloc.max_size(), pre_cond);
  ASSERT_EQ(alloc.num_allocations(), post_cond);
  ASSERT_EQ(alloc.num_frees(), post_cond);
}

TEST(AllocatorAllocate, SuccessiveAllocations) {
  const allocator::value_type pre_cond = 128;
  const allocator::size_type post_cond = 128 / sizeof(value_type);
  const allocator::size_type post_cond2 = 0;
  //
  allocator alloc(pre_cond);
  allocator::pointer objs[post_cond];
  for (allocator::size_type i = 0; i < post_cond; ++i) {
    bool flag;
    try {
      objs[i] = alloc.allocate_object(NEW_ALLOC);
      flag = true;
    } catch (const std::bad_alloc &) {
      flag = false;
    }
    ASSERT_TRUE(flag) << "System may have ran out of heap memory";
  }
  ASSERT_EQ(alloc.num_allocations(), post_cond);
  ASSERT_EQ(alloc.num_frees(), post_cond2);
  ASSERT_EQ(alloc.available(), post_cond2);
  //
  allocator::size_type i = 0;
  int post_cond3 = 0;
  for (allocator::size_type i = 0; i < post_cond; ++i) {
    int flag;
    try {
      alloc.allocate_object(NEW_ALLOC);
      flag = 1;
    } catch (const std::runtime_error &) {
      flag = 0;
    } catch (const std::bad_alloc &) {
      flag = 2;
    }
    ASSERT_EQ(flag, post_cond3);
  }
  ASSERT_EQ(alloc.num_allocations(), post_cond);
  ASSERT_EQ(alloc.num_frees(), post_cond2);
  ASSERT_EQ(alloc.available(), post_cond2);
  //
}
