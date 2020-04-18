/*
 * Étienne Bellerose / William Rousseau
 * march 31st 2020
 * MAIN PROGRAM OF OPENCR CARD
 * 
 * ******IMPORTANT*****
 * IF YOU HAVE NO SAVINGFILE OR NOT CONNECTED TO A RASPBERRY PI, PUT SavingFile to false at line 90
 * 
 * this code receive an input command named DataIn from the Serial and run the appropriate function  
 * All Input must be write between start markers and end markers: <function> <drawer> <open>
 * 
 * All positions already exist (15 by default) change the variable "nbpts" for less or more                   
 * All programs already exist (10 by default) Change the variable "nb_prog" for less or more
 * 
 ****IMPORTANT FOR PROGRAM SECTION*****
 * Programs are initialized with a position that is initialized to 0.
 * If you want to create a program, make sure to start modifying the first position with the command ""<ProgModify><X><1>"  
 * 
 * Before Opening the Drawer, make sure that the ReerSwitch is activated (It means that the drawer is completely closed and a magnet is on the Reerswitch)
 *                    
 *LIST OF ALL FUNCTIONS USED BY THE APPLICATION OR SERIAL MONITOR:
 *                                     POSITION SECTION
 *                       - <position> ------------------------------------> To enter in the position loop
 *                                    - <motors> -------------------------> To move Joint by Joint
 *                                             - <X> ---------------------> To move the motor X
 *                                                 - <left> --------------> To move to the left
 *                                                 - <right> -------------> To move to the right
 *                                                          **NOTE**
 *                                                            To Stop, send any input to the serial and then <stop>
 *                                                          - <stop> ------> To stop the movement
 *                                                 - <back> --------------> To Select another Servo
 *                                             - <save> ------------------> To save the actual position
 *                                                    - <X> --------------> To save in the position No.X
 *                                             - <back> ------------------> To Select another function in position loop   
 *                                    - <saved_positions> ----------------> To go to a position previously saved
 *                                                      - <X> ------------> To go to the position X
 *                                                           **NOTE**
 *                                                            To Stop, send any input to the serial and then <stop>
 *                                                            - <stop> ---> To stop the movement
 *                                                      - <back> ---------> To return to the position loop         
 *                                    - <back> ---------------------------> To return to the main loop
 *                               
 *                                     FUNCTION SECTION                               
 *                       - <function> ------------------------------------> To enter in the function loop
 *                                   - <drawer> --------------------------> To Open or Close the Drawer 
 *                                             - <open> ------------------> To Open the drawer (Careful, the reerswitch must be on. It means that it need a magnet on it)
 *                                             - <close> -----------------> To Close the drawer
 *                                             - <reset> -----------------> To reset the position of the Drawer to the actual position
 *                                             - <back> ------------------> To return to the function loop
 *                                   - <gripper> -------------------------> To Open or Close the gripper
 *                                              - <open> -----------------> To open the gripper
 *                                              - <close> ----------------> To close the gripper
 *                                                       **NOTE**
 *                                                         To Stop, send any input to the serial and then <stop>
 *                                                       - <stop> --------> To stop when its enough opened or closed
 *                                              - <back> -----------------> To return to the function loop
 *                                   - <icecube> -------------------------> To Open or Close the IceCube finger
 *                                              - <open> -----------------> To open the gripper
 *                                              - <close> ----------------> To close the gripper
 *                                                       **NOTE**
 *                                                         To Stop, send any input to the serial and then <stop>
 *                                                       - <stop> --------> To stop when its enough opened or closed
 *                                              - <back> -----------------> To return to the function loop                             
 *                                              
 *                                              
 *LIST OF ALL FUNCTIONS ONLY USED BY SERIAL MONITOR:                                              
 *                                             
 *                      - <setspeed><X> ----------------------------------> To modify the speed at X
 *                        
 *                                     POSITION SECTION
 *                      - <PtDisplay><X>  --------------------------------> To display the position X
 *                      - <PtAllDisplay>  --------------------------------> To display all the positions (Used to save all positions each time a position is changed)
 *                      - <PtRename><XX><HOME> ---------------------------> To name the position XX HOME 
 *                      - <PtInit><1> ------------------------------------> To reset the position 1
 *                        
 *                                    PROGRAM SECTION
 *                      - <ProgAddPt><X><Z> ------------------------------> To add a position in the program X after the position Z 
 *                      - <ProgRun><X> -----------------------------------> To run the program No.X 
 *                      - <ProgDisplay><X> -------------------------------> To display program No.X 
 *                      - <ProgAllDisplay> -------------------------------> To display all programs
 *                      - <ProgRename><X><GoHome> ------------------------> To change the Name of the program for GoHome
 *                      - <ProgModify><X><H> -----------------------------> To modify position H in program X
 *                      - <ProgPtRename><X><W><RETREAT> ------------------> To change the name of the position W in the program X for RETREAT
 *                      - <ProgSupressPt><X><Z> --------------------------> To supress position Z of program X 
 */        

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <DynamixelSDK.h>

bool SavingFile = true;

//*********************************************************************************************************************************************************************//
//                                                                     DINAMIXEL VARIABLES INIT                                                                         //
//*********************************************************************************************************************************************************************//

#define DEVICENAME "OpenCR_DXL_Port"

//*************************************************************************************//
//     The BAUDRATE and the DXL_ID must be the same as the dynamixel you are using     //
//                        THE BAUDRATE OF THE SERIAL IS LOWER                          //
//*************************************************************************************//

#define BAUDRATE  57600
#define DXL_ID    1

//************************************************************//
//     These variables must be change base on your drawer     //
//************************************************************//

const float drawer_depth = 0.470;  // in meters

//************************************************************//

//************************************************************//
//        Control table adress, based on the DXL model        //
//************************************************************//

