cmake_minimum_required(VERSION 2.8)
project(gz_msgs)

#list all proto files used
get_filename_component(PROTO_DIR src/main/proto ABSOLUTE)
set(msgs
  "${PROTO_DIR}/bool.proto"
  "${PROTO_DIR}/driver-station.proto"
  "${PROTO_DIR}/float64.proto"
  "${PROTO_DIR}/frc_joystick.proto"
)

set(PROTO_SRCS)
set(PROTO_HDRS)
set(MSGS_HEADER "${CMAKE_CURRENT_BINARY_DIR}/msgs.h")
foreach(FIL ${msgs})
  get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
  get_filename_component(FIL_WE ${FIL} NAME_WE)

  list(APPEND PROTO_SRCS "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.cc")
  list(APPEND PROTO_HDRS "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.h")

  add_custom_command(
    OUTPUT
      "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.cc"
      "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.h"
    COMMAND  ${PROTOBUF_PROTOC_EXECUTABLE}
    ARGS --cpp_out=${CMAKE_CURRENT_BINARY_DIR} --proto_path=${PROTO_DIR} ${ABS_FIL}
    COMMENT "compiling ${ABS_FIL}"
    VERBATIM)
endforeach()

set_source_files_properties(${PROTO_SRCS} ${PROTO_HDRS} PROPERTIES GENERATED TRUE)


###############################################
#Generating msgs.h
#create the message_headers and keep it in cache
set (message_headers "" CACHE INTERNAL "Include dirs description")

#add includes to the msgs.h file
foreach (hdr ${PROTO_HDRS})
  string (REPLACE "${CMAKE_CURRENT_BINARY_DIR}/" "" hdr ${hdr})
  APPEND_TO_CACHED_STRING(message_headers
    "Message Types" "#include \"${hdr}\"\n")
endforeach()
configure_file(msgs.h.in ${MSGS_HEADER})

file(GLOB_RECURSE COM_SRC_FILES msgs/*.cc)
include_directories(msgs ${PROTOBUF_INCLUDE_DIR})
if (WIN32)
  add_library(${PROJECT_NAME} ${PROTO_SRCS} ${SRC_FILES})
else()
  add_library(${PROJECT_NAME} SHARED ${PROTO_SRCS} ${SRC_FILES})
endif()

target_link_libraries(${PROJECT_NAME} ${PROTOBUF_LIBRARIES})


#we want msgs to be used by the plugins and wpilibC++Sim, so we used a gz_msgs-config.cmake
#install
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME})
install(FILES ${MSGS_HEADER} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/msgs)
install(FILES ${PROTO_HDRS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/msgs)

get_filename_component(CONF_INCLUDE_DIRS ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR} ABSOLUTE)
get_filename_component(CONF_LIBRARY_DIRS ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME} ABSOLUTE)
set (CONF_LIBRARY ${PROJECT_NAME})

#fill in all the variables in the cmake file
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/gz_msgs-config.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/cmake/gz_msgs-config.cmake @ONLY)

#install gz_msgs.cmake
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/cmake/gz_msgs-config.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME})

#install gz_msgs to wpilib folder as well
install(TARGETS ${PROJECT_NAME} DESTINATION $ENV{HOME}/wpilib/simulation/plugins/)
