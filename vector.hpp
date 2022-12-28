#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "ft.hpp"

namespace ft {
// the programmer must ensure that alloctor has same typing
template <typename T, typename Allocator = default_allocator<T>> class vector {
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

  explicit vector(const allocator &alloc) : mem_(alloc), size(0) {}
  explicit vector(size_type count = 0) : mem_(allocator()), size(count) {}
  explicit vector(size_type count, const allocator &alloc)
      : mem_(alloc), size(count), arr_(unique_ptr(mem_.allocate(count), mem_)) {
  }

  vector(const vector &) { throw std::bad_exception("Unimplemented"); }
  template <typename OtherAllocator = allocator>
  vector(const vector &vec, const OtherAllocator &alloc) {
    throw std::bad_exception("Unimplemented");
  }

  vector(vector &&) { throw std::bad_exception("Unimplemented"); }
  template <typename OtherAllocator = allocator>
  vector(vector &&vec, const OtherAllocator &alloc) {
    throw std::bad_exception("Unimplemented");
  }

  explicit vector(size_type count, const value_type &value = value_type())
      : mem_(allocator()), size(count),
        arr_(unique_ptr(mem_.allocate(count), mem_)) {}

  explicit vector(size_type count, const value_type &value,
                  const allocator &alloc)
      : mem_(alloc), size(count), arr_(unique_ptr(mem_.allocate(count), mem_)) {
  }

  template <typename InputIt>
  vector(InputIt begin, InputIt end) : vector(std::distance(begin, end)) {}

  template <typename InputIt>
  vector(InputIt begin, InputIt end, const allocator &alloc)
      : vector(std::distance(begin, end), alloc) {}

protected:
  const allocator &mem_;
  size_type size_;
  unique_ptr<value_type> arr_;

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
