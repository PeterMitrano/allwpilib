#first arg is jar file
#second arg is path to sim/lib folder
#third arg is things to append to LD_LIBRARY_PATH
echo "Running robot Program"
export LD_LIBRARY_PATH=$3
echo "ld lib path ="
echo $LD_LIBRARY_PATH
java -Djava.library.path=$2 -jar $1
