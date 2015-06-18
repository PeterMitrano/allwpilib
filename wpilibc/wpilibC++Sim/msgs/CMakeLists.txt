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

#check to see if protobuf is installed correctly
find_package(Protobuf REQUIRED)
if (NOT PROTOBUF_FOUND)
	MESSAGE ("Missing: Google Protobuf (libprotobuf-dev)")
endif()
if (NOT PROTOBUF_PROTOC_EXECUTABLE)
	MESSAGE ( "Missing: Google Protobuf Compiler (protobuf-compiler)")
endif()
if (NOT PROTOBUF_PROTOC_LIBRARY)
	MESSAGE ("Missing: Google Protobuf Compiler Library (libprotoc-dev)")
endif()

#list all proto files used
get_filename_component(PROTO_DIR proto ABSOLUTE)
set(msgs
  "${PROTO_DIR}/bool.proto"
  "${PROTO_DIR}/driver-station.proto"
  "${PROTO_DIR}/float64.proto"
  "${PROTO_DIR}/joystick.proto"
)

#on windows we produce .dlls with no prefix
if(WIN32)
  SET(CMAKE_FIND_LIBRARY_PREFIXES "")
  SET(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll")
else()
  SET(CMAKE_FIND_LIBRARY_PREFIXES "lib")
  SET(CMAKE_FIND_LIBRARY_SUFFIXES ".so" ".a")
endif()

set(PROTO_SRCS)
set(PROTO_HDRS)
set(MSGS_HEADER "${CMAKE_CURRENT_BINARY_DIR}/msgs/msgs.h")
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
configure_file(msgs.h.in ${MSGS_HEADER})

file(GLOB_RECURSE COM_SRC_FILES msgs/*.cc)
include_directories(msgs ${PROTOBUF_INCLUDE_DIR})
add_library(${PROJECT_NAME} SHARED ${PROTO_SRCS} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} ${PROTOBUF_LIBRARIES})

#install
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME})
install(FILES ${MSGS_HEADER} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/msgs)
install(FILES ${PROTO_HDRS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/msgs)

#we want msgs to be used by the plugins and wpilibC++Sim, so we used a gz_msgs-config.cmake
#first step is find out where Cmake is installed
include(GNUInstallDirs)

set (CONF_INCLUDE_DIRS ${PROJECT_BINARY_DIR})
get_filename_component(LIB_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR} ABSOLUTE)
set (CONF_LIB_DIRS ${LIB_INSTALL_DIR})

#fill in all the variables in the cmake file 
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/gz_msgs-config.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/cmake/gz_msgs-config.cmake @ONLY)
#install gz_msgs.cmake
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/cmake/gz_msgs-config.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/..) #pretty sure this only works on windows, and also it's a hack
