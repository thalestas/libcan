add_executable(TestCAN test.cpp)
add_executable(device1 device1.cpp)
add_executable(device2 device2.cpp)

include_directories(${PROJECT_SOURCE_DIR}/include)
add_library(utils utils.cpp)
target_link_libraries(TestCAN SocketCAN)
target_link_libraries(device1 SocketCAN utils pthread)
target_link_libraries(device2 SocketCAN utils)