#define ADDR_PRO_TORQUE_ENABLE          64                 
#define ADDR_PRO_GOAL_POSITION          116
#define ADDR_PRO_PRESENT_POSITION       132
#define ADDR_OPERATING_MODE             11
#define ADDR_PWM_LIMIT                  36

//************************************************************//
//    Protocol version is different based on the DXL model    //
//************************************************************//

#define PROTOCOL_VERSION                2.0   

#define PWM_LIMIT                       442
#define TORQUE_ENABLE                   1                   // Value for enabling the torque
#define TORQUE_DISABLE                  0                   // Value for disabling the torque
#define DXL_MOVING_STATUS_THRESHOLD     10                  // Dynamixel moving status threshold
#define DXL_EXT_POSTION_MODE            4                   // Extended position control mode value
#define DXL_ROTATION_THRESHOLD          60                  // The Dxl must not move this amount between instruction

const float pi = 3.14159265359;
const float dia_pitch = 0.07;                               // in meters
const float nb_tick_per_turn = 4095;                        // depends on the dynamixel model, refer to the e-manual
float nb_turn = drawer_depth/(pi*dia_pitch);
float nb_tick = nb_turn*nb_tick_per_turn;                   // this number will be add to the present position to set the goal position
int GOAL_POSITION =0;                                       // this number will be send to the dxl to turn.
int32_t dxl_present_position = 0;                           // Present position
int32_t dxl_actual_position = 0;                            // Dxl position to compared with the most recent position after an instruction (close or open)                           
uint8_t dxl_id = DXL_ID;                                    // Set the Dxl id

bool opened = 0;                                            // When opened == 1, the drawer is open (when opened == 0 the drawer is closed)
const int ReerSwitch = 13;                                   //number of the the pin where is plugged the reer switch

String instruction = "";                                    //phase de test reset

// Initialize PortHandler instance
// Set the port path
// Get methods and members of PortHandlerLinux or PortHandlerWindows
dynamixel::PortHandler *portHandler; 

// Initialize PacketHandler instance
// Set the protocol version
// Get methods and members of Protocol1PacketHandler or Protocol2PacketHandler
dynamixel::PacketHandler *packetHandler;



//*********************************************************************************************************************************************************************//
//                                                              SERVOS AND ROBOT FUNCTION VARIABLES INIT                                                               //
//*********************************************************************************************************************************************************************//

Adafruit_PWMServoDriver servos = Adafruit_PWMServoDriver();  // We use a PWMDriver for the servos


//************************************************************//
//    Thoses variable depends on how many servos you have,    //
//     the number of positions, the number of programs        //
//        and the number of position in a program             //
//************************************************************//

const int nb_servos = 6;
const int nb_prog = 10;
const int nbpts_prog = 15;
const int nbpts = 15;

//************************************************************//
//                  Other Variables                           //
//************************************************************//

int LEDPin = 7;

int servo;
int point;
int Speed = 1;

int Posmin[nb_servos] = {0, 0, 0, 0, 0, 0};
int Posmax[nb_servos] = {180, 180, 180, 180, 180, 180};
int pulseMin[nb_servos] = {200, 200, 200, 200, 200, 200};
int pulseMax[nb_servos] = {600, 600, 600, 600, 600, 600};

int curPos = 90;
int NoPoint = 0;

int prog=0;
int pts=0;
int pos=0;
int modifprog=0;
int affprog=0;
int runprog=0;
int nameprog=0;

int servosPos[nb_servos]; // The current position of the Servos

char startmarker =  '<';
char endmarker = '>';
char Savingstartmarker = '[';
char Savingendmarker = ']';
String DataIn = "";
String cote = "";
String stops = "";
String Name = "";
String Nameprog= "";

//************************************************************//
//           Structures of Positions and Programs             //
//************************************************************//

struct PositionStruct{
  int numpts = 0;
  String nom = "No Name";
  int Pos[nb_servos];
};

struct ProgStruct{
  int numprog;
  int nb_pts_dans_prog = 1;
  String nomProg = "No Name";
  PositionStruct pointProg[nbpts_prog];
};

PositionStruct Position[nbpts];    // Set the Variable Position as a structure PositionStruct
ProgStruct Program[nb_prog];       // Set the Variable Program as a structure PorgStruct


