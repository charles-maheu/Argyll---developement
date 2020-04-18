Last edited: 18-04-2020 by William Rousseau
 Author:William Rousseau

# 1. Codes Explination
Both codes are similare, but one can uses Dynamixel (For OpenCr) and the other cannot (For Arduino).

Those codes are used to control the Servo motors or the Dynamixel with many functions. 

It is made to be controlled via the Serial Monitor of the Arduino IDE, the Raspberry Pi’s Command Window or the Application.

All Input must be write between start markers and end markers: ```<function> <drawer> <open>```
  
IF YOU HAVE NO SAVINGFILE OR NOT CONNECTED TO A RASPBERRY PI, PUT ```SavingFile=false``` at line 90

Before Opening the Drawer, make sure that the ReerSwitch is activated (It means that the drawer is completely closed and a magnet is on the Reerswitch)

# 2. Functions
When you call a function, you enter in a loop and you can't come back until you send ```<back>```
## position SECTION
 ```
  <position> ---------------------------------------> To enter in the position loop 
            |<motors> ------------------------------> To move Joint by Joint   
            |        |<X> --------------------------> To move the motor X
            |        |   |<left> -------------------> To move to the left
            |        |   |<right> ------------------> To move to the right
            |        |   |       |<stop> -----------> To stop the movement
            |        |   |       |**NOTE**
            |        |   |       |  To Stop, send any input to the serial and then <stop>
            |        |   |<back> -------------------> To Select another Servo
            |        |<save> -----------------------> To save the actual position
            |        |      |<X> -------------------> To save in the position No.X
            |        |<back> -----------------------> To Select another function in position loop   
            |<saved_positions> ---------------------> To go to a position previously saved
            |                 |<X> -----------------> To go to the position X
            |                 |   |<stop> ----------> To stop the movement
            |                 |   |**NOTE**
            |                 |   |  To Stop, send any input to the serial and then <stop>
            |                 |<back> --------------> To return to the position loop         
            |<back> --------------------------------> To return to the main loop
   ```                              
 ## function SECTION  
 ```
    <function> -------------------------------------> To enter in the function loop
              |<drawer> ----------------------------> To Open or Close the Drawer 
              |        |<open> ---------------------> To Open the drawer (Careful, the reerswitch must be on. It means that it need a magnet on it)
              |        |<close> --------------------> To Close the drawer
              |        |<reset> --------------------> To reset the position of the Drawer to the actual position
              |        |<back> ---------------------> To return to the function loop
              |<gripper> ---------------------------> To Open or Close the gripper
              |         |<open> --------------------> To open the gripper
              |         |<close> -------------------> To close the gripper
              |         |       |<stop> ------------> To stop when its enough opened or closed
              |         |       | **NOTE**
              |         |       |   To Stop, send any input to the serial and then <stop>
              |         |<back> --------------------> To return to the function loop
              |<icecube> ---------------------------> To Open or Close the IceCube finger
              |         |<open> --------------------> To open the gripper
              |         |<close> -------------------> To close the gripper
              |         |       |<stop> ------------> To stop when its enough opened or closed
              |         |       |**NOTE**
              |         |       |  To Stop, send any input to the serial and then <stop>
              |         |<back> --------------------> To return to the function loop                             
   ```                                             
                                               
 ## LIST OF ALL FUNCTIONS ONLY USED BY SERIAL MONITOR:                                              
 ```                                            
    <setspeed><X> ----------------------------------> To modify the speed at X
                        
                              POSITION SECTION
    <PtDisplay><X>  --------------------------------> To display the position X
    <PtAllDisplay>  --------------------------------> To display all the positions (Used to save all positions each time a position is changed)
    <PtRename><XX><HOME> ---------------------------> To name the position XX HOME 
    <PtInit><1> ------------------------------------> To reset the position 1
                         
                              PROGRAM SECTION
     <ProgAddPt><X><Z> ------------------------------> To add a position in the program X after the position Z 
     <ProgRun><X> -----------------------------------> To run the program No.X 
     <ProgDisplay><X> -------------------------------> To display program No.X 
     <ProgAllDisplay> -------------------------------> To display all programs
     <ProgRename><X><GoHome> ------------------------> To change the Name of the program for GoHome
     <ProgModify><X><H> -----------------------------> To modify position H in program X
     <ProgPtRename><X><W><RETREAT> ------------------> To change the name of the position W in the program X for RETREAT
     <ProgSupressPt><X><Z> --------------------------> To supress position Z of program X 
   
 ```
