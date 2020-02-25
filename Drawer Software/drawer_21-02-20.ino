/*
 * Étienne Bellerose
 * 21 février 2020
 * Open/close drawer
 * this code receive a input command from the button on the OpenCr card
 * Button 2 is closing the drawer
 * Button 1 is opening the drawer
 * Next implementation: Add a feedback from the Dynamixel so that it knows when the drawer is opened or closed
 */


#include <DynamixelWorkbench.h>
/*#include <iostream>
#include <string>
using namespace std;*/

#if defined(__OPENCM904__)
  #define DEVICE_NAME "3" //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP
#elif defined(__OPENCR__)
  #define DEVICE_NAME ""
#endif   

#define BAUDRATE  57600
#define DXL_ID    1

DynamixelWorkbench dxl_wb;


void setup() {
  Serial.begin(57600);
  //while(!Serial); // Wait for Opening Serial Monitor
  pinMode(BDPIN_PUSH_SW_1, INPUT);
  pinMode(BDPIN_PUSH_SW_2, INPUT);
  const char *log;
  bool result = false;

  uint8_t dxl_id = DXL_ID;
  uint16_t model_number = 0;

  result = dxl_wb.init(DEVICE_NAME, BAUDRATE, &log);
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

}

void loop() {
bool result = false;  
String instruction = "";
uint8_t dxl_id = DXL_ID;
int push_state_1;
int push_state_2;
const char *log;

push_state_1  = digitalRead(BDPIN_PUSH_SW_1);
push_state_2  = digitalRead(BDPIN_PUSH_SW_2);

 //while(!Serial);
 //Serial.println("Le moniteur est ouvert");
 
   if(push_state_2 == 1)
  {
      result = dxl_wb.wheelMode(dxl_id, 0, &log);
  if (result == false)
  {
    Serial.println(log);
    Serial.println("Failed to change wheel mode");
  }
  else
  {
    Serial.println("Succeed to change wheel mode");
}
    Serial.println("Closing the drawer");
    dxl_wb.goalVelocity(dxl_id, (int32_t)50);
     delay(3900*2);
    dxl_wb.goalVelocity(dxl_id, (int32_t)0); 
  }
  if(1 == push_state_1)
  {
      result = dxl_wb.wheelMode(dxl_id, 0, &log);
  if (result == false)
  {
    Serial.println(log);
    Serial.println("Failed to change wheel mode");
  }
  else
  {
    Serial.println("Succeed to change wheel mode");
}
    Serial.println("Opening the drawer");
    dxl_wb.goalVelocity(dxl_id, (int32_t)-50);
     delay(3900*2);
    dxl_wb.goalVelocity(dxl_id, (int32_t)0); 
  }
 /*if (Serial.available()>0)
  {
    instruction = Serial.readStringUntil('\n');
    Serial.println(instruction);
  }
  if (instruction == "ouvrir")
  {
    Serial.println("j'ouvre le tiroir");
    dxl_wb.goalVelocity(dxl_id, (int32_t)-200);
     delay(3000);
    dxl_wb.goalVelocity(dxl_id, (int32_t)0); 
  }
  if (instruction == "fermer")
  {
    Serial.println("je ferme le tiroir");
    dxl_wb.goalVelocity(dxl_id, (int32_t)200);
     delay(3000);
    dxl_wb.goalVelocity(dxl_id, (int32_t)0); 
  }
  */
  dxl_wb.torqueOff(dxl_id, &log);
  delay(200);//buffer
}
