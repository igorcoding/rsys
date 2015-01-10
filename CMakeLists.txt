cmake_minimum_required(VERSION 2.8.4)
project(rsys)

# find_package(Boost 1.56 EXACT REQUIRED)

set (SRC "src")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(HEADER_FILES ${SRC}/core/rsys.h
                 ${SRC}/core/svd.h
                 ${SRC}/core/rbm.h
                 ${SRC}/core/item_score.h
                 ${SRC}/core/config/config.h
                 ${SRC}/core/config/svd_config.h
                 ${SRC}/data_structures/mvector.h
                 ${SRC}/data_structures/imatrix.h
                 ${SRC}/data_structures/matrix.h
                 ${SRC}/data_structures/sparse_matrix.h
                 ${SRC}/data_structures/mexception.h
                 ${SRC}/util/traits.h)

set(SOURCE_FILES ${SRC}/data_structures/mexception.cc
                 ${SRC}/main.cc)
add_executable(rsys ${HEADER_FILES} ${SOURCE_FILES})
