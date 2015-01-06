cmake_minimum_required(VERSION 2.8.4)
project(rsys)

# find_package(Boost 1.56 EXACT REQUIRED)

set (SRC "src")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(HEADER_FILES ${SRC}/algs/rsys.h
                 ${SRC}/algs/svd.h
                 ${SRC}/math/mvector.h
                 ${SRC}/math/matrix.h
                 ${SRC}/math/sparse.h
                 ${SRC}/math/mexception.h
                 ${SRC}/util/traits.h)

set(SOURCE_FILES ${SRC}/math/mexception.cc
                 ${SRC}/main.cc)
add_executable(rsys ${HEADER_FILES} ${SOURCE_FILES})
