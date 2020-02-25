/*
     Servo Motor Control using Arduino and PCA9685 Driver
           by Dejan, https://howtomechatronics.com
           
     Library: https://github.com/NachtRaveVL/PCA9685-Arduino
*/
#include <Wire.h>
#include "PCA9685.h"
PCA9685 driver;
// PCA9685 outputs = 12-bit = 4096 steps
// 2.5% of 20ms = 0.5ms ; 12.5% of 20ms = 2.5ms
// 2.5% of 4096 = 102 steps; 12.5% of 4096 = 512 steps
PCA9685_ServoEvaluator pwmServo(102, 470); // (-90deg, +90deg)
// Second Servo
// PCA9685_ServoEvaluator pwmServo2(102, 310, 505); // (0deg, 90deg, 180deg)
void setup() {
  Wire.begin();                 // Wire must be started first
  Wire.setClock(400000);        // Supported baud rates are 100kHz, 400kHz, and 1000kHz
  driver.resetDevices();        // Software resets all PCA9685 devices on Wire line
  driver.init(B000000);         // Address pins A5-A0 set to B000000
  driver.setPWMFrequency(50);   // Set frequency to 50Hz
}
void loop() {
  driver.setChannelPWM(0, pwmServo.pwmForAngle(-90));
  delay(1000);
  driver.setChannelPWM(0, pwmServo.pwmForAngle(0));
  delay(1000);
  driver.setChannelPWM(0, pwmServo.pwmForAngle(90));
  delay(1000);
}
