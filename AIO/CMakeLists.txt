project(aio_wrapper LANGUAGES C CXX)

add_library(${PROJECT_NAME} STATIC AIOWrapper.cpp AIOReader.cpp AIOWriter.cpp AIONotifier.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<INSTALL_INTERFACE:include>"
        )

find_package(Threads REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE -lrt Threads::Threads)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
