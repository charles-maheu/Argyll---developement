//Tests pour la lectures des boutons

int PinG = 12;
int PinD = 13;


void setup() {
  Serial.begin(9600);

  pinMode(PinG, INPUT);
  pinMode(PinD, INPUT);
}

void loop() {
  
    Serial.println(digitalRead(PinG));
     Serial.println(digitalRead(PinD));

}
