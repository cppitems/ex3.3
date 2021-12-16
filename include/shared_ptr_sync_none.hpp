#pragma once
#include <atomic>
#include <cstddef>
#include <functional>
#include <iostream>
#include <mutex>
#include <utility>

namespace sync_none {

template <typename T> class shared_ptr {
public:
  using element_type = T;
  using pointer = T *;

private:
  T *ptr = nullptr;
  using SharedOwners = long;
  SharedOwners *shared_owners = nullptr;

  void swap(shared_ptr &other) {
    std::swap(ptr, other.ptr);
    std::swap(shared_owners, other.shared_owners);
  }

public:
  // constructs empty shared_ptr
  shared_ptr() = default;

  // constructs empty shared_ptr from nullptr
  shared_ptr(std::nullptr_t) : shared_ptr() {}

  // construction from raw ptr (sharedowners == 0)
  shared_ptr(T *ptr) : ptr(ptr) { shared_owners = new SharedOwners{0}; }

  // copy construction: become a shared owner of other resource
  // 1. (increment shared_owners)
  shared_ptr(const shared_ptr &other)
      : ptr(other.ptr), shared_owners(other.shared_owners) {
    ++(*shared_owners);
  }

  // move construction:
  // 1. become (shared) owner of the other resource
  // 2. release other from (shared) ownership of other resource
  shared_ptr(shared_ptr &&other)
      : ptr(other.ptr), shared_owners(other.shared_owners) {
    other.ptr = nullptr;
    other.shared_owners = nullptr;
  }

  // copy assignment:
  // 1. release (shared) ownership of current resource
  // 2. become shared owner of other resource
  shared_ptr &operator=(const shared_ptr &other) {
    shared_ptr(other).swap(*this);
    return *this;
  }

  // move assignment:
  // 1. release (shared) ownership of current resource
  // 2. become (shared) owner of the other resource
  // 3. release other from (shared) ownership of other resource
  shared_ptr &operator=(shared_ptr &&other) {
    shared_ptr(std::move(other)).swap(*this);
    return *this;
  }

  // destructor:
  // 1. release (shared) ownership of current resource
  ~shared_ptr() {
    if (shared_owners) {
      if (--(*shared_owners) == -1) {
        delete shared_owners;
        delete ptr;
      }
    }
  }

  // reset:
  // 1. release ownership of current (shared) resource
  // 2. become new owner of passed raw resource
  void reset(T *ptr) { shared_ptr(ptr).swap(*this); }
  void reset(std::nullptr_t) { shared_ptr().swap(*this); }
  void reset() { shared_ptr().swap(*this); }

  // obtain number of handles to resource
  long use_count() const {
    if (shared_owners) {
      return *shared_owners + 1;
    } else {
      return 0;
    }
  }

  // comparison
  bool operator==(const shared_ptr &p) { return ptr == p.ptr; }
  bool operator!=(const shared_ptr &p) { return ptr != p.ptr; }

  // observers:
  T *get() { return ptr; }
  T &operator*() { return *get(); }
  T *operator->() { return get(); }
};
} // namespace sync_none