:: This file is a helper for frc_gazebo_plugin configuration (cmake) on Windows
:: 
:: Usage: cd /build && ../configure
::
:: WARNING -- this is only temporary, and only meant for debug, and only works on my computer
@set WS=C:\Users\peter\gz-ws
@set BOOST_PATH=%WS%\boost_1_56_0
@set BOOST_LIBRARY_DIR=%BOOST_PATH%\lib64-msvc-12.0

@set PROTOBUF_PATH=%WS%\protobuf-2.6.0-win64-vc12
@set PROTOBUF_INCLUDE_DIR=%PROTOBUF_PATH%\src

@set OGRE_SOURCE=%WS%\ogre_src_v1-8-1-vc12-x64-release-debug
@set OGRE_INCLUDE_DIR=%OGRE_PATH%\include;%OGRE_PATH%\include\OGRE;%OGRE_PATH%\include\OGRE\RTShaderSystem;%OGRE_PATH%\include\OGRE\Terrain;%OGRE_PATH%\include\OGRE\Paging
@set OGRE_BUILD=%OGRE_SOURCE%/build

@set FREEIMAGE_PATH=%WS%\FreeImage-vc12-x64-release-debug
@set FREEIMAGE_INCLUDE_DIR=%FREEIMAGE_PATH%\Source
@set FREEIMAGE_INCLUDE_DIR=%FREEIMAGE_PATH%\Source

@set TBB_PATH=%WS%\tbb43_20141023oss
@set TBB_LIBRARY_DIR=%TBB_PATH%\lib\intel64\vc12
@set TBB_INCLUDEDIR=%TBB_PATH%\include

@set DLFCN_WIN32_PATH=%WS%\dlfcn-win32-vc12-x64-release-debug\build\install\Debug
@set DLFCN_WIN32_INCLUDE_DIR=%DLFCN_WIN32_PATH%\include

@set TINY_XML_INCLUDE_DIR=%WS%\sdformat\src\win\tinyxml

@set IGNITION-MATH_PATH=%WS%\ign-math\build\install\Debug

@set gz_msgs_DIR=C:\Program Files\gz_msgs
@set GZ_MSGS_LIBRARY_DIR=msgs\build

@set INCLUDE=%PROTOBUF_INCLUDE_DIR%;%TINY_XML_INCLUDE_DIR%;%FREEIMAGE_INCLUDE_DIR%;%TBB_INCLUDEDIR%;%DLFCN_WIN32_INCLUDE_DIR%;%INCLUDE%
@set LIB=%LIB%

cmake -G "NMake Makefiles"^
    -DCMAKE_PREFIX_PATH="../../wpilibc/wpilibC++Sim/msgs/build/"^
    -DOGRE_SOURCE="%OGRE_SOURCE%"^
    -DOGRE_BUILD="%OGRE_BUILD%"^
    -DPROTOBUF_SRC_ROOT_FOLDER="%PROTOBUF_PATH%"^
    -DBOOST_ROOT="%BOOST_PATH%"^
    -DBOOST_LIBRARYDIR="%BOOST_LIBRARY_DIR%"^
    ..
