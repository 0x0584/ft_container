#ifndef VECTOR_PLUS
#define VECTOR_PLUS

#include "vector.hpp"

namespace ft {
template <typename T, typename Allocator>
class vector_plus : vector<T, Allocator> {
public:
  vector_plus();

  template <typename ValueType>
  vector_plus(const vector<ValueType> &vec);

  template <typename ValueType>
  vector_plus(vector<ValueType> &&vec);
};
} // namespace ft

#endif // VECTOR_PLUS
