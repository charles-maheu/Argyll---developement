//PROGRAMME MAIN
//Gere les fonction suivante:

//TOUT ÉCRIRE ENTRE < > EXEMPLE: <mvt> <1> <4>
      //                        SECTION DÉPLACEMENT
      //Déplacement par joint -----------------------> Écrice "<mvt><1><4>" pour que le moteur 1 tourne sens horaire "<mvt>" "<1>" "<6>" pour qu'il tourne sens anti-horaire. Peser sur Enter pour arrêter la rotation  "<0>" to return to choose servo and "<0>" to return to menu
      //Déplacement vers un point ajouté ------------> Écrire "<Goto><1>" pour aller au Point 1 
      //Modifier la vitesse -------------------------> Écrire "<setspeed><X>" pour mettre la vitesse à X
      
      //                         SECTION POINT
      //Ajout d'un point ----------------------------> Écrire "<setpos><1>" to register actual position in position 1
      //Afficher un point ---------------------------> Écrire "<PtDisplay><X>" pour afficher le point No.X
      //Afficher tous les points --------------------> Écrire "<PtAllDisplay>" pour afficher tous les points
      //Changer le nom d'un point -------------------> Écrire "<PtRename><XX><HOME>" pour mettre le nom du point XX à HOME     
      //Réinitialiser un point ----------------------> Écrire "<PtInit><1>" pour réinitialiser le Point 1

      //Copier une position -------------------------> Écrire "PtCopy" pour copier la position actuel
      

      //                       SECTION PROGRAMME
      //Créer un programme --------------------------> Écrire "<prog>" pour créer un programme
      //Rouler un programme -------------------------> Écrire "<ProgRun><X>" pour rouler le programme No.X
      //Afficher un programme -----------------------> Écrire "<ProgDisplay><X>" pour afficher le programme No.X
      //Afficher tous les programmes ----------------> Écrire "<ProgAllDisplay>" pour afficher tous les programmes  
      //Changer le nom d'un programme ---------------> Écrire "<ProgRename><X><Approche>" pour mettre le nom du programme à Approche
      //Modifier un point dans un programme ---------> Écrire "<ProgModify><X><H>" pour modifier le point H du programme X                         //Si on demande le point 3, il modifie le dernier point, donc le P1, des points P2,P3,P1. Pas le point P3   
      //Changer le nom d'un point dans un programme -> Écrire "<ProgPtRename><X><W><RETRAIT>" pour nommer le nom du point X dans le programme W RETRAIT
      //Suprimer un point dans un programme ---------> Écrire "<ProgSupressPt><X><Z>" pour suprimer le point X du programme Z                        //Si on demande le point 3, il suprimme le dernier point, donc le P1, des points P2,P3,P1. Pas le point P3
      //Ajouter un point dans un programme ----------> Écrire "<ProgAddPt><X><Z>" pour ajouter un point dans le programmeX après le point Z 
      
      //Effacer un programme ------------------> Écrire "ProgErase" "X" pour réinitialiser le programme X
      //linker un point avec un PositionRegister     

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver servos = Adafruit_PWMServoDriver();

const int nb_servos = 6;
const int nb_prog = 15;
const int nbpts_prog = 20;
const int nbpts = 30;

int servo;
int point;
float Speed = 3;
int Posmin = 0;
int Posmax = 180;
int pulseMin = 125;
int pulseMax = 665;
int cote;
int curPos = 90;
int NoPoint = 0;
int timedelay = 3000;

int prog=0;
int pts=0;
int pos=0;
int modifprog=0;
int affprog=0;
int runprog=0;
int nameprog=0;

int servosPos[nb_servos];
int servosP_Pos[nb_servos];
int PosPoint[nb_servos];
int pin = 5;

char firstchar;
String DataIn = "";
String stops = "";
String Name = "";
String Nameprog= "";

