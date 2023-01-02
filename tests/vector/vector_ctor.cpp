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

  EXPECT_EQ(v.data(), post_cond);
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

  EXPECT_EQ(v.data(), post_cond);
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

TEST(VectorCtor, Iterator) {
  const vector::size_type pre_cond = 10;
  const vector::pointer post_cond = NEW_ALLOC;
  vector::value_type tmp[pre_cond];
  for (vector::value_type i = 0; i < pre_cond; ++i) {
    tmp[i] = i + 1;
  }
  vector v(tmp, tmp + pre_cond);
  ASSERT_EQ(v.size(), pre_cond);
  ASSERT_NE(v.data(), post_cond);
  for (vector::size_type i = 0; i < pre_cond; ++i) {
    ASSERT_EQ(v.data()[i], tmp[i]);
  }
}
TEST(VectorCtor, Dtor_Clear) { ASSERT_TRUE(false); }

TEST(VectorFuncs, Resize_Reserve_Default) {
  vector::value_type pre_cond = 1, post_cond = 2;
  vector::size_type pre_cond2 = 10;
  vector v(pre_cond2, pre_cond);
  v.reserve(pre_cond2 / 2);
  ASSERT_EQ(v.size(), pre_cond2);
  ASSERT_EQ(v.capacity(), pre_cond2);
  vector::size_type pre_cond3 = pre_cond2 * 2;
  v.reserve(pre_cond3);
  ASSERT_EQ(v.size(), pre_cond2);
  ASSERT_EQ(v.capacity(), pre_cond3);
  v.resize(pre_cond2, post_cond);
  ASSERT_EQ(v.size(), pre_cond2);
  ASSERT_EQ(v.capacity(), pre_cond3);
  v.resize(pre_cond3, post_cond);
  ASSERT_EQ(v.size(), pre_cond3);
  ASSERT_EQ(v.capacity(), pre_cond3);
  for (vector::size_type i = pre_cond2; i < pre_cond3; ++i) {
    ASSERT_EQ(v.data()[i], post_cond);
  }
}

TEST(VectorFuncs, Resize_Reserve_Empty) {
  vector::value_type pre_cond = 1;
  vector::size_type pre_cond2 = 10;
  vector::pointer pre_cond3 = NEW_ALLOC;
  vector v;
  ASSERT_TRUE(v.empty());
  ASSERT_EQ(v.size(), 0);
  ASSERT_EQ(v.capacity(), 0);
  ASSERT_EQ(v.data(), pre_cond3);
  v.reserve(pre_cond2);
  ASSERT_TRUE(v.empty());
  ASSERT_EQ(v.size(), 0);
  ASSERT_EQ(v.capacity(), pre_cond2);
  ASSERT_NE(v.data(), pre_cond3);
  v.resize(pre_cond2, pre_cond);
  ASSERT_EQ(v.size(), pre_cond2);
  ASSERT_EQ(v.capacity(), pre_cond2);
  ASSERT_NE(v.data(), pre_cond3);
  for (vector::size_type i = 0; i < pre_cond2; ++i) {
    ASSERT_EQ(v.data()[i], pre_cond);
  }
}

TEST(VectorFuncs, PushPop_FrontBack_Default) {
  const vector::size_type post_cond = 10;
  vector v;
  vector::value_type tmp[post_cond];
  for (vector::value_type i = 0; i < post_cond; ++i) {
    tmp[i] = i + 1;
    v.push_back(i + 1);
    ASSERT_EQ(v.back(), tmp[i]);
  }
  ASSERT_EQ(v.size(), post_cond);
  ASSERT_EQ(v.front(), *tmp);
  for (vector::size_type i = post_cond; i != 0;) {
    --i;
    ASSERT_EQ(v.back(), tmp[i]);
    v.pop_back();
  }
  ASSERT_TRUE(v.empty());
  ASSERT_EQ(v.size(), 0);
  v.push_back(1);
  ASSERT_EQ(v.front(), v.back());
  ASSERT_EQ(v.size(), 1);
}

TEST(VectorFuncs, PushPop_FrontBack_MoreThanCapacity) {
  const vector::size_type pre_cond = 0x0584;
  const vector::value_type post_cond = 10;
  vector v(post_cond, pre_cond); // FIXME: ambigoues ctor
  vector::value_type tmp[post_cond];
  for (vector::value_type i = 0; i < post_cond; ++i) {
    tmp[i] = i + 1;
    v.push_back(i + 1);
    ASSERT_EQ(v.back(), tmp[i]);
  }
  ASSERT_EQ(v.front(), pre_cond);
  ASSERT_EQ(v.size(), 2 * post_cond);
  ASSERT_GT(v.capacity(), post_cond);
  const vector::size_type post_cond2 = v.capacity();
  while (not v.empty()) {
    v.pop_back();
  }
  ASSERT_EQ(v.size(), 0);
  ASSERT_EQ(v.capacity(), post_cond2);
  for (vector::value_type i = 0; i < post_cond2; ++i) {
    v.push_back(i + 1);
  }
  ASSERT_EQ(v.size(), post_cond2);
  ASSERT_EQ(v.capacity(), post_cond2);
}

TEST(VectorFuncs, At_IndexOP) {

  const vector::size_type post_cond = 10;
  vector v;
  int tmp[post_cond];
  for (vector::value_type i = 0; i < post_cond; ++i) {
    tmp[i] = i + 1;
    v.push_back(i + 1);
    ASSERT_EQ(v.back(), tmp[i]);
  }
}

TEST(VectorFuncs, Swap) { ASSERT_TRUE(false);}

TEST(VectorIterators, Default) { ASSERT_TRUE(false); }

TEST(VectorIterators, Const) { ASSERT_TRUE(false); }
