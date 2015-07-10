Building gz_msgs
================

Currently uses cmake, but eventually this will also use gradle.
First, create a `build` directory and enter it.

    #on linux
    cmake .. -DCMAKE_INSTALL_PREFIX=build #the last part is needed if you want to install into your build directory, which you probably do
    make
    make install #look at where it installed, make sure that's what you want


    #on windows
    ..\configure.bat
    nmake
    nmake install
