cmake_minimum_required(VERSION 3.3)

# Generate Protobuf for Hive Communications
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})
protobuf_generate_cpp(HIVE_PROTO_CPP_SRCS HIVE_PROTO_CPP_HDRS HiveProtocol.proto)
protobuf_generate_python(HIVE_PROTO_PY HiveProtocol.proto)

add_custom_target(HiveProtobuf ALL
        SOURCES ${HIVE_PROTO_CPP_SRCS} ${HIVE_PROTO_CPP_HDRS} ${HIVE_PROTO_PY})
add_custom_command(TARGET HiveProtobuf POST_BUILD
        COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/HiveProtocol
        COMMAND cp ${HIVE_PROTO_CPP_SRCS} ${HIVE_PROTO_CPP_HDRS} ${HIVE_PROTO_PY} ${CMAKE_CURRENT_BINARY_DIR}/HiveProtocol)

option(INSTALL_HIVEPROTO "Install the Hive protocol buffer sources" ON)
if(INSTALL_HIVEPROTO)
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/HiveProtocol
            DESTINATION "${CMAKE_INSTALL_LIBDIR}/MediaCourier/HiveProtobuf"
            PATTERN "*.py" EXCLUDE)
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/HiveProtocol
            DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/MediaCourier/HiveProtobuf"
            PATTERN "*.py")
endif()