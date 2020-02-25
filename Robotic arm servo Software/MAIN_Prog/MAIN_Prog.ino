//PROGRAMME MAIN
//Gere les fonction suivante:
      //Déplacement par joint ---------------> écrice "mvt14" pour qu'il tourne sens horaire "deplace16" pour qu'il tourne sens anti-horaire. Peser sur Enter pour arrêter la rotation
      //Ajout d'un point --------------------> mettre les joints aux position désiré avec l'a fonction si-dessus, puis écrire "setposition"
      //Afficher un point -------------------> Écrire "afficherPX" pour afficher le point No.X
      //Déplacement vers un point ajouté ----> Écrire "gotoP1" pour aller au Point 1 
      //Déplacement entre deux points -------> Écrire "FromP1ToP2" pour aller du point 1 au Point 2
      //Créer un programme ------------------> Écrire "prog" pour créer un programme
      //Rouler un programme ------------------> Écrire "RunProg" pour rouler un programme  
      //Modifier un point -------------------> Écrire "modifierP1" pour modifier le Point 1

#include <Servo.h>

const int nb_servos = 6;
const int nb_points = 20;
const int nb_prog = 20;
int servo;
int point;
float Speed = 3;
int Posmin = 0;
int Posmax = 180;
int cote;
int curPos = 90;
int NoPoint = 0;

int prog=0;
int pts=0;
int pos =0;

Servo Servos[nb_servos];

int servosPos[nb_servos];
int servosP_Pos[nb_servos];
int PosPoint[nb_servos];
int Points[nb_points][nb_servos];
int Programme[nb_prog][nb_points][nb_servos];
int nb_point_par_prog[nb_prog];
int pin = 5;


String DataIn = "";
String stops = "";


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

  //Set Tous les points à 0
  for (int j=0; j<nb_points;j++){
   // Serial.print("Point: ");
    //Serial.println(j);
   for (int i = 0; i<nb_servos; i++){
    Points[j][i] = 90;
    //Serial.println(Points[j][i]);
  }   
  }

  // Initialise le nombre de points par programme à 0
  for (int i=0; i<nb_prog;i++){
    nb_point_par_prog[i]= 0;
  }

}

void loop() {

  if (Serial.available()>0){

      DataIn = Serial.readString();  //Lectures de la données écrite dans le serial
      

//------------------------DÉPLACEMENT PAR JOINT--------------------------------

       if (DataIn.startsWith("mvt")){

          //Extraction du Servo et de gauche(4) ou droite(6)
          servo = DataIn.substring(3, 4).toInt()-1;
          cote = DataIn.substring(4,5).toInt();       
                    
          deplacement_joint(servo, cote, servosPos[servo]); 
       }


//---------------------MODIFICATION DE LA VITESSE------------------------------


       if (DataIn.startsWith("setspeed")){

          //Extraction du Servo et de gauche(4) ou droite(6)
          Speed = DataIn.substring(8, 9).toInt();

         Serial.print("Vitesse modifié à ");
         Serial.println(Speed);
                    
       }



//--------------------------AJOUT D'UN POINT-----------------------------------

       if (DataIn.startsWith("setposition")){

        SetPoint();
        
       }


//--------------------------MODIFIER UN POINT-----------------------------------

       if (DataIn.startsWith("modifierP")){

        point = DataIn.substring(9, 10).toInt()-1;

        modifierPoint(point);
        
       }

//--------------------------AFFICHER UN POINT-----------------------------------

       if (DataIn.startsWith("afficherP")){

        point = DataIn.substring(9, 10).toInt()-1; 

        afficherPoint(point);
        
       }


//--------------------------DÉPLACEMENT VERS UN POINT-----------------------------------

       if (DataIn.startsWith("gotoP")){

        point = DataIn.substring(5, 6).toInt()-1; 
        
        for(int i=0;i<nb_servos;i++){
          PosPoint[i]= Points[point][i];
        }

        gotoPosition(servosPos,PosPoint);
        
       }


//--------------------------AJOUT D'UN PROGRAMME-----------------------------------

       if (DataIn.startsWith("prog")){

        creerprog();
        prog = prog+1;
        Serial.print("Programme ");
        Serial.print(prog);
        Serial.println(" Enregistré");
        
       }


//--------------------------ROULER UN PROGRAMME-----------------------------------

       if (DataIn.startsWith("RunProg")){
        
         prog = DataIn.substring(7, 8).toInt()-1;

         RunProg(prog);
        
       }
       
    Serial.println("done");
  }

}





//-------------------------------------------------------------------------------------------------------------------------FONCTIONS------------------------------------------------------------------------------------------------------------------------

