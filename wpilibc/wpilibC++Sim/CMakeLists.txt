cmake_minimum_required(VERSION 2.8)
project(WPILibSim)

include(CheckCXXCompilerFlag)

macro(filter_valid_compiler_flags) 
  foreach(flag ${ARGN})
    CHECK_CXX_COMPILER_FLAG(${flag} R${flag})
    if(${R${flag}})
      set(VALID_CXX_FLAGS "${VALID_CXX_FLAGS} ${flag}")
    endif()
  endforeach()
endmacro()


filter_valid_compiler_flags(-Wno-unused-parameter -pthread -fPIC -fpermissive -D_USE_MATH_DEFINES -DFRC_SIMULATOR /MDd /Z7)


CHECK_CXX_COMPILER_FLAG("-std=c++11" HAS_CPP_11)


if(${HAS_CPP_11})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

#prevent windows.h from including winsock.h
add_definitions(-DWIN32_LEAN_AND_MEAN)

# defines things like M_PI
add_definitions(-D_USE_MATH_DEFINES)

# get rid of min max macros on windows 
add_definitions(-DNOMINMAX)

# Use dynamic linking for boost
add_definitions(-DBOOST_ALL_DYN_LINK)

get_filename_component(HAL_API_INCLUDES ../../hal/include REALPATH)
get_filename_component(NWT_API_INCLUDES ../../networktables/cpp/include REALPATH)

set(PTHREAD_INCLUDE_DIR "C:/Users/peter/gz-ws/pthread-w32/include")
set(PTHREAD_LIBRARY "C:/Users/peter/gz-ws/pthread-w32/libs/x64/pthreadVC2.lib")

find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(gazebo REQUIRED)
find_package(gz_msgs REQUIRED)


# on windows we produce .dlls with no prefix
if(WIN32)
  SET(CMAKE_FIND_LIBRARY_PREFIXES "")
  SET(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll")
endif()

# also on windows use sprintf_s instead of snprintf
# TODO: find a more permenenant solution
if (WIN32)
  add_definitions(-Dsnprintf=sprintf_s)
endif()

if (WIN32)
file(GLOB_RECURSE SRC_FILES src/*.cpp
  ../../networktables/cpp/lib/share/*.cpp
  ../../networktables/cpp/lib/WIN32/*.cpp
  ../../hal/lib/Athena/Task.cpp
  ../../hal/lib/Athena/Semaphore.cpp
  ../../hal/lib/Athena/cpp/Synchronized.cpp
)
else()
file(GLOB_RECURSE SRC_FILES src/*.cpp ../../networktables/cpp/lib/share/*.cpp
                                      ../../networktables/cpp/lib/Athena/*.cpp
                                      ../../hal/lib/Athena/Task.cpp
                                      ../../hal/lib/Athena/Semaphore.cpp
                                      ../../hal/lib/Athena/cpp/Synchronized.cpp)
endif()

file(GLOB_RECURSE COM_SRC_FILES ../wpilibC++/src/*.cpp)

include_directories(
  include/
  ../../wpilibc/wpilibC++/include
  ../../networktables/cpp/include
  ../../hal/include
  ../../hal/lib/Athena
  C:/Users/peter/gz-ws/protobuf-2.6.0-win64-vc12/src
  C:/Users/peter/gz-ws/sdformat/src/win/tinyxml
  C:/Users/peter/gz-ws/FreeImage-vc12-x64-release-debug/Source
  C:/Users/peter/gz-ws/tbb43_20141023oss/include
  ${PTHREAD_INCLUDE_DIR}
  ${Boost_INCLUDE_DIR}
  ${GAZEBO_INCLUDE_DIRS}
  ${GZ_MSGS_INCLUDE_DIRS}
  )

add_library(WPILibSim ${SRC_FILES} ${COM_SRC_FILES})

target_link_libraries(WPILibSim ${PTHREAD_LIBRARY} ${Boost_LIBRARIES} ${GAZEBO_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT}  -fPIC) # NetworkTables
set_target_properties(WPILibSim  PROPERTIES LINK_FLAGS "/DEBUG")

INSTALL(TARGETS WPILibSim DESTINATION src COMPONENT src)
INSTALL(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX} COMPONENT headers)