struct Point{
  int numpts = 0;
  String nom = "No Name";
  int Position[nb_servos];
};

struct Prog{
  int numprog;
  int nb_pts_dans_prog = 1;
  String nomProg = "No Name";
  Point pointProg[nbpts_prog];
};

Point points[nbpts];
Prog progs[nb_prog];


void setup() {

  Serial.begin(9600);

  // start the Drive of the servos
  servos.begin();
  servos.setPWMFreq(60);
  
  // Set the positions of the servos at 90deg
  for (int i = 0; i<nb_servos; i++){
    servosPos[i] = 90;
    servos.setPWM(i,0, angleToPulse(90));
    servosP_Pos[i] = 90; 
  }


  //Initialisation des point;
  for (int i=0; i<nbpts; i++){
    points[i].numpts = i;
    initPoint(i); 
  }

  //Initialisation des programmes;
  for (int i=0; i<nb_prog; i++){
    progs[i].numprog = i; 
  }

}

void loop() {

  if (Serial.available()>0){
    
      firstchar = Serial.read();

      if (firstchar != '<'){
        Serial.println("Error");
        flushserial();
      }

      if (firstchar == '<'){
        
         DataIn = Serial.readStringUntil('>');  //Lectures de la données écrite dans le serial
         //Serial.println(DataIn);

//--------------------------------------------------------------------- SECTION DÉPLACEMENT ---------------------------------------------------------------------
      

//------------------------DÉPLACEMENT PAR JOINT--------------------------------

       if (DataIn.startsWith("mvt")){
        
          SERVO:
          //flushserial(); // Suppress all data in the serial
          //while(Serial.available()<=0){} //Wait for Data in serial
          firstchar = Serial.read();

          if (firstchar != '<'){
            Serial.println("Error");
            flushserial();
            //goto SERVO; //Return to SERVO to not quit the loop
          }

          if (firstchar == '<'){
            servo = Serial.readStringUntil('>').toInt()-1;
            //side of the servo
            if (servo+1 !=0){
              SIDE:
              //flushserial();    
              //while(Serial.available()<=0){}
              firstchar = Serial.read();

              if (firstchar != '<'){
                Serial.println("Error");
                flushserial();
                //goto SIDE; //Return to SIDE to not quit the loop
              }

              if (firstchar == '<'){
                cote = Serial.readStringUntil('>').toInt();
                if (cote != 0){
                  flushserial();
                  deplacement_joint(servo, cote, servosPos[servo]);
                  //goto SIDE; 
                }
              //goto SERVO;     
              }         
            }    
          }
          flushserial();                                     
        }


//---------------------MODIFICATION DE LA VITESSE------------------------------


       if (DataIn.startsWith("setspeed")){

          //ask for the speed
          //flushserial();
          //while(Serial.available()<=0){} //Wait for Data in serial
          firstchar = Serial.read();

          if (firstchar != '<'){
            Serial.println("Error");
            flushserial();
          }

          if (firstchar == '<'){
           Speed = Serial.readStringUntil('>').toInt();
           Serial.println(Speed); 
          }         
       }


//----------------------------------------------------------------------- SECTION POSITION -----------------------------------------------------------------------

//--------------------------SET A POSITION TO CURRENT POSITION-----------------------------------

       if (DataIn.startsWith("setpos")){

        firstchar = Serial.read();

        if (firstchar != '<'){
          Serial.println("Error");
          flushserial();
          }

        if (firstchar == '<'){
          point = Serial.readStringUntil('>').toInt()-1;
          SetPoint(point);
          Serial.println("<");
          affichertous();
          Serial.println(">");
          flushserial();        
        }
       }

//----------------------CHANGE NAME OF A POSITION--------------------------------

       if (DataIn.startsWith("PtRename")){

        firstchar = Serial.read();

        if (firstchar != '<'){
          Serial.println("Error");
          flushserial();
          }

        if (firstchar == '<'){
          point = Serial.readStringUntil('>').toInt()-1;
        }

        firstchar = Serial.read();
        if (firstchar != '<'){
          Serial.println("Error");
          flushserial();
          }

        if (firstchar == '<'){
          Name = Serial.readStringUntil('>');
          points[point].nom = Name;
          Serial.println("<");
          affichertous();
          Serial.println(">");
          flushserial();
        }        
       }


//--------------------------AFFICHER UN POINT-----------------------------------

       if (DataIn.startsWith("PtDisplay")){

          firstchar = Serial.read();

          if (firstchar != '<'){
            Serial.println("Error");
            flushserial();
            }

          if (firstchar == '<'){
            point = Serial.readStringUntil('>').toInt()-1;
            Serial.println('<');
            afficherPoint(point);
            Serial.println(">");
            flushserial();
          }
       }


//--------------------------AFFICHER TOUS LES POINTS-----------------------------------

       if (DataIn.startsWith("PtAllDisplay")){
        Serial.println('<');
        affichertous();
        Serial.println('>');
        flushserial();
       }


//--------------------------DÉPLACEMENT VERS UN POINT-----------------------------------

       if (DataIn.startsWith("Goto")){
        
          firstchar = Serial.read();

          if (firstchar != '<'){
            Serial.println("Error");
            flushserial();
            }

          if (firstchar == '<'){
            point = Serial.readStringUntil('>').toInt()-1;
            gotoPosition(servosPos,points[point].Position);
            flushserial();
          }
       }
 

//----------------------RÉINITIALISER UN POINT--------------------------------

       if (DataIn.startsWith("PtInit")){

          firstchar = Serial.read();

          if (firstchar != '<'){
            Serial.println("Error");
            flushserial();
          }

          if (firstchar == '<'){
            point = Serial.readStringUntil('>').toInt()-1;
            initPoint(point);
            Serial.println("<");
            affichertous();
            Serial.println(">");
            flushserial();
          }
       }



//--------------------------------------------------------------------- SECTION PROGRAMME ---------------------------------------------------------------------


//--------------------------AJOUT D'UN PROGRAMME-----------------------------------

       if (DataIn.startsWith("prog")){

        creerprog();
        Serial.println('<');
        Progaffichertous();
        Serial.println(">");
        prog = prog+1;
       }
       

//--------------------------AFFICHER LES POINTS UN PROGRAMME-----------------------------------

       if (DataIn.startsWith("ProgDisplay")){

          firstchar = Serial.read();

          if (firstchar != '<'){
            Serial.println("Error");
            flushserial();
          }

          if (firstchar == '<'){
            affprog = Serial.readStringUntil('>').toInt()-1;
            Serial.println('<');
            afficherprog(affprog);
            Serial.println(">");
            flushserial();
          }
       }


//--------------------------AFFICHER TOUS LES PROGRAMMES-----------------------------------

       if (DataIn.startsWith("ProgAllDisplay")){

            Serial.println('<');
            Progaffichertous();
            Serial.println(">");
            flushserial();
       }
       

//--------------------------ROULER UN PROGRAMME-----------------------------------

       if (DataIn.startsWith("ProgRun")){

          firstchar = Serial.read();

          if (firstchar != '<'){
            Serial.println("Error");
            flushserial();
          }

          if (firstchar == '<'){
            runprog = Serial.readStringUntil('>').toInt()-1;
            RunProg(runprog);
            flushserial();
          }     
       }


//----------------------CHANGE NAME OF A PROGRAMME--------------------------------

       if (DataIn.startsWith("ProgRename")){

          firstchar = Serial.read();

          if (firstchar != '<'){
            Serial.println("Error");
            flushserial();
          }

          if (firstchar == '<'){
            nameprog = Serial.readStringUntil('>').toInt()-1;

            firstchar = Serial.read();

            if (firstchar != '<'){
              Serial.println("Error");
              flushserial();
            }

            if (firstchar == '<'){

              Nameprog = Serial.readStringUntil('>');
              progs[nameprog].nomProg = Nameprog;
              Serial.println("<");
              Progaffichertous();
              Serial.println(">");
              flushserial();
            }  
          }
       }


       
//----------------------MODIFIER UN POINT DANS UN PROGRAMME-------------------------------  

       if (DataIn.startsWith("ProgModify")){

          firstchar = Serial.read();

          if (firstchar != '<'){
            Serial.println("Error");
            flushserial();
          }

          if (firstchar == '<'){
            modifprog = Serial.readStringUntil('>').toInt()-1;

            firstchar = Serial.read();

            if (firstchar != '<'){
              Serial.println("Error");
              flushserial();
            }

            if (firstchar == '<'){

              point = Serial.readStringUntil('>').toInt()-1;
              modifierPointProg(progs[modifprog].pointProg[point].Position);
              Serial.println("<");
              Progaffichertous();
              Serial.println(">");
              flushserial();
            }  
          }
       }


//----------------------CHANGE NAME OF A POSITION IN A PROGRAM-------------------------------
        
   if (DataIn.startsWith("ProgPtRename")){

          firstchar = Serial.read();

          if (firstchar != '<'){
            Serial.println("Error");
            flushserial();
          }

          if (firstchar == '<'){
            
            modifprog = Serial.readStringUntil('>').toInt()-1;
            firstchar = Serial.read();

            if (firstchar != '<'){
              Serial.println("Error");
              flushserial();
            }

            if (firstchar == '<'){

              point = Serial.readStringUntil('>').toInt()-1;
              firstchar = Serial.read();

              if (firstchar != '<'){
                Serial.println("Error");
                flushserial();
              }

              if (firstchar == '<'){
                Name = Serial.readStringUntil('>');
                progs[modifprog].pointProg[point].nom = Name;
                Serial.println('<');
                Progaffichertous();
                Serial.println(">");
                flushserial();
              }            
            }  
          }    
    }
   

//----------------------SUPRESS POSITION IN A PROGRAM-------------------------------
        
   if (DataIn.startsWith("ProgSupressPt")){

          firstchar = Serial.read();

          if (firstchar != '<'){
            Serial.println("Error");
            flushserial();
          }

          if (firstchar == '<'){
            modifprog = Serial.readStringUntil('>').toInt()-1;

            firstchar = Serial.read();

            if (firstchar != '<'){
              Serial.println("Error");
              flushserial();
            }

            if (firstchar == '<'){

              point = Serial.readStringUntil('>').toInt()-1;
              supprimer(modifprog, point);
              Serial.println("<");
              Progaffichertous();
              Serial.println(">");
              flushserial();
            }  
          }
   }


//----------------------AJOUTER UN POINT DANS UN PROGRAMME-------------------------------

    if (DataIn.startsWith("ProgAddPt")){

          firstchar = Serial.read();

          if (firstchar != '<'){
            Serial.println("Error");
            flushserial();
          }

          if (firstchar == '<'){
            modifprog = Serial.readStringUntil('>').toInt()-1;

            firstchar = Serial.read();

            if (firstchar != '<'){
              Serial.println("Error");
              flushserial();
            }

            if (firstchar == '<'){

              point = Serial.readStringUntil('>').toInt()-1;
              addpoint(modifprog, point);
              Serial.println("<");
              Progaffichertous();
              Serial.println(">");
              flushserial();
            }  
          }
    }
   
// FIN

    }
  }
}

