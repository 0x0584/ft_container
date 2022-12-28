#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "ft.hpp"

namespace ft {
template <typename T, typename Allocator = std::allocator<T>> class vector {
public:
  using value_type = T;
  using allocator = Allocator;
  using size_type = default_size_type;
  using difference_type = default_difference_type;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = Allocator::pointer;
  using const_pointer = Allocator::const_pointer;
  using iterator = struct iterator;
  using const_iterator = const iterator;
  using reverese_iterator = std::reverse_iterator<iterator>;
  using const_reverese_iterator = std::reverse_iterator<const_iterator>;

  vector() = default;
  explicit vector(const allocator &alloc);

  vector(const vector<value_type> &vec);
  template <typename OtherAllocator = allocator>
  vector(const vector<value_type> &vec,
         const OtherAllocator &alloc = OtherAllocator());

  vector(vector<value_type> &&vec);
  template <typename OtherAllocator = allocator>
  vector(vector<value_type> &&vec,
         const OtherAllocator &alloc = OtherAllocator());

  explicit vector(size_type count, const allocator &alloc = allocator());
  explicit vector(size_type count, const value_type &value = value_type(),
                  const allocator &alloc = allocator());

  template <typename InputIt> vector(InputIt begin, InputIt end);

protected:

  unique_ptr<value_type> arr;


public:
  struct iterator
      : std::iterator<random_access_iterator_tag, value_type, difference_type>,
  {
    explicit iterator() {}

    iterator &operator++() { return *this; }

    iterator operator++(int) {
      iterator retval = *this;
      ++(*this);
      return retval;
    }

    bool operator==(iterator other) const { return false; }

    bool operator!=(iterator other) const { return false; }

    reference operator*() const { throw std::bad_exception("unimplemented"); }
  };
};
} // namespace ft

#endif // VECTOR_HPP
