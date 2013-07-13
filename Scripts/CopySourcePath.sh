#!/bin/sh

IP="$1"
PP="$2"

function ncopy
{
   p=$1
   scp $p/*.h $p/*.cpp $p/*.pro roboard@${IP}:/home/roboard/svn/Informatique/PC/$p 
}

ncopy $2


