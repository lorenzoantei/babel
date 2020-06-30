void establishContact() {
  while (Serial.available() <= 0) {
    Serial.print('A');   // send a capital A
    delay(300);
  }
}

void checkUltrasuoni() {
  
  digitalWrite(pinTrigger27, HIGH);
    delay(10);
    digitalWrite(pinTrigger27, LOW);
    durata = pulseIn(pinEcho26, HIGH);
    distanza = durata / 29 / 2;
    valUltrasuoni = distanza;

    //verifico osservatoreIsPresent
    if (mode == 2 && valUltrasuoni < distanzaMinima) { // if osservatoreIsPresent >> mode 3
      lcd.clear();
      lcd.setCursor (0, 0);
      lcd.print("           babel 0.7");
      //MODE 3 - //////////////////////////////////////////////////////////////////////////////////
      mode = 3;
    } 
    if (mode >= 3 && mode < 5 && valUltrasuoni > distanzaMinima) { //se !osservatoreIsPresent
      reset++; //inizio il conteggio per il reset
      if (reset > resetTimeout) { //if conteggio reset > timeout
        reset = 0; //reset del reset
        entropyTX = 0;
        caricamento = 0;
        
        mode = 1; //mode 1 - "avvicinati"
      }
    }
    
}

void randomizeLed() {
  /* aggiungere blink without delay
    https://www.arduino.cc/en/tutorial/BlinkWithoutDelay
  */
  analogWrite(led5, random(0, 255));
  analogWrite(led6, random(0, 255));
  analogWrite(led3, random(0, 255));
  analogWrite(led2, random(0, 255));
}