//---------------------------------------FONCTION DEPLACEMENT PAR JOINTS------------------------------------
void deplacement_joint(int Noservo, int cote, int Pos){


      //Rotation vers la gauche
     if(cote == 4){
      while(cote == 4){
        
          if (Pos >= Posmin){
            Pos = Pos-Speed;
          }
          if (Pos<Posmin){
            Pos = Posmin; // Si j devient plus petit que 0, on le met à 0
          }
        Servos[Noservo].write(Pos); //Envoi de la donnée itérer au servo
        Serial.println(Servos[Noservo].read());
        delay(50);

           //Afin de pouboir arrêter n'importe quand
          if (Serial.available()>0){
             //dataIn = Serial.readString();
             //cote = dataIn.substring(1,2).toInt();
             cote = 0;
          }   
      }
     }

      //Rotation vers la droite
     if(cote == 6){
      while(cote == 6){
        
          if (Pos <= Posmax){
            Pos = Pos+Speed;
          }
          if (Pos>Posmax){
            Pos = Posmax;
          }
        Servos[Noservo].write(Pos); //Envoi de la donnée itérer au servo
        Serial.println(Servos[Noservo].read());
        delay(50);

           //Afin de pouboir arrêter n'importe quand
           //on peut l'enlever si on a un bouton car la boucle while va fonctionner
          if (Serial.available()>0){
             cote = 0;
          }   
      }
     }
             servosPos[Noservo] = Pos;
}




//---------------------------------------FONCTION INSERTION D'UNE NOUVELLE POSITION--------------------------------------------

void SetPoint(){

  for (int j=0;j<nb_servos;j++){
    Points[NoPoint][j] = servosPos[j];
  }
  
    Serial.print("Point ");
    Serial.print(NoPoint+1);
    Serial.println(" enregistré");
    NoPoint++;
}


//---------------------------------------------FONCTION AFFICHAGE D'UN POINT---------------------------------------------------

void afficherPoint(int point){

  Serial.print("Point: ");
    Serial.println(point+1);
    for (int i=0;i<nb_servos;i++){
      Serial.println(Points[point][i]);
    }
}


//---------------------------------------------FONCTION DEPLACEMENT D'UN POINT À UN AUTRE--------------------------------------------------

/*void gotoPosition(int CurPos[nb_servos],int PosToGo[nb_servos]){
  
  int TempPos[nb_servos];
  int serv = 0;

  //Set le TempPos au CurPos pour ne pas changer la CurPos
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
      //Serial.println(CurPos[0]);
       Servos[serv].write(CurPos[serv]);
    }
    
    // aller de l'angle actuel à l'autre d'un servo
    if (CurPos[serv]> PosToGo[serv]){ 
      CurPos[serv]-=Speed;
      /*Serial.print("Servo ");
      Serial.print(serv);
      Serial.print(": ");
      Serial.println(CurPos[serv]);
      //Serial.println(CurPos[0]);
      Servos[serv].write(CurPos[serv]);
    }

    delay(50);
    
    }

    for(int i=0;i<nb_servos;i++){
      if (CurPos[i] != PosToGo[i]){
       goto start; 
      }
    }
}*/


void gotoPosition(int CurPos[nb_servos],int PosToGo[nb_servos]){


    for(int serv=0;serv<nb_servos;serv++){

     while(CurPos[serv] != PosToGo[serv]){

      if (CurPos[serv]< PosToGo[serv]){
      CurPos[serv]+=Speed;
     }
     
     if (CurPos[serv]> PosToGo[serv]){ 
      CurPos[serv]-=Speed;
     }
      Serial.print("Servo ");
      Serial.print(serv);
      Serial.print(": ");
      Serial.println(CurPos[serv]);
      Servos[serv].write(CurPos[serv]);
      delay(50);
     
     }
    }
}


//---------------------------------------------FONCTION MODIFIER UN POINT--------------------------------------------------

void modifierPoint(int Point){
  
  for (int j=0;j<nb_servos;j++){
    Points[Point][j] = servosPos[j];
  }
  
    Serial.print("Point ");
    Serial.print(Point+1);
    Serial.println(" modifié");
}


//---------------------------------------------FONCTION CRÉER PROGRAMME--------------------------------------------------


void creerprog(){
  
    Serial.println("Entrer un point");
  Start:
   while (Serial.available()<=0){
    //Serial.println("Entrer un point");
   }
   
  if (Serial.available()>0){

    DataIn = Serial.readString();

    if (DataIn.startsWith("P")){

          pts = DataIn.substring(1, 2).toInt()-1;
          

          for(pos=0;pos<nb_servos;pos++){
            
          Programme[prog][pts][pos] = Points[pts][pos];
          //Serial.println(Programme[prog][pts][pos]);  
               
          }
      nb_point_par_prog[prog]+=1;
      Serial.print("Point ");
      Serial.print(pts+1);
      Serial.println(" Enregistré");
          
    }
    
        if (DataIn.startsWith("ok")){

          goto End;
    }
    goto Start;
  }
  
  End:
  ;
}


//---------------------------------------------FONCTION RUN PROGRAMME--------------------------------------------------

void RunProg(int NoProg){

  //Serial.println(nb_point_par_prog[NoProg]);
  
  Serial.println("Points: 1");
  gotoPosition(servosPos,Programme[NoProg][0]);

   for(int i=1;i<nb_point_par_prog[NoProg];i++){
    
    Serial.print("Points: ");
    Serial.println(i+1);
    gotoPosition(Programme[NoProg][i-1],Programme[NoProg][i]);


      
   }
  
}
