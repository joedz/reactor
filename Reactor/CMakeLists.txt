cmake_minimum_required(VERSION 3.0)
project(MyProject)

# 设置编译器选项
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加库文件的源文件
set(LIB_SOURCES
    src/Channel.cpp
    src/Epoll.cpp
    src/InetAddress.cpp
)

# 添加可执行文件的源文件
set(EXECUTABLE_SOURCES
    test/server.cpp
    test/client.cpp
)

# 添加库文件的头文件路径
include_directories(
    ./
)

# 生成库文件
add_library(Reactor ${LIB_SOURCES})

# 设置可执行文件输出目录为./bin
# 设置库文件输出目录为./lib
SET(LIBRARY_OUTPUT_PATH    ${PROJECT_SOURCE_DIR}/lib)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)  
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)  

# 生成可执行文件
add_executable(server test/server.cpp)
add_executable(client test/client.cpp)

# 链接库文件
target_link_libraries(server Reactor)
target_link_libraries(client Reactor)