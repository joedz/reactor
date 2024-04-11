cmake_minimum_required(VERSION 3.0)
project(MyProject)

# 设置编译器选项
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加库文件的源文件 -> 动态库/静态库
set(LIB_SOURCES
    Reactor/Channel.cpp
    Reactor/Epoll.cpp
    Reactor/InetAddress.cpp
    Reactor/log/LogAppender.cpp
    Reactor/log/LogFormat.cpp
    Reactor/log/LogLevel.cpp
    Reactor/log/Logger.cpp
    Reactor/log/LogManager.cpp
    Reactor/thread/Thread.cpp
)

# aux_source_directory({PROJECT_SOURCE_DIR}/Reactor/log SRC_Log)

# 添加库文件的头文件路径
include_directories(
    ${PROJECT_SOURCE_DIR}/Reactor/
    ${PROJECT_SOURCE_DIR}/Reactor/utils
    ${PROJECT_SOURCE_DIR}/Reactor/log
    ${PROJECT_SOURCE_DIR}/Reactor/thread

)

# 添加库文件搜索路径
# link_directories()

# 添加可执行文件的源文件
# set(EXECUTABLE_SOURCES
#     test/server.cpp
#     test/client.cpp
#     test/log/test_log.cpp
#     test/testHelloworld.cpp
#     test/thread/test_stl_thread.cpp
#     test/thread/test_thread_big_shu.cpp
# )

## 添加宏
function(add_macro targetname)
    ## 获取源文件
    get_target_property(sourcefile "${targetname}" SOURCES)

    #添相对路径的宏
    list(APPEND defs "__RELATIVE_PATH__=\"${sourcefile}\"")

    #重新设置源文件的编译参数
    set_property(
        SOURCE "${sourcefile}"
        PROPERTY COMPILE_DEFINITIONS ${defs}
    )
endfunction()


## 执行函数
function(execute_add_executable targetname srcs depends libs)
    ## 添加可行性文件
    add_executable(${targetname} ${srcs})
    ## 添加依赖
    add_dependencies(${targetname} ${depends})
    ## 添加宏
    add_macro(${targetname})
    ## 添加连接库
    target_link_libraries(${targetname} ${libs})
endfunction()

# 生成库文件
add_library(Reactor SHARED ${LIB_SOURCES})

# 设置依赖的库文件
set(depends
    Reactor
)

# 设置可执行文件输出目录为./bin
# 设置库文件输出目录为./lib
SET(LIBRARY_OUTPUT_PATH    ${PROJECT_SOURCE_DIR}/lib)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

execute_add_executable(server "test/server.cpp" Reactor "${depends}")
execute_add_executable(client "test/client.cpp" Reactor "${depends}")
execute_add_executable(test_log "test/log/test_log.cpp" Reactor "${depends}")
execute_add_executable(testHelloworld "test/testHelloworld.cpp" Reactor "${depends}")
execute_add_executable(test_stl_thread "test/thread/test_stl_thread.cpp" Reactor "${depends}")
execute_add_executable(test_thread_big_shu "test/thread/test_thread_big_shu.cpp" Reactor "${depends}")
execute_add_executable(test_my_thead "test/thread/test_my_thead.cpp" Reactor "${depends}")
execute_add_executable(test_func "test/thread/test_func.cpp" Reactor "${depends}")
