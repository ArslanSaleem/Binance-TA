project(BINANCE-TA)
cmake_minimum_required(VERSION 3.2)

add_compile_options(-std=c++17)

set(BINANCE_LIB ${CMAKE_CURRENT_SOURCE_DIR}/src/binance)
set(JSON_LIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/jsoncpp-1.8.3)
set(JSON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/jsoncpp-1.8.3/include)

# GRPC and Protocol Buffers libraries location
list(APPEND CMAKE_PREFIX_PATH "/opt/grpc" "/opt/protobuf")

# Cmake find modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(Protobuf REQUIRED)
find_package(GRPC REQUIRED)
find_package(Websocket REQUIRED)

set(PROTOS 
    ${CMAKE_CURRENT_SOURCE_DIR}/protos/ticker.proto
)

set(PROTO_SRC_DIR ${CMAKE_CURRENT_BINARY_DIR}/proto-src)
file(MAKE_DIRECTORY ${PROTO_SRC_DIR})
include_directories(${PROTO_SRC_DIR})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_SRC_DIR} ${PROTOS})
grpc_generate_cpp(GRPC_SRCS GRPC_HDRS ${PROTO_SRC_DIR} ${PROTOS})

# Sub Directories
add_subdirectory(${JSON_LIB})
add_subdirectory(${BINANCE_LIB})
include_directories(${BINANCE_LIB})
include_directories(${JSON_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${LIBWEBSOCKETS_INCLUDE_DIR})


# Building server
add_executable(ticker_server
    ${CMAKE_CURRENT_SOURCE_DIR}/server.cpp
    ${PROTO_SRCS}
    ${GRPC_SRCS}
)
target_link_libraries(ticker_server
    gRPC::grpc++_reflection
    protobuf::libprotobuf
    libbinance
    libjsoncpp
    libwebsockets
)

# Building client
add_executable(ticker_client
    ${CMAKE_CURRENT_SOURCE_DIR}/client.cpp
    ${PROTO_SRCS}
    ${GRPC_SRCS}
)
target_link_libraries(ticker_client
    gRPC::grpc++_reflection
    protobuf::libprotobuf
    libbinance
    libjsoncpp
)
