:: This file is a helper for frc_gazebo_plugin configuration (cmake) on Windows
:: 
:: Usage: cd /build && ../configure
::
:: WARNING -- this is only temporary, and only meant for debug, and only works on my computer
@set WS=C:\Users\peter\gz-ws
@set BOOST_PATH=%WS%\boost_1_56_0
@set BOOST_LIBRARY_DIR=%BOOST_PATH%\lib64-msvc-12.0

@set GAZEBO_PATH=%WS%\gazebo\build\install\Debug\lib\cmake\gazebo
@set SDFORMAT_PATH=%WS%\sdformat\build\install\Debug\lib\cmake\sdformat
@set IGNITION-MATH_PATH=%WS%\ign-math\build\install\Debug\lib\cmake\ignition-math2

@set OGRE_SDK=C:\Users\peter\gz-ws\ogre_src_v1-8-1-vc12-x64-release-debug\build\install\Debug
@set BOOST_PATH=%WS%\boost_1_56_0
@set PROTOBUF_PATH=%WS%\protobuf-2.6.0-win64-vc12

cmake -G "NMake Makefiles"^
	-DCMAKE_PREFIX_PATH="%GAZEBO_PATH%;%SDFORMAT_PATH%;%IGNITION-MATH_PATH%"^
	-DBOOST_ROOT="%BOOST_PATH%"^
  -DBOOST_LIBRARYDIR="%BOOST_LIBRARY_DIR%"^
  -DPROTOBUF_SRC_ROOT_FOLDER="%PROTOBUF_PATH%"^
 	..
