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
    arr_ = mem_.allocate(NEW_ALLOC, count);
  }

  vector(size_type count, const allocator &alloc)
      : mem_(alloc), arr_(), size_(count),
        arr_(mem_.allocate(NEW_ALLOC, count)) {}

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
      : mem_(allocator()), arr_(mem_.allocate(nullptr, count)), size_(count),
        capacity_(count) {}

  explicit vector(size_type count, const value_type &value,
                  const allocator &alloc)
      : mem_(alloc), arr_(mem_.allocate(nullptr, count)), size_(count),
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
      scale_capacity_(new_cap);
    }
  }

  // equivalent to ctor with intialisation
  void assign(size_type count, const T &value) {
    if (count < capacity_) {
      scale_capacity_(count);
      size_ = count;
    }
    for (size_type i = 0; i < count; ++i) {
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
  /////////////////////////////////////////////////////////////////////////////

  iterator insert(const_iterator pos, const value_type &value) {
    return insert(pos, 1, value);
  }

  iterator insert(const_iterator pos, size_type count,
                  const value_type &value) {
    assert(iterator_factory<const_iterator>::range_validator(pos, size_));
    assert(iterator_factory<const_iterator>::range_validator(pos, cbegin()));
    assert(iterator_factory<const_iterator>::range_validator(pos, cend()));

    size_type size = size_ + count;
    if (size > capacity_) {
      scale_capacity_(size);
    }

    difference_type index = std::distance(cbegin(), pos);
    for (difference_type i = index; i < size_; ++i) {
      arr_[i + count] = arr_[i];
    }
    for (size_type i = 0; i < count; ++i) {
      arr_[index + i] = value;
    }
    size_ = size;
    return iterator_factory<iterator>::construct(std::make_pair(arr_, size_),
                                                 index);
  }

  template <class InputIt>
  iterator insert(const_iterator pos, InputIt first, InputIt last) {
    assert(iterator_factory<const_iterator>::range_validator(pos, size_));
    assert(iterator_factory<const_iterator>::range_validator(pos, cbegin()));
    assert(iterator_factory<const_iterator>::range_validator(pos, cend()));
    assert(iterator_factory<const_iterator>::range_validator(first, last));

    size_type count = std::distance(first, last) + 1, size = size_ + count;
    if (size > capacity_) {
      scale_capacity_(size);
    }

    difference_type index = std::distance(cbegin(), pos);
    for (difference_type i = index; i < size_; ++i) {
      arr_[i + count - 1] = arr_[i];
    }
    size_type i = 0;
    for (InputIt i = 0; i < count; ++i) {
      arr_[index + i] = *first++;
    }
    assert(first == last);
    size_ = size;
    return iterator_factory<iterator>::construct(std::make_pair(arr_, size_),
                                                 index);
  }

  /////////////////////////////////////////////////////////////////////////////

  iterator erase(iterator pos) {
    assert(iterator_factory<const_iterator>::range_validator(pos, size_));

    if (empty() || pos == end()) {
      return end();
    }

    difference_type index = std::distance(cbegin(), pos);
    for (difference_type i = index; i < size_ - 1; ++i) {
      arr_[i] = arr_[i + 1];
    }
    --size_;
    return iterator_factory<iterator>::construct(std::make_pair(arr_, size_),
                                                 index);
  }

  iterator erase(iterator first, iterator last) {
    assert(iterator_factory<const_iterator>::range_validator(first, size_));
    assert(iterator_factory<const_iterator>::range_validator(first, last));

    difference_type index = std::distance(begin(), first),
                    range = std::distance(first, last);
    for (difference_type i = index; i < size_ - 1; ++i) {
      arr_[i] = arr_[i + range];
    }
    size_ -= range;
    if (empty() || last == end()) {
      return iterator_factory<iterator>::construct(size_);
    } else {
      return iterator_factory<iterator>::construct(std::make_pair(arr_, size_),
                                                   index);
    }
  }

  /////////////////////////////////////////////////////////////////////////////

  void resize(size_type count, value_type value = value_type()) {
    if (count == size_) {
      return;
    } else if (count > size_) {
      if (count > capacity_) {
        scale_capacity_(count);
      }
      for (size_type i = count - size_; i != 0;) {
        --i;
        arr_[size_ + i] = value;
      }
    } else {
      for (size_type i = size_ - count; i != 0;) {
        --i;
        arr_[size_ + i].~value_type();
      }
    }
  }

  void swap(vector &other) {
    std::swap(arr_, other.arr_);
    std::swap(capacity_, other.capacity_);
    std::swap(size_, other.size_);
  }

  /////////////////////////////////////////////////////////////////////////////

  void push_back(const value_type &value) {
    if (size_ == capacity_) {
      grow_capacity_();
    }
    arr_[++size_] = value;
  }

  void pop_back() {
    if (not empty()) {
      --size_;
    }
  }

  /////////////////////////////////////////////////////////////////////////////

  // TODO: at
  value_type &at(size_type index) {
    if (size_ > index) {
      return arr_[index];
    } else {
      throw std::out_of_range("invalid index");
    }
  }

  const value_type &at(size_type index) const {
    if (size_ > index) {
      return arr_[index];
    } else {
      throw std::out_of_range("invalid index");
    }
  }

  // TODO: front
  value_type &front() {
    if (not empty()) {
      return *arr_;
    } else {
      throw std::out_of_range("vector is empty");
    }
  }

  const value_type &front() const {
    if (not empty()) {
      return *arr_;
    } else {
      throw std::out_of_range("vector is empty");
    }
  }

  // TODO: back
  value_type &back() {
    if (not empty()) {
      return *(arr_ + size_ - 1);
    } else {
      throw std::out_of_range("vector is empty");
    }
  }

  const value_type &back() const {
    if (not empty()) {
      return *(arr_ + size_ - 1);
    } else {
      throw std::out_of_range("vector is empty");
    }
  }

protected:
  static const double growth_factor = 1.5;

  const allocator &mem_;
  pointer arr_;
  size_type size_, capacity_;

  inline void scale_capacity_(size_type new_cap) {
    assert(new_cap != 0);
    arr_ = mem_.allocate(arr_ == nullptr ? NEW_ALLOC : arr_, new_cap);
    capacity_ = new_cap;
  }

  inline void grow_capacity_() {
    scale_capacity_(static_cast<size_type>(growth_factor * size_));
  }

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

    iterator(const iterator &other)
        : data_(other.data_), index_(other.index_), count_(other.count_),
          flag_(other.flag_) {}

    iterator &operator=(const iterator &other) { return *this = other; }

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
    const bool flag_;

    explicit iterator(size_type count)
        : data_(nullptr), index_(count), size_(count), flag_(true) {}

    iterator(std::pair<value_type *, size_type> data, size_type index)
        : data_(data.first), index_(index), size_(data.second), flag_(false) {}
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

  iterator begin() {
    return iterator_factory<iterator>::construct(std::make_pair(arr_, size_),
                                                 0);
  }
  const_iterator cbegin() const {
    return iterator_factory<const_iterator>::construct(
        std::make_pair(arr_, size_), 0);
  }

  iterator end() { return iterator_factory<iterator>::construct(size_); }
  const_iterator cend() const {
    return iterator_factory<const_iterator>::construct(size_);
  }

  reverese_iterator rbegin() {
    return iterator_factory<reverse_iterator>::construct(
        std::make_pair(arr_, size_), 0);
  }
  const_reverese_iterator rcbegin() const {
    return iterator_factory<const_reverese_iterator>::construct(
        std::make_pair(arr_, size_), 0);
  }

  reverese_iterator rend() {
    return iterator_factory<reverese_iterator>::construct(size_);
  }
  const_reverese_iterator rcend() const {
    return iterator_factory<const_reverese_iterator>::construct(size_);
  }

protected:
  template <typename Iterator> class iterator_factory : protected Iterator {
    explicit iterator_factory(size_type count) : Iterator(count) {}
    iterator_factory(std::pair<value_type *, size_type> data, size_type index)
        : Iterator(data, index) {}

  public:
    static Iterator construct(size_type count) {
      return reinterpret_cast<Iterator>(iterator_factory<Iterator>(count));
    }
    static Iterator construct(std::pair<value_type *, size_type> data,
                              size_type index) {
      return reinterpret_cast<Iterator>(
          iterator_factory<Iterator>(data, index));
    }

    // FIXME: check data as well
    static size_type range_validator(const Iterator &it, size_type count) {
      return count == count_;
    }

    static size_type range_validator(const Iterator &it1, const Iterator &it2) {
      return it1.count == it2.count_;
    }
  };

  // FIXME: factory should hold the instance array by design
};

namespace impl {
template <class T, class Alloc, typename Predicat>
static bool
operator_handler(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs,
                 vector<T, Alloc>::size_type size, Predicat predicat) {
  for (vector<T, Alloc>::size_type i = 0; i < size; ++i) {
    if (not predicat(lhs[i], rhs[i])) {
      return false;
    }
  }
  return true;
}
} // namespace impl

template <class T, class Alloc>
static bool operator==(const vector<T, Alloc> &lhs,
                       const vector<T, Alloc> &rhs) {
  return lhs.size() == rhs.size()
             ? impl::operator_handler(lhs, rhs, lhs.size(), std::equal<T>)
             : false;
}

template <class T, class Alloc>
static bool operator!=(const vector<T, Alloc> &lhs,
                       const vector<T, Alloc> &rhs) {
  return not operator==(lhs, rhs);
}

template <class T, class Alloc>
static bool operator<(const vector<T, Alloc> &lhs,
                      const vector<T, Alloc> &rhs) {
  return impl::operator_handler(lhs, rhs, std::min(lhs.size(), rhs.size()),
                                std::less<T>);
}

template <class T, class Alloc>
bool operator<=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) {
  return impl::operator_handler(lhs, rhs, std::min(lhs.size(), rhs.size()),
                                std::less_equal<T>);
}

template <class T, class Alloc>
bool operator>(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) {
  return impl::operator_handler(lhs, rhs, std::min(lhs.size(), rhs.size()),
                                std::greater<T>);
}

template <class T, class Alloc>
bool operator>=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) {
  return impl::operator_handler(lhs, rhs, std::min(lhs.size(), rhs.size()),
                                std::greater_equal<T>);
}
} // namespace ft
#endif // VECTOR_HPP
