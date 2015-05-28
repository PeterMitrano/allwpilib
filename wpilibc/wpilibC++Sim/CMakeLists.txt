cmake_minimum_required(VERSION 2.8)
project(WPILibSim)

#copied from GazeboUtils.cmake
MACRO (APPEND_TO_CACHED_STRING _string _cacheDesc)
  FOREACH (newItem ${ARGN})
    SET (${_string} "${${_string}} ${newItem}" CACHE INTERNAL ${_cacheDesc} FORCE)
  ENDFOREACH (newItem ${ARGN})
  #STRING(STRIP ${${_string}} ${_string})
ENDMACRO (APPEND_TO_CACHED_STRING)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wformat=2 -Wall -Wextra -Wno-unused-parameter -fPIC -std=c++1y -DFRC_SIMULATOR")

get_filename_component(HAL_API_INCLUDES ../../hal/include REALPATH)
get_filename_component(NWT_API_INCLUDES ../../networktables/cpp/include REALPATH)
add_subdirectory(../wpilibC++ wpilibC++)

include (FindPkgConfig)
if (PKG_CONFIG_FOUND)
  pkg_check_modules(GAZEBO gazebo)
endif()

find_package(gazebo REQUIRED)

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
get_filename_component(PROTO_DIR ../../simulation/frc_gazebo_plugins/msgs/proto ABSOLUTE)
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

  list(APPEND PROTO_SRCS "${CMAKE_CURRENT_BINARY_DIR}/../include/simulation/msgs/${FIL_WE}.pb.cc")
  list(APPEND PROTO_HDRS "${CMAKE_CURRENT_BINARY_DIR}/../include/simulation/msgs/${FIL_WE}.pb.h")

  add_custom_command(
    OUTPUT
      "${CMAKE_CURRENT_BINARY_DIR}/../include/simulation/msgs/${FIL_WE}.pb.cc"
      "${CMAKE_CURRENT_BINARY_DIR}/../include/simulation/msgs/${FIL_WE}.pb.h"
    COMMAND  ${PROTOBUF_PROTOC_EXECUTABLE}
    ARGS --cpp_out=${CMAKE_CURRENT_BINARY_DIR}/../include/simulation/msgs/ --proto_path=${PROTO_DIR} ${ABS_FIL} 
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
set (SIM_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include/simulation")
file(MAKE_DIRECTORY "${SIM_INCLUDE_DIR}/msgs/")
configure_file(${SIM_INCLUDE_DIR}/msgs.h.in
                ${SIM_INCLUDE_DIR}/msgs/msgs.h)

file(GLOB_RECURSE SRC_FILES src/*.cpp ../../networktables/cpp/lib/share/*.cpp
                                      ../../networktables/cpp/lib/Athena/*.cpp
                                      ../../hal/lib/Athena/Task.cpp
                                      ../../hal/lib/Athena/Semaphore.cpp
                                      ../../hal/lib/Athena/cpp/Synchronized.cpp)

file(GLOB_RECURSE COM_SRC_FILES ../wpilibC++/src/*.cpp)

include_directories(include/ ../../wpilibc/wpilibC++/include
                    ../../networktables/cpp/include
                    ../../hal/include
                    ${Boost_INCLUDE_DIR} ${GAZEBO_INCLUDE_DIRS}) # ${NWT_API_INCLUDES}
add_library(WPILibSim SHARED ${PROTO_SRCS} ${SRC_FILES} ${COM_SRC_FILES})
target_link_libraries(WPILibSim ${GAZEBO_LIBRARIES}  -fPIC) # NetworkTables
INSTALL(TARGETS WPILibSim LIBRARY DESTINATION src COMPONENT src)
INSTALL(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX} COMPONENT headers)
# ni_emb
# HAL NWT
