cmake_minimum_required(VERSION 2.8.3)
project(frc_gazebo_plugins)

if (UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

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

find_package(gazebo REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
find_package(gz_msgs REQUIRED)

if (WIN32)

	# defines things like M_PI
	add_definitions(-D_USE_MATH_DEFINES)

	# get rid of min max macros on windows 
	add_definitions(-DNOMINMAX)

	# aww yea
	add_definitions(-DWIN32_LEAN_AND_MEAN)
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/plugins)

link_directories(${GAZEBO_LIBRARY_DIRS} ${GZ_MSGS_LIBRARY_DIRS})

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
	
	file(GLOB_RECURSE SRC_FILES ${PLUGIN}/src/*.cpp)

	include_directories(src ${Boost_INCLUDE_DIR} ${GAZEBO_INCLUDE_DIRS} ${GZ_MSGS_INCLUDE_DIRS})

	if (WIN32)
		add_library(${PLUGIN} ${SRC_FILES})
	else()
		add_library(${PLUGIN} SHARED ${SRC_FILES})
	endif()

	target_link_libraries(${PLUGIN} ${GZ_MSGS_LIBRARY} ${GAZEBO_LIBRARIES} ${Boost_LIBRARIES})

  install(TARGETS ${PLUGIN} DESTINATION $ENV{HOME}/wpilib/simulation/plugins)

endforeach()
