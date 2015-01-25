cmake_minimum_required(VERSION 2.8.4)
project(rsys)

# find_package(Boost 1.56 EXACT REQUIRED)

set(SRC "src")
set(SRC_RSYS "src/rsys")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(HEADER_FILES ${SRC_RSYS}/recommender.h
                 ${SRC_RSYS}/model.h
                 ${SRC_RSYS}/svd.h
                 ${SRC_RSYS}/rbm.h
                 ${SRC_RSYS}/item_score.h

                 ${SRC_RSYS}/config/config.h
                 ${SRC_RSYS}/config/svd_config.h

                 ${SRC_RSYS}/ensemblers/ensembler.h

                 ${SRC_RSYS}/data_sources/data_source.h
                 ${SRC_RSYS}/data_sources/mvector.h
                 ${SRC_RSYS}/data_sources/imatrix.h
                 ${SRC_RSYS}/data_sources/matrix.h
                 ${SRC_RSYS}/data_sources/sparse_matrix.h
                 ${SRC_RSYS}/data_sources/mexception.h

                 ${SRC_RSYS}/util/base_iterator.h
                 ${SRC_RSYS}/util/traits.h)

set(SOURCE_FILES ${SRC_RSYS}/data_sources/mexception.cc
                 ${SRC_RSYS}/core.cc)
add_library(rsys SHARED ${HEADER_FILES} ${SOURCE_FILES})

add_executable(rsystest ${SRC}/main.cc)
target_link_libraries(rsystest rsys)


set(VKRSYS ${SRC}/vkrsys)

set(VK_HEADERS )
set(VK_SOURCES ${VKRSYS}/main.cc)


add_executable(vkrsys ${VK_HEADERS} ${VK_SOURCES})
target_link_libraries(vkrsys rsys)

