#include "vector.hpp"
#include "gtest/gtest.h"

using value_type = int;
using vector = ft::vector<value_type>;

TEST(VectorCtor, Default) {
  const vector::size_type post_cond1 = 0;
  const vector::pointer post_cond2 = NEW_ALLOC;
  //
  vector v;

  EXPECT_EQ(v.data(), post_cond2);
  ASSERT_EQ(v.size(), post_cond1);
  ASSERT_EQ(v.capacity(), post_cond1);
  ASSERT_TRUE(v.empty());
}

TEST(VectorCtor, ZeroSizeDefault) {
  const vector::size_type pre_cond = 0;
  const vector::pointer post_cond = NEW_ALLOC;
  //
  vector v(pre_cond);

  EXPECT_NE(v.data(), post_cond);
  ASSERT_EQ(v.size(), pre_cond);
  ASSERT_EQ(v.capacity(), pre_cond);
  ASSERT_TRUE(v.empty());
}

TEST(VectorCtor, ZeroSizeWithValue) {
  const vector::size_type pre_cond1 = 0;
  const value_type pre_cond2{};
  const vector::pointer post_cond = NEW_ALLOC;
  //
  vector v(pre_cond1, pre_cond2);

  EXPECT_NE(v.data(), post_cond);
  ASSERT_EQ(v.size(), pre_cond1);
  ASSERT_EQ(v.capacity(), pre_cond1);
  ASSERT_TRUE(v.empty());
}

TEST(VectorCtor, SizeWithDefault) {
  const vector::size_type pre_cond = 0x0584;
  const vector::pointer post_cond1 = NEW_ALLOC;
  const value_type post_cond2{};
  //
  vector v(pre_cond);

  ASSERT_NE(v.data(), post_cond1);
  ASSERT_EQ(v.size(), pre_cond);
  ASSERT_EQ(v.capacity(), pre_cond);
  for (vector::size_type i = 0; i < v.size(); ++i) {
    ASSERT_EQ(v[i], post_cond2);
  }
}

TEST(VectorCtor, SizeWithValue) {
  const vector::size_type pre_cond1 = 0x0584;
  const value_type pre_cond2{};
  const vector::pointer post_cond = NEW_ALLOC;
  //
  vector v(pre_cond1, pre_cond2);

  ASSERT_NE(v.data(), post_cond);
  ASSERT_EQ(v.size(), pre_cond1);
  ASSERT_EQ(v.capacity(), pre_cond1);
  for (vector::size_type i = 0; i < v.size(); ++i) {
    assert(v[i] == pre_cond2);
  }
}
