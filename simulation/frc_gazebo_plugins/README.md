Notes for building the gazebo plugins
=====================================

the resulting plugins (shared libraries, .so/.dll) are delivered to students via the eclipse simulation plugins, and unzipped to ${HOME}/wpilib/simulation/plugins

## Building
If it doesn't exist, create a build directory.
In that directory, run the following

    //on windows
    ..\configure.bat
    nmake

    //on linux
    cmake ..
    make
