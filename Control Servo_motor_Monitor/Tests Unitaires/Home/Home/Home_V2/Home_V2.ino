#include <Servo.h>
Servo servo01;
Servo servo02;
Servo servo03;
Servo servo04;
Servo servo05;
Servo servo06;

int servo1Home , servo2Home , servo3Home, servo4Home, servo5Home, servo6Home; // current Home position
int servo1HHome , servo2HHome , servo3HHome, servo4HHome, servo5HHome, servo6HHome; // previous Home position


String dataIn = "";


void setup() {

  Serial.begin(9600);

    servo1Home = 90;
  servo2Home = 90;
  servo3Home = 90;
  servo4Home = 90;
  servo5Home = 90;
  servo6Home = 90;

  servo1HHome = 90;
  servo2HHome = 90;
  servo3HHome = 90;
  servo4HHome = 90;
  servo5HHome = 90;
  servo6HHome = 90;

}


void Print(){
               Serial.println("");
               Serial.println("Current Home:"); 
               Serial.println(servo1Home);
               Serial.println(servo2Home);
               Serial.println(servo3Home); 
               Serial.println(servo4Home); 
               Serial.println(servo5Home);
               Serial.println(servo6Home);    
                                             
               Serial.println("Previous Home:");
               Serial.println(servo1HHome);
               Serial.println(servo2HHome);
               Serial.println(servo3HHome);
               Serial.println(servo4HHome);
               Serial.println(servo5HHome);
               Serial.println(servo6HHome); 
  
}

void Previous(){
              servo1HHome = servo1Home;
              servo2HHome = servo2Home;
              servo3HHome = servo3Home;
              servo4HHome = servo4Home;
              servo5HHome = servo5Home;
              servo6HHome = servo6Home;
}

void loop() {
  
    while (Serial.available()==0){}
  
   dataIn = Serial.readString();
  
          
          if (dataIn.startsWith("Hs1")){  
              Previous();   
             servo1Home = dataIn.substring(3, dataIn.length()).toInt(); // Extract only the number. E.g. from "s1120" to "120" 
          }

          if (dataIn.startsWith("Hs2")){
              Previous();
             servo2Home = dataIn.substring(3, dataIn.length()).toInt(); // Extract only the number. E.g. from "s1120" to "120"
          }

           if (dataIn.startsWith("Hs3")){
              Previous();
             servo3Home = dataIn.substring(3, dataIn.length()).toInt(); // Extract only the number. E.g. from "s1120" to "120"
          }

          if (dataIn.startsWith("Hs4")){
              Previous();
             servo4Home = dataIn.substring(3, dataIn.length()).toInt(); // Extract only the number. E.g. from "s1120" to "120"
          }

          if (dataIn.startsWith("Hs5")){
              Previous();
             servo5Home = dataIn.substring(3, dataIn.length()).toInt(); // Extract only the number. E.g. from "s1120" to "120"
          }
          
          if (dataIn.startsWith("Hs6")){
              Previous();
             servo6Home = dataIn.substring(3, dataIn.length()).toInt(); // Extract only the number. E.g. from "s1120" to "120"
          }


          if (dataIn.startsWith("Home")){
            Print();  
          }
          
}
