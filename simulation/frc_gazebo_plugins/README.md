Notes for building the gazebo pluginos
=================

run make in this directory. If you get errors about proto buf being uncompatable, install protobuf compiler and recompile the .proto files

    $> sudo apt-get install protobuf-compiler
    $> cd msgs/proto
    $> protoc --cpp_out ../src/msgs *.proto

Then it should work
