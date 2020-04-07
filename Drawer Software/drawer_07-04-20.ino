/*
 * Étienne Bellerose
 * march 31st 2020
 * Open/close drawer
 * this code receive a input command from the button on the OpenCr card
 * Button 2 is closing the drawer
 * Button 1 is opening the drawer
 * Next implementation: Add a feedback from the Dynamixel so that it knows when the drawer is opened or closed
 * Control with the control table
 * limit the PWM output on the DXL (to limit output velocity)
 */

#include <DynamixelSDK.h>
#include <DynamixelWorkbench.h>
/*#include <iostream>
#include <string>
using namespace std;*/


#define DEVICENAME "OpenCR_DXL_Port"


//*************************************************************************************//
//     The BAUDRATE and the DXL_ID must be the same as the dynamixel you are using     //
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

const float pi = 3.14159265359;
const float dia_pitch = 0.07;                               // in meters
const float nb_tick_per_turn = 4095;                        // depends on the dynamixel model, refer to the e-manual
float nb_turn = drawer_depth/(pi*dia_pitch);
float nb_tick = nb_turn*nb_tick_per_turn;                   // this number will be add to the present position to set the goal position
int GOAL_POSITION =0;                                       // this number will be send to the dxl to turn.

bool opened = 0;                                            // When opened == 1, the drawer is open (when opened == 0 the drawer is closed)
const int pinSwitch = 13;                                   //number of the the pin where is plugged the reer switch

String instruction = "";                                    //phase de test reset

DynamixelWorkbench dxl_wb;

// Initialize PortHandler instance
// Set the port path
// Get methods and members of PortHandlerLinux or PortHandlerWindows
dynamixel::PortHandler *portHandler; 

// Initialize PacketHandler instance
// Set the protocol version
// Get methods and members of Protocol1PacketHandler or Protocol2PacketHandler
dynamixel::PacketHandler *packetHandler;


void setup() {
  Serial.begin(57600);
  //while(!Serial); // Wait for Opening Serial Monitor
  pinMode(BDPIN_PUSH_SW_1, INPUT);
  pinMode(BDPIN_PUSH_SW_2, INPUT);
  pinMode(pinSwitch, INPUT);
  const char *log;
  bool result = false;
  int dxl_comm_result = COMM_TX_FAIL;
  uint8_t dxl_id = DXL_ID;
  uint16_t model_number = 0;


/*  result = dxl_wb.init(DEVICENAME, BAUDRATE, &log);
  if (result == false)
    {
      Serial.println(log);
      Serial.println("Failed to init");
    }
  else
    {
      Serial.print("Succeeded to init : ");
      Serial.println(BAUDRATE);  
    }

  result = dxl_wb.ping(dxl_id, &model_number, &log);
  if (result == false)
    {
      Serial.println(log);
      Serial.println("Failed to ping");
    }
  else
    {
      Serial.println("Succeeded to ping");
      Serial.print("id : ");
      Serial.print(dxl_id);
      Serial.print(" model_number : ");
      Serial.println(model_number);
    }
*/

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
/********************************************ICIC*******************************************/

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

}