void setup() {

  Serial.begin(9600);
  while(!Serial){}
  pinMode(LEDPin, OUTPUT);
  digitalWrite(LEDPin, LOW);


//*********************************************************************************************************************************************************************//
//                                                                            OPENCR AND FUNCTIONS SETUP                                                               //
//*********************************************************************************************************************************************************************//

  // start the Drive of the servos
  servos.begin();
  servos.setPWMFreq(60);
  
  // Set the positions of the servos at 90deg
  for (int i = 0; i<nb_servos; i++){
    servosPos[i] = 90;
    servos.setPWM(i,0, angleToPulse(i,90));
  }

  //Initialisation des point;
  for (int i=0; i<nbpts; i++){
    Position[i].numpts = i;
    initPoint(i); 
  }

  //Initialisation des programmes;
  for (int i=0; i<nb_prog; i++){
    Program[i].numprog = i; 
  }


     // SETUP the position from the saving file
  if (SavingFile == true)
  {
    Serial.println("ReadyToSetup");

       char POINT[50][100];
       char check;
       int p = 0;
    
    
       check = Serial.read(); //Check for the first char
       Serial.flush();        // can't work if the serial is not flushed
    
       while (check != ']'){ //To check every position
       
          check = Serial.read(); //to check when '{' or ']' will come 
          Serial.flush();

          if (p == nbpts){
              break;   // Need it to stop when we have the desired number of positions
          }

          if(check == '{'){
      
              Serial.print(check);
              DataIn = Serial.readStringUntil(':');
              Position[p].numpts = DataIn.toInt(); // Set Position ID
              delay(2);
              Serial.print(Position[p].numpts);
              Serial.print(':');
              Position[p].nom = Serial.readStringUntil(':'); // Set Position Name
              delay(1);
              Serial.print(Position[p].nom);
              Serial.print(':');
      
              for (int i=0; i<nb_servos;i++){
                  Position[p].Pos[i] = Serial.readStringUntil(',').toInt(); //Set Position Pos
                  delay(2);
                  Serial.print(Position[p].Pos[i]);
                  Serial.print(',');
              }
              Serial.println("}");
              p++; 
          }

        }
      //Serial.println("<Done>"); 
  }

//*********************************************************************************************************************************************************************//
//                                                                            DINAMIXEL SETUP                                                                          //
//*********************************************************************************************************************************************************************//

  pinMode(ReerSwitch, INPUT);
  const char *log;
  int dxl_comm_result = COMM_TX_FAIL;
  uint16_t model_number = 0;

// Initialize PortHandler instance
// Set the port path
// Get methods and members of PortHandlerLinux or PortHandlerWindows
portHandler = dynamixel::PortHandler::getPortHandler(DEVICENAME);

// Initialize PacketHandler instance
// Set the protocol version
// Get methods and members of Protocol1PacketHandler or Protocol2PacketHandler
packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);

  uint8_t dxl_error = 0;                          // Dynamixel error

  // Open port
  if (portHandler->openPort())
    {
      Serial.print("Succeeded to open the port!\n");
    }
  else
    {
      Serial.print("Failed to open the port!\n");
      return;
    }

  // Set port baudrate
  if (portHandler->setBaudRate(BAUDRATE))
    {
      Serial.print("Succeeded to change the baudrate!\n");
    }
  else
    {
      Serial.print("Failed to change the baudrate!\n");
      return;
    }

  dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL_ID, ADDR_OPERATING_MODE, DXL_EXT_POSTION_MODE, &dxl_error);
  
  if (dxl_comm_result != COMM_SUCCESS)
    {
      Serial.print(packetHandler->getTxRxResult(dxl_comm_result));
    }
  else if (dxl_error != 0)
    {
      Serial.print(packetHandler->getRxPacketError(dxl_error));
    }
  else
    {
      Serial.print("Dynamixel has been successfully switch to extended position mode \n");
    }

  dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, DXL_ID, ADDR_PWM_LIMIT, PWM_LIMIT, &dxl_error);
  
  if (dxl_comm_result != COMM_SUCCESS)
    {
      Serial.print(packetHandler->getTxRxResult(dxl_comm_result));
    }
  else if (dxl_error != 0)
    {
      Serial.print(packetHandler->getRxPacketError(dxl_error));
    }
  else
    {
      Serial.print("Dynamixel PWM limit has been switched \n");
    }

  dxl_comm_result = packetHandler->read4ByteTxRx(portHandler, DXL_ID, ADDR_PRO_PRESENT_POSITION, (uint32_t*)&dxl_present_position, &dxl_error);
  
  if (dxl_comm_result != COMM_SUCCESS)
    {
      Serial.print(packetHandler->getTxRxResult(dxl_comm_result));
    }
  else if (dxl_error != 0)
    {
      Serial.print(packetHandler->getRxPacketError(dxl_error));
    }
  else
    {
      Serial.print("DXL present position initialized \n");
      Serial.print("  PresPos:"); Serial.println(dxl_present_position);
    }

    digitalWrite(LEDPin, HIGH); //It means that the Card is ready!
}




//*********************************************************************************************************************************************************************//
//                                                                            VOID LOOP                                                                                //
//*********************************************************************************************************************************************************************//

