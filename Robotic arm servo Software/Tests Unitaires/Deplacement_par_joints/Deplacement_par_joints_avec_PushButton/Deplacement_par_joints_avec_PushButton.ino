
//Afin de pouvoir déplacer n'importe quel joint à l'aide d'un bouton
//Pour que ca fonctionne, ecrire 14 ou 16, 1 pour le servo choisis et 4 ou 6 pour gauche ou droite
//Pour changer la vitesse, écrire setspeed1 (vitesse de 1), ou setspeed2 (vitesse de 2), ou setspeedX (vitesse de X)

#include <Servo.h>

const int nb_servos = 6;
int servo;
int val;
float Speed = 2;
float pos;
float CurrentPos;
int Posmin = 0;
int Posmax = 180;
float j = 90;
int pin = 5;
int pinG = 12;
int pinD = 13;

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
    pin++;
  }

  pinMode(pinG, INPUT);
  pinMode(pinD, INPUT);

}

void loop() {
  
    //if (Serial.available()>0){

      //DataIn = Serial.readString();  //Lectures de la données écrite dans le serial

      //Pour la modification de la vitesse avec le moniteur
      if (DataIn.startsWith("setspeed")){
        Speed =  DataIn.substring(8, DataIn.length()).toInt();   
        //Serial.println(Speed);
      }

      //Extraction du Servo et de gauche(4) ou droite(6)
      //servo = DataIn.substring(0, 1).toInt();
      //val = DataIn.substring(1,2).toInt();

      int Gauche = digitalRead(pinG);
      int Droite = digitalRead(pinD);

    //Rotation vers la gauche
     if(Gauche == HIGH){
      while(Gauche == HIGH){
        
          if (j >= Posmin){
            j = j-Speed;
          }
          if (j<Posmin){
            j = Posmin; // Si j devient plus petit que 0, on le met à 0
          }
        Servos[0].write(j); //Envoi de la donnée itérer au servo
        Serial.println(Servos[0].read());
        delay(50);
          Gauche = digitalRead(pinG);
           //Afin de pouboir arrêter n'importe quand
          /*if (Serial.available()>0){
             DataIn = Serial.readString();
             val = DataIn.substring(1,2).toInt();
          }*/   
      }
     }
        //Comme le cote gauche mais pour le cote droite
          if(Droite == HIGH){
      while(Droite == HIGH){
        
          if (j <= Posmax){
            j = j+Speed;
          }
          if (j>Posmax){
            j = Posmax;
          }
         Servos[0].write(j);
         Serial.println(j);
         delay(50);
          /*if (Serial.available()>0){
             DataIn = Serial.readString();
             val = DataIn.substring(1,2).toInt();
          }  */
          Droite = digitalRead(pinD);
      }
     }
    //}

}
