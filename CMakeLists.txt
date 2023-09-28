cmake_minimum_required(VERSION 3.8)

project(asyncio LANGUAGES CXX)

add_executable(${PROJECT_NAME} main.cpp)

find_package(Threads REQUIRED)
add_subdirectory(AIO)
add_subdirectory(File)
target_link_libraries(${PROJECT_NAME} PRIVATE aio_wrapper file_handle Threads::Threads)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
