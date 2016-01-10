cmake_minimum_required(VERSION 3.3)

# Generate Protobuf for Hive Communications
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})
protobuf_generate_cpp(HIVE_PROTO_CPP_SRCS HIVE_PROTO_CPP_HDRS HiveProtocol.proto)
protobuf_generate_python(HIVE_PROTO_PY HiveProtocol.proto)
add_custom_target(HiveProtobuf
        SOURCES ${HIVE_PROTO_CPP_SRCS} ${HIVE_PROTO_CPP_HDRS} ${HIVE_PROTO_PY})
add_custom_command(TARGET HiveProtobuf POST_BUILD
        COMMAND mkdir -p ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/HiveProtocol
        COMMAND cp ${HIVE_PROTO_CPP_SRCS} ${HIVE_PROTO_CPP_HDRS} ${HIVE_PROTO_PY} ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/HiveProtocol)