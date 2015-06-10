cmake_minimum_required(VERSION 2.8.3)
project(frc_gazebo_plugins)

add_subdirectory(msgs)

set (PLUGINS
	clock
	dc_motor
	encoder
	gyro
	limit_switch
	pneumatic_piston
	potentiometer
	rangefinder
	servo)

include (FindPkgConfig)
if (PKG_CONFIG_FOUND)
  pkg_check_modules(GAZEBO gazebo)
endif()

find_package(gazebo REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)


foreach(PLUGIN ${PLUGINS})
	MESSAGE("configuring ${PLUGIN}")

	#on windows we produce .dlls with no prefix
	if(WIN32)
		SET(CMAKE_FIND_LIBRARY_PREFIXES "")
		SET(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll")
	else()
		SET(CMAKE_FIND_LIBRARY_PREFIXES "lib")
		SET(CMAKE_FIND_LIBRARY_SUFFIXES ".so" ".a")
	endif()
	
  #find_library(GZ_MSGS NAMES gz_msgs PATHS msgs/build)
  set(GZ_MSGS_INCLUDE_DIR ${CMAKE_BINARY_DIR}/msgs)

	file(GLOB_RECURSE SRC_FILES ${PLUGIN}/src/*.cpp)

	include_directories(src ${Boost_INCLUDE_DIR} ${GAZEBO_INCLUDE_DIRS} ${GZ_MSGS_INCLUDE_DIR})

	add_library(${PLUGIN} SHARED ${SRC_FILES})

	link_directories(${GAZEBO_LIBRARY_DIRS} msgs/build)

  add_dependencies(${PLUGIN} gz_msgs)

	target_link_libraries(${PLUGIN} gz_msgs ${GAZEBO_LIBRARIES} ${Boost_LIBRARIES})

endforeach()
