cmake_minimum_required(VERSION 3.3)
if(PRIMARY_INC)
    option(INSTALL_TRNSMSSNSRV "Install the transmission server" ON)
    set(SOURCE_FILES main.cpp StreamProcess.cpp StreamProcess.hpp Util.cpp Util.hpp)
    add_definitions(-DBINDIR=\"${CMAKE_CURRENT_BINARY_DIR}\")
    add_executable(MediaCourier-TransmissionServer ${SOURCE_FILES} ${HIVE_PROTO_CPP_SRCS} ${HIVE_PROTO_CPP_HDRS})
    add_dependencies(MediaCourier-TransmissionServer HiveProtobuf MCNetRedir)
    target_link_libraries(MediaCourier-TransmissionServer -lpthread ${PROTOBUF_LIBRARIES})
    add_library(MCNetRedir SHARED socketredirect.c)

    if(INSTALL_TRNSMSSNSRV)
        install(TARGETS MediaCourier-TransmissionServer MCNetRedir
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/MediaCourier/)
    endif()
else()
    message(FATAL_ERROR "Your IDE is incorrectly configured. Please make it execute the CMakeLists.txt in the parent directory.")
endif()