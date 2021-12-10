add_executable(TestCAN test.cpp)

include_directories(${PROJECT_SOURCE_DIR}/include)
target_link_libraries(TestCAN SocketCAN)
