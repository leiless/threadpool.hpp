cmake_minimum_required(VERSION 3.2)

project(threadpool.hpp.test)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra -g -DDEBUG -O0)
#add_compile_options(-Wall -Wextra -O2 -DASSERTF_DISABLE)

add_executable(
        ${PROJECT_NAME}
        threadpool.hpp
        test.cpp
)

# see: https://stackoverflow.com/questions/1620918/cmake-and-libpthread
find_package(Threads REQUIRED)
if (THREADS_HAVE_PTHREAD_ARG)
    target_compile_options(${PROJECT_NAME} PUBLIC "-pthread")
endif()
if (CMAKE_THREAD_LIBS_INIT)
    target_link_libraries(${PROJECT_NAME} "${CMAKE_THREAD_LIBS_INIT}")
endif()

