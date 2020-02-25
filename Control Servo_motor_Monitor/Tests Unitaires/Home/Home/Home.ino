
//Inserer un nouvelle position
//Entrer dans le moniteur s14 pour faire tourner le servo 1 à gauche ou s16 à droite
//Pour changer la vitesse a X, ecrire vitX. exemple vit0.5
//Pour aller au Home, ecrire goto

#include <Servo.h>

const int nb_servos = 6;
int servo;
float Speed = 1;
int j = 90;
int Posmin = 50;
int Posmax = 110;
int val;
int curPos = 90;

Servo Servos[nb_servos];

int servosPos[nb_servos];
int servosP_Pos[nb_servos];
int pin = 5;


String dataIn = "";


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
    servosP_Pos[i] = 90; 
  }
}


void loop() {

    if (Serial.available()>0){

      //Lecture de la donneée recu du moniteur
      dataIn = Serial.readString();  // Read the data as string
      String dataInS = dataIn.substring(2, dataIn.length()); // Extrtait le dernier nombre pour savoir si on tourne à gauche ou à droite
      val = dataInS.toInt();  // Convert the string into integer

      //Pour la modification de la vitesse avec le moniteur
      //Ne fonctionne pas encore
      if (dataIn.startsWith("vit")){
        Speed =  dataIn.substring(3, dataIn.length()).toInt();   
        Serial.println(Speed);
      }

      servo = dataIn.substring(1,2).toInt() - 1;
      curPos = Servos[servo].read();
      deplacement_joint(servo, val, curPos);
           
     //Affichage de la nouvelle position du robot
    if (dataIn.startsWith("ok")) {
            Serial.println("Current Home:");

        
            for (int i = 0; i<nb_servos;i++){
                Serial.println(servosPos[i]);
            }

       //Affichage de l'ancienne position du robot
      Serial.println("Previous Home:");
            for (int i = 0; i<nb_servos;i++){
                Serial.println(servosP_Pos[i]);
            }

          SetPreviousRobotPos(servosPos,servosP_Pos); 

    }

      
    }
}



//Set l'ancienne position du joint
void SetPreviousPos(int CurrentPos[nb_servos], int PreviousPos[nb_servos], int servo){

    PreviousPos[servo] = CurrentPos[servo];
}


//Set l'ancienne position du robot
void SetPreviousRobotPos(int CurrentPos[nb_servos], int PreviousPos[nb_servos]){

  for (int i=0; i<nb_servos;i++){
    PreviousPos[i] = CurrentPos[i];
  }
}


//Set la nouvelle position du joint
void SetPos(int servo,int Pos){
      
      servosPos[servo] = Pos;
}

void deplacement_joint(int Noservo, int cote, int curPos){

  j = curPos;

      //Rotation vers la gauche
     if(cote == 4){
      while(cote == 4){
        
          if (j >= Posmin){
            j = j-Speed;
          }
          if (j<Posmin){
            j = Posmin; // Si j devient plus petit que 0, on le met à 0
          }
        Servos[Noservo].write(j); //Envoi de la donnée itérer au servo
        Serial.println(Servos[Noservo].read());
        delay(50);

           //Afin de pouboir arrêter n'importe quand
          if (Serial.available()>0){
             //dataIn = Serial.readString();
             //cote = dataIn.substring(1,2).toInt();
             cote = 0;
          }   
      }
            SetPos(Noservo,Servos[Noservo].read()); //lit l'angle du servo et l'insere comme nouvelle position
     }

      //Rotation vers la droite
     if(cote == 6){
      while(cote == 6){
        
          if (j <= Posmax){
            j = j+Speed;
          }
          if (j>Posmax){
            j = Posmax;
          }
        Servos[Noservo].write(j); //Envoi de la donnée itérer au servo
        Serial.println(Servos[Noservo].read());
        delay(50);

           //Afin de pouboir arrêter n'importe quand
           //on peut l'enlever si on a un bouton car la boucle while va fonctionner
          if (Serial.available()>0){
             cote = 0;
          }   
      }
            SetPos(Noservo,Servos[Noservo].read()); //lit l'angle du servo et l'insere comme nouvelle position
     }
}
}


//Pour aller a une position
void gotoPos(RobotPos[nb_servos]){
  
  int tempPos[nb_servos];

  for (int i=0;i<=nb_servos; i++){
    tempPos[i] = Servos[i].read();
  }

  
  
}
  
