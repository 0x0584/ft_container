#ifndef FT_HPP
#define FT_HPP

#include "debug.h"

#include <iterator>
#include <limits>
#include <memory>
#include <stdexcept>
#include <typeinfo>
#include <utility>

namespace ft {
using default_size_type = std::size_t;
using default_difference_type = std::ptrdiff_t;

template <typename T> static inline const T &as_const(T &obj) {
  return static_cast<const decltype(obj)>(obj);
}

#define AVAILABLE_THEORITICAL_MEMORY(S_, V_)                                   \
  (std::numeric_limits<static_cast<S_>>::max() / sizeof(V_))

#define AVAILABLE_MEMORY(T1_, T2_, V_)                                         \
  (AVAILABLE_THEORITICAL_MEMORY(T1_, V_) <                                     \
           AVAILABLE_THEORITICAL_MEMORY(T2_, V_)                               \
       ? AVAILABLE_THEORITICAL_MEMORY(T1_, V_)                                 \
       : AVAILABLE_THEORITICAL_MEMORY(T2_, V_))

#define NEW_ALLOC nullptr

template <typename T> struct allocator {
  using value_type = T;
  using pointer = value_type *;
  using const_pointer = const pointer;
  using reference = value_type &;
  using const_reference = const reference;
  using size_type = default_size_type;
  using difference_type = default_difference_type;

  explicit allocator() = default;

  inline virtual pointer allocate(pointer hint, size_type n) {
    if (n > mem_.max_size()) {
      throw std::runtime_error(
          "cannot reserve memory: maximum allowed by allocator");
    }
    value_type *mem = realloc(hint, n * sizeof(value_type));
    if (mem == NULL) {
      throw std::bad_alloc("Really? System has ran out of memory")
    }
    return mem;
  }

  inline void deallocate(pointer &ptr) {
    assert(ptr != NULL);
    free(ptr);
    ptr = NEW_ALLOC;
  }

  inline virtual bool construct(pointer &ptr, size_type n,
                                const_reference val) {
    // FIXME: deallocate(ptr);
    ptr = allocate(ptr, n);
    *ptr = val;
    return true;
  }

  inline pointer address(reference ptr) const {
    throw std::exception_ptr("Unimplemented: Returns the actual address of ptr "
                             "even in presence of overloaded operator&");
  }

  template <class U> inline void destroy(pointer ptr) {
    allocator<U>::deallocate(reinterpret_cast<allocator<U>::pointer>(ptr));
  }

  inline size_type max_size() const {
    return AVAILABLE_MEMORY(size_type, difference_type, value_type);
  }
};

template <typename T> struct allocator_no_throw : public allocator<T> {
  using allocator = allocator<T>;
  inline pointer allocate(pointer hint, size_type n) override {
    try {
      return allocator::allocate(hint, n);
    } catch (const std::bad_alloc &e) {
      return mem_ = NULL;
    }
  }

  inline bool construct(pointer &ptr, size_type n, const_reference val) {
    try {
      allocator<T>::allocate(ptr, n);
      return true;
    }
    else {
      return false;
    }
  }
};

template <typename T> class shreded_allocator : public allocator<T> {};
template <typename T> using default_allocator = allocator<T>;

template <typename T, typename Allocator = allocator<T>> struct unique_ptr {
  using value_type = T;
  using raw_pointer = value_type *;
  using allocator_type = Allocator;

  static_assert(typeid(raw_pointer) == typeid(allocator_type::pointer),
                "fatal: incompatible pointer type");

  unique_ptr() : mem_(allocator_type()) {}

  unique_ptr(unique_ptr &&) = default;
  unique_ptr(const unique_ptr &) = delete;

  unique_ptr(raw_pointer ptr) : mem_(allocator_type()), ptr_(ptr) {}
  unique_ptr(raw_pointer ptr, const allocator_type &alloc)
      : ptr_(ptr), mem_(alloc) {}
  ~unique_ptr() { release(); }

  unique_ptr(const unique_ptr &other) = delete;


  unique_ptr(unique_ptr &&other) = default;

  unique_ptr &operator=(unique_ptr &&) = default;
  unique_ptr &operator=(const unique_ptr &) = delete;
  raw_pointer operator*() { return ptr_; }
  raw_pointer operator->() { return ptr_; }

  raw_pointer get() const { return ptr_; }
  raw_pointer release() {
    if (ptr_ != nullptr) {
      mem_.deallocate(ptr_);
    }
  }

  void reset(raw_pointer ptr = nullptr) {
    release(ptr);
    ptr_ = std::move(ptr);
  }

  void swap(unique_ptr &other) {
    raw_pointer ptr = ptr_;
    ptr_ = other.ptr_;
    other.ptr_ = ptr;
  }

  allocator_type &get_allocator() { return &del_; }
  const allocator_type &get_allocator() const { return &del_; }

protected:
  const allocator_type &mem_;
  raw_pointer ptr_ = nullptr;
};
} // namespace ft

#endif // FT_HPP