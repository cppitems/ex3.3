#include <shared_ptr.hpp>

#include "testasserts.hpp"

struct Widget {
  double m = 42;
};

int main() {

  using T = Widget;

  { // reset(arg) and reset() and  reset(nullptr)
    auto sp = shared_ptr(new T{});
    auto *ptr = new T{};
    sp.reset(ptr);
    TEST_ASSERT(sp.get() == ptr, "reset(ptr) NOT resetting resource to ptr");
    TEST_ASSERT(sp.use_count() == 1, "unexpected use_count");
    sp.reset();
    TEST_ASSERT(sp.get() == nullptr, "reset() NOT resetting to nullptr");
    TEST_ASSERT(sp.use_count() == 0, "unexpected use_count");
    sp.reset(nullptr);
    TEST_ASSERT(sp.get() == nullptr, "reset() NOT resetting to nullptr");
    TEST_ASSERT(sp.use_count() == 0, "unexpected use_count");    
  }

  { // assign from nullptr
    auto sp = shared_ptr(new T{});
    TEST_ASSERT(sp.use_count() == 1, "unexpected use_count");
    sp = nullptr;
    TEST_ASSERT(sp.get() == nullptr, "object which was assigned using a "
                                     "nullptr does not point to nullptr");
    TEST_ASSERT(sp.use_count() == 0, "unexpected use_count");
  }

  return 0;
}