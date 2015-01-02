cmake_minimum_required(VERSION 2.8.4)
project(rsys)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(HEADER_FILES algs/svd.h
                 math/mvector.h
                 math/matrix.h
                 math/sparse.h
                 math/mexception.h
                 util/traits.h)

set(SOURCE_FILES ${HEADER_FILES}
                 math/mexception.cc
                 main.cc)
add_executable(rsys ${SOURCE_FILES})