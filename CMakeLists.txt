cmake_minimum_required(VERSION 2.8.4)
project(rsys)

# find_package(Boost 1.56 EXACT REQUIRED)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-Wall)
endif()

add_subdirectory(src)
add_subdirectory(ex)
add_subdirectory(tests)