void loop() {
 
  const char *log;
  int dxl_comm_result = COMM_TX_FAIL;     //
  uint8_t dxl_error = 0;   

  //Serial.println("<Waiting for positions or function>");
  
  if (Serial.available()>0)
  {

    //************************************************************//
    //       Waiting for position or function or SetupDone        //
    //************************************************************//
    while(Serial.read()!=startmarker){} //Wait for Real Data in serial
        
    DataIn = Serial.readStringUntil(endmarker);  //Read the Data between the startmarker and endmarker
    delay(1); // The Arduino/OpenCr need a delay to read a string or it won't read the second char
    Serial.println(startmarker + DataIn + endmarker); // Send to The Serial what we read


//------------------------------------------------------------------ WHEN THE SETUP IS DONE -------------------------------------------------------------------
  
  if (DataIn.startsWith("ReadyToUse"))
  {
    digitalWrite(LEDPin, HIGH); //It means that the Card is ready!
  }

    
//--------------------------------------------------------------------- POSITIONS SECTION ---------------------------------------------------------------------

  if (DataIn.startsWith("position"))
  {
    while (DataIn != "back")
    {
      //************************************************************//
      //     Waiting for motors or saved_positions or back        //
      //************************************************************//
      Serial.println("<Waiting for motors or saved_positions or back>");
      while(Serial.read()!=startmarker){} //Wait for the next task
      DataIn = Serial.readStringUntil(endmarker);
      delay(1);
      Serial.println(startmarker + DataIn + endmarker); // Sending what we received

       
//------------------------------MOVEMENT MOTORS BY MOTORS--------------------------------------
      if (DataIn.startsWith("motors"))
      {
        while (DataIn != "back")
        {
          //************************************************************//
          //         Waiting for servo numbers or save or back          //
          //************************************************************//
          Serial.println("<Waiting for the servo wanted or save or back>");
          while(Serial.read()!=startmarker){} //Wait for the servo number
          DataIn = Serial.readStringUntil(endmarker);
          delay(1);
          Serial.println(startmarker + DataIn + endmarker); // Sending what we received 

          if(DataIn != "back")
          {
            if (DataIn == "save")
            {
              Serial.println("<Waiting for the position wanted>");
              while(Serial.read()!=startmarker){} //Wait for the servo number
              DataIn = Serial.readStringUntil(endmarker);
              delay(1);
              Serial.println(startmarker + DataIn + endmarker); // Sending what we received

              point = DataIn.toInt()-1;
              delay(1);
              SetPoint(point);
        
              // Saving all the positions 
              Serial.println(Savingstartmarker);
              affichertous();
              Serial.println(Savingendmarker);
              DataIn = "";
            }

            if(DataIn == "1"||DataIn == "2"||DataIn == "3"||DataIn == "4"||DataIn == "5"||DataIn == "6")
            {
              servo = DataIn.toInt()-1;
              delay(2);
              
              while (DataIn != "back")
              {
                //************************************************************//
                //              Waiting for the Side or back                //
                //************************************************************//
                Serial.println("<Waiting for the side wanted or back>");
                while(Serial.read()!=startmarker){} //Wait for the SIDE
                DataIn = Serial.readStringUntil(endmarker);
                delay(1);
                Serial.println(startmarker + DataIn + endmarker); // Sending what we received
                
                if(DataIn == "left"||DataIn =="right")
                { 
                  cote = DataIn;
                  flushserial();                                      
                  deplacement_joint(servo, cote, servosPos[servo]);
            
                  //while(Serial.read()!=startmarker){}         // Wait for the stop 
                  //DataIn = Serial.readStringUntil(endmarker);
                  //delay(1);
                  Serial.println(startmarker + DataIn + endmarker);
                }
              }
              DataIn = ""; //Reset the variable to stay in the other while loop
            }   
          }        
        }
        DataIn = ""; //Reset the variable to stay in the other while loop        
      }
     
//-------------------------------GO TO A POSITION----------------------------------------
      if (DataIn.startsWith("saved_positions"))
      {
        while (DataIn != "back")
        {
          //************************************************************//
          //        Waiting for the position wanted or back             //
          //************************************************************//
          Serial.println("<Waiting for the position wanted or back>");
          while(Serial.read()!=startmarker){} //Wait for the position wanted
          DataIn = Serial.readStringUntil(endmarker);
          delay(1);
          Serial.println(startmarker + DataIn + endmarker); // Sending what we received 

          if(DataIn == "1"||DataIn == "2"||DataIn == "3"||DataIn == "4"||DataIn == "5"||DataIn == "6"||DataIn == "7"||DataIn == "8"||DataIn == "9")
          {
            point = DataIn.toInt()-1;
            delay(2);
            gotoPosition(servosPos,Position[point].Pos);  
          }         
        }
        DataIn = ""; //Reset the variable to stay in the other while loop        
      }     
    }
    DataIn = "";  
  }
      
//--------------------------------------------------------------------- FUNCTION SECTION ---------------------------------------------------------------------

  if (DataIn.startsWith("function"))
  {
    while (DataIn != "back")
    {
      //************************************************************//
      //     Waiting for drawer or gripper or icecube or back     //
      //************************************************************//
      Serial.println("<Waiting for drawer or gripper or icecube or back>");
      while(Serial.read()!=startmarker){} //Wait for the next task
      DataIn = Serial.readStringUntil(endmarker);
      delay(1);
      Serial.println(startmarker + DataIn + endmarker); // Sending what we received

       
//------------------------------ DRAWER SECION --------------------------------------
      if (DataIn.startsWith("drawer"))
      {
        while (DataIn != "back")
        {
          //************************************************************//
          //           Waiting for open or close or back              //
          //************************************************************//
          Serial.println("<Waiting for open or close or reset or back>");
          while(Serial.read()!=startmarker){} //Wait to open or close or back
          DataIn = Serial.readStringUntil(endmarker);
          delay(1);
          Serial.println(startmarker + DataIn + endmarker); // Sending what we received 

          if(DataIn == "open" and digitalRead(ReerSwitch) and opened == 0)
          {
              packetHandler->read4ByteTxRx(portHandler, DXL_ID, ADDR_PRO_PRESENT_POSITION, (uint32_t*)&dxl_actual_position, &dxl_error);
              if (abs(dxl_actual_position - dxl_present_position)>= DXL_ROTATION_THRESHOLD)
                 {
                   Serial.println("the drawer must be closed and reset before any other operation");
                 }
              else
                 {
                  dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL_ID, ADDR_PRO_TORQUE_ENABLE, TORQUE_ENABLE, &dxl_error);   // Enable torque
                 if (dxl_comm_result != COMM_SUCCESS)
                    {
                      //Serial.println(packetHandler->getTxRxResult(dxl_comm_result));
                    }
                 else if (dxl_error != 0)
                    {
                      //Serial.println(packetHandler->getRxPacketError(dxl_error));
                    }
                 else
                    {
                      //Serial.println("Torque is enable");
                      dxl_comm_result = packetHandler->read4ByteTxRx(portHandler, DXL_ID, ADDR_PRO_PRESENT_POSITION, (uint32_t*)&dxl_present_position, &dxl_error);
                      GOAL_POSITION = nb_tick + dxl_present_position;
                      packetHandler->write4ByteTxRx(portHandler, DXL_ID, ADDR_PRO_GOAL_POSITION, GOAL_POSITION, &dxl_error);    //set the goal position 
                      do
                        {
                          // Read present position
                          dxl_comm_result = packetHandler->read4ByteTxRx(portHandler, DXL_ID, ADDR_PRO_PRESENT_POSITION, (uint32_t*)&dxl_present_position, &dxl_error);
                          if (dxl_comm_result != COMM_SUCCESS)
                            {
                              //Serial.print(packetHandler->getTxRxResult(dxl_comm_result));
                            }
                          else if (dxl_error != 0)
                            {
                              //Serial.print(packetHandler->getRxPacketError(dxl_error));
                            }
                  
                          /*Serial.print("[ID:"); Serial.print(DXL_ID);
                          Serial.print("] GoalPos:"); Serial.print(GOAL_POSITION);
                          Serial.print("  PresPos:"); Serial.print(dxl_present_position);
                          Serial.println(" ");*/
                  
                        }while((abs(GOAL_POSITION - dxl_present_position) > DXL_MOVING_STATUS_THRESHOLD));
                      //Serial.println("Drawer is opened");
                      dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL_ID, ADDR_PRO_TORQUE_ENABLE, TORQUE_DISABLE, &dxl_error);   // disable torque96
                      opened = 1;
                    }
                 }      
          }

          if(DataIn == "close" and opened == 1)
          {
            packetHandler->read4ByteTxRx(portHandler, DXL_ID, ADDR_PRO_PRESENT_POSITION, (uint32_t*)&dxl_actual_position, &dxl_error);
            if (abs(dxl_actual_position - dxl_present_position)>= DXL_ROTATION_THRESHOLD)
              {
                Serial.println("the drawer must be closed and reset before any other operation");
              }
            else  
              {
                dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL_ID, ADDR_PRO_TORQUE_ENABLE, TORQUE_ENABLE, &dxl_error);   // Enable torque
                if (dxl_comm_result != COMM_SUCCESS)
                   {
                      //Serial.println(packetHandler->getTxRxResult(dxl_comm_result));
                   }
                else if (dxl_error != 0)
                    {
                      //Serial.println(packetHandler->getRxPacketError(dxl_error));
                    }
                 else
                    {
                      //Serial.println("Torque is enable");
                      dxl_comm_result = packetHandler->read4ByteTxRx(portHandler, DXL_ID, ADDR_PRO_PRESENT_POSITION, (uint32_t*)&dxl_present_position, &dxl_error);
                      GOAL_POSITION = dxl_present_position -nb_tick;
                      packetHandler->write4ByteTxRx(portHandler, DXL_ID, ADDR_PRO_GOAL_POSITION, GOAL_POSITION, &dxl_error);    //set the goal position 
                      do
                        {
                          // Read present position
                          dxl_comm_result = packetHandler->read4ByteTxRx(portHandler, DXL_ID, ADDR_PRO_PRESENT_POSITION, (uint32_t*)&dxl_present_position, &dxl_error);
                          if (dxl_comm_result != COMM_SUCCESS)
                            {
                              //Serial.print(packetHandler->getTxRxResult(dxl_comm_result));
                            }
                          else if (dxl_error != 0)
                            {
                              //Serial.print(packetHandler->getRxPacketError(dxl_error));
                            }
                  
                          /*Serial.print("[ID:"); Serial.print(DXL_ID);
                          Serial.print("] GoalPos:"); Serial.print(GOAL_POSITION);
                          Serial.print("  PresPos:"); Serial.print(dxl_present_position);
                          Serial.println(" ");*/
                  
                        }while((abs(GOAL_POSITION - dxl_present_position) > DXL_MOVING_STATUS_THRESHOLD));
                      //Serial.println("Drawer is closed");
                      dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL_ID, ADDR_PRO_TORQUE_ENABLE, TORQUE_DISABLE, &dxl_error);   // disable torque
                      opened = 0;
                    }  
              }         
          }

              //----------------------RESET THE DRAWER-------------------------------
      
          if (DataIn.startsWith("reset"))
          {
        
           dxl_comm_result = packetHandler->read4ByteTxRx(portHandler, DXL_ID, ADDR_PRO_PRESENT_POSITION, (uint32_t*)&dxl_present_position, &dxl_error);
           if (dxl_comm_result != COMM_SUCCESS)
            {
              Serial.print(packetHandler->getTxRxResult(dxl_comm_result));
            }
          else if (dxl_error != 0)
            {
              Serial.print(packetHandler->getRxPacketError(dxl_error));
            }
          else
            {
              Serial.print("DXL present position initialized \n");
              Serial.print("  PresPos:"); Serial.println(dxl_present_position);
            }      
          opened = 0;
          }
    
        }
        DataIn = ""; //Reset the variable to stay in the other while loop        
      }
     
