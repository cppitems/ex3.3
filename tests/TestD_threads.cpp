#include "testasserts.hpp"
#include <cassert>
#include <shared_ptr.hpp>
#include <thread>

struct Widget {
  long m = 42;
};
using T = Widget;

int main() {

  auto task = [](shared_ptr<T> sp) {
    TEST_ASSERT(sp.use_count() >= 2, "unexpected use_count");

    auto sp1 = sp;
    TEST_ASSERT(sp.use_count() >= 3, "unexpected use_count");

    auto sp2 = sp1;
    TEST_ASSERT(sp.use_count() >= 4, "unexpected use_count");

    auto sp3 = std::move(sp2);
    TEST_ASSERT(sp.use_count() >= 4, "unexpected use_count");

    auto sp4(std::move(sp3));
    TEST_ASSERT(sp.use_count() >= 4, "unexpected use_count");
  };

  auto sp = shared_ptr(new Widget{});

  assert(sp.use_count() == 1);

  std::thread t1(task, sp);
  std::thread t2(task, sp);
  std::thread t3(task, sp);
  std::thread t4(task, sp);

  t1.join();
  t2.join();
  t3.join();
  t4.join();

  TEST_ASSERT(sp.use_count() == 1, "unexpected use_count");

  return 0;
}