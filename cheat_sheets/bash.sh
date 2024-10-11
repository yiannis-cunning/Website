#!/bin/bash



# COMMENT
echo "HI, arg1 = $1"
a=20
b=10

if [ $a == $b ]
then
    echo "a == b"
else
    echo "a != b"
fi

if [ "$1" == "x" ] 
then
    echo "First argument is x"
fi
