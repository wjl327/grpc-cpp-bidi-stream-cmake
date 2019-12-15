cmake_minimum_required(VERSION 3.3)
project(grpc_helloworld)

find_package(Protobuf REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

include_directories(protos)
link_libraries(grpc++_unsecure grpc gpr ${PROTOBUF_LIBRARY})

set(GENERATED_PROTOBUF_FILES protos/helloworld.pb.h protos/helloworld.pb.cc
        protos/helloworld.grpc.pb.h protos/helloworld.grpc.pb.cc)

set(DIR_SRCS src/multithread_greeter_stream.cpp)

add_executable(greeter_server src/greeter_server.cc ${GENERATED_PROTOBUF_FILES})
add_executable(greeter_client src/greeter_client.cc ${GENERATED_PROTOBUF_FILES})
add_executable(greeter_stream_server src/greeter_stream_server.cc ${GENERATED_PROTOBUF_FILES})
add_executable(greeter_stream_client src/greeter_stream_client.cc ${GENERATED_PROTOBUF_FILES})
add_executable(multithread_stream_server src/multithread_stream_server.cc ${DIR_SRCS} ${GENERATED_PROTOBUF_FILES})
add_executable(multithread_stream_client src/multithread_stream_client.cc ${DIR_SRCS} ${GENERATED_PROTOBUF_FILES})