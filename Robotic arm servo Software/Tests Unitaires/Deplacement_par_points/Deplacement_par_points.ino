//Permettre au bras de se déplacer d'un point à un autre.

#include <Servo.h>

const int nb_servos = 6;
int servo;
float Speed = 1;
int j = 90;
int Posmin = 0;
int Posmax = 180;
int val;
int curPos[nb_servos];
int nb_positions = 5;
int servosPos[nb_servos];
int P1[] = {90, 90, 90, 90, 90, 90};
int P2[] = {16, 43, 146, 94, 7, 101};
int pin = 5;
int pinP1 = 12;
int pinP2 = 13;

Servo Servos[nb_servos];

void setup() {
  
 Serial.begin(9600);

  //attache les servos aux pin du arduino
  for(int i=0;i<nb_servos;i++){
    Servos[i].attach(pin);
    pinMode(pin, OUTPUT); 
    pin++;
  }

  // Set la position de départ des servo a 90
  for (int i = 0; i<nb_servos; i++){
    servosPos[i] = 90;
    Servos[i].write(90);
  }

}

void loop() {

  if (Serial.available() > 0){

       gotoPosition(P1,P2);
    }

}

//Fonction pour aller d'une position à une autre selon le nombre de servos
void gotoPosition(int CurPos[nb_servos],int PosToGo[nb_servos]){
  
  int TempPos[nb_servos];
  int serv = 0;

  //Set le TempPos à 0
  for (int i=0;i<nb_servos; i++){
    TempPos[i] = CurPos[i];
  }

    start:
    //Afin d'accèder à tous les servos
    for (serv = 0;serv<nb_servos;serv++){
      
      // aller de l'angle actuel à l'autre d'un servo
    if (CurPos[serv]< PosToGo[serv]){
      //Serial.println(CurPos[serv]); 
      CurPos[serv]+=Speed;
      /*Serial.print("Servo ");
      Serial.print(serv);
      Serial.print(": ");
      Serial.println(CurPos[serv]);
      //Serial.println(CurPos[0]);*/
       Servos[serv].write(CurPos[serv]);
    }
    
    // aller de l'angle actuel à l'autre d'un servo
    if (CurPos[serv]> PosToGo[serv]){ 
      CurPos[serv]-=Speed;
      /*Serial.print("Servo ");
      Serial.print(serv);
      Serial.print(": ");
      Serial.println(CurPos[serv]);
      //Serial.println(CurPos[0]);*/
      Servos[serv].write(CurPos[serv]);
    }

    delay(50);
    
    }

    for(int i=0;i<nb_servos;i++){
      if (CurPos[i] != PosToGo[i]){
       goto start; 
      }
    }

    /*for(int j=0;j<nb_servos;j++){
      Serial.println(CurPos[j]);
    }*/

    

    
}
