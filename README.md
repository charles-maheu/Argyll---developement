# Argyll - development
Open source projet from the "Robot Exécutif" team
All documentation must be in english
____
## Automatic drawer mechanism
The mechanic assembly is based on the [XM430-W350-T Dynamixel smart actuator from Robotis](http://emanual.robotis.com/docs/en/dxl/x/xm430-w350/). The motor provides plenty of torque, as well as feedback on the force, the speed and the position which allows for easier control. To sum up, it presents itself as a reliable, although slightly expensive solution.

### Drawer mechanism setup
#### 1. Redimensioning CADs
The parts have been custom fitted to the exact dimension of a drawer used for testing. This means any attempt to reproduce the setup will require to ajust the dimensions of some components. There are three main components to adjust in this case :
 - **Spur gear**
    * Radius of the gear to allow good contact with the rack
 - **Motor support**
    * Thickness of the drawer walls to ensure good grip and tight fit.
    * Clearance for the gear
 - **Rack**
    * Length to cover full drawer motion
    * Thickness/height to allow full contact at all times with the gear

>> The drawer itself has been drawn in the SolidWorks assembly to help dimensionning the rest. It's recommended to use that to verify dimensions.
#### 2. Printing components
The parts can be 3D printed from the files available in the _Drawer mechanism CADs_ folder. Most of those parts may be subject to non-negligible stress from daily use and it is highly suggested the parts are printed with a good infill value to avoid problems. Support must aslo be used to ensure the parts print correctly.
The rack must, in most case be splitted into two different parts since most printers canno't print objects of this length.

#### 3. Assembling components
First, the smart actuator must be properly screwed-in using M2.5 screws in the holes provided for this purpuse. The screws that come with the motor were used in this case. When done, the gear should be screwed in the motor using included screws.
The rack should then be carefully mounted on the inside of the drawer unit to match with the height of the gear. It sould be mounted with thin double sided tape, but it can also be temporarily fixed using blu tack. Both part of the rack should be installed with minimal gap between them. They can also be glued together if needed.
The motor support can then be installed. To ensure the motor support doesn't move when using it, a piece of elastic can be inserted between the side of the drawer and the fixation point on the motor support. Without this, undesired mouvement was observed while testing.

### Software setup
Firstly, the workspace must be set up. [this website contains all the relevant information](http://emanual.robotis.com/docs/en/parts/controller/opencr10/#install-on-windows), but the main step for a setup on Windows10
will be described here.
#### Worskspace Setup :

##### 1. Arduino IDE
Download and install Arduino Ide on this website:
https://www.arduino.cc/en/Main/Software
Keep all the default options during the installation.

##### 2. Preferences
On arduino IDE, on File-> preferences. When the Preferences window appears, copy and paste following link to the 
Additional Boards Manager URLs textbox. (This step may take about 20 min.)
https://raw.githubusercontent.com/ROBOTIS-GIT/OpenCR/master/arduino/opencr_release/package_opencr_index.json

##### 3. OpenCR setup
Click Tools → Board → Boards Manager.
Type OpenCR into the textbox to find the OpenCR by ROBOTIS package. After it finds out, click Install.
After the installation, “INSTALLED” will be appeared.
See if OpenCR Board is now on the list of Tools → Board. Click this to import the OpenCR Board source

##### 4. Testing
To see if the installation worked, plug your OpenCr card via an USB port, it should appears in Tools → Port → COMX where X is the
number of the usb port (1,2,3...)
You can also test the exemples in FILE → EXEMPLES → OpenCr → "any exemple code"
Your workspace should be fine. If not refers to [this documentation](http://emanual.robotis.com/docs/en/parts/controller/opencr10/#install-on-windows)

#### Code Setup :
Simply upload the latest drawer code file in the Drawer_software folder.
