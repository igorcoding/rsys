cmake_minimum_required(VERSION 2.8.4)
project(rsys)

# find_package(Boost 1.56 EXACT REQUIRED)

set(SRC "src")
set(SRC_CORE "src/core")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(HEADER_FILES ${SRC_CORE}/rsys.h
                 ${SRC_CORE}/svd.h
                 ${SRC_CORE}/rbm.h
                 ${SRC_CORE}/item_score.h

                 ${SRC_CORE}/config/config.h
                 ${SRC_CORE}/config/svd_config.h

                 ${SRC_CORE}/data_sources/data_source.h
                 ${SRC_CORE}/data_sources/simple_ds.h
                 ${SRC_CORE}/data_sources/mvector.h
                 ${SRC_CORE}/data_sources/imatrix.h
                 ${SRC_CORE}/data_sources/matrix.h
                 ${SRC_CORE}/data_sources/sparse_matrix.h
                 ${SRC_CORE}/data_sources/mexception.h

                 ${SRC_CORE}/util/base_iterator.h
                 ${SRC_CORE}/util/traits.h)

set(SOURCE_FILES ${SRC_CORE}/data_sources/mexception.cc
                 ${SRC}/main.cc)
add_executable(rsys ${HEADER_FILES} ${SOURCE_FILES})
