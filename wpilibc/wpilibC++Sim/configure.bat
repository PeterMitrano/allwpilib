:: This file is a helper for frc_gazebo_plugin configuration (cmake) on Windows
:: 
:: Usage: cd /build && ../configure
::
:: WARNING -- this is only temporary, and only meant for debug, and only works on my computer
@set WS=C:\Users\peter\gz-ws

@set GAZEBO_PATH=%WS%\gazebo\build\install\Debug\lib\cmake\gazebo
@set SDFORMAT_PATH=%WS%\sdformat\build\install\Debug
@set IGNITION-MATH_PATH=%WS%\ign-math\build\install\Debug

cmake -G "NMake Makefiles"^
	-DCMAKE_PREFIX_PATH="%GAZEBO_PATH%;%SDFORMAT_PATH%;%IGNITION-MATH_PATH%"^
    ..
