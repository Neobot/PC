# [Neobot Script Reference](id:top)
 
## Table of content

- [Introduction](#intro)
- [BasicTypes](#types)
- [Aliases](#aliases)
- [Basic actions](#basic)
- [Movements](#movements)
- [Sensors](#sensors)
- [Actuators](#actuators)
- [Lists](#lists)
- [Conditions](#conditions)
- [Functions](#functions)
- [Examples](#examples)

---

## [Introduction](id:intro)

#### Neobot Script
Neobot Script is an interpreted language which generates actions that can be sent to the robot. It can execute complex sequences of basic actions. However, the level of intelligence which can be contained in a program is very limited. 

#### How to read this reference
- Words between \< > have to be replace by an appropriate value. The type of the value is generally obvious or defined in the document.
- Words between [ ] are optional.
- The character | means *"or"*.
- Other words have to be written as is.
- Examples:
	- [\<newline>] means an optional carriage return.
	- [deg|rad] means the word *"deg"* or the word *"rad"* or nothing.
	- \<statement> means any statement defined in this document.
	
#### Language syntax
- Statements are separated by carriage returns or by a semi-colon
- Commented lines starts by with '//'

<br>[Back to top](#top)

---

## [Basic types](id:types)

#### Angles
**Fixed angle:** \<value> [deg|rad]<br>
**Reversed angle:** reversed \<value>[deg|rad]<br>
**Auto reversed angle:** auto reversed \<value>[deg|rad]<br>

###### Examples:
	45deg
	90
	reversed 72deg
	
*Note*
> The default unit is the degree.

#### Points

**Fixed point:** \<x>,\<y>,[\<angle>]<br>
**Reversed point:** reversed \<x>,\<y>,[\<angle>]<br>
**Auto reversed point:** auto reversed \<x>,\<y>,[\<angle>]<br>

###### Examples:
	0,0
	auto reversed 200,250,45

#### Rectangles
**Fixed rectangle:** rect \<x>,\<y>,\<width>,\<height><br>
**Reversed rectangle:** rect \<x>,\<y>,\<width>,\<height><br>
**Auto reversed rectangle:** auto reversed rect \<x>,\<y>,\<width>,\<height><br>
**Rectangle constructed from a circle:** circle \<center>,\<radius><br> 

*Notes:*
> When using the circle construction, point coordinates need to be surrounded by '[' ']'. 
A point [alias](#aliases) can also be used to define the "circle" center.

###### Examples:
	rect 0,0,500,500
	reversed 50,50,10,30
	auto reversed 1000,300,200,300
	circle [200,1000],150
	auto reversed circle [900,550],10
	declare 1000,1500 as center
	circle center,300

#### Time
**Time:** \<value>s|ms<br>

###### Examples:
	500ms
	3s

<br>[Back to top](#top)

---

## [Aliases](id:aliases)

Aliases are custom variables which can be used as shortcuts.

#### Point alias
**Declare an alias to a point:** declare \<point> as \<name>

###### Examples:
	declare auto reversed 0,0 as origin
	declare 1000,1500 as center

#### Rectangle alias
**Declare an alias to a rectangle:** declare \<rectangle> as \<name>

###### Examples:
	declare auto reversed 0,0,500,500 as startZone

#### Sensor alias
**Declare an alias to a sensor:** declare \<sensor type> \<sensor id> as \<name>

###### Examples:
	declare color sensor 0 as leftSensor	
	declare microswitch 1 as middleSwitch

#### Parameter alias
**Declare an alias to a robot parameter:** declare parameter \<parameter id> as \<name>

###### Examples:
	declare parameter 0 as maxSpeed
	
#### Action alias
**Declare an alias to a robot actuator action:** declare action \<action id>,\<action parameter>,\<estimated time> as \<name>

###### Examples:
	declare action 0,0,200ms takePawn

#### AX-12 alias
**Declare an alias to an AX-12:** declare ax12  \<ax12_id> as \<name>

###### Examples:
	declare ax12 1 as leftAx12

#### String alias
**Declare an alias to a string:** declare \<string> as \<name>

###### Examples:
	declare "leftArm" as arm

<br>[Back to top](#top)

---

## [Basic actions](id:basic)

#### Imports
**Importing an external script file:** import "\<filename.ns>"<br>
**Importing a grid:** import "\<filename>.ngrid"

*Notes:*
> - The filename extension is used to make the distinction between a grid and a script file: "ngrid" for a grid and any other extension for a script.
> - When a script is imported, all actions it contain are executed. All top level variables and functions are imported.
> - When a grid is imported, all defined node aliases are imported as point [aliases] (#aliases) (with a null angle), and every defined areas are imported as rectangle [aliases](#aliases).

#### Wait
**Wait for a time period:** wait \<time>

###### Examples:
	wait 500ms
	wait 3s

#### Timeout
**Define a timeout for an action:** timeout \<time> [\<newline>] \<statement>

*Note:*
> The statement controlled by a timeout can be any other statement, including a list.

###### Examples:
	timeout 500ms go to 300,300
	timeout 90s
	{
	  go to 50,50
	  ...
	}

#### Parameter change
**Set a robot parameter:** set parameter \<id> = \<value>

*Note:*
> A parameter [alias](#aliases) can be used to define a parameter.

###### Examples:
	set parameter 2 = 42.42
	declare parameter 1 as speed //alias declaration
	set speed = 80

<br>[Back to top](#top)

---

## [Movements](id:movements)

#### Teleport
**Teleport the robot to a point:** teleport \<point>

*Note:*
> A point [alias](#aliases) can be used to define the point.

###### Examples:
	declare auto reversed 0,0 as origin //alias declaration
	teleport origin
	teleport 300,450,90deg

#### Go to
**Send the robot to a point:** go to \<point> [speed \<value>%] [<direction=forward|backward|auto>]

*Notes:*
> - A point [alias](#aliases) can be used to define the point.
- Speed and direction is optional, default speed is 100% and default direction is auto.

###### Examples:
	declare 1000,1500 as center //alias declaration
	go to center
	go to 1800, 1500 speed 50% forward
	go to 1800 2300 backward
	go to 0,0 speed 10%

#### Rotate
**Rotate the robot to an absolute angle:** rotate to \<angle>

###### Examples:
	rotate to 90deg
	rotate to auto reversed 30deg
	
<br>[Back to top](#top)
	
---
	
## [Sensors](id:sensors)

#### Sensor activation
**Activate a sensor:** enable \<sensor type> \<sensor id><br>
**Deactivate a sensor:** disable \<sensor type> \<sensor id><br>
**Activate all sensors of a type:** enable all \<sensor type><br>
**Deactivate all sensors of a type:** disable all \<sensor type><br>

**type of sensor:** color sensor, sharp, microswitch

*Notes:*
> - A sensor sensor [alias](#aliases) can be used to define the sensor (type + id).<br>
- Sensors ids starts at 1 (each sensor type has its enumeration of sensors starting at 1).

###### Examples:
	enable color sensor 1
	disable microswitch 2
	declare sharp 4 as middleSharp //alias declaration
	enable middlesharp
	enable all microswitch
	disable all color sensor

<br>[Back to top](#top)

---
	
## [Actuators](id:actuators)

**Make an action:** do action \<id>, \<param>, \<estimated_time>

*Note:*
> An action [alias](#aliases) can be used to define an action.

###### Examples:
	do action 1,0,500ms
	declare action 2,42,1s as takePawn //alias declaration
	do takePawn

#### Single AX-12
**move an ax12:** move ax12 \<ax12_id> to \<angle> [speed \<value>%]

*Note:*
> An ax12 [alias](#aliases) can be used to define the ax12 id.

###### Examples:
	move ax12 4 to 180deg speed 50%
	move ax12 7 to 220deg
	declare ax12 2 as mainAx12 //alias declaration
	move mainAx12 to 30deg

#### AX-12 movements
**Make a movement:** make ax12 movement  "\<group>" "\<movement_name>" [speed \<value>%]<br>
**Make an asynchronous movement:** make ax12 async movement  "\<group>" "\<movement_name>" [speed \<value>%]<br>

*Note:*
> A string [alias](#aliases) can be used to define the group or/and the movement name.

###### Examples:
	make ax12 movement "leftArm" "takePawn"
	make ax12 movement "leftArm" "takePawn" 50%
	declare "rightArm" as group //alias declaration
	make ax12 movement group "takePawn"
	make ax12 async movement "leftArm" "takePawn"

<br>[Back to top](#top)

---

## [Lists](id:lists)

#### List of synchronous actions
**List of actions:** { [\<newline>] \<statements> [\<newline>]}

*Note:*
> \<statements> is a succession of any kind of statement defined in this reference.

######Example:
	{
	   teleport 500,500
	   go to 800,500
	}

#### List of asynchronous actions
**List of asynchronous actions:** concurrent [\<stop_condition>] [\<newline>] { [\<newline>] \<statements> [\<newline>] }

**Stop conditions:** AllActionsFinished | OneActionFinished | FirstActionFinished | LastActionFinished

*Notes:*
> - All statements are launched at the same time. Can contain any other statements, including synchronous lists.
- Default stop condition is AllActionFinished.

###### Examples:
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
	
<br>[Back to top](#top)

---

## [Conditions](id:conditions)

###### Supported conditions in *if* and *while* statements:
**Condition on robot position:** position in \<rect><br>
**Condition on robot orientation:** orientation in \<angleMin>-\<angleMax><br>
**Condition on opponent position:** opponent in \<rect><br>
**Condition on sensor value:** \<sensor> is \<sensor value><br>
**Condition on strategy color:** strategy is reversed<br>
 
**Reversed condition:** replace *"in"* by *"not in*" and *"is"* by *"is not"*
 
###### Acceptable values for sensors in conditions:
**Values for color sensors:** unknown, red, green, blue, yellow, white, black  
**Values for sharps:** nearby, on, off  
**Values for microswitches:** on, off  
 
*Note:*
> Conditions on opponent position require a beacon installed on the opponent robot.

#### If statement
**"if" condition:** if \<condition> [\<newline>] \<action><br>
**"if / else" condition:** if \<condition> [\<newline>] \<action> [\<newline>] else [\<newline>] \<action>
 
 
###### Examples:
	if position in rect 0,0,300,300
	   go to 500,500
	 
	if orientation in 0-90
	{
	   rotate 90deg
	   wait 1s
	   rotate 0deg
	}
	else
	{
	   rotate 270deg
	   wait 1s
	   rotate 0deg
	}
	
	declare color sensor 1 as leftColorSensor
	if leftColorSensor is red
		make ax12 movement "leftArm" "takePawn"
 
#### While loop
**while loop:** while \<condition> \<action><br>
**infinite loop:** while true \<action>
 
###### Examples:
	//wait for the robot to be in the middle of the table, check every 200ms
	while position not in rect 950,1450,100,100
	   wait 200ms
	 
	//wait for the color sensor 1 to be green
	while color sensor 1 is green
	   wait 200ms
 
	while microswitch 1 is off
	{
	   go to 600,600
	   make ax12 movement "leftArm" "drop" 
	   go to 0,0
	}
	
<br>[Back to top](#top)

---

## [Functions](id:functions)

**Function declaration:** function \<name> ( \<parameter names> ) [\<newline>] { [\<newline>] \<actions> [\<newline>] }<br>
**Function call:** \<name> (\<parameters>)<br>

*Notes:*
> - Function declaration must be done before the call.
- Parameters can be any types of [alias](#aliases).
- In function call, points and rects must be surrounded by "[" "]".

###### Examples:
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

<br>[Back to top](#top)

---

## [Examples](id:examples)

#### Example 1: Move and scan

	//The robot moves toward a destination while scanning the table with its color sensors
	//The table is green, table elements are red or yellow

	declare 50,50 as origin
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



#### Example 2: Turn around

	//Turn around the table during 90 seconds
	//Stop during at least 1 second if the sharp 1 is detecting something
	declare 200,200 as startPos
	declare sharp 1 as avoidingSharp

	function smartGoto(destination)
	{
		declare circle destination,30 as destinationZone
		while position not in destinationZone
		{
			// stop 
			if sharp 1 is not close
			{
				concurrent OneActionFishished
				{
					//Advance toward the destination while testing the sharp every 100ms
					//Stop when arrived or when teh sharp is detecting something
					go to destination
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



#### Example 3: Avoid opponent

	//Avoid the opponent by going where he is not going
	//A beacon is required to locate the opponent
	//Note: there are obvious flaws to this logic, it is just worth as an example
	
	declare auto reverse 0,0,1000,1500 as zone1
	declare auto reverse 0,1500,1000,1500 as zone2
	declare auto reverse 1000,0,1000,1500 as zone3
	declare auto reverse 1000,1500,1000,1500 as zone4
	
	declare auto reverse 500,750 as zone1_middle
	declare auto reverse 500,2250 as zone2_middle
	declare auto reverse 1500,750 as zone3_middle
	declare auto reverse 1500,2250 as zone4_middle
	
	teleport 1000,1500 //Starts on the middle of the table
	
	function takeDecision()
	{
		if opponent in zone1
		{
			if position not in zone4
			{
				go to zone4_middle //go to zone 4
				rotate to 225deg
			}
		}
		else if opponent in zone2
		{
			if position not in zone3
			{
				go to zone3_middle //go to zone 3
				rotate to 135deg
			}
		}
		else if opponent in zone3
		{
			if position not in zone2
			{
				go to zone2_middle //go to zone 2
				rotate to 315deg
			}
		}
		else if opponent in zone4
		{
			if position not in zone1
			{
				go to zone1_middle //go to zone 1
				rotate to 45deg
			}
		}
	}

	while true
	{
		takeDecision()
		wait 1s
	}



#### Example 4: Arm movement while moving

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
			//Infinite loop. But because this happens inside a concurrent action list, 
			//it will terminate when the go to statement finishes
			
			if position in zone1
			{
				make ax12 movement "arm1" "shoot"
			}
			else if position in zone2
			{
				make ax12 movement "arm2" "shoot"
			}
		}
	}