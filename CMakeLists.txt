cmake_minimum_required(VERSION 3.14)
project(py_guard C)

set(CMAKE_C_STANDARD 99)


set(py2pyx_src py2pyx.c boot.c bytecode_helper.c py_module_helper.c transform.c  py_guard.c)


include_directories(D:\\python\\python-3.7.3\\include)

add_library(py2pyx SHARED ${py2pyx_src})

link_directories(D:\\python\\python-3.7.3) #添加动态连接库的路径
target_link_libraries(py2pyx  python37) #添加libmxnet.so


add_executable(py_guard main.c)