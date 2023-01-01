#include "ft.hpp"
#include "gtest/gtest.h"

using value_type = int;
using allocator = ft::allocator<value_type>;

#define SIZE 128

TEST(AllocatorCtor, Default) {
  const allocator::size_type pre_cond = 0;
  const allocator::size_type post_cond = AVAILABLE_MEMORY(
      allocator::size_type, allocator::difference_type, allocator::value_type);
  const allocator::size_type post_cond2 =
      post_cond / sizeof(allocator::value_type);
  //
  allocator alloc;

  ASSERT_EQ(alloc.num_allocations(), pre_cond);
  ASSERT_EQ(alloc.num_frees(), pre_cond);
  ASSERT_EQ(alloc.available(), post_cond);
  ASSERT_EQ(alloc.max_size(), post_cond);
  ASSERT_EQ(alloc.max_spots(), post_cond2);
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
  ASSERT_EQ(alloc.max_spots(), post_cond);
  allocator::pointer objs[post_cond];
  for (allocator::size_type i = 0; i < post_cond; ++i) {
    int flag;
    try {
      objs[i] = alloc.allocate(NEW_ALLOC);
      flag = 0;
    } catch (const std::runtime_error &) {
      flag = 1;
    } catch (const std::bad_alloc &) {
      flag = -1;
    }
    EXPECT_NE(flag, -1) << "System may have ran out of heap memory";
    ASSERT_EQ(flag, 0);
  }
  ASSERT_EQ(alloc.num_allocations(), post_cond);
  ASSERT_EQ(alloc.num_frees(), post_cond2);
  ASSERT_EQ(alloc.available(), post_cond2);
  //
  for (allocator::size_type i = 0; i < post_cond; ++i) {
    int flag;
    try {
      alloc.allocate(NEW_ALLOC);
      flag = 0;
    } catch (const std::runtime_error &) {
      flag = 1;
    } catch (const std::bad_alloc &) {
      flag = -1;
    }
    EXPECT_NE(flag, -1) << "System may have ran out of heap memory";
    ASSERT_EQ(flag, 1);
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
  ASSERT_EQ(alloc.available(), pre_cond);
}

TEST(AllocatorAllocate, AllocationLargeSize) {
  const allocator::size_type pre_cond = 10;
  const allocator::size_type pre_cond2 = 1;
  const allocator::size_type pre_cond3 =
      pre_cond * sizeof(value_type) + pre_cond2;
  //
  allocator alloc(pre_cond3);
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
  ASSERT_EQ(alloc.available(), pre_cond2);
}

TEST(AllocatorConstruct, ConstructDestroy) {
  enum {
    DEFAULT = 0b00000,
    CTOR = 0b00001,
    DTOR = 0b00010,
    MOVE = 0b00100,
    COPY = 0b01000,
    OP = 0b10000,
  };

  struct S {
    int state() const { return s_; }
    int s_;
    int *count;
    S() = default;
    S(int *count) : s_(CTOR), count(count) {}
    S(const S &other) : s_(COPY | CTOR), count(other.count) {}
    S(S &&other) : s_(MOVE | CTOR), count(other.count) {}
    S &operator=(const S &other) {
      count = other.count;
      s_ = other.s_ | COPY | OP;
      return *this;
    }
    S &operator=(S &&other) {
      count = other.count;
      s_ = other.s_ | MOVE | OP;
      return *this;
    }
    ~S() { *count += 1; }
  };

  {
    int count = 0;
    S s(&count);
    ASSERT_EQ(s.state(), CTOR);
    S s2 = s;
    ASSERT_EQ(s2.state(), COPY | CTOR);
    S s3;
    s3 = s;
    ASSERT_EQ(s3.state(), CTOR | COPY | OP);
    S s4 = std::move(s);
    ASSERT_EQ(s4.state(), CTOR | MOVE);
    S s5;
    s5 = std::move(s3);
    ASSERT_EQ(s5.state(), CTOR | COPY | MOVE | OP);
  }
  using S_allocator = ft::allocator<S>;
  int count = 0;
  S_allocator alloc;
  S_allocator::pointer pre_cond = S_allocator::init();
  {
    S_allocator::value_type post_cond{&count};
    ASSERT_EQ(post_cond.state(), CTOR);
    //
    alloc.construct(pre_cond, 1, post_cond);
    EXPECT_EQ(pre_cond->state(), CTOR | COPY | OP);
    ASSERT_EQ(post_cond.state(), CTOR);
    ASSERT_EQ(count, 0);
    alloc.destroy(pre_cond, 1);
    ASSERT_EQ(count, 1);
  }
  ASSERT_EQ(count, 2);
  //
  alloc.construct(pre_cond, 1, S_allocator::value_type{&count});
  ASSERT_EQ(pre_cond->state(), CTOR | MOVE | OP);
  ASSERT_EQ(count, 3);
  //
  alloc.destroy(pre_cond, 1);
  ASSERT_EQ(count, 4);
}
