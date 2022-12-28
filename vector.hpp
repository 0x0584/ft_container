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

  ~vector() { delete[] arr_; }

  explicit vector(const allocator &alloc) : mem_(alloc), size(0) {}
  explicit vector(size_type count = 0)
      : mem_(allocator()), size(count), arr_(mem_.allocate(count)) {}
  explicit vector(size_type count, const allocator &alloc)
      : mem_(alloc), size(count), arr_(mem_.allocate(count)) {}

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
      : mem_(allocator()), size(count), arr_(mem_.allocate(count)) {}

  explicit vector(size_type count, const value_type &value,
                  const allocator &alloc)
      : mem_(alloc), size(count), arr_(mem_.allocate(count)) {}

  template <typename InputIt>
  vector(InputIt begin, InputIt end) : vector(std::distance(begin, end)) {}

  template <typename InputIt, typename OtherAllocator>
  vector(InputIt begin, InputIt end, const OtherAllocator &alloc)
      : vector(std::distance(begin, end), alloc) {}

  inline vector &operator=(vector &&vec) {
    throw std::bad_exception("Unimplemented");
  }

  inline const vector &operator=(const vector &other) const {
    throw std::bad_exception("Unimplemented");
  }

  inline value_type &operator[](size_type index) {
    assert(size_ > index);
    return arr_[index];
  }

  inline const value_type &operator[](size_type index) const {
    return operator[](index);
  }

  inline size_type size() { return size_; }
  void assign(size_type count, const T &value) {
    if (count < size_) {
      mem_.deallocate(arr_);
      arr_ = mem_.allocate(growth_factor * size_)];
      size_ *= growth_factor;
    }

    for (size_type i = 0; i < size_; ++i) {
      arr_[i] = value;
    }
  }

  inline value_type &at(size_type index) {
    if (size_ > index) {
      return arr_[index];
    } else {
      throw std::out_of_range("invalid index");
    }
  }

  inline const value_type &at(size_type index) const { return at(index); }

  inline const allocator &get_allocator() const { return mem_; }

protected:
  static const double growth_factor = 1.5;

  const allocator &mem_;
  size_type size_;
  value_type *arr_ = nullptr;

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
