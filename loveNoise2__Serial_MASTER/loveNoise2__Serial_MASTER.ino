int entropyTX = 0;
#define singularity 200

void setup() {
  Serial1.begin(9600);
  Serial.begin(9600);
  pinMode(13,OUTPUT);
}

void loop() {
  delay(50);
  if (entropyTX < singularity) {
  entropyTX++;
  }
  
  //Serial.print("entropyTX = ");
  Serial.println(entropyTX);
   
  //Serial.println("scrittura data al SLAVE");
  digitalWrite(13,HIGH);
  Serial1.println(entropyTX); //invio dati via Serial1 (pin18)
  digitalWrite(13,LOW);
  //Serial.println("dati inviati");

}
