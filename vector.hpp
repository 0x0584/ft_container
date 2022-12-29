#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "ft.hpp"

// TODO: avoid code duplication

namespace ft {
// the programmer must ensure that alloctor has same typing
template <typename T, typename Allocator = default_allocator<T>> class vector {
public:
  using value_type = T;
  using allocator = Allocator;
  using size_type = allocator::size_type;
  using difference_type = allocator::difference_type;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = allocator::pointer;
  using const_pointer = allocator::const_pointer;
  using reverese_iterator = std::reverse_iterator<struct iterator>;
  using const_reverese_iterator = std::reverse_iterator<struct const_iterator>;

  ~vector() { delete[] arr_; }

  vector() : mem_(allocator()), arr_(nullptr), size_(0), capacity_(0) {}

  explicit vector(const allocator &alloc)
      : mem_(alloc), arr_(nullptr), size_(0), capacity_(0) {}

  explicit vector(size_type count)
      : mem_(allocator()), size_(count), capacity_(count) {
    assert(count != 0); // FIXME: handle allocation
    arr_ = mem_.allocate(count);
  }

  vector(size_type count, const allocator &alloc)
      : mem_(alloc), arr_() size_(count), arr_(mem_.allocate(count)) {}

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
      : mem_(allocator()), arr_(mem_.allocate(count)), size_(count),
        capacity_(count) {}

  explicit vector(size_type count, const value_type &value,
                  const allocator &alloc)
      : mem_(alloc), arr_(mem_.allocate(count)), size_(count),
        capacity_(count) {}

  template <typename InputIt>
  vector(InputIt begin, InputIt end) : vector(std::distance(begin, end)) {}

  template <typename InputIt, typename OtherAllocator>
  vector(InputIt begin, InputIt end, const OtherAllocator &alloc)
      : vector(std::distance(begin, end), alloc) {}

  /////////////////////////////////////////////////////////////////////////////

  inline bool const empty() const { return arr_ == nullptr || size_ == 0; }
  inline size_type size() const { return size_; }
  inline size_type capacity() const { return capacity_; }
  inline const allocator &get_allocator() const { return mem_; }
  inline size_type max_size() const { mem_.max_size(); }

  // TODO: data
  inline const value_type *data() { return arr_; }
  inline const value_type *data() const { return arr_; }

  void reserve(size_type new_cap) {
    if (new_cap > capacity_) {
      if (new_cap > mem_.max_size()) {
        throw std::runtime_error(
            "cannot reserve memory: maximum allowed by allocator");
      }
      mem_.deallocate(arr_);
      arr_ = mem_.allocate(new_cap);
      capacity_ = new_cap;
    }
  }

  // equivalent to ctor with intialisation
  void assign(size_type count, const T &value) {
    if (count < size_) {
      reserve(count);
      size_ = count;
    }
    for (size_type i = 0; i < size_; ++i) {
      arr_[i] = value;
    }
  }

  void clear() {
    for (size_type i = 0; i < size_; ++i) {
      arr[i].~value_type();
    }
    size_ = 0;
  }

  /////////////////////////////////////////////////////////////////////////////

  iterator insert(const_iterator pos, size_type count, const T &value) {}

  iterator insert(const_iterator pos, const T &value) {
    difference_type index = std::distance(begin(), pos);
    return iterator(std::make_pair(arr_, size_), index));
  }

  template <class InputIt>
  iterator insert(const_iterator pos, InputIt first, InputIt last) {}

  /////////////////////////////////////////////////////////////////////////////

  // TODO: at
  inline value_type &at(size_type index) {
    if (size_ > index) {
      return arr_[index];
    } else {
      throw std::out_of_range("invalid index");
    }
  }

  inline const value_type &at(size_type index) const {
    if (size_ > index) {
      return arr_[index];
    } else {
      throw std::out_of_range("invalid index");
    }
  }

  // TODO: front
  inline value_type &front() {
    if (not empty()) {
      return *arr_;
    } else {
      throw std::out_of_range("vector is empty");
    }
  }

  inline const value_type &front() const {
    if (not empty()) {
      return *arr_;
    } else {
      throw std::out_of_range("vector is empty");
    }
  }

  // TODO: back
  inline value_type &back() {
    if (not empty()) {
      return *(arr_ + size_ - 1);
    } else {
      throw std::out_of_range("vector is empty");
    }
  }