//------------------------------- GRIPPER SECTION ----------------------------------------
      if (DataIn.startsWith("gripper"))
      {
        while (DataIn != "back")
        {
          //************************************************************//
          //          Waiting for open or close or back               //
          //************************************************************//
          Serial.println("<Waiting for open or close or back for gripper >");
          while(Serial.read()!=startmarker){} //Wait for open or close or back
          DataIn = Serial.readStringUntil(endmarker);
          delay(1);
          Serial.println(startmarker + DataIn + endmarker); // Sending what we received 

          if(DataIn == "open")
          {
            cote = "left";
            flushserial();                                      
            deplacement_joint(4, cote, servosPos[4]);
              
          }
          
          if(DataIn == "close")
          {
            cote = "right";
            flushserial();                                      
            deplacement_joint(4, cote, servosPos[4]);
          }         
        }
        DataIn = ""; //Reset the variable to stay in the other while loop        
      }


//------------------------------- ICECUBE SECTION ----------------------------------------
      if (DataIn.startsWith("icecube"))
      {
        while (DataIn != "back")
        {
          //************************************************************//
          //          Waiting for open or close or back               //
          //************************************************************//
          Serial.println("<Waiting for open or close or back for icecube>");
          while(Serial.read()!=startmarker){} //Wait for open or close or back
          DataIn = Serial.readStringUntil(endmarker);
          delay(1);
          Serial.println(startmarker + DataIn + endmarker); // Sending what we received 

          if(DataIn == "open")
          {
            cote = "left";
            flushserial();                                      
            deplacement_joint(5, cote, servosPos[5]);
              
          }
          
          if(DataIn == "close")
          {
            cote = "right";
            flushserial();                                      
            deplacement_joint(5, cote, servosPos[5]);
          }         
        }
        DataIn = ""; //Reset the variable to stay in the other while loop        
      }
    }
    DataIn = ""; //Reset the variable to stay in the other while loop
  }



  


