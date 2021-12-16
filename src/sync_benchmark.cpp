#include "shared_ptr_sync_atomics.hpp"
#include "shared_ptr_sync_critical_sections.hpp"
#include "shared_ptr_sync_none.hpp"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <thread>

using Clock = std::chrono::steady_clock;
using Duration = std::chrono::duration<double>;

struct Widget {
  long m = 1;
};
using T = Widget;

template <typename SP> __attribute__((noinline)) long func(SP sp) {
  return sp->m;
}

template <typename SP> auto benchmark() {

  auto task = [](SP sp, long &counter, long N) {
    long local_counter = 0;
    for (int n = 0; n != N; ++n) {
      local_counter += func(sp);
    }
    counter = local_counter;
  };

  auto sp = SP(new Widget{});

  long count1 = 0;
  long count2 = 0;
  long count3 = 0;
  long count4 = 0;
  const long N = 1e5;

  // start clock

  std::thread t1(task, sp, std::ref(count1), N);
  std::thread t2(task, sp, std::ref(count2), N);
  std::thread t3(task, sp, std::ref(count3), N);
  std::thread t4(task, sp, std::ref(count4), N);

  auto start = Clock::now();

  t1.join();
  t2.join();
  t3.join();
  t4.join();

  long count = count1 + count2 + count3 + count4;

  // end clock
  auto stop = Clock::now();
  auto duration = Duration(stop - start);

  std::cout << "count = " << count << " (expected count == " << N * 4 << ")"
            << std::endl;

  return duration.count();
}

int main() {
//   auto time_sync_none = benchmark<sync_none::shared_ptr<T>>();
//   std::cout << std::left << std::setw(40)
//             << "Benchmark using SYNC_NONE:" << std::setw(34) << std::right
//             << std::setw(20) << time_sync_none << " s" << std::endl;
  auto time_sync_atomics = benchmark<sync_atomics::shared_ptr<T>>();
  std::cout << std::left << std::setw(40)
            << "Benchmark using SYNC_ATOMICS:" << std::setw(34) << std::right
            << std::setw(20) << time_sync_atomics << " s" << std::endl;
  auto time_sync_critical_sections =
      benchmark<sync_critical_sections::shared_ptr<T>>();
  std::cout << std::left << std::setw(40)
            << "Benchmark using SYNC_CRITICAL_SECTIONS:" << std::setw(34)
            << std::right << std::setw(20) << time_sync_critical_sections << " s"
            << std::endl;
}