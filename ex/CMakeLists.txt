set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -std=c++14")
include_directories(../src)
add_executable(rsys_ex rsys_ex.cc)
target_link_libraries(rsys_ex rsys)

add_executable(exporters_ex exporters_ex.cc)
target_link_libraries(exporters_ex rsys)