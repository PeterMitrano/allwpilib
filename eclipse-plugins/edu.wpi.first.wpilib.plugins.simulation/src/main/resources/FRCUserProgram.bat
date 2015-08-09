@set GAZEBO_MASTER_URI=%2
@set GAZEBO_IP=%3
cmd /k %home%\wpilib\simulation\win_addpath.bat
@echo PATH = %PATH%
@rem first arg is executable
%1
