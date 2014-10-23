#Table of conteny

* [Introduction](#intro)
* [BasicTypes](#types)

---

# [Introduction](id:intro)

### Neobot Script
Neobot Script is an interpreted language which generates actions that can be sendt to the robot.

### Syntax

* Statements are separated by carriage returns or ';'  carriage return me semble trop severe
* Commented lines starts by with '//'

---


# [Basic Types](id:types)

### Angles
**Fixed angle:** \<value> [deg|rad]<br>
**Reversed angle:** reversed \<value>[deg|rad]<br>
**Auto reversed angle:** auto reversed \<value>[deg|rad]<br>

######Examples:
	45deg
	90
	reversed 72deg
	
### Points

**Fixed point:** \<x>,\<y>,[angle]<br>
**Reversed point:** reversed \<x>,\<y>,[angle]<br>
**Auto reversed point:** auto reversed \<x>,\<y>,[angle]<br>

######Examples:
	0,0
	auto reversed 200,250,45

### Rectangles
**Fixed rectangle:** rect \<x>,\<y>,\<width>,\<height><br>
**reversed rectangle:** rect \<x>,\<y>,\<width>,\<height><br>
**Auto reversed rectangle:** auto reversed rect \<x>,\<y>,\<width>,\<height><br>
**Rectangle constructed from a circle:** circle \<center>,\<radius><br> 

*Notes:*
When using the circle construction, points coordinates need to be surrounded by '[' ']'. 
A point alias can also be used to define the "circle" center.

######Examples:
	rect 0,0,500,500
	reversed 50,50,10,30
	auto reversed 1000,300,200,300
	circle [200,1000],150
	auto reversed circle [900,550],10
	declare 1000,1500 as center
	circle center,300

### Time
**Time:** \<value>s|ms<br>

######Examples:
	500ms
	3s

---

# [Aliases](id:aliases)

Aliases are custom variables which can be used as shortcuts.

### Point alias
**Declare an alias to a point:** declare \<point> as \<name>

######Examples:
	declare auto reversed 0,0 as origin
	declare 1000,1500 as center

### Rectangle alias
**Declare an alias to a rectangle:** declare \<rectangle> as \<name>

######Examples:
	declare auto reversed 0,0,500,500 as startZone


### Sensor alias
**Declare an alias to a sensor:** declare \<sensor typer> \<sensor id> as \<name>

######Examples:
	declare color sensor 0 as leftSensor	declare microswitch 1 as middleSwitch

### Parameter alias
**Declare an alias to a robot parameter:** declare parameter \<parameter id> as \<name>

######Examples:
	declare parameter 0 as maxSpeed
	
### Action alias
**Declare an alias to a robot actuator action:** declare action \<action id>,\<action parameter>,\<estimated time> as \<name>

######Examples:
	declare action 0,0,200ms takePawn

### AX-12 alias
**Declare an alias to an AX-12:** declare ax12  \<ax12_id> as \<name>

######Examples:
	declare ax12 1 as leftAx12

### String alias
**Declare an alias to a string:** declare \<string> as \<name>

######Examples:
	declare "leftArm" as arm
	Time alias
	Angle alias


# Basic Actions

### Wait
Wait for a time period: wait \<time>

######Examples:
wait 500ms
wait 3s

### Timeout
Define a timeout for an action: timeout \<time> \<action>  curieux comment ca va interagir avec un ‘action alias’ (concernant le parametre temps)

######Examples:
timeout 500ms go to 300,300
timeout 90s --> ici, on passe a la ligne suivante mais le statement n’est pas terminé? Aussi, je voit ici une autre syntaxe d’action?
{
  go to 50,50
  ...
}

### Parameter change
Set a parameter in the robot: set parameter \<id> = \<value>

Note:
A parameter alias can be used to define a parameter.

######Examples:
set parameter 2 = 42.42
declare parameter 1 as speed
set speed = 80

# Movements

### Teleport
Teleport the robot to a point: teleport \<point>

Note:
A point alias

######Examples:
declare auto reversed 0,0 as origin
teleport origin
teleport 300,450,90deg

### Go to
Send the robot to a point: go to \<point> [speed \<value>%] [ [direction=] forward|backward|auto]
ha bon, pardon…

Notes:
A point 
Speed and direction are optional, default speed is 100% and default direction is auto.

######Examples:
declare 1000,1500 as center
go to center
go to 1800, 1500 speed 50% forward
go to 1800 2300 backward
go to 0,0 speed 10%

### Rotate
Rotate the robot to an absolute angle: rotate to \<angle>


rotate to 90deg
rotate to auto reversed 30deg

Relative rotation?
rotate over 45deg
rotate over auto reversed 30deg
# Sensors

### Sensor activation
Activate a sensor: enable \<sensor type> \<sensor id>  pas logique? un senseur ne peut etre que d’un type (je suppose), ce syntax permet par exemple suivant:
“declare microswitch 1 as middleSwitch; enable color sensor microswitch”.
Je suppose que le but devrait etre “enable middleSwitch ou “enable sensor 1”, tout court.
En lisant “AX12”, en fait, \<sensor id> n’est pas unique, mais chaque type de senseur a sa numerotation (qui debute a 1, je suppose)
Deactivate a sensor: disable \<sensor type> \<sensor id>
Activate all sensors of a type:
Deactivate all sensors of a type: disable all \<sensor type>

type of sensor: color sensor, sharp, microswitch

Note:
A sensor sensor alias can be used to define a sensor.
Sensors ids starts at 1 (each sensor type has its enumeration of sensors starting at 1).

######Examples:
enable color sensor 1
disable microswitch 2
declare sharp 4 as middleSharp
enable middlesharp
enable all microswitch
disable all colorSensor  ou color sensor? ;-)

# Actuators


Make an action: do action \<id>, \<param>, \<estimated_time>

Note:
An action alias can be used to define an action.

######Examples:
do action 1,0,500ms
declare action 2,42,1s as takePawn
do takePawn

### Single AX-12
move an ax12: move ax12 \<ax12_id> to \<angle> [speed\<value>%]

Note:
An ax12 alias can be used to define an ax12 id.

######Examples:
move ax12 4 to 180deg speed 50%
move ax12 7 to 220deg
declare ax12 2 as mainAx12
move mainAx12 to 30deg

### AX-12 movements
Make a movement: make ax12 movement  "\<group>" "\<name>" [speed\<value>%]
Make an asynchronous movement:

Note:
A string alias can be used to define the group or/and the movement.

######Examples:
make ax12 movement "leftArm" "takePawn"
make ax12 movement "leftArm" "takePawn" 50%
declare "rightArm" as group
make ax12 movement group "takePawn"
make ax12 async movement "leftArm" "takePawn"




# Lists

### List of synchronous actions
List of actions: {\<actions>}

Example:
{
   teleport 500,500
   go to 800,500
}

List of asynchronous actions
List of asynchronous actions: 
concurrent [\<stop_condition=AllActionsFinished | OneActionFinished | FirstActionFinished | LastActionFinished>] {\<actions>}

Notes:
All statements are launched at the same time. Can contain any other statements, including synchronous lists.
Default stop condition is AllActionFinished.

######Examples:
//Equivalent to a timeout of 3s on a go to action
concurrent OneActionFinished
{
   wait 3s
   go to 800,500
}

concurrent AllActionsFinished
{
   run ax12 4 to 80deg
   run ax12 6 to 180deg
}

concurrent FirstActionFinished
{
   wait 5s
   {
      go to 400,400
      go to 800,500
   }
}

### Conditions

Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aliquam velit risus, placerat et, rutrum nec, condimentum at, leo. Aliquam in augue a magna semper pellentesque. Suspendisse augue. Nullam est nibh, molestie eget, tempor ut, consectetuer ac, pede. Vestibulum sodales hendrerit augue. Suspendisse id mi. Aenean leo diam, sollicitudin adipiscing, posuere quis, venenatis sed, metus. Integer et nunc. Sed viverra dolor quis justo. Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Duis elementum. Nullam a arcu. Vivamus sagittis imperdiet odio. Nam nonummy. Phasellus ullamcorper velit vehicula lorem. Aliquam eu ligula. Maecenas rhoncus. In elementum eros at elit. Quisque leo dolor, rutrum sit amet, fringilla in, tincidunt et, nisi. 
Donec ut eros faucibus lorem lobortis sodales. Nam vitae lectus id lectus tincidunt ornare. Aliquam sodales suscipit velit. Nullam leo erat, iaculis vehicula, dignissim vel, rhoncus id, velit. Nulla facilisi. Fusce tortor lorem, mollis sed, scelerisque eget, faucibus sed, dui. Quisque eu nisi. Etiam sed erat id lorem placerat feugiat. Pellentesque vitae orci at odio porta pretium. Cras quis tellus eu pede auctor iaculis. Donec suscipit venenatis mi. 
Aliquam erat volutpat. Sed congue feugiat tellus. Praesent ac nunc non nisi eleifend cursus. Sed nisi massa, mattis eu, elementum ac, luctus a, lacus. Nunc luctus malesuada ipsum. Morbi aliquam, massa eget gravida fermentum, eros nisi volutpat neque, nec placerat nisi nunc non mi. Quisque tincidunt quam nec nibh sagittis eleifend. Duis malesuada dignissim ante. Aliquam erat volutpat. Proin risus lectus, pharetra vel, mollis sit amet, suscipit ac, sapien. Fusce egestas. Curabitur ut tortor id massa egestas ullamcorper. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec fermentum. Curabitur ut ligula ac ante scelerisque consectetuer. Nullam at turpis quis nisl eleifend aliquam. Sed odio sapien, semper eget, rutrum a, tempor in, nibh.

### Functions

Functions
function \<name> (\<parameter names>) {\<actions>}
function call: \<name> (\<parameters>)

Note:
•	Function declaration must be done before the call.
•	Parameters can be any types of alias.
•	In function call, points and rects shall be surrounded by "[" "]".  hehe 

######Examples:
function doAction(arm)
{
   make ax12 movement arm "take"
   wait 1s
   make ax12 movement arm "drop"
}

doAction("rightArm")

function checkColorSensorAtPosition(sensor, pos)
{
   go to pos
   enable cs
   if sensor is red
      do action 1,0,300ms //activate pump
   disable cs
}

checkColorSensorAtPosition(color sensor 1, [300,400]);
checkColorSensorAtPosition(color sensor 2, [800,900]);

---

# Examples

### Example 1: Move and scan

//The robot moves toward a destination while scanning the table with its color sensors
//The table is green, table elements are red or yellow

declare color sensor 1 as leftSensor
declare color sensor 2 as rightSensor

declare action 1,0,500ms as stopLeftPump
declare action 2,1,500ms as startRightPump
declare action 2,0,500ms as stopRightPump

function moveArm(arm, mvt, startPumpAction, stopPumpAction)
{
	make ax12 movement arm "pump"
	do startPumpAction
	make ax12 movement arm mvt
	do stopPumpAction
	make ax12 movement arm "scan"
}

fuction moveAndScan(destination)
{
	declare circle destination,50 as destRect
	while position not in destRect
	{
		//Go the the destination while scanning with both sensors every 50ms
		//Stop when something is found or when destination reached
		concurrent OneActionFishished
		{
			go to destination

			while leftSensor is not green
			{
				wait 50ms
			} 

			while rightSensor is not green
			{
				wait 50ms
			}
		}
		
		//Do an action with the ax12 depending on what has been found by the sensors
		concurrent AllActionFinished
		{
			if leftSensor is red
			{
				moveArm("leftArm", "turn", startLeftPump, stopLeftPump)
			}
			else if leftSensor is yellow
			
				moveArm("leftArm", "move", startLeftPump, stopLeftPump)
			}
			
			if rightSensor is red
			{
				moveArm("rightArm", "turn", startRightPump, stopRightPump)
			}
			else if rightSensor is yellow
			{
				moveArm("rightArm", "move", startRightPump, stopRightPump)
			}
		}
	}
}

