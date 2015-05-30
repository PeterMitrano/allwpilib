cmake_minimum_required(VERSION 2.8)
project(gz_msgs)

#copied from GazeboUtils.cmake
MACRO (APPEND_TO_CACHED_STRING _string _cacheDesc)
  FOREACH (newItem ${ARGN})
    SET (${_string} "${${_string}} ${newItem}" CACHE INTERNAL ${_cacheDesc} FORCE)
  ENDFOREACH (newItem ${ARGN})
  #STRING(STRIP ${${_string}} ${_string})
ENDMACRO (APPEND_TO_CACHED_STRING)

include (FindPkgConfig)
#if (PKG_CONFIG_FOUND)
#  pkg_check_modules(GAZEBO gazebo)
#endif()

#find_package(gazebo REQUIRED)

#check to see if protobuf is installed correctly
find_package(Protobuf REQUIRED)
if (NOT PROTOBUF_FOUND)
  BUILD_ERROR ("Missing: Google Protobuf (libprotobuf-dev)")
endif()
if (NOT PROTOBUF_PROTOC_EXECUTABLE)
  BUILD_ERROR ("Missing: Google Protobuf Compiler (protobuf-compiler)")
endif()
if (NOT PROTOBUF_PROTOC_LIBRARY)
  BUILD_ERROR ("Missing: Google Protobuf Compiler Library (libprotoc-dev)")
endif()

#list all proto files used
get_filename_component(PROTO_DIR proto ABSOLUTE)
set(msgs
  "${PROTO_DIR}/bool.proto"
  "${PROTO_DIR}/driver-station.proto"
  "${PROTO_DIR}/float64.proto"
  "${PROTO_DIR}/joystick.proto"
)
set(TAG "[======DEBUG======] ")
set(PROTO_SRCS)
set(PROTO_HDRS)
foreach(FIL ${msgs})
  get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
  get_filename_component(FIL_WE ${FIL} NAME_WE)

  list(APPEND PROTO_SRCS "${CMAKE_CURRENT_BINARY_DIR}/msgs/${FIL_WE}.pb.cc")
  list(APPEND PROTO_HDRS "${CMAKE_CURRENT_BINARY_DIR}/msgs/${FIL_WE}.pb.h")

  add_custom_command(
    OUTPUT
      "${CMAKE_CURRENT_BINARY_DIR}/msgs/${FIL_WE}.pb.cc"
      "${CMAKE_CURRENT_BINARY_DIR}/msgs/${FIL_WE}.pb.h"
    COMMAND  ${PROTOBUF_PROTOC_EXECUTABLE}
    ARGS --cpp_out=${CMAKE_CURRENT_BINARY_DIR}/msgs --proto_path=${PROTO_DIR} ${ABS_FIL} 
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
configure_file(msgs.h.in msgs/msgs.h)


file(GLOB_RECURSE COM_SRC_FILES msgs/*.cc)
include_directories(msgs ${Boost_INCLUDE_DIR} ${GAZEBO_INCLUDE_DIRS})
add_library(${PROJECT_NAME} SHARED ${PROTO_SRCS} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} ${GAZEBO_LIBRARIES})
