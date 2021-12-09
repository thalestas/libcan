cmake_minimum_required(VERSION 3.10)

project(libcan)

include_directories(include)

#file(GLOB SOURCE "src/*.cpp")
file(GLOB SOURCE "test/*.cpp")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(testcan ${SOURCE})


