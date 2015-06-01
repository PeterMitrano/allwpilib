:: This file is a helper for frc_gazebo_plugin configuration (cmake) on Windows
:: 
:: Usage: cd /build && ../configure
@set WS=C:\Users\peter\gz-ws
@set BOOST_PATH=%WS%\boost_1_56_0
@set BOOST_LIBRARY_DIR=%BOOST_PATH%\lib64-msvc-12.0

@set PROTOBUF_PATH=%WS%\protobuf-2.6.0-win64-vc12

@set LIB=%BOOST_LIBRARY_DIR%;%LIB%

cmake -G "NMake Makefiles"^
    -DPROTOBUF_SRC_ROOT_FOLDER="%PROTOBUF_PATH%"^
    -DBOOST_ROOT="%BOOST_PATH%"^
    -DBOOST_LIBRARYDIR="%BOOST_LIBRARY_DIR%"^
    ..