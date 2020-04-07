
How to set up an Xcode project for Android
==========================================

How to extract the build steps
------------------------------

How to extract the programms that are needed to compile and upload an Arduino
sketch

 - Run Arduino.app and make sure it can compile and upload your program
 - Get the process ID of Arduino.app: 
    > ps -ef | grep Arduino.app
 - Clear the log file 
    > rm t
 - watch processes as they come and go, using the pid you found
    > for n in {1..1000}; do ps -e -o pid,ppid,command | grep 16163 >> t ; done
 - compile and upload you sketch in Arduino.app
 - Ctrl-C out of the for-loop above and extract what you need from "t"
    > sort -u t | grep -v "grep "

You now have a list of commands that Android.app runs to compile
and to upload your entire project. Yeah, they are long lines, but
you can check if they work by simply copying and pasting them into
the shell. ALl pathnames are absolute, no environment variables
needed AFAIK.


How to set up a new project in Xcode
------------------------------------

 - Launch Xcode
    - New Project...
       - Select Cross-platform, Empty, Next
       - Product Name: Spot (Xcode will create a directory for you with that name)
       - Directory: choose a dir; all stuff goes into the 'Spot' subdir
    - New Target...
       - Select Cross-platform, External buildsystem
       - Product name: 'Motion' (this time, no subdirectory will be created)
         Buildtool: make
    - New File...
       - C++, Motion/Motion.cpp, check target Motion
       - if you have a headre file, remove "#include <stdio.h>"
 - Create a Makefile by using the scripts above and the Makefile here as atemplate
    - In Xcode, select the project, in the setting panel, the target, then Info,
      then External Build Tool Configuartion
       - Build tool is "make", path is wherever your Makefile lives (".../Spot/Motion")
 - Create the "upload" script or make it part of the Makefile, see template here
    - In Xcode, modify the Scheme, so you can upload by aiply pressing "Run" or Cmd-R
       - Select "Motion > Edit Scheme..."
       - In "Run", "Info"-tab, set "Executable" to "upload" and uncheck "Debug Executable"
- Press Cmd-B to build, Cmd-R to upload and receive feedback on the serial port. 
- Tadaa!


Spotter
=======

Spotter is a cross-platform app that controls SpotMicro via Bluetooh.

Tasks from low level to high level:

Trim
----

Position each servo in predefined angles. Write the uSec value into EEPROM
in the controller so we have a calibrated motor setup.

- switch power on and off for one motor, one leg, or the entire robot
- select one joint to manipulate
- show calibration positions for the eslected joint
- allow user to position and trim raw position
- store current position as one of three calibration positions in Bot

Posing
-----

Position the tip of the foot in 3D Cartesian space

- switch legs on and off
- move in X/Y, X/Z, Y/Z
- save pose to disk or as a "C" file to compile into Motion
- load pose from disk
- select pose from list or dropdown menu
- 2-pose inbetweening (linear/trapezoidal/S-curve
- 3-pose inbetweening

Sensors
-----

Sensor testing and visualisation

- show 9-DOF innertia, accelerometer, magnetometer data
- show power sensor data

IK and Balance
--------

Move the robot chassis in relation to the ground, sue sensors to andjust
pose for weight distribution and unven surfaces.

- visualize robot knowledge of environment
- visualize robtos compensation for changes in environment
- trim value for closed loop  (PID)

Static Balance in Motion
---------------------------

- blend between poses while keeping active balance at all times
- pose selector
- create pose sequences

Dynamic Balance
---------------------

- calculate center of gravity ahead of motion
- allow pose blending with balance goal

Environment
-------------

- new sensors, video support, 3D scan
- floor profile, adaptive walking
- object recognition, stairs
- environment knowledge, positional knowledge

Tasks
---------

- walk to position, motion path planning
- find objects and visual codes
- find charging station

Beyond
---------

- manipulator arm to pick up and move things
- scale up to the next bigger size
- dual interacting manipulators
- biped motion


