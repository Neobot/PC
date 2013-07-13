#!/bin/sh

IP="$1"

function ncopy
{
   p=$1
   scp $p/*.h $p/*.cpp $p/*.pro roboard@${IP}:/home/roboard/svn/Informatique/PC/$p 
}

ncopy Apps/NServer
ncopy Libraries/CommLib
ncopy Libraries/GraphicStrategy
ncopy Libraries/GraphicStrategy/Strategies
ncopy Libraries/Simulation
ncopy Libraries/ToolsLib
ncopy Libraries/WidgetToolsLib

