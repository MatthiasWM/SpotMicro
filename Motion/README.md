
Motion
=====

`Motion` is the firmware for our little robot. `Spotter` is the remote control for it. Find more details 
on the development in the `Spotter` folder. 

How to set up an Xcode project for Android
==========================================

How to extract the build steps
------------------------------

The project 'arduino-cli' on Github solved a lot of issues for me, but if you would like to know 
what ist going on in the Arduino IDE, this is a neat way to find out:

How to extract the programms that are needed to compile and upload an Arduino
sketch

 - Run Arduino.app and make sure it can compile and upload your program
 - Get the process ID of Arduino.app: 
    `> ps -ef | grep Arduino.app`
 - Clear the log file 
    `> rm t`
 - watch processes as they come and go, using the pid you found
    `> for n in {1..1000}; do ps -e -o pid,ppid,command | grep 16163 >> t ; done`
 - compile and upload you sketch in Arduino.app
 - Ctrl-C out of the for-loop above and extract what you need from "t"
    `> sort -u t | grep -v "grep "`

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
 - Create a Makefile by using the scripts above and the Makefile here as a template
    - In Xcode, select the project, in the setting panel, the target, then Info,
      then External Build Tool Configuartion
       - Build tool is "make", path is wherever your Makefile lives (".../Spot/Motion")
 - Create the "upload" script or make it part of the Makefile, see template here
    - In Xcode, modify the Scheme, so you can upload by aiply pressing "Run" or Cmd-R
       - Select "Motion > Edit Scheme..."
       - In "Run", "Info"-tab, set "Executable" to "upload" and uncheck "Debug Executable"
- Press Cmd-B to build, Cmd-R to upload and receive feedback on the serial port. 
- Tadaa!


