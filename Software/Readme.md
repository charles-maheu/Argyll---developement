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

### 4. Testing
To see if the installation worked, plug your OpenCr card via an USB port, it should appears in Tools → Port → COMX where X is the
number of the usb port (1,2,3...)
You can also test the exemples in FILE → EXEMPLES → OpenCr → "any exemple code"
Your workspace should be fine. If not refers to [this documentation](http://emanual.robotis.com/docs/en/parts/controller/opencr10/#install-on-windows)

### Code Setup :
Simply upload the latest code version in the OpenCr.




