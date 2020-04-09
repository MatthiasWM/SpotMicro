
SpotMicro
=========

SpotMicro is a tiny version of the walking robotic dog Spot, 
created by Deok-Yeon Kim (https://www.instagram.com/kim.d.yeon/).

This is my personal hardware and software setup.

![alt text](https://raw.githubusercontent.com/MatthiasWM/SpotMicro/master/Media/spot_the_chaos.jpg "Spot The Chaos")


Arduino and Xcode
=================

Yes, you can write Arduino software in Xcode. It's done here. And it works very well.
Just edit your Arduino code, press Apple-B to build or Apple-R to run. The scripts
will compile your code, show warnings and errors and link tem, and finally upload
the image to the Arduino withou any furtehr input.

Check the 'Motion' folder to see how it's done


Spot Hardware
===========

I printed the original STL files from https://www.thingiverse.com/thing:3445283 as they were
uploaded by the designer Deok-yeon Kim. There are many derived files out there now. No 
clue if they are better or not.

My current hardware is an Arduino Mega that manages all servo control and sensors. I may
have to replace it with a Due at some point. 

I use a PCA9685 based 16 channel PWM board
to drive the servos. It delivers a resolution of 9 bit PWM to the servos at 50 Hz (the other three 
bits are wasted in the long 'off' phases of the PWM).

Servo power comes from a 7.4V Lipo or a 25A power supply. Don't try to run this robot with 
a 3A supply like I did at first. The servos will behave unexpectedly and break parts of the robot.
The power wiring does not go through any PCB except for one ACS712 power sensor per leg.
By time-shifting the PWM for the motor control, I can measure the current requirements of 
each motor every 20ms with only four analog port.

The Mega is also hooked to a balance system based aroudn the DSD TECH MPU-9250 
9 DOF 3-axis-gyroskop, 3-axis accelerometer, and a 3-axis magnetometer.

Communications and vision are managed by an ESP32 CAM module. I use the Bluetooth
serial connection to send commands and receive data and debug information. The module
also has a Wifi antenna which is great for more range and throughput.

The ESP32 comes with a tiny camera that will find QR codes on the walls for orientation and
help me implement a minimalistic lidar aroudn rotating line laser pointers.