teleport origin
moveAndScan([2000,3000])
moveAndScan(origin)



### Example 2: Turn around

//Turn around the table during 90 seconds
//Stop during at least 1 second if the sharp 1 is detecting something
declare 200,200 as startPos
declare sharp 1 as avoidingSharp

function smartGoto(destination)
{
	delclare circle destination,30 as destinationZone
	while position not in destinationZone
	{
		// stop 
		if sharp 1 is not close  ‘nearby’ me semble plus clair; ‘close’ peut etre confondu avec “fermer”
		
			concurrent OneActionFishished  pas besoin de ‘concurrent’ dans cet exemple, je suppose
			{
				
				while avoidingSharp is not close
					wait 100ms
			}
		}
		else
		{
			wait 1s
		}
	}
}

teleport startPos
timeout 90s
{
	while true
	{
		smartGoto([auto reversed 500,500])
		smartGoto([auto reversed 1500,500])
		smartGoto([auto reversed 1500,2500]])
		smartGoto([auto reversed 500,2500])
		smartGoto([auto reversed 500,500])
	}
}



### Example 3: Avoid opponent

	//Avoid the opponent by going where he is not going
	//A beacon is required to locate the opponent
	declare auto reverse 0,0,1000,1500 as zone1
	declare auto reverse 0,1500,1000,1500 as zone2
	declare auto reverse 1000,0,1000,1500 as zone3


	teleport 1000,1500  declare start 1000,1500

	function takeDecision()
	{

		if opponent in zone1
		{
			if position not in zone4
			{
				
				rotate to 225deg
			}
		}
		else if opponent in zone2
		{
			if position not in zone3
			{
				go to 1500,500 //go to zone 3  declare zone3_start 1500,500
				rotate to 135deg
			}
		}
		else if opponent in zone3
		{
			if position not in zone2
			{
				go to 500,2500 //go to zone 2
				rotate to 315deg
			}
		}
		else if opponent in zone4
		{
			if position not in zone1
			{
				go to 500,500 //go to zone 1
				rotate to 45deg
			}
		}

		wait 1s
	}

	while true
	{
		takeDecision()
	}



### Example 4: Arm movement while moving

	//Move along a line and make movements with an ax12 arm 
	//at specific positions without stopping
	declare rect 0,950,2000,100 zone1
	declare rect 0,1950,2000,100 zone2

	teleport 1400,300
	concurrent FirstActionFinished
	{
		go to 1400,2700
		while true
		{
			if position in zone1
			{
				make ax12 movement "arm" "shoot"
			}
			else if position in zone2
			{
				make ax12 movement "arm" "shoot"
			}
		}
	}