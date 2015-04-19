set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories(../src)

set(HEADER_FILES matrix_test.h

                )

add_executable(rsys_test test.cc ${HEADER_FILES})
target_link_libraries(rsys_test cpptest)
target_link_libraries(rsys_test rsys)