## Simulation Directory

Observe the following directory structure

.
|-- frc_gazebo_plugins (contains Gazebo Plugins)
|
|-- gz_msgs (contains the gazebo transport messages used to talk between wpilibC++Sim and Gazebo+Plugins
|   
|-- frcsim.bat (launches gazebo with model/plugin paths on windows)
|-- frcsim.sh (launches gazebo with model/plugin paths on linux/mac)
|-- install.sh (convenient linux script to install Eclipse, Eclipse Plugins, and Gazebo)
|-- JavaGazebo (java library used by java simulation. Equivelant of the C++ gazebo_transport)
|-- SimDS (linux driverstation)

The gazbeo plugins are currently built with CMake. 
Eventually they will be built with gradle. 
Read the Readme in that folder for more information

All of this is delivered to students via the eclipse plugins
