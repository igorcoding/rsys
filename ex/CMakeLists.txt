set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
include_directories(../src)
add_executable(rsystest main.cc)
target_link_libraries(rsystest rsys)