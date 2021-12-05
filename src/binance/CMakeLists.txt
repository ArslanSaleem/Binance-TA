project(BINANCE-TA)
cmake_minimum_required(VERSION 3.2)

add_compile_options(-std=c++17)

INCLUDE_DIRECTORIES(${JSON_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${LIBWEBSOCKETS_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIR})


set(SRC_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/binacpp_utils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/binacpp.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/binacpp_websocket.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/binacpp_logger.cpp
)

add_library (libbinance ${SRC_FILES})
# add_dependencies(libbinance libjsoncpp libwebsockets)


install(TARGETS libbinance ARCHIVE DESTINATION lib )
