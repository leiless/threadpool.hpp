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
