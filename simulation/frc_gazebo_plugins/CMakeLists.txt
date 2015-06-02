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
	MESSAGE("\nconfiguring ${PLUGIN}\n")

	#on windows we produce .dlls with no prefix
	if(WIN32)
		SET(CMAKE_FIND_LIBRARY_PREFIXES "")
		SET(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll")
	else()
		SET(CMAKE_FIND_LIBRARY_PREFIXES "lib")
		SET(CMAKE_FIND_LIBRARY_SUFFIXES ".so" ".a")
	endif()

  

	#the plugins need the gz_msgs library
	find_library(GZ_MSGS NAMES gz_msgs PATHS msgs/build)

	#the header files are all in the build folder so they don't pollute the src files
	set(GZ_MSGS_INCLUDE_DIR msgs/build)

	file(GLOB_RECURSE SRC_FILES ${PLUGIN}/src/*.cpp)

	include_directories(src ${Boost_INCLUDE_DIR} ${GAZEBO_INCLUDE_DIRS} ${GZ_MSGS_INCLUDE_DIR})

	add_library(${PLUGIN} SHARED ${SRC_FILES})

	link_directories(${GAZEBO_LIBRARY_DIRS} msgs/build)

	target_link_libraries(${PLUGIN} ${GZ_MSGS} ${GAZEBO_LIBRARIES} ${Boost_LIBRARIES})

endforeach()
