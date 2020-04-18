> Last edited: 15-04-20 by Étienne Bellerose
> Author: Étienne Bellerose

# Software setup
Firstly, the workspace must be set up. [this website contains all the relevant information](http://emanual.robotis.com/docs/en/parts/controller/opencr10/#install-on-windows), but the main step for a setup on Windows10 will be described here.
## Worskspace Setup :

### 1. Arduino IDE
Download and install Arduino Ide on this website:
https://www.arduino.cc/en/Main/Software
Keep all the default options during the installation.

### 2. Preferences
On arduino IDE, on File-> preferences. When the Preferences window appears, copy and paste following link to the 
Additional Boards Manager URLs textbox. (This step may take about 20 min.)
https://raw.githubusercontent.com/ROBOTIS-GIT/OpenCR/master/arduino/opencr_release/package_opencr_index.json

### 3. OpenCR setup
Click Tools → Board → Boards Manager.
Type OpenCR into the textbox to find the OpenCR by ROBOTIS package. After it finds out, click Install.
After the installation, “INSTALLED” will be appeared.
See if OpenCR Board is now on the list of Tools → Board. Click this to import the OpenCR Board source

### 4. PWMServoDriver setup
You need to download the library PWM-Servo-Driver via https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library to the library file of the Arduino IDE in your PC.
After the library has been downloaded, in Arduino IDE → Click sketch → Add a library → Manage the lybrary and then the PWM-Servo-Driver library must appear.

### 5. OpenCR Testing
To see if the installation worked, plug your OpenCr card via an USB port, it should appears in Tools → Port → COMX where X is the
number of the usb port (1,2,3...)
You can also test the exemples in FILE → EXEMPLES → OpenCr → "any exemple code"
Your workspace should be fine. If not refers to [this documentation](http://emanual.robotis.com/docs/en/parts/controller/opencr10/#install-on-windows)

### 6. Code Setup :
Simply upload the latest code version in the OpenCr.


# Drawer related Documentation
## Drawer Setup :
The code relative to the drawer is identified in the XXXX.ino file. In this section the user MUST adjust the drawer_depth variable value based on his own drawer. We recommend to begin with a value under the one measured, and slightly increasing it after some tests.

## normal running :
The drawer will only open if when it receives the instruction to open, the reer switch in the back of the drawer detect a magnet (signifying that the drawer if closed).
Also, the drawer will only close if it is open AND he has not been moved since his last opening operation.
If it has been moved, the user must closed completely the drawer and send a reset request with the app.
With those constraints, the software protect the hardware from a request that could lead the Dynamixel to force against a completely open/closed drawer.



