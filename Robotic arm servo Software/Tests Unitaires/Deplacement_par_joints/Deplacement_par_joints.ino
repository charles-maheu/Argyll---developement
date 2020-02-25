#include <Servo.h>

const int nb_servos = 6;
int servo;
int val;
float Speed = 0.5;
float pos;
float CurrentPos;
int Posmin = 0;
int Posmax = 180;
float j = 90;
int pin = 5;

String DataIn = "";
String stops = "";

int servosPos[nb_servos];
int servosP_Pos[nb_servos];

Servo Servos[nb_servos];


void setup() {

  Serial.begin(9600);

    for(int i=0;i<nb_servos;i++){
    Servos[i].attach(pin);
    pinMode(pin, OUTPUT); 
    //Serial.print("Servo:");
    //Serial.print(i);
    //Serial.print(" a la pin: ");
    //Serial.println(pin); 
    pin++;
  }

}

void loop() {

  if (Serial.available()>0){
    
    DataIn = Serial.readString();

      label:

      servo = DataIn.substring(0, 1).toInt();
      //Serial.print("Servo: ");
      //Serial.println(servo);
      val = DataIn.substring(1,2).toInt();
      //Serial.println(DataIn.substring(1,2));
      Serial.println(val);

      //DataIn = Serial.readString();
      //while (DataIn.startsWith("14")){
      if (val == 4){
        Serial.println("te");
        if (val != 4){
          Serial.println("te");
          goto label6;
        }
        
        String stops = Serial.readString();

         if (stops.startsWith("ok")){
            goto finish;
         }
        else{
          if (j >= Posmin){
            j = j-Speed;
          }
          if (j<Posmin){
            j = Posmin;
          }  
          //Serial.println(j);
          Servos[servo-1].write(j);
          //delay(50);                  
        }  
      }

    label6:

    while (val = 6){
     // if (DataIn.substring(1,2) != 6){
       // }
        String stops = Serial.readString();

         if (stops.startsWith("ok")){
            goto finish;
         }

          if (j <= Posmax){
            j = j+Speed;
          }
          if (j>Posmax){
            j = Posmax;
          }
          
          Serial.println(j);
          Servos[servo-1].write(j);
          delay(50);
      }

       goto label;
    //}
    finish:
    Serial.println("done");
  }
        Serial.println("redone");
}
