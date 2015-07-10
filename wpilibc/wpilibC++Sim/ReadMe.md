# Building WPILib C++ Sim

when you first clone allwpilib, this will tell you how to build it.

### Linux

    # first build gs_msgs
    cd msgs
    mkdir build
    cd build
    cmake ..
    make
    
    # now you can build wpilibC++Sim
    cd .. #navigate to wpilibC++Sim
    mkdir build
    cd build
    cmake .. -DCMAKE_PREFIX_PATH=msgs/build #setting this prefix path tells cmake to look for a gz_msgs-config.cmake in that location
    make


NOTE: In the future, this will all be built with gradle like the rest of wpilib (except the plugins, which are still maven...)    