/*//----------------------RÉINITIALISER UN PROGRAMME--------------------------------

       if (DataIn.startsWith("proginit")){

        modifprog = DataIn.substring(5, 6).toInt()-1;
        initPoint(point); 
        Serial.print("Point ");
        Serial.print(point+1);
        Serial.println(" réinitialisé");
       }
*/

//-------------------------------------------------------------------------------------------------------------------------FONCTIONS------------------------------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------- SECTION DÉPLACEMENT ---------------------------------------------------------------------

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
        servos.setPWM(Noservo,0, angleToPulse(Pos)); //Envoi de la donnée itérer au servo
        delay(50);

           //Afin de pouboir arrêter n'importe quand
          if (Serial.available()>0){
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
        servos.setPWM(Noservo,0, angleToPulse(Pos)); //Envoi de la donnée itérer au servo
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


//---------------------------------------------FONCTION DEPLACEMENT D'UN POINT À UN AUTRE--------------------------------------------------


void gotoPosition(int CurPos[nb_servos],int PosToGo[nb_servos]){

  int TempPos[nb_servos];

  for(int i=0;i<nb_servos;i++){
    TempPos[i] =CurPos[i];
  }
     
    
    for(int serv=0;serv<nb_servos;serv++){

     while(TempPos[serv] != PosToGo[serv]){

      if (Serial.available()>0){
        firstchar = Serial.read();

        if (firstchar == '<'){
          DataIn = Serial.readStringUntil('>');
          flushserial();

          if (DataIn == "p"){
            while(Serial.available()<0){}
          }

          if (DataIn == "s"){
            goto STOPS;
          }
        }
        } 

      if (TempPos[serv]< PosToGo[serv]){
      TempPos[serv]+=Speed;
     }
     
     if (TempPos[serv]> PosToGo[serv]){ 
      TempPos[serv]-=Speed;
     }
      STOPS:
      servos.setPWM(serv,0, angleToPulse(TempPos[serv]));
      servosPos[serv] = TempPos[serv];   // Met la position du servo a la position actuel
      delay(10);
     
     }
    }
}


//----------------------------------------------------------------------- SECTION POINT -----------------------------------------------------------------------

//---------------------------------------FONCTION INSERTION D'UNE NOUVELLE POSITION--------------------------------------------

void SetPoint(int pt){

  for (int j=0;j<nb_servos;j++){
    points[pt].Position[j] = servosPos[j];
  }
    points[pt].numpts = pt;
}


//---------------------------------------------FONCTION AFFICHAGE D'UN POINT---------------------------------------------------

void afficherPoint(int point){
  
    Serial.println('!');  // To differ of a Program
    Serial.println(points[point].numpts);
    Serial.println(points[point].nom);
    for (int i=0;i<nb_servos;i++){
      Serial.println(points[point].Position[i]);
    }
}

//---------------------------------------------FONCTION AFFICHAGE DE TOUS LES POINTS---------------------------------------------------

void affichertous(){

  for (int j=0; j<nbpts; j++){
      afficherPoint(j);
  }

}


//---------------------------------------------FONCTION MODIFIER UN POINT--------------------------------------------------

void modifierPoint(int Point){
  
  for (int j=0;j<nb_servos;j++){
    points[Point].Position[j] = servosPos[j];
  }
}


//----------------------------------------FONCTION INITIALISATION D'UN POINT------------------------------------------------

void initPoint(int pointt){

  // Réinitialisation des positions
        for (int k=0; k<nb_servos; k++){
           points[pointt].Position[k] = 90;
        } 
        //Réinitialisation du nom
        points[pointt].nom  = "No Name";
  
}


//--------------------------------------------------------------------- SECTION PROGRAMME ---------------------------------------------------------------------

//---------------------------------------------FONCTION CRÉER PROGRAMME--------------------------------------------------


void creerprog(){

  int nopoint=0; // ON COMMENCE LES NUMÉROS DE POINTS À 1

  progs[prog].numprog = prog;

  int pt = 0;
  
  Start:
   while (Serial.available()<=0){} //Waiting for a point
   
  if (Serial.available()>0){

    DataIn = Serial.readString();

    if (DataIn.startsWith("P")){
        
        pts = DataIn.substring(1, 2).toInt()-1;
        
        progs[prog].pointProg[pt] = points[pts];
        progs[prog].pointProg[pt].numpts = nopoint;
        nopoint+=1;
        progs[prog].nb_pts_dans_prog +=1;
      
       pt+=1;

    }
    
        if (DataIn.startsWith("ok")){

          goto End;
    }
    goto Start;
  }
  
  End:
  ;
}


//---------------------------------------------FONCTION ROULER UN PROGRAMME--------------------------------------------------

void RunProg(int NoProg){

  afficherPoint(0);
  
  gotoPosition(servosPos, progs[NoProg].pointProg[0].Position);
  //delay(timedelay);

   for(int i=1;i<progs[NoProg].nb_pts_dans_prog;i++){

    //afficherPoint(i);
    gotoPosition(progs[NoProg].pointProg[i-1].Position,progs[NoProg].pointProg[i].Position);
    //delay(timedelay);
 
      
   }
  
}


//----------------------------------------FONCTION AFFICHER POINTS D'UN PROGRAMME-----------------------------------------------

void afficherprog(int programme){
  
  Serial.println('&');  // To differ of a Point
  Serial.println(progs[programme].numprog);
  Serial.println(progs[programme].nomProg);
  for(int ptt=0;ptt<progs[programme].nb_pts_dans_prog; ptt++){
    Serial.println('!');
    Serial.println(progs[programme].pointProg[ptt].numpts);
    Serial.println(progs[programme].pointProg[ptt].nom);
    
    for(int i=0;i<nb_servos;i++){
      Serial.println(progs[programme].pointProg[ptt].Position[i]);
    }
  }
}


//----------------------------------------FONCTION AFFICHER TOUS LES PROGRAMMES-----------------------------------------------

void Progaffichertous(void){

   for (int j=0; j<nb_prog; j++){
      afficherprog(j);
  }
  
}


//----------------------------------------FONCTION MODIFIER POINT D'UN PROGRAMME-----------------------------------------------

void modifierPointProg(int Position[nb_servos]){

  for (int i=0; i<nb_servos; i++){
    Position[i] =  servosPos[i];
  }
}


//----------------------------------------FONCTION SUPRIMER UN POINT D'UN PROGRAMME-----------------------------------------------

void supprimer(int prog, int point){

  for (int i=point; i<progs[prog].nb_pts_dans_prog; i++){
    
     progs[prog].pointProg[i]=progs[prog].pointProg[i+1];
      progs[prog].pointProg[i].numpts-=1;
     
  }
 
  progs[prog].nb_pts_dans_prog-=1;
  
}


//----------------------------------------FONCTION AJOUTER UN POINT DANS UN PROGRAMME-----------------------------------------------

void addpoint(int prog, int point){

  progs[prog].nb_pts_dans_prog +=1;
  
  for (int i=progs[prog].nb_pts_dans_prog; i>point; i--){
     progs[prog].pointProg[i]=progs[prog].pointProg[i-1];
     progs[prog].pointProg[i].numpts=i;
  }

    for (int j=0; j<nb_servos; j++){
      progs[prog].pointProg[point+1].Position[j] = servosPos[j];
    }
}


//----------------------------------------FONCTION AJOUTER UN POINT DANS UN PROGRAMME-----------------------------------------------

/*void initprog(prog){

  for (int i=0;i<nb_pts_dans_prog; i++){
    initPoint(progs[prog].pointProg[i]);
  }

   progs[prog].numprog = 0;
   progs[prog.nomProg = "No Name";
   nb_pts_dans_prog = 0;
  
}*/


//---------------------------------------- FONCTION ANGLE TO PULSE ------------------------------------------------
int angleToPulse(int angle){
  int pulse = map(angle, Posmin, Posmax, pulseMin, pulseMax);
  return pulse;
}


void flushserial(void){
  while (Serial.available())
  {
      Serial.read();  
  }
}



  
