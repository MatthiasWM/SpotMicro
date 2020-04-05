
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

You now have a list of commands that Android.app run to copile
and to upload your entire project. Yeah, they are long lines, but
you can check if they work by simply copying and pasting them into
the shell. ALl pathnames are absolute, no environment variables
needed AFAIK.

Not sure if we can put these mammoth command lines in CLion or 
even Xcode (unlikely), but at least we can put then in a makefile
or a script.

