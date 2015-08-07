@rem first arg is jar file
@rem second arg is path to sim/lib folder
@rem third arg is things to append to LD_LIBRARY_PATH
@set GAZEBO_MASTER_URI="%4"
@set GAZEBO_IP="%5"
@set PATH="%3";%PATH%
java -Djava.library.path="%2" -jar "%1"
