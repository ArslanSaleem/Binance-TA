project(jsoncpp-1.8.3)
cmake_minimum_required(VERSION 3.2)

add_compile_options(-std=c++17)

INCLUDE_DIRECTORIES(${JSON_INCLUDE_DIR})

set(SRC_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/jsoncpp.cpp 
)

add_library (libjsoncpp ${SRC_FILES})