//--------------------------------------------------------------------- OTHER FUNCTIONS ---------------------------------------------------------------------

//---------------------MODIFY THE SPEED------------------------------


       if (DataIn.startsWith("setspeed")){

           while(Serial.read()!=startmarker){} //Wait for the Speed in serial
           Speed = Serial.readStringUntil(endmarker).toInt();
           delay(2); // The Arduino need a delay to convert a string to a Int
           DataIn = Speed;
           Serial.println(startmarker + DataIn + endmarker);
           flushserial(); 
                 
       }


//----------------------------------------------------------------------- POSITION SECTION -----------------------------------------------------------------------


//----------------------CHANGE NAME OF A POSITION--------------------------------

       if (DataIn.startsWith("PtRename")){

        while(Serial.read()!=startmarker){} //Wait for Position ID in serial   
        point = Serial.readStringUntil('>').toInt()-1;
        delay(2);
        DataIn = point+1;
        Serial.println(startmarker + DataIn + endmarker);
          
        while(Serial.read()!=startmarker){} //Wait for the new Position Name in serial
        Name = Serial.readStringUntil(endmarker);
        delay(1);
        Serial.println(startmarker + Name + endmarker);
        Position[point].nom = Name;
        
        // Saving all the positions 
        Serial.println(Savingstartmarker);
        affichertous();
        Serial.println(Savingendmarker);    
       }


//--------------------------DISPLAY A POSITION-----------------------------------

       if (DataIn.startsWith("PtDisplay")){

          while(Serial.read()!=startmarker){} //Wait for Position ID in serial
          point = Serial.readStringUntil(endmarker).toInt()-1;
          delay(2);
          DataIn = point+1;
          Serial.println(startmarker + DataIn + endmarker);
          
          // Saving all the positions 
          Serial.println(Savingstartmarker);
          afficherPoint(point);
          Serial.println(Savingendmarker);
       }


//--------------------------DISPLAY ALL POSITIONS-----------------------------------

       if (DataIn.startsWith("PtAllDisplay")){
        // Usually used to safe all the positions 
        Serial.println(Savingstartmarker);
        affichertous();
        Serial.println(Savingendmarker);
       }

//----------------------RESET A POSITION--------------------------------

       if (DataIn.startsWith("PtInit")){

          while(Serial.read()!=startmarker){} //Wait for Position ID in serial
          point = Serial.readStringUntil(endmarker).toInt()-1;
          delay(2);
          DataIn = point+1;
          Serial.println(startmarker + DataIn + endmarker);
          
          initPoint(point);
          
          // Saving all the positions 
          Serial.println(Savingstartmarker);
          affichertous();
          Serial.println(Savingendmarker);
       }



//--------------------------------------------------------------------- PROGRAM SECTION ---------------------------------------------------------------------
       

//--------------------------DISPLAY ALL POSITIONS OF A PROGRAM-----------------------------------

       if (DataIn.startsWith("ProgDisplay")){

          while(Serial.read()!=startmarker){} //Wait for Prog ID in serial}
          affprog = Serial.readStringUntil(endmarker).toInt()-1;
          delay(2);
          DataIn = affprog+1;
          Serial.println(startmarker + DataIn + endmarker);
          
          Serial.println(Savingstartmarker);
          afficherprog(affprog);
          Serial.println(Savingendmarker);
       }


//--------------------------DISPLAY ALL PROGRAMS-----------------------------------

       if (DataIn.startsWith("ProgAllDisplay")){
           // Usually used to save all the programs 
          //Serial.println(Savingstartmarker);
          Progaffichertous();
          //Serial.println(Savingendmarker);
       }
       

//--------------------------RUN A PROGRAM-----------------------------------

       if (DataIn.startsWith("ProgRun")){

          while(Serial.read()!=startmarker){} //Wait for Prog ID in serial    
          runprog = Serial.readStringUntil(endmarker).toInt()-1;
          delay(2);
          DataIn = runprog+1;
          Serial.println(startmarker + DataIn + endmarker);
          
          RunProg(runprog);  
       }


//----------------------CHANGE NAME OF A PROGRAMME--------------------------------

       if (DataIn.startsWith("ProgRename")){

          while(Serial.read()!=startmarker){} //Wait for Prog ID in serial 
          nameprog = Serial.readStringUntil(endmarker).toInt()-1;
          delay(2);
          DataIn = nameprog+1;
          Serial.println(startmarker + DataIn + endmarker);

          while(Serial.read()!=startmarker){} //Wait for new Prog Name in serial
          Nameprog = Serial.readStringUntil(endmarker);
          delay(2);
          Serial.println(startmarker + Nameprog + endmarker);
          
          Program[nameprog].nomProg = Nameprog;

          Serial.println(Savingstartmarker);
          Progaffichertous();
          Serial.println(Savingendmarker);
       }


       
//----------------------MODIFY A POSITION IN A PROGRAM-------------------------------  

       if (DataIn.startsWith("ProgModify")){

          while(Serial.read()!=startmarker){} //Wait for Prog ID in serial 
          modifprog = Serial.readStringUntil(endmarker).toInt()-1;
          delay(2);
          DataIn = modifprog+1;
          Serial.println(startmarker + DataIn + endmarker);

          while(Serial.read()!=startmarker){} //Wait for Position ID in serial  
          point = Serial.readStringUntil(endmarker).toInt()-1;
          delay(2);
          DataIn = point+1;
          Serial.println(startmarker + DataIn + endmarker);
          
          modifierPointProg(Program[modifprog].pointProg[point].Pos);
          
          // Saving all the programs    
          Serial.println(Savingstartmarker);
          Progaffichertous();
          Serial.println(Savingendmarker);
 
       }


