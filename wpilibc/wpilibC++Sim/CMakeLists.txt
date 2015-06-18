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


filter_valid_compiler_flags(-Wno-unused-parameter -fPIC -std=c++11 -DFRC_SIMULATOR)

get_filename_component(HAL_API_INCLUDES ../../hal/include REALPATH)
get_filename_component(NWT_API_INCLUDES ../../networktables/cpp/include REALPATH)
add_subdirectory(../wpilibC++ wpilibC++)

find_package(gazebo REQUIRED)
find_package(gz_msgs REQUIRED)

file(GLOB_RECURSE SRC_FILES src/*.cpp ../../networktables/cpp/lib/share/*.cpp
                                      ../../networktables/cpp/lib/Athena/*.cpp
                                      ../../hal/lib/Athena/Task.cpp
                                      ../../hal/lib/Athena/Semaphore.cpp
                                      ../../hal/lib/Athena/cpp/Synchronized.cpp)

file(GLOB_RECURSE COM_SRC_FILES ../wpilibC++/src/*.cpp)

MESSAGE("\n\n ${GAZEBO_INCLUDE_DIRS} \n\n")

include_directories(include/ ../../wpilibc/wpilibC++/include
                    ../../networktables/cpp/include
                    ../../hal/include
                    ${Boost_INCLUDE_DIR}
                    ${GAZEBO_INCLUDE_DIRS}
                    ${GZ_MSGS_INCLUDE_DIRS})
add_library(WPILibSim SHARED ${SRC_FILES} ${COM_SRC_FILES})
target_link_libraries(WPILibSim ${GAZEBO_LIBRARIES}  -fPIC) # NetworkTables

INSTALL(TARGETS WPILibSim DESTINATION src COMPONENT src)
INSTALL(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX} COMPONENT headers)