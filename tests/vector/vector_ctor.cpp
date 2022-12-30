#include "vector.hpp"

// #include <gtest.gtest.h>

int main(int argc, const char *argv[]) {
  const int n = std::atoi(argv[1]), m = std::atoi(argv[2]);

  {
    using value_type = int;
    using vector = ft::vector<value_type>;
    {
      // TEST_START: default constructor
      vector::size_type post_cond1 = 0;
      vector::pointer post_cond2 = NEW_ALLOC;

      vector v;
      assert(v.size() == post_cond1);
      assert(v.capacity() == post_cond1);
      assert(v.empty());
      assert(v.data() == post_cond2);
      // TEST_END
    }
    {
      // TEST_START: alias to default constructor
      vector::size_type pre_cond = 0;
      vector::pointer post_cond = NEW_ALLOC;
      vector v(pre_cond);
      assert(v.size() == pre_cond);
      assert(v.capacity() == pre_cond);
      assert(v.empty());
      assert(v.data() != post_cond);
      // TEST_END
    }
    {
      // TEST_START: default construct with size
      vector::size_type pre_cond = m;
      value_type post_cond1{};
      vector::pointer post_cond2 = NEW_ALLOC;
      vector v(pre_cond1);
      assert(v.size() == pre_cond);
      assert(v.capacity() == pre_cond);
      assert(v.data() != post_cond2);
      for (vector::size_type i = 0; i < v.size(); ++i) {
        assert(v[i] == post_cond1);
      }
      // END_TEST
    }
    {
      // TEST_START: construct array with initial value
      vector::size_type pre_cond1 = m;
      value_type pre_cond2{std::numeric_limits<value_type>::max()};
      vector::pointer post_cond = NEW_ALLOC;
      vector v(pre_cond1, pre_cond2);
      assert(v.size() == pre_cond1);
      assert(v.capacity() == pre_cond1);
      assert(v.data() != post_cond);
      for (vector::size_type i = 0; i < v.size(); ++i) {
        assert(v[i] == pre_cond2);
      }
      // END_TEST
    }
    {
    }
  }

  {}
  return 0;
}
