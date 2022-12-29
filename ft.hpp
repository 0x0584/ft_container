#ifndef FT_HPP
#define FT_HPP

#include <assert.h>

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

template <typename T> struct allocator {
  using value_type = T;
  using pointer = value_type *;
  using const_pointer = const pointer;
  using reference = value_type &;
  using const_reference = const reference;
  using size_type = default_size_type;
  using difference_type = default_difference_type;

  explicit allocator() = default;

  inline virtual pointer allocate(size_type n) {
    value_type *mem = malloc(n * sizeof(value_type));
    if (mem == NULL) {
      throw std::bad_alloc("Really? System has ran out of memory")
    }
  }

  inline void deallocate(pointer &ptr) {
    assert(ptr != NULL);
    free(ptr);
  }

  inline virtual void construct(pointer &ptr, size_type n,
                                const_reference val) {
    deallocate(ptr);
    ptr = allocate(n);
    *ptr = val;
  }

  inline pointer address(reference ptr) const {
    throw std::exception_ptr("Unimplemented: Returns the actual address of ptr "
                             "even in presence of overloaded operator&");
  }

  template <class U> inline void destroy(pointer ptr) {
    allocator<U>::deallocate(reinterpret_cast<allocator<U>::pointer>(ptr));
  }

  inline size_type max_size() const { return AVAILABLE_THEORITICAL_MEMORY(size_type, value_type); }
};

template <typename T> struct allocator_no_throw : allocator<T> {
  using allocator = allocator<T>;
  inline pointer allocate(size_type n) override {
    try {
      return allocator::allocate(n);
    } catch (const std::bad_alloc &e) {
      return mem_ = NULL;
    }
  }

  inline bool construct(pointer &ptr, size_type n,
                        const_reference val) override {
    allocator::deallocate(ptr);
    if ((ptr = allocate(n)) != NULL) {
      *ptr = val;
      return true;
    } else {
      return false;
    }
  }
};

template <typename T> using contiguous_allocator = allocator_no_throw<T>;
template <typename T> class shreded_allocator {};

template <typename T> using default_allocator = contiguous_allocator<T>;

template <typename T, typename Allocator = allocator<T>> struct unique_ptr {
  using value_type = T;
  using raw_pointer = value_type *;
  using allocator_type = Allocator;

  static_assert(typeid(raw_pointer) == typeid(allocator_type::pointer),
                "You may have swapped Deleter<T[]> and Deleter<T>");

  unique_ptr() = default;
  unique_ptr(raw_pointer ptr, const allocator &alloc)
      : ptr_(ptr), mem_(alloc) {}
  ~unique_ptr() { release(); }

  unique_ptr(const unique_ptr &other) = delete;
  unique_ptr &operator=(const unique_ptr &ptr) = delete;

  unique_ptr(unique_ptr &&other) = default;
  unique_ptr &operator=(unique_ptr &&rhs) { return *this = std::move(ptr); }

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
