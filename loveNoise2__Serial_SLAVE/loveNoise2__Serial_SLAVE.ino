#define pinSpeaker 3
#define singularity 200

float delayLimit = 200;
float love;
//int entropyRX = 0; //DEBUG

void setup() {
  pinMode(13, OUTPUT);
  pinMode(pinSpeaker, OUTPUT);
  Serial.begin(9600);
  //delay(2000); //da tarare con il tempo necessario a processing e al MASTER  
}

void loop() {
  int entropyRX = Serial.parseInt(); //Legge l'intero ricevuto
  
  /* DEBUG
   * if (entropyRX < 200) {
  entropyRX++;
  }*/
  
  if (entropyRX > 0) {
    if (entropyRX < singularity && delayLimit != 0) {
      digitalWrite(13, LOW);
      delayLimit = map(entropyRX, 0, singularity, singularity, 0);
      love = random(0, entropyRX);
      tone(pinSpeaker, love);
      delay(delayLimit);
    }
    if (entropyRX >= singularity) {
      digitalWrite(13, HIGH);
      int love = random(500);
      tone(pinSpeaker, love);
      //delayMicroseconds(20);
      //noTone(pinSpeaker);
      //delayMicroseconds(10);
    }
  }

  if (entropyRX == 0) {
    noTone(pinSpeaker);
    delay(10);
    tone(pinSpeaker, 1);
    delay(10);
    noTone(pinSpeaker);
    }

}
