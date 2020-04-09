


Spotter
=======

Spotter is a cross-platform app that controls SpotMicro via Bluetooh.

Tasks from low level to high level:

Trim
----

Position each servo in predefined angles. Write the uSec value into EEPROM
in the controller so we have a calibrated motor setup.

- (done) switch power on and off for one motor, one leg, or the entire robot
- (done) select one joint to manipulate
- (done) show calibration positions for the selected joint
- (done) allow user to position and trim raw position
- (done) store current position as one of three calibration positions in Bot

Posing
-----

Position the tip of the foot in 3D Cartesian space

- (active) inverse kinematics to convert cartesion coordinates into servo angles
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


