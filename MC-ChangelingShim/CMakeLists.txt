cmake_minimum_required(VERSION 3.3)
if(PRIMARY_INC)
    set(SOURCE_FILES main.cpp)
    add_definitions(-DBINDIR=\"${CMAKE_CURRENT_BINARY_DIR}\")
    add_executable(MediaCourier-ChangelingShim ${SOURCE_FILES} ${HIVE_PROTO_CPP_SRCS} ${HIVE_PROTO_CPP_HDRS})
    add_dependencies(MediaCourier-ChangelingShim HiveProtobuf)
    target_link_libraries(MediaCourier-ChangelingShim ${PROTOBUF_LIBRARIES})

    install(TARGETS MediaCourier-ChangelingShim
            COMPONENT ChangelingShim
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
else()
    message(FATAL_ERROR "Your IDE is incorrectly configured. Please make it execute the CMakeLists.txt in the parent directory.")
endif()