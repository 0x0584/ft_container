#include "ft.hpp"
#include "gtest/gtest.h"

using unique_ptr = ft::unique_ptr<int>;

struct custom {
  bool *st_;
  int id_;
  custom(custom &&other) : st_(other.st_), id_(other.id_) {}
  custom &operator=(custom &&other) {
    st_ = other.st_;
    id_ = other.id_;
    return *this;
  }

  custom(bool *st, int id = 0) : st_(st), id_(id) { *st_ = false; }
  void operator()(int *ptr) {
    free(ptr);
    *st_ = true;
  }

  static int *sample(int i = 0) {
    int *ptr = static_cast<int *>(calloc(1, sizeof(int)));
    assert(ptr != NULL);
    *ptr = i;
    return ptr;
  }
};

TEST(UniquePtrCtor, Default) {
  unique_ptr ptr;
  //
  ASSERT_EQ(ptr.get(), nullptr);
  ASSERT_EQ(ptr.release(), nullptr);
  bool post_cond;
  try {
    *ptr = unique_ptr::value_type{2};
    post_cond = false;
  } catch (const std::runtime_error &) {
    post_cond = true;
  }
  ASSERT_TRUE(post_cond);
}

TEST(UniquePtrCtor, WithPtr) {
  unique_ptr::value_type pre_cond1 = 1;
  unique_ptr::pointer pre_cond = new unique_ptr::value_type(pre_cond1);
  unique_ptr::value_type post_cond = 2;
  unique_ptr ptr(pre_cond);
  //
  ASSERT_EQ(ptr.get(), pre_cond);
  ASSERT_EQ(*ptr, *pre_cond);
  *ptr = post_cond;
  ASSERT_NE(*ptr, pre_cond1);
  ASSERT_EQ(*ptr, post_cond);
  ASSERT_EQ(ptr.release(), pre_cond);
  ASSERT_EQ(ptr.get(), nullptr);
}

TEST(UniquePtrAllocator, Default) {
  using unique_ptr = ft::unique_ptr<int, ft::deleter<int>>;
  unique_ptr::pointer pre_cond1 = new unique_ptr::value_type(1);
  unique_ptr::deleter pre_cond2;
  unique_ptr ptr1(pre_cond1, pre_cond2);
  ASSERT_NE(&pre_cond2, &ptr1.get_deleter());
}

TEST(UniquePtrAllocator, Custom) {
  using unique_ptr = ft::unique_ptr<int, custom>;
  unique_ptr::pointer pre_cond1 = custom::sample();
  bool post_cond;
  unique_ptr::deleter pre_cond2(&post_cond);
  { unique_ptr ptr1(pre_cond1, std::move(pre_cond2)); }
  ASSERT_TRUE(post_cond);
}

TEST(UniquePtrFuncs, Get_Reset) {
  unique_ptr::pointer pre_cond1 = new unique_ptr::value_type(1);
  unique_ptr::pointer pre_cond2 = new unique_ptr::value_type(2);
  unique_ptr ptr(pre_cond1);
  //
  ASSERT_EQ(ptr.get(), pre_cond1);
  ptr.reset(pre_cond2);
  ASSERT_EQ(ptr.get(), pre_cond2);
  //
  unique_ptr ptr2;
  ptr2.reset(ptr.get());
  ASSERT_EQ(ptr2.get(), ptr.get());
  unique_ptr::pointer post_cond1 = ptr.get(), post_cond2 = nullptr;
  ASSERT_EQ(ptr.release(), post_cond1);
  ASSERT_EQ(ptr.get(), post_cond2);
  ASSERT_EQ(ptr2.get(), pre_cond2) << "fatal: double free";
  ptr2.reset();
  ASSERT_EQ(ptr2.get(), post_cond2);
}

TEST(UniquePtrFuncs, Swap) {
  using unique_ptr = ft::unique_ptr<int, custom>;
  bool post_cond1, post_cond2;
  unique_ptr::value_type pre_cond1 = 1, pre_cond2 = 2;
  unique_ptr::deleter del1(&post_cond1, pre_cond1),
      del2(&post_cond2, pre_cond2);
  unique_ptr::pointer pre_cond3 = custom::sample(pre_cond1),
                      pre_cond4 = custom::sample(pre_cond2);
  unique_ptr ptr1(pre_cond3, std::move(del1)), ptr2(pre_cond4, std::move(del2));
  ptr1.swap(ptr2);
  ASSERT_EQ(ptr1.get(), pre_cond4);
  ASSERT_EQ(ptr2.get(), pre_cond3);
  ASSERT_EQ(ptr1.get_deleter().id_, pre_cond2);
  ASSERT_EQ(ptr2.get_deleter().id_, pre_cond1);
  std::swap(ptr1, ptr2);
  ASSERT_EQ(ptr1.get(), pre_cond3);
  ASSERT_EQ(ptr2.get(), pre_cond4);
  ASSERT_EQ(ptr1.get_deleter().id_, pre_cond1);
  ASSERT_EQ(ptr2.get_deleter().id_, pre_cond2);
}
