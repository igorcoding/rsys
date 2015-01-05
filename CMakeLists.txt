cmake_minimum_required(VERSION 2.8.4)
project(rsys)

# find_package(Boost 1.56 EXACT REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(HEADER_FILES src/algs/svd.h
                 src/math/mvector.h
                 src/math/matrix.h
                 src/math/sparse.h
                 src/math/mexception.h
                 src/util/traits.h)

set(SOURCE_FILES ${HEADER_FILES}
                 # math/mexception.cc
                 src/main.cc)
add_executable(rsys ${SOURCE_FILES})
