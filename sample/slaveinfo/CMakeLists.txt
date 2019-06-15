cmake_minimum_required(VERSION 3.5)

include_directories(/usr/local/include/soem/)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(SOURCES slaveinfo.c)
add_executable(slaveinfo ${SOURCES})
target_link_libraries(slaveinfo soem Threads::Threads)
install(TARGETS slaveinfo DESTINATION bin)
