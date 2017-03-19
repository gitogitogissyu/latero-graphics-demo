cmake_minimum_required(VERSION 2.8)

if(APPLE)


else(APPLE)

# without this option, we cannnot build on cygwin and lunux(ubuntu 16)
set (CMAKE_CXX_STANDARD 11)
endif(APPLE)

Project(demo)
add_subdirectory(src)