void loop() {





int result = false;  
uint8_t dxl_id = DXL_ID;
int push_state_1;
int push_state_2;
const char *log;
bool opening = 0;
bool closing = 0;
int dxl_comm_result = COMM_TX_FAIL;     //
uint8_t dxl_error = 0;   
push_state_1  = digitalRead(BDPIN_PUSH_SW_1);
push_state_2  = digitalRead(BDPIN_PUSH_SW_2);
int32_t dxl_present_position = 0;               // Present position

 //while(!Serial);
 //Serial.println("Le moniteur est ouvert");
 
   if((push_state_2 )and opened ==1) //or closing
    {
       dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL_ID, ADDR_PRO_TORQUE_ENABLE, TORQUE_ENABLE, &dxl_error);   // Enable torque
       if (dxl_comm_result != COMM_SUCCESS)
          {
            Serial.println(packetHandler->getTxRxResult(dxl_comm_result));
          }
       else if (dxl_error != 0)
          {
            Serial.println(packetHandler->getRxPacketError(dxl_error));
          }
       else
          {
            Serial.println("Torque is enable");
            dxl_comm_result = packetHandler->read4ByteTxRx(portHandler, DXL_ID, ADDR_PRO_PRESENT_POSITION, (uint32_t*)&dxl_present_position, &dxl_error);
            GOAL_POSITION = dxl_present_position -nb_tick;
            packetHandler->write4ByteTxRx(portHandler, DXL_ID, ADDR_PRO_GOAL_POSITION, GOAL_POSITION, &dxl_error);    //set the goal position 
            do
              {
                // Read present position
                dxl_comm_result = packetHandler->read4ByteTxRx(portHandler, DXL_ID, ADDR_PRO_PRESENT_POSITION, (uint32_t*)&dxl_present_position, &dxl_error);
                if (dxl_comm_result != COMM_SUCCESS)
                  {
                    Serial.print(packetHandler->getTxRxResult(dxl_comm_result));
                  }
                else if (dxl_error != 0)
                  {
                    Serial.print(packetHandler->getRxPacketError(dxl_error));
                  }
        
                Serial.print("[ID:"); Serial.print(DXL_ID);
                Serial.print("] GoalPos:"); Serial.print(GOAL_POSITION);
                Serial.print("  PresPos:"); Serial.print(dxl_present_position);
                Serial.println(" ");
        
              }while((abs(GOAL_POSITION - dxl_present_position) > DXL_MOVING_STATUS_THRESHOLD));
            Serial.println("Drawer is closed");
            dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL_ID, ADDR_PRO_TORQUE_ENABLE, TORQUE_DISABLE, &dxl_error);   // disable torque
            closing = 0;
            opened = 0;
            
          }
    }
  if(((push_state_1 )and digitalRead(pinSwitch)) and opened == 0) //or opening
    {
     dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL_ID, ADDR_PRO_TORQUE_ENABLE, TORQUE_ENABLE, &dxl_error);   // Enable torque
     if (dxl_comm_result != COMM_SUCCESS)
        {
          Serial.println(packetHandler->getTxRxResult(dxl_comm_result));
        }
     else if (dxl_error != 0)
        {
          Serial.println(packetHandler->getRxPacketError(dxl_error));
        }
     else
        {
          Serial.println("Torque is enable");
          dxl_comm_result = packetHandler->read4ByteTxRx(portHandler, DXL_ID, ADDR_PRO_PRESENT_POSITION, (uint32_t*)&dxl_present_position, &dxl_error);
          GOAL_POSITION = nb_tick + dxl_present_position;
          packetHandler->write4ByteTxRx(portHandler, DXL_ID, ADDR_PRO_GOAL_POSITION, GOAL_POSITION, &dxl_error);    //set the goal position 
          do
            {
              // Read present position
              dxl_comm_result = packetHandler->read4ByteTxRx(portHandler, DXL_ID, ADDR_PRO_PRESENT_POSITION, (uint32_t*)&dxl_present_position, &dxl_error);
              if (dxl_comm_result != COMM_SUCCESS)
                {
                  Serial.print(packetHandler->getTxRxResult(dxl_comm_result));
                }
              else if (dxl_error != 0)
                {
                  Serial.print(packetHandler->getRxPacketError(dxl_error));
                }
      
              Serial.print("[ID:"); Serial.print(DXL_ID);
              Serial.print("] GoalPos:"); Serial.print(GOAL_POSITION);
              Serial.print("  PresPos:"); Serial.print(dxl_present_position);
              Serial.println(" ");
      
            }while((abs(GOAL_POSITION - dxl_present_position) > DXL_MOVING_STATUS_THRESHOLD));
          Serial.println("Drawer is opened");
          dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL_ID, ADDR_PRO_TORQUE_ENABLE, TORQUE_DISABLE, &dxl_error);   // disable torque
          opening = 0;
          opened = 1;
          
        }
    }
 if (Serial.available()>0)
  {
    instruction = Serial.readStringUntil('\n');
    Serial.println(instruction);
  }
  if (instruction == "reset")
  {
    Serial.println("je reset le tiroir, veillez le fermer au complet avant de l'ouvrir");
    instruction = "";
    opened = 0;
  }

 
  
  delay(1);//buffer
}
