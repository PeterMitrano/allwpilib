export GAZEBO_MASTER_URI=$2
export GAZEBO_IP=$3
export LD_LIBRARY_PATH=$4:$5:$LD_LIBRARY_PATH
#first arg is executable
$1