//----------------------CHANGE NAME OF A POSITION IN A PROGRAM-------------------------------
        
   if (DataIn.startsWith("ProgPtRename")){

          while(Serial.read()!=startmarker){} //Wait for Prog ID in serial 
          modifprog = Serial.readStringUntil(endmarker).toInt()-1;
          delay(2);
          DataIn = modifprog+1;
          Serial.println(startmarker + DataIn + endmarker);

          while(Serial.read()!=startmarker){} //Wait for Position ID in serial 
          point = Serial.readStringUntil(endmarker).toInt()-1;
          delay(2);
          DataIn = point+1;
          Serial.println(startmarker + DataIn + endmarker);

          while(Serial.read()!=startmarker){} //Wait for new Position Name in serial 
          Name = Serial.readStringUntil(endmarker);
          delay(1);
          Serial.println(startmarker + Name + endmarker);
          
          Program[modifprog].pointProg[point].nom = Name;
          
          // Saving all the Programs
          Serial.println(Savingstartmarker);
          Progaffichertous();
          Serial.println(Savingendmarker);
    }
   

//----------------------SUPRESS POSITION IN A PROGRAM-------------------------------
        
   if (DataIn.startsWith("ProgSupressPt")){

          while(Serial.read()!=startmarker){} //Wait for Prog ID in serial 
          modifprog = Serial.readStringUntil(endmarker).toInt()-1;
          delay(2);
          DataIn = modifprog+1;
          Serial.println(startmarker + DataIn + endmarker);

          while(Serial.read()!=startmarker){} //Wait for Position ID in serial  
          point = Serial.readStringUntil(endmarker).toInt()-1;
          delay(2);
          DataIn = point+1;
          Serial.println(startmarker + DataIn + endmarker);

          supprimer(modifprog, point);
          
          // Saving all the programs
          Serial.println(Savingstartmarker);
          Progaffichertous();
          Serial.println(Savingendmarker);
   }


//----------------------ADD A POSITION IN A PROGRAM-------------------------------

    if (DataIn.startsWith("ProgAddPt")){

          while(Serial.read()!=startmarker){} //Wait for Prog ID in serial 
          modifprog = Serial.readStringUntil(endmarker).toInt()-1;
          delay(2);
          DataIn = modifprog+1;
          Serial.println(startmarker + DataIn + endmarker);

          while(Serial.read()!=startmarker){} //Wait for Position ID to put the Position after 
          point = Serial.readStringUntil(endmarker).toInt()-1;
          delay(2);
          DataIn = point+1;
          Serial.println(startmarker + DataIn + endmarker);

          addpoint(modifprog, point);

          // Saving all the Programs
          Serial.println(Savingstartmarker);
          Progaffichertous();
          Serial.println(Savingendmarker); 
    }
    
// FIN
  }
}


//*****************************************************************************************************************************************************************************************//
//                                                                                           FUNCTIONS                                                                                     //
//*****************************************************************************************************************************************************************************************//


//--------------------------------------------------------------------- MOVEMENT FUNCTIONS SECTION ---------------------------------------------------------------------

//------------------------------MOVEMENTS BY JOINTS FUNCTION-------------------------------
void deplacement_joint(int Noservo, String cote, int Pos){


      //left Rotation
     if(cote == "left"){
      while(cote == "left")
      {
        
          if (Pos >= Posmin[Noservo])
          {
            Pos = Pos-Speed;
          }
          if (Pos<Posmin[Noservo])
          {
            Pos = Posmin[Noservo]; // Set Pos to PosMin if it is less than PosMin
            cote = ""; // Stop the servo
            DataIn = "stop";
          }
        servos.setPWM(Noservo,0, angleToPulse(Noservo, Pos)); //Send the iterate data to the servo
        delay(20); // Wait to reduce the speed

           //To stop whenever we receive a Data from the Serial
          if (Serial.available()>0){
             cote = "";
             DataIn = "stop";
          }   
      }
     }

      //right Rotation
     if(cote == "right"){
      while(cote == "right"){
        
          if (Pos <= Posmax[Noservo]){
            Pos = Pos+Speed;
          }
          if (Pos>Posmax[Noservo]){
            Pos = Posmax[Noservo]; // Set Pos to PosMax if it is more than PosMax
            cote = ""; //Stop the servo
            DataIn = "stop";
            ;
          }
        servos.setPWM(Noservo,0, angleToPulse(Noservo, Pos)); //Send the iterate data to the servo
        delay(20); // Wait to reduce the speed

           //To stop whenever we receive a Data from the Serial
          if (Serial.available()>0){
             cote = "";
             DataIn = "stop";
          }   
      }
     }
             servosPos[Noservo] = Pos; // Set the New position 

}


//---------------------------GO TO A POSITION FUNCTION-------------------------------------

void gotoPosition(int CurPos[nb_servos],int PosToGo[nb_servos]){

  int TempPos[nb_servos];
  int stops = 0;
  flushserial();

  for(int i=0;i<nb_servos;i++){
    TempPos[i] =CurPos[i];     //Don't modify the actual position but a temporary position
  }  
      
    for(int serv=0;serv<nb_servos;serv++){
           
     while(TempPos[serv] != PosToGo[serv]){

      if (stops == 0){
        
        if (TempPos[serv]< PosToGo[serv]){
          TempPos[serv]+=Speed;
        }
     
        if (TempPos[serv]> PosToGo[serv]){ 
          TempPos[serv]-=Speed;
        }

        //To stop whenever we receive a Data from the Serial
        if (Serial.available()>0){
          stops = 1;
        }   
       }
      servos.setPWM(serv,0, angleToPulse(serv, TempPos[serv])); //Send the iterate data to the servo
      servosPos[serv] = TempPos[serv];   // Set the New position
      delay(20); // Wait to reduce the speed
      if (stops == 1){
        break;
      }    
     }
    }
}


