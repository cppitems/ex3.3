cmake_minimum_required(VERSION 3.0)

# set up the project for building all tests
project(Tests LANGUAGES CXX)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# sync options for the tests
set(SYNC_NONE 1)
set(SYNC_ATOMICS 2)
set(SYNC_CRITICAL_SECTIONS 3)
set(SYNC_METHOD ${SYNC_NONE} CACHE STRING "selects the include file for shared_ptr via preprocessor defines")

set(OPT2 "Default" CACHE STRING "Helpstring")
# make a custom target for usage with make
# all tests can be built using `make myTests`
add_custom_target(myTests)

set(MY_TEST_NAMES "")
list(APPEND MY_TEST_NAMES "TestA_SMF_and_get")
list(APPEND MY_TEST_NAMES "TestB_assign_reset")
list(APPEND MY_TEST_NAMES "TestC_compare")
list(APPEND MY_TEST_NAMES "TestD_threads")

foreach(MY_TEST_NAME ${MY_TEST_NAMES})
  add_executable(${MY_TEST_NAME} ${MY_TEST_NAME}.cpp testasserts.hpp)
  target_link_libraries(${MY_TEST_NAME} PRIVATE ${MY_LIBRARY})
  target_link_libraries(${MY_TEST_NAME} PRIVATE Threads::Threads)
  target_compile_definitions(${MY_TEST_NAME} PRIVATE SYNC_METHOD=${SYNC_METHOD})
  # Add to test framework, so test can be built with `make myTests`
  add_dependencies(myTests ${MY_TEST_NAME})
  add_test(NAME ${MY_TEST_NAME} COMMAND ${MY_TEST_NAME})
endforeach(MY_TEST_NAME ${MY_TEST_NAMES})



