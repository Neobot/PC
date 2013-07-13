#!/bin/sh

IP=$1
PATH2=$2
FILE=$3

scp ${PATH2}/${FILE} roboard@${IP}:/home/roboard/svn/Informatique/PC/${PATH2} 


