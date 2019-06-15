cmake_minimum_required(VERSION 3.5)

include_directories(/usr/local/include/soem/)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(SOURCES simple_test.c)
add_executable(simple_test ${SOURCES})
target_link_libraries(simple_test soem Threads::Threads)
install(TARGETS simple_test DESTINATION bin)
