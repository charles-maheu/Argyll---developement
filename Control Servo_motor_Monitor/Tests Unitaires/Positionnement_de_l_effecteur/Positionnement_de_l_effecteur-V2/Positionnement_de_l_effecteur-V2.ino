
//Inserer un nouvelle position
//Entrer dans le moniteur s14 pour faire tourner le servo 1 à gauche ou s16 à droite
//Pour changer la vitesse a X, ecrire vitX. exemple vit0.5

#include <Servo.h>

const int nb_servos = 6;
int servo;
float Speed = 1;
int j = 90;
int Posmin = 0;
int Posmax = 180;
int val;
int curPos = 90;
const int nbpts = 40;
int pts = 1;

Servo Servos[nb_servos];

int servosPos[nb_servos];
int servosP_Pos[nb_servos];
int pin = 5;


String dataIn = "";

struct Point{
  int numpts;
  String nom;
  int ServosPos[nb_servos];
};

Point Points[nbpts];


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
    Serial.println(Servos[i].read());
  }

  for (int i = 1; i<=nbpts; i++){
    Points[i].numpts = i;
    Points[i].nom = "EMPTY";
    for (int k=0;k<nb_servos;k++){
      Points[i].ServosPos[k] = 90;
    }
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

      //Triage des données reçu
      servo = dataIn.substring(1,2).toInt() - 1;
      curPos = Servos[servo].read();
      deplacement_joint(servo, val, curPos);
           
     //Affichage de la nouvelle position du robot
    if (dataIn.startsWith("ok")) {
            Serial.println("Current Position:");

        
            for (int i = 0; i<nb_servos;i++){
                Serial.println(servosPos[i]);
            }

       //Affichage de l'ancienne position du robot
      Serial.println("Previous Position:");
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

      Points[pts].numpts = pts;
      Serial.print("Position du Point: ");
      Serial.println(pts);

      for (int i=0;i<nb_servos; i++){

        Points[pts].ServosPos[i] = servosPos[i];
        Serial.println(Points[pts].ServosPos[i]);
      }

 

      

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
