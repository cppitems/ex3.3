#include "testasserts.hpp"
#include <shared_ptr.hpp>

struct Widget {
  double m = 42;
};

int main() {

  using T = Widget;

  { // compare to same type
    auto sp1 = shared_ptr(new T{});
    auto sp2 = shared_ptr(new T{});
    TEST_ASSERT(sp1 != sp2,
                "compare unequal to other unique_ptr not working as expected");
  }

  { // compare to nullptr
    auto sp = shared_ptr(new T{});
    TEST_ASSERT(sp != nullptr,
                "compare unequal to nullptr not working as expected");
    sp = nullptr;
    TEST_ASSERT(sp == nullptr,
                "compare equal to nullptr not working as expected");
  }  

  return 0;
}