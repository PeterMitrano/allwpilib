Notes for building the gazebo plugins
=====================================

the resulting plugins (shared libraries, `.so/.dll`) are delivered to students via the eclipse simulation plugins, and unzipped to `${HOME}/wpilib/simulation/plugins`

## Building
If it doesn't exist, `create a build` directory.
In that directory, run the following

    //on linux
    cmake .. -DCMAKE_PREFIX_PATH=../../wpilibc/wpilibC++Sim/msgs/build # you only need this if you didn't install gz_msgs
    make

    //on windows
    #note that this script already sets CMAKE_PREFIX_PATH
    ..\configure.bat
    nmake


#installing

you can run `make install` (or nmake on windows) to install locally. The plugins will be copied to HOME/wpilib/simulation/plugins. it **WILL** overwrite anything there, so be careful!