  inline const value_type &back() const {
    if (not empty()) {
      return *(arr_ + size_ - 1);
    } else {
      throw std::out_of_range("vector is empty");
    }
  }

protected:
  static const double growth_factor = 1.5;

  const allocator &mem_;
  allocator::pointer arr_;
  size_type size_, capacity_;

public:
  // TODO: op=
  inline vector &operator=(vector &&vec) {
    throw std::bad_exception("Unimplemented");
  }

  inline const vector &operator=(const vector &other) const {
    throw std::bad_exception("Unimplemented");
  }

  // TODO: op[]
  inline value_type &operator[](size_type index) {
    assert(size_ > index);
    return arr_[index];
  }

  inline const value_type &operator[](size_type index) const {
    return operator[](index);
  }

  /////////////////////////////////////////////////////////////////////////////

  struct iterator
      : std::iterator<random_access_iterator_tag, value_type, difference_type> {

    explicit iterator(size_type count)
        : data_(nullptr), index_(count), size_(count), flag_(true) {}

    iterator(std::pair<value_type *, size_type> data, size_type index)
        : data_(data.first), index_(index), size_(data.second), flag_(false) {}

    difference_type operator-(const iterator &other) const {
      return other.index_ - index_;
    }

    iterator &operator++() {
      if (flag_ || index_ == size_) {
        throw std::out_of_range("invalid iterator: cannot increment");
      } else {
        index_++;
        return *this;
      }
    }

    iterator operator++(int) {
      iterator retval = *this;
      operator++();
      return retval;
    }

    iterator &operator--() {
      if (!flag || index_ == 0) {
        throw std::out_of_range("invalid iterator: cannot decrement");
      } else {
        index_--;
        return *this;
      }
    }

    iterator operator--(int) {
      iterator retval = *this;
      operator--();
      return retval;
    }

    bool operator==(const iterator &other) const {
      return index_ == other.index_;
    }

    bool operator!=(const iterator &other) const { return !operator==(); }

    // TODO: op*
    reference operator*() {
      if (flag_ || index_ == size_) {
        throw std::out_of_range("invalid iterator: cannot access");
      } else {
        return *(data_ + index_);
      }
    }

    const_reference operator*() const {
      if (flag_ || index_ == size_) {
        throw std::out_of_range("invalid iterator: cannot access");
      } else {
        return *(data_ + index_);
      }
    }

  protected:
    value_type *data_;
    size_type index_;
    const size_type size_;
    const bool flag_
  };

  struct const_iterator
      : std::iterator<random_access_iterator_tag, value_type, difference_type> {

    explicit const_iterator(size_type count)
        : data_(nullptr), index_(count), size_(count), flag_(true) {}

    const_iterator(std::pair<value_type *, size_type> data, size_type index)
        : data_(data.first), index_(index), size_(data.second), flag_(false) {}

    const_iterator &operator++() {
      if (flag_ || index_ == size_) {
        throw std::out_of_range("invalid iterator: cannot increment");
      }
      index_++;
      return *this;
    }

    const_iterator operator++(int) {
      const_iterator retval = *this;
      operator++();
      return retval;
    }

    const_reference operator*() const {
      if (flag_ || index_ == size_) {
        throw std::out_of_range("invalid iterator: cannot access");
      } else {
        return *(data_ + index_);
      }
    }

    bool operator==(const const_iterator &other) const {
      return index_ == other.index_;
    }

    bool operator!=(const const_iterator &other) const { return !operator==(); }

  protected:
    const value_type *data_;
    size_type index_;
    const size_type size_;
    const bool flag_;
  };

  iterator begin() { return iterator(std::make_pair(arr_, size_), 0); }
  const_iterator cbegin() const {
    return const_iterator(std::make_pair(arr_, size_), 0);
  }

  iterator end() { return iterator(size_); }
  const_iterator cend() const { return const_iterator(size_); }

  reverese_iterator rbegin() {
    return reverese_iterator(iterator(std::make_pair(arr_, size_), 0));
  }
  const_reverese_iterator rcbegin() const {
    return const_reverese_iterator(iterator(std::make_pair(arr_, size_), 0));
  }

  reverese_iterator rend() { return reverese_iterator(iterator(size_)); }
  const_reverese_iterator rcend() const {
    return const_reverese_iterator(iterator(size_));
  }
};
} // namespace ft
#endif // VECTOR_HPP
