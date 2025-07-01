It better to use "SimpleTimerWP.c", more easier and flexible to use, but mayb lil bit complicated to understand (look the simpler code on "OldVersion/SimpleTimer.c").

Not RTOS required. Just config some timers of any microcontroller to ticking as a reference for and use this lib.
If you don't want use the Timers interrupt then just configurate the timer as counter. 

You can launch the console app to see what happens on this example application. To do it: go to the folder "DemoAppl/" and open "TimerWithCBdemo.exe" Download this demo "*.exe" by clicking "Download raw file" and run it.
~~~
PROJECT'S STATUS:
Completed.
~~~
Arduino compatible lib located on OldVersion/SimpleTimer.cpp

How To:
-------

Just include to your project two files: *`SimpleTimerWP.c`* and *`SimpleTimerWP.h`*.

For using on Arduino get: *`SimpleTimerWP.cpp`* and `SimpleTimerWP.h` files.