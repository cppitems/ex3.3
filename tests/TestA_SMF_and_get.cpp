#include "testasserts.hpp"
#include <shared_ptr.hpp>

struct Widget {
  double m = 42;
};

int main() {

  using T = Widget;
  using SP = shared_ptr<T>;

  { // nested types
    using expected_pointer = T *;
    using expected_element = T;
    static_assert(
        std::is_same<SP::pointer, expected_pointer>::value,
        "Typedef \"pointer\" not defined in unique_ptr (or unexpected type)");
    static_assert(std::is_same<SP::element_type, expected_element>::value,
                  "Typedef \"element_type\" not defined in unique_ptr (or "
                  "unexpected type)");
  }

  { // .get() member
    static_assert(
        std::is_same<decltype(std::declval<SP>().get()), SP::pointer>::value,
        ".get() did not return expected type");
  }

  { // constructor from raw pointer
    const auto ptr = new T{};

    auto sp = shared_ptr(ptr);

    TEST_ASSERT(ptr == sp.get(),
                "shared pointer not constructed from raw pointer");
    TEST_ASSERT(sp.use_count() == 1, "unexpected use_count");
  }

  { // copy constructor
    const auto ptr1 = new T{};
    auto sp1 = shared_ptr(ptr1);

    auto sp2 = shared_ptr(sp1);

    TEST_ASSERT(sp1.use_count() == 2, "unexpected use_count");

    TEST_ASSERT(
        sp2.get() == ptr1,
        "copy-constructed shared pointer not pointing to original resource");
    TEST_ASSERT(sp2.use_count() == 2, "unexpected use_count");
  }

  { // copy assignment
    auto sp1 = shared_ptr(new Widget{});
    auto sp2 = shared_ptr(new Widget{});

    TEST_ASSERT(sp1.use_count() == 1, "unexpected use_count");
    TEST_ASSERT(sp2.use_count() == 1, "unexpected use_count");

    const auto ptr1 = sp1.get();

    sp2 = sp1;

    TEST_ASSERT(sp1.get() == ptr1,
                "copy-assigned object does not point to copied-from object");
    TEST_ASSERT(sp1.use_count() == 2, "unexpected use_count");

    TEST_ASSERT(sp2.get() == ptr1,
                "original object does not point to original resouece anymore");
    TEST_ASSERT(sp2.use_count() == 2, "unexpected use_count");
  }

  { // copy self-assignment
    auto sp1 = shared_ptr(new Widget{});
    sp1 = *(&(sp1)); // trick the compiler to not warn self
  }

  { // move constructor
    auto sp1 = shared_ptr(new T{});
    const auto ptr1 = sp1.get();

    auto sp2 = shared_ptr(std::move(sp1));

    TEST_ASSERT(sp1.get() == nullptr,
                "moved-from object not pointing to nullptr");
    TEST_ASSERT(sp1.use_count() == 0, "unexpedted use_count");

    TEST_ASSERT(sp2.get() == ptr1,
                "move-constructed object does not manage moved-from resource");
    TEST_ASSERT(sp2.use_count() == 1, "unexpedted use_count");
  }

  { // move assignment
    auto sp1 = shared_ptr(new T{});
    auto sp2 = shared_ptr(new T{});
    const auto ptr1 = sp1.get();

    sp2 = std::move(sp1);

    TEST_ASSERT(sp1.get() == nullptr, "move-assigned object points to nullptr");
    TEST_ASSERT(sp1.use_count() == 0, "unexpected use_count");

    TEST_ASSERT(sp2.get() == ptr1,
                "move-assigned object does not point to moved-from object");
    TEST_ASSERT(sp2.use_count() == 1, "unexpected use_count");
  }

  return 0;
}