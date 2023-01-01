#include "ft.hpp"
#include "gtest/gtest.h"

using value_type = int;
using allocator = ft::allocator<value_type>;

#define SIZE 128

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

TEST(AllocatorAllocate, AllocateDeallocate) {
  const allocator::size_type pre_cond = SIZE;
  const allocator::size_type post_cond = SIZE / sizeof(value_type);
  const allocator::size_type post_cond2 = 0;
  //
  allocator alloc(pre_cond);
  allocator::pointer objs[post_cond];
  for (allocator::size_type i = 0; i < post_cond; ++i) {
    bool flag;
    try {
      objs[i] = alloc.allocate(NEW_ALLOC);
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
  int post_cond3 = 0;
  for (allocator::size_type i = 0; i < post_cond; ++i) {
    int flag;
    try {
      alloc.allocate(NEW_ALLOC);
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
  for (allocator::size_type i = 0; i < post_cond; ++i) {
    alloc.deallocate(objs[i]);
  }
  ASSERT_EQ(alloc.num_allocations(), post_cond);
  ASSERT_EQ(alloc.num_frees(), post_cond);
  ASSERT_EQ(alloc.available(), post_cond);
}

TEST(AllocatorAllocate, AllocationLargeSize) {
  const allocator::size_type pre_cond = 10;
  const allocator::size_type post_cond1 = 1;
  const allocator::size_type post_cond2 =
      pre_cond * sizeof(value_type) + post_cond1;
  //
  allocator alloc(pre_cond);
  int n = 0, m = 0;
  for (allocator::size_type i = 0; i < pre_cond * 2; ++i) {
    bool flag = true;
    try {
      alloc.allocate(NEW_ALLOC);
      n++;
    } catch (const std::runtime_error &) {
      m++;
    } catch (const std::bad_alloc &) {
      flag = false;
    }
    ASSERT_TRUE(flag);
  }
  ASSERT_EQ(n, pre_cond);
  ASSERT_EQ(m, pre_cond);
  ASSERT_EQ(alloc.num_allocations(), n);
  ASSERT_EQ(alloc.available(), post_cond1);
}

TEST(AllocatorConstruct, ConstructDestroy) {
  enum object_states {
    DEFAULT = 0b00000,
    OP = 0b10000,
    CTOR = 0b00001,
    DTOR = 0b00010,
    COPY = 0b01000,
    MOVE = 0b00100,
  };

  int cond;
  struct S {
    int &s_;
    // S() = default;
    S(int &s) : s_(s) { s_ = CTOR; }
    S(const S &other) : s_(other.s_) { s_ = COPY; }
    S(S &&other) : s_(other.s_) { s_ = MOVE; }
    S &operator=(const S &other) {
      other.s_ = COPY;
      return *this;
    }
    S &operator=(S &&other) {
      other.s_ = MOVE;
      return *this;
    }
    ~S() { s_ = DTOR; }
  };

  using S_allocator = ft::allocator<S>;

  S_allocator alloc;
  S_allocator::pointer ptr = S_allocator::init();
  ASSERT_EQ(cond, object_states::DEFAULT);
  //
  S_allocator::value_type dumb(cond);
  ASSERT_EQ(cond, object_states::CTOR);
  alloc.construct(ptr, 1, dumb);
  ASSERT_EQ(cond, object_states::COPY);
  //
  alloc.construct(ptr, 1, S_allocator::value_type(cond));
  ASSERT_EQ(cond, object_states::MOVE);
  //
  alloc.destroy(ptr, 1);
  ASSERT_EQ(cond, object_states::DTOR);
}
