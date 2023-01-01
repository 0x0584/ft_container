#ifndef FT_HPP
#define FT_HPP

#include "debug.h"

#include <iterator>
#include <limits>
#include <memory>
#include <new>
#include <stdexcept>
#include <typeinfo>
#include <utility>

namespace ft {
using default_size_type = std::size_t;
using default_difference_type = std::ptrdiff_t;

template <typename T> static inline const T &as_const(T &obj) {
  return static_cast<const decltype(obj)>(obj);
}

#define CAPACITY(S_, V_) (std::numeric_limits<S_>::max() / sizeof(V_))

#define AVAILABLE_MEMORY(T1_, T2_, V_)                                         \
  (CAPACITY(T1_, V_) < CAPACITY(T2_, V_) ? CAPACITY(T1_, V_)                   \
                                         : CAPACITY(T2_, V_))

#define NEW_ALLOC NULL

template <typename T> struct allocator {
  using value_type = T;
  using pointer = value_type *;
  using const_pointer = const pointer;
  using reference = value_type &;
  using const_reference = const reference;
  using size_type = default_size_type;
  using difference_type = default_difference_type;
  using object_type = struct c_object;

  allocator(const allocator &) = delete;
  allocator(allocator &&) = default;

  allocator &operator=(const allocator &) = delete;
  allocator &operator=(allocator &&) = default;

  explicit allocator(size_type size = AVAILABLE_MEMORY(difference_type, size_type,
                                              value_type))
      : capacity_(size), allocated_(0), n_allocs_(0), n_frees_(0) {}

  virtual ~allocator() {}

  inline virtual pointer allocate(pointer hint) { return allocate(hint, 1); }

  inline virtual pointer allocate(pointer hint, size_type n) {
    assert(allocated_ < capacity_);
    const size_type footprint = n * sizeof(value_type);
    if (footprint + allocated_ > capacity_) {
      throw std::runtime_error(
          "cannot reserve memory: maximum allowed by allocator");
    }
    pointer mem =
        static_cast<pointer>(realloc(static_cast<void *>(hint), footprint));
    if (mem == NEW_ALLOC) {
      throw std::bad_alloc(); // out of memory
    }
    allocated_ += footprint;
    ++n_allocs_;
    return mem;
  }

  void deallocate(pointer &ptr) { deallocate(ptr, 1); }

  inline void deallocate(pointer ptr, size_type n) {
    // if (c_array_factory::validator(this, arr)) {
    //  throw std::runtime_error(
    //      "fatal error: attempt to free object with different allocator ");
    //}
    assert(allocated_ >= n * sizeof(value_type));
    if (ptr != NEW_ALLOC) {
      for (size_type i = 0; i < n; ++i) {
        ptr[i].~value_type();
      }
      free(ptr);
      allocated_ -= (n * sizeof(value_type));
      --n_frees_;
      ptr = NEW_ALLOC;
    }
  }

  inline virtual bool construct(pointer &ptr, size_type n,
                                const_reference val) {
    ptr = allocate(NEW_ALLOC, n);
    for (size_type i = 0; i < n; ++i) {
      ptr[i] = value_type(val);
    }
    return true;
  }

  inline virtual bool construct(pointer &ptr, size_type n, value_type &&val) {
    ptr = allocate(NEW_ALLOC, n);
    for (size_type i = 0; i < n; ++i) {
      ptr[i] = std::move(val);
    }
    return true;
  }

  inline pointer address(reference ptr) const {
    throw std::bad_exception("Unimplemented: Returns the actual address of ptr "
                             "even in presence of overloaded operator&");
  }

  template <class U = value_type>
  inline void destroy(pointer ptr, size_type n) {
    allocator<U>::deallocate(reinterpret_cast<allocator<U>::pointer>(ptr), n);
  }

  inline size_type num_allocations() const { return n_allocs_; }
  inline size_type num_frees() const { return n_frees_; }
  inline size_type max_size() const { return capacity_; }
  inline size_type max_spots() const { return capacity_ / sizeof(value_type); }
  inline size_type available() const {
    assert(capacity_ >= allocated_);
    return capacity_ - allocated_;
  }

  static pointer init() { return NEW_ALLOC; }

protected:
  size_type capacity_, allocated_, n_allocs_, n_frees_;
  /*
  template <typename Ptr>
  struct c_object {
   Ptr mem = NEW_ALLOC;
    c_object(const c_object &) = delete;
    c_object(c_object &&) = default;
    c_object &operator=(const c_object &) = delete;
    c_object &operator=(c_object &&) = default;
    value_type *pointer() { return mem; }

    inline size_type footprint() const { return n * sizeof(value_type); }

  protected:
    const allocator *alloc_;
  };
  template <typename Ptr>
  struct c_array : public c_object<Ptr> {
    size_type n = 0;

  protected:
    c_array(const allocator *alloc, Ptr mem, size_type n)
        : alloc_(alloc), mem(mem), n(n) {}
  };

  template <typename Ptr = allocator::pointer>
  struct c_array_facatory : protected c_array<Ptr> {
    static c_array<Ptr> construct(const allocator *alloc, Ptr ptr, size_type n)
  { return c_array(alloc, ptr, n);
    }

    static bool validator(const allocator *alloc, const c_array<Ptr> &a) {
      return alloc == a.alloc_;
    }

    static bool validator(const c_array &a, const c_array &b) {
      return a.alloc_ == b.alloc_;
    }
  };
  */
};

#define INVALID_ALLOCATION NULL
template <typename T> struct allocator_no_throw : public allocator<T> {
  using allocator = allocator<T>;

  inline pointer allocate(pointer hint) override {
    return allocate(hint, 1);
  }

  inline pointer allocate(pointer hint, size_type n) override {
    try {
      return allocator::allocate(hint, n);
    } catch (const std::bad_alloc &e) {
      return INVALID_ALLOCATION;
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
  using allocator = Allocator;
  using value_type = typename allocator::value_type;
  using pointer = typename allocator::pointer;

  static_assert(typeid(pointer) == typeid(allocator::pointer),
                "fatal: incompatible pointer type");

  unique_ptr() : mem_(allocator()) {}
  ~unique_ptr() { release(); }

  unique_ptr(unique_ptr &&) = default;
  unique_ptr(const unique_ptr &) = delete;

  unique_ptr(pointer ptr) : mem_(allocator()), ptr_(ptr) {}
  unique_ptr(pointer ptr, const allocator &alloc) : ptr_(ptr), mem_(alloc) {}

  unique_ptr &operator=(unique_ptr &&) = default;
  unique_ptr &operator=(const unique_ptr &) = delete;

  pointer get() const { return ptr_; }

  pointer operator*() {
    if (ptr_ == nullptr) {
      throw std::runtime_error(
          "fatal error: tried to access pointer set to nullptr");
    }
    return *ptr_;
  }
  pointer operator->() { return operator*(); }

  pointer release() {
    if (ptr_ != nullptr) {
      mem_.destroy(ptr);
    }
  }

  void reset(pointer ptr = NEW_ALLOC) {
    release(ptr);
    ptr_ = std::move(ptr);
  }

  void swap(unique_ptr &other) {
    pointer ptr = ptr_;
    ptr_ = other.ptr_;
    other.ptr_ = ptr;
  }

  allocator &get_allocator() { return &del_; }
  const allocator &get_allocator() const { return &del_; }

protected:
  const allocator &mem_;
  pointer ptr_ = nullptr;
};
} // namespace ft

#endif // FT_HPP
