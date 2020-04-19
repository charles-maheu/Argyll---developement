# Content
This folder contains all the CADs for the drawer and the gripper assembly. Associated BOMs can also be found.
Instructions are on the wiki

# Circuit
### To make the circuit, you need:
 - 1X OpenCR card
 - 1X DEL
 - 2X 250ohm resistor
 - 1X [SUNFOUNDER PCA9685 16 CHANNEL 12 BIT PWM SERVO DRIVER](https://www.amazon.ca/gp/product/B014KTSMLA/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1)
 - 4X HS-422 Servo motors
 - 2X 9G90 Servo motors mini
 - 1X [Power Supply 5V](https://www.amazon.ca/Trnaroy-Universal-Multi-Function-Replacement-Electronic/dp/B07BTW25S1/ref=sr_1_3?keywords=5v+2A+power+supply&qid=1587254674&sr=8-3)


OpenCR | PWM Driver
------ | ----------
Pin SDA | Pin SDA
Pin SCL | Pin SDE

OpenCR | ReerSwitch
------ | ----------
Pin 13 | Negative

OpenCR | Bredboard
------ | ---------
5V | Positive
Grnd | Grnd


![github-large](https://github.com/charles-maheu/Argyll---developement/blob/master/Hardware/Circuit.png)
