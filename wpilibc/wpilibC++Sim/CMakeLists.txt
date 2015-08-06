cmake_minimum_required(VERSION 2.8)
project(WPILibC++Sim)

if (WIN32)
  #used in common headers like Notifier.h
  add_definitions(-DFRC_SIMULATOR)

  #temporary until we build dlls
  add_definitions(-DBUILDING_STATIC_LIBS)

  # XXX: remove this once network tables is replaced
  set(PTHREAD_INCLUDE_DIR "C:/Users/peter/gz-ws/pthread-w32/include")
  set(PTHREAD_LIBRARY "C:/Users/peter/gz-ws/pthread-w32/libs/x64/pthreadVC2.lib")

endif()

get_filename_component(HAL_API_INCLUDES ../../hal/include REALPATH)
get_filename_component(NWT_API_INCLUDES ../../networktables/cpp/include REALPATH)




# also on windows use sprintf_s instead of snprintf
# TODO: find a more permenenant solution
if (WIN32)
  add_definitions(-Dsnprintf=sprintf_s)
endif()

if (WIN32)
  file(GLOB_RECURSE SRC_FILES src/*.cpp
    ../../networktables/cpp/lib/share/*.cpp
    ../../networktables/cpp/lib/WIN32/*.cpp)
else()
  file(GLOB_RECURSE SRC_FILES src/*.cpp
    ../../networktables/cpp/lib/share/*.cpp
    ../../networktables/cpp/lib/Athena/*.cpp)
endif()

file(GLOB_RECURSE COM_SRC_FILES ../wpilibC++/src/*.cpp)


MESSAGE("include dir = ${GZ_MSGS_INCLUDE_DIR}")

set (INCLUDE_FOLDERS include
  ../wpilibC++/include
  ../../networktables/cpp/include
  ../../hal/include
  ${GZ_MSGS_INCLUDE_DIR}
  ${Boost_INCLUDE_DIR}
  ${GAZEBO_INCLUDE_DIRS})

if (WIN32)
  #these paths will be fixed when a more permenant windows development solution is found
  set(INCLUDE_FOLDERS ${INCLUDE_FOLDERS}
    C:/Users/peter/gz-ws/protobuf-2.6.0-win64-vc12/src
    C:/Users/peter/gz-ws/sdformat/src/win/tinyxml
    C:/Users/peter/gz-ws/FreeImage-vc12-x64-release-debug/Source
    C:/Users/peter/gz-ws/tbb43_20141023oss/include
    ${PTHREAD_INCLUDE_DIR})
endif()

include_directories(${INCLUDE_FOLDERS})

get_filename_component(HALDESKTOP_LIBRARY_DIR
  "../../hal/build/binaries/hALDesktopStaticLibrary"
  ABSOLUTE)

get_filename_component(NI_LIBRARY_DIR "../../ni-libraries/desktop" ABSOLUTE)

link_directories(${GAZEBO_LIBRARY_DIRS} ${HALDESKTOP_LIBRARY_DIR} ${NI_LIBRARY_DIR})

if (WIN32)
  add_library(${PROJECT_NAME} ${SRC_FILES} ${COM_SRC_FILES})
else()
  add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${COM_SRC_FILES})
endif()

# gz msgs can be found because it's in the build directory
# and that was added to link_directories in the global cmake
target_link_libraries(${PROJECT_NAME}
  gz_msgs
  HALDesktop
  FRC_NetworkCommunication
  ${PTHREAD_LIBRARY}
  ${Boost_LIBRARIES}
  ${GAZEBO_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
  -fPIC) # NetworkTables
