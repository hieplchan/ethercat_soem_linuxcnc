cmake_minimum_required(VERSION 3.5)

include_directories(/usr/local/include/soem/)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(SOURCES soem_linuxcnc_embedded.c)
add_executable(soem_linuxcnc_embedded ${SOURCES})
target_link_libraries(soem_linuxcnc_embedded soem Threads::Threads)
install(TARGETS soem_linuxcnc_embedded DESTINATION bin)