//----------------------------------------------------------------------- POSITIONS FUNCTIONS SECTION -----------------------------------------------------------------------

//--------------------------------- SET POSITION ---------------------------------------
// Not used because positions already exist

void SetPoint(int pt){

  for (int j=0;j<nb_servos;j++){
    Position[pt].Pos[j] = servosPos[j];
  }
    Position[pt].numpts = pt;
}


//--------------------------DISPLAY A POSITION FUNCTION----------------------------------

void afficherPoint(int point){


    String pos = String(Position[point].Pos[0]);
    for (int i=1;i<nb_servos;i++){
      pos = pos + ',' + Position[point].Pos[i];
    }
    
    String string1 = String(Position[point].numpts);
    String string2 = String(Position[point].nom + ':' + pos);
    Serial.println('{'+string1+':'+string2+'}');
}

//----------------------DISPLAY ALL POSITIONS FUNCTION------------------------------------

void affichertous(){

  for (int j=0; j<nbpts; j++){
      afficherPoint(j);
  }

}


//-------------------------MODIFY A POSITION FUNCTION-------------------------------------

void modifierPoint(int Point){
  
  for (int j=0;j<nb_servos;j++){
    Position[Point].Pos[j] = servosPos[j];
  }
}


//---------------------------RESET POSITION FUNCTION--------------------------------------

void initPoint(int pointt){

  // Réinitialisation des positions
        for (int k=0; k<nb_servos; k++){
           Position[pointt].Pos[k] = 90; // Reset position to 90deg
        } 
        // Reset the name
        Position[pointt].nom  = "No Name";
  
}


//--------------------------------------------------------------------- PROGRAMS FUNCTIONS SECTION ---------------------------------------------------------------------

//--------------------------ADD A POSITION IN A PROGRAM FUNCTION---------------------------------

void addpoint(int prog, int point){

  Program[prog].nb_pts_dans_prog +=1;
  
  for (int i=Program[prog].nb_pts_dans_prog; i>point; i--){
     Program[prog].pointProg[i]=Program[prog].pointProg[i-1];
     Program[prog].pointProg[i].numpts=i;
  }

    for (int j=0; j<nb_servos; j++){
      Program[prog].pointProg[point+1].Pos[j] = servosPos[j];
    }
}


//------------------------------RUN PROGRAM FUNCTION---------------------------------------

void RunProg(int NoProg){
  
  gotoPosition(servosPos, Program[NoProg].pointProg[0].Pos);

   for(int i=1;i<Program[NoProg].nb_pts_dans_prog;i++){

    //afficherPoint(i);
    gotoPosition(Program[NoProg].pointProg[i-1].Pos,Program[NoProg].pointProg[i].Pos);
    //delay(1000); //Uncomment if you want to make it stop at each positions
 
      
   }
  
}


//-----------------------DISPLAY POSITIONS OF A PROGRAM FUNCTION----------------------------

void afficherprog(int programme){

  
  String string = String(Program[programme].numprog);
  string = String('('+string+ ':');
  string = String(string+ Program[programme].nomProg + ':');

  
 

  String pos = String(Program[programme].pointProg[0].Pos[0]);
  for (int i=1;i<nb_servos;i++){
      pos = pos + ',' + Program[programme].pointProg[0].Pos[i];
    }
  
  String string2 = String(Program[programme].pointProg[0].numpts);
  string2 = String("{"+string2+':');
  String string3 = String(Program[programme].pointProg[0].nom + ':' + pos+'}');
  String string4 = String(string2+string3);
  
  for(int ptt=1;ptt<Program[programme].nb_pts_dans_prog; ptt++){
    
    pos = String(Program[programme].pointProg[ptt].Pos[0]);
    for (int i=1;i<nb_servos;i++){
      pos = pos + ',' + Program[programme].pointProg[ptt].Pos[i];
    }
    
    string2 = String(Program[programme].pointProg[ptt].numpts);
    string2 = String("{"+string2+':');
    string3 = String(Program[programme].pointProg[ptt].nom + ':' + pos+'}');
    string4 = String(string4+string2+string3);
  }
  Serial.println(string+string4+')');

}


//-----------------------------DISPLAY ALL PROGRAMS FUNCTION-----------------------------------

void Progaffichertous(void){

   for (int j=0; j<nb_prog; j++){
      afficherprog(j);
  }
  
}


//-------------------------MODIFY A POSITION IN A PROGRAM FUNCTION-------------------------------

void modifierPointProg(int Position[nb_servos]){

  for (int i=0; i<nb_servos; i++){
    Position[i] =  servosPos[i];
  }
}


//------------------------SUPRESS A POSITION IN A PROGRAM FUNCTION-------------------------------

void supprimer(int prog, int point){

  for (int i=point; i<Program[prog].nb_pts_dans_prog; i++){
    
     Program[prog].pointProg[i]=Program[prog].pointProg[i+1];
      Program[prog].pointProg[i].numpts-=1;
     
  }
 
  Program[prog].nb_pts_dans_prog-=1;
  
}


//--------------------------------------------------------------------- OTHER FUNCTIONS ---------------------------------------------------------------------


//------------------------------- ANGLE TO PULSE FUNCTION ---------------------------------------

int angleToPulse(int Noservo,int angle){
  int pulse = map(angle, Posmin[Noservo], Posmax[Noservo], pulseMin[Noservo], pulseMax[Noservo]);
  return pulse;
}


//------------------------------- FLUSH SERIAL FUNCTION -----------------------------------------

void flushserial(void){
  Serial.flush();
  while (Serial.available())
  {
      Serial.read(); 
  }
}
