> Last edited: 25-02-20 by Charles Maheu
> Author: Charles Maheu
> 
# Automatic drawer mechanism
The mechanic assembly is based on the [XM430-W350-T Dynamixel smart actuator from Robotis](http://emanual.robotis.com/docs/en/dxl/x/xm430-w350/). The motor provides plenty of torque, as well as feedback on the force, the speed and the position which allows for easier control. To sum up, it presents itself as a reliable, although slightly expensive solution.

## Drawer mechanism setup
### 1. Redimensioning CADs
The parts have been custom fitted to the exact dimension of a drawer used for testing. This means any attempt to reproduce the setup will require to ajust the dimensions of some components. There are three main components to adjust in this case :
 - **Spur gear**
    * Radius of the gear to allow good contact with the rack
 - **Motor support**
    * Thickness of the drawer walls to ensure good grip and tight fit.
    * Clearance for the gear
 - **Rack**
    * Length to cover full drawer motion
    * Thickness/height to allow full contact at all times with the gear

> The drawer itself has been drawn in the SolidWorks assembly to help dimensionning the rest. It's recommended to use that to verify dimensions.

### 2. Printing components
The parts can be 3D printed from the files available in the _Drawer mechanism CADs_ folder. Most of those parts may be subject to non-negligible stress from daily use and it is highly suggested the parts are printed with a good infill value to avoid problems. Support must aslo be used to ensure the parts print correctly.
The rack must, in most case be splitted into two different parts since most printers canno't print objects of this length.

### 3. Assembling components
First, the smart actuator must be properly screwed-in using M2.5 screws in the holes provided for this purpuse. The screws that come with the motor were used in this case. When done, the gear should be screwed in the motor using included screws.
The rack should then be carefully mounted on the inside of the drawer unit to match with the height of the gear. It sould be mounted with thin double sided tape, but it can also be temporarily fixed using blu tack. Both part of the rack should be installed with minimal gap between them. They can also be glued together if needed.
The motor support can then be installed. To ensure the motor support doesn't move when using it, a piece of elastic can be inserted between the side of the drawer and the fixation point on the motor support. Without this, undesired mouvement was observed while testing.


## Drawer usage
> To use the mechanical setup of the drawer, the electronic and software setup must be completed. When done, some buttons can be used to test the opening and closing sequence of the drawer.

The setup can be tested by slowly opening and closing the drawer while making sure the gear always stays in contact with the rack.