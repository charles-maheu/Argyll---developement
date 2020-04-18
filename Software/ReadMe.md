Last edited: 18-04-2020 by William Rousseau

# Code For OpenCR or Arduino
You can use one of them, it depend of if you have an Arduino or an OpenCr.

The OpenCR is used to control the Servo motors and the Dynamixel(for the Drawer).

The Arduino can only control the Servo motors. So, this is why another program doesn't contain the Dynamixel Library.

It is made to be controlled via the list below:
- Serial Monitor of the Arduino IDE
- Raspberry Pi Terminal Window
- Application

Before Opening the Drawer, make sure that the ReerSwitch is activated (It means that the drawer is completely closed and a magnet is on the Reerswitch)

IF YOU DON'T HAVE ANY SAVINGFILE OR NOT CONNECTED TO A RASPBERRY PI, PUT ```SavingFile=false``` at line 90

#### To see how to setup the OpenCR or Arduino cards go to the wiki p.XXX

## Functions
Many functions can be used the control the Servos or the Dynamixel.

All Input must be write between start markers and end markers: ```<function> <drawer> <open>```

When you call a function, you enter in a loop and you can't come back until you send ```<back>```

#### To see all functions and how to call them, go to the wiki p.XXX

# Code For Raspberry Pi

The RaspberryPi is used to sending and receiving Data from to Android App to the OpenCr card. Also, it is used to save The Data of the positions to a csv files named Data.JSON. The RaspberryPi start the bluetooth connection and the Main program for the data transmission on startup.
#### To see how to setup the Raspberry Pi, go to the wiki p.XXX

