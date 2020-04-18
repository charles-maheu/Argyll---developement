Last edited: 18-04-2020 by William Rousseau

# Code For OpenCR or Arduino
### Use one of them, it depends if you have an Arduino or an OpenCr.

The OpenCR is used to control the Servo motors and the Dynamixel(for the Drawer).

The Arduino can only control the Servo motors. So, this is why the other program doesn't contain the Dynamixel Library.

**To see how to setup the OpenCR or Arduino cards go to the wiki**

# Code For Raspberry Pi

The RaspberryPi is used to sending and receiving Data from to Android App to the OpenCr card. Also, it is used to save The Data of the positions to a csv files named Data.JSON. The RaspberryPi start the bluetooth connection and the Main program for the data transmission on startup.
**To see how to setup the Raspberry Pi, go to the wiki**

