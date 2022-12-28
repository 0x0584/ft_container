#ifndef FT_HPP
#define FT_HPP

#include <iterator>
#include <limits>
#include <memory>
#include <stdexcept>
#include <typeinfo>

namespace ft {
using default_size_type = std::size_t;
using default_difference_type = std::ptrdiff_t;

template <typename T> class allocator {
public:
  using value_type = T;
  using pointer = value_type *;
  using const_pointer = const pointer;
  using reference = value_type &;
  using const_reference = const reference;
  using size_type = default_size_type;
  using difference_type = default_difference_type;

  pointer allocate(size_type n) {
    throw std::exception_ptr("Unimplemented: allocate sizeof(size_type) * n");
  }

  void deallocate(pointer ptr, size_type n) {
    throw std::exception_ptr("Unimplemented: delete memory");
  }

  void construct(pointer p, const_reference val) {
    throw std::exception_ptr("Unimplemented: copy-ctor");
  }

  pointer address(reference x) const {
    throw std::exception_ptr("Unimplemented: Returns the actual address of x "
                             "even in presence of overloaded operator&");
  }

  template <class U> void destroy(allocator<U>::pointer ptr) { ptr->~U(); }

  size_type max_size() {
    return std::numeric_limits<size_type>::max() / sizeof(value_type);
  }

protected:
public:
};

template <typename T> struct Deleter {
  using raw_pointer = T *;

  Deleter() = default;
  void operator(T *ptr) { delete ptr; }
};

template <typename T> struct Deleter<T[]> {
  using raw_pointer = const T *;

  Deleter() = default;
  void operator(T *ptr) const { delete[] ptr; }
};

template <typename T, typename Deleter = Deleter<T>> struct unique_ptr {
  using size_type = default_size_type;
  using type = T;
  using raw_pointer = type *;
  using deleter_type = Deleter;

  static_assert(typeid(raw_pointer) == typeid(raw_pointer),
                "You may have swapped Deleter<T[]> and Deleter<T>");

  unique_ptr(raw_pointer ptr) : ptr_(ptr), del_(Deleter()) {}
  ~unique_ptr() { release(); }

  unique_ptr(const unique_ptr &other) = delete;
  unique_ptr &operator=(const unique_ptr &ptr) = delete;

  unique_ptr(unique_ptr &&other) = default;
  unique_ptr &operator=(unique_ptr &&rhs) { return *this = std::move(ptr); }

  raw_pointer get() const { return ptr_; }
  raw_pointer release() {
    if (ptr_ != nullptr) {
      del_(ptr_);
    }
  }

  void reset(raw_pointer ptr = nullptr) {
    release(ptr);
    ptr_ = std::move(ptr);
  }

  void swap(unique_ptr& other) { 
    raw_pointer ptr = ptr_;
    ptr_ = other.ptr_;
    other.ptr_ = ptr;
  }

  Deleter &get_deleter() { return del_; }
  const Deleter &get_deleter() const { return del_; }

private:
  Deleter &del_;
  raw_pointer ptr_;
};
} // namespace ft

#endif // FT_HPP
