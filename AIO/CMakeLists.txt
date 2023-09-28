project(aio_wrapper LANGUAGES C CXX)

add_library(${PROJECT_NAME} STATIC AIOWrapper.cpp AIOReader.cpp AIOWriter.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<INSTALL_INTERFACE:include>"
        )

target_link_libraries(${PROJECT_NAME} PRIVATE -lrt)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
