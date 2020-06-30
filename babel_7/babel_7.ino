
//SERIALE
int inByte;
//LED
#define led2 2
#define led3 3
#define led4 4
#define led5 5
#define led6 6
//PULSANTI
#define pinTasto34 34
#define pinTasto35 35
#define pinTasto30 30
#define pinTasto9 9
#define pinSwitch48 48
#define pinSwitch49 49
#define pinSwitch50 50
//HCSR04
#define pinTrigger27 27
#define pinEcho26 26
#define timeoutUltrasuoni 100
#define distanzaMinima 70
//LCD I2C 20x04
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

//lcd blink
int time_now;
int periodBlinkLcdON = 1000;
int periodBlinkLcdOFF = 300;
boolean lcdOn = true;

//barra di caricamento
int caricamento; //barra di caricamento
int cursoreCaricamento; //per scegliere l'aspetto della barra di caricamento
#define caricamentoMax 100
int randomEntry;

//VALORI INPUT
int valSlider1, valSlider2, valSlider3, valSlider4 = 0;
int valTasto34, valTasto35, valTasto30, valTasto9 = 1;
int valSwitch48, valSwitch49, valSwitch50 = 0;
int valUltrasuoni = 3000; //impostato ad un valore alto per lag ricezione iniziale
int valSlider1_OLD, valSlider2_OLD, valSlider3_OLD, valSlider4_OLD = 0;
int valTasto34_OLD, valTasto35_OLD, valTasto30_OLD, valTasto9_OLD = 1;
int valSwitch48_OLD, valSwitch49_OLD, valSwitch50_OLD;
long durata = 0;
long distanza = durata / 29 / 2; //il suono viaggia a 29 micrometri al centimetro, .5 perchè considerare A/R

//ALTRE VARIABILI
int entropyTX = 0; //viene inviato a Processing e nano
int reset; //contatore per reset in caso di assenza
#define resetTimeout 5
#define singularity 200

//MODE 0  ///////////////////////////////////////////////////////////////////////////////////////////////////////
int mode = 0; //modalità iniziale (ricerca primo segnale)

void setup() {

  //LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor (0, 0);
  lcd.print("           babel 0.7");

  //****************************************************debug
  lcd.setCursor (0, 0);
  lcd.print(mode);

  //INIT PINS
  pinMode(led2, OUTPUT); //led zona crt
  pinMode(led3, OUTPUT); //UP estremo DX
  pinMode(led4, OUTPUT); //UP DX interno
  pinMode(led5, OUTPUT); //UP estremo SN
  pinMode(led6, OUTPUT); //UP SN interno
  pinMode(pinTasto34, INPUT_PULLUP); //UP SN interno
  pinMode(pinTasto35, INPUT_PULLUP); //UP estremo SN
  pinMode(pinTasto30, INPUT_PULLUP); //UP DX interno
  pinMode(pinTasto9, INPUT_PULLUP); // zona CRT
  pinMode(pinSwitch48, INPUT_PULLUP);
  pinMode(pinSwitch49, INPUT_PULLUP);
  pinMode(pinSwitch50, INPUT_PULLUP);
  pinMode(pinTrigger27, OUTPUT);
  pinMode(pinEcho26, INPUT);
  digitalWrite(pinTrigger27, LOW);
  digitalWrite(pinEcho26, LOW);

  //SERIALE
  Serial.begin(9600); //COM PROCESSING
  Serial1.begin(9600); //COM NANO
  lcd.setCursor (0, 1);
  lcd.print("Attesa prima");; // Attende la presenza di una connessione seriale usb
  lcd.setCursor(0,2);
  lcd.print("connessione...");
  while (!Serial) {
  }

  establishContact();  // Stabilisce la connessione con processing

  //MODE 1  ///////////////////////////////////////////////////////////////////////////////////////////////////////
  mode = 1; // connessione con processing effettuata >> mode 1
}

void loop() {
  Serial1.println(entropyTX); //invio a NANO
  if (Serial.available() > 0) { //Se Processing comunica >> manda entropyTX
    inByte = Serial.read(); //necessario per verificare se PDE è collegato

    // send values:
    digitalWrite(led4, HIGH);
    Serial.write(entropyTX); //invia a Processing

    //invia a nano
    delay(100);
    digitalWrite(led4, LOW);
  }

  //MODE 1 - print avvicinati //////////////////////////////////////////////////////////////////
  if (mode == 1) {
    lcd.clear();

    //****************************************************debug
    lcd.setCursor (0, 0);
    lcd.print("           babel 0.7");
    lcd.setCursor (0, 0);
    lcd.print(mode);

    lcd.setCursor (0, 2);
    lcd.print("      avvicinati      ");
    //MODE 2  //////////////////////////////////////////////////////////////////////////////////
    mode = 2;
  }

  //MODE 2 /////////////////////////////////////////////////////////////////////////////////////
  if (mode == 2) { // if !osservatoreIsPresent >> lcdBlink & attesaPresenza)

    //lcd backlight blinkin' noDelay
    if (millis() > time_now + periodBlinkLcdON && lcdOn == true) {
      time_now = millis();
      lcd.noBacklight();
      lcdOn = false;
    } else if (millis() > time_now + periodBlinkLcdOFF && lcdOn == false) {
      time_now = millis();
      lcd.backlight();
      lcdOn = true;
    }

    checkUltrasuoni();
  }

checkUltrasuoni();

  //MODE 3  /////////////////////////////////////////////////////////////////////////////////////////////
  if (mode == 3) { //se l'osservatore si è avvicinato

    
    
    //****************************************************debug
    lcd.setCursor (0, 0);
    lcd.print(mode);

    if (entropyTX < singularity) { //caricamento "regolare"
      cursoreCaricamento = int (random(1, 9)); //scelgo un cursore
      randomEntry = int (random (1, 37));
    }

    mostraRandomEntry();
    //MODE 4  ///////////////////////////////////////////////////////////////////////////////////////
    mode = 4; //passo quindi alla scelta della randomEntry >> mode 4
  }
  //MODE 4 - caricamento, lettura input, entropyTX++ //////////////////////////////////////////////
  if (mode == 4) {

    //****************************************************debug
    lcd.setCursor (0, 0);
    lcd.print(mode);

    entropyTX++;

    if (entropyTX > singularity) { //caricamento "overflow"
      cursoreCaricamento = int (random(1, 9));
      randomEntry = 39;
    }

    //barra di caricamento LCD
    for (int i = 0; i < map(caricamento, 0, 100, 0, 20); i++) {
      lcd.setCursor(i, 3);

      //lista dei cursori
      if (cursoreCaricamento == 1) {
        lcd.print("#");
      } else if (cursoreCaricamento == 2) {
        lcd.print("$");
      } else if (cursoreCaricamento == 3) {
        lcd.print("&");
      } else if (cursoreCaricamento == 4) {
        lcd.print("£");
      } else if (cursoreCaricamento == 5) {
        lcd.print("€");
      } else if (cursoreCaricamento == 6) {
        lcd.print("@");
      } else if (cursoreCaricamento == 7) {
        lcd.print("*");
      } else if (cursoreCaricamento == 8) {
        lcd.print("=");
      } else if (cursoreCaricamento == 9) {
        lcd.print("+");
      }
    }

    //ANALISI DI TUTTI GLI INPUT
    //analogici
    valSlider1  = analogRead(A1);
    caricamento = caricamento + map(valSlider1, 0, 1023, 0, 10);
    //delay(10); // delay 10ms to let the ADC recover
    valSlider2  = analogRead(A2);
    caricamento = caricamento + map(valSlider2, 0, 1023, 0, 10);
    //delay(10); // delay 10ms to let the ADC recover
    valSlider3  = analogRead(A3);
    //delay(10); // delay 10ms to let the ADC recover
    caricamento = caricamento + map(valSlider3, 0, 1023, 0, 10);
    valSlider4  = analogRead(A4);
    //delay(10); // delay 10ms to let the ADC recover
    caricamento = caricamento + map(valSlider4, 0, 1023, 0, 10);

    //digitali
    valTasto34  = digitalRead(pinTasto34);
    valTasto35  = digitalRead(pinTasto35);
    valTasto30  = digitalRead(pinTasto30);
    valTasto9   = digitalRead(pinTasto9);
    //switch
    valSwitch48 = digitalRead(pinSwitch48);
    valSwitch49 = digitalRead(pinSwitch49);
    valSwitch50 = digitalRead(pinSwitch50);


    //confronto val attuale con precedente
    if ( valTasto34 != valTasto34_OLD) {
      randomizeLed();
      //caricamento = caricamento + entropyTX;
      entropyTX++;
      valTasto34_OLD = valTasto34;
    } if ( valTasto35 != valTasto35_OLD) {
      randomizeLed();
      //caricamento = caricamento + entropyTX;
      entropyTX++;
      valTasto35_OLD = valTasto35;
    } if ( valTasto30 != valTasto30_OLD) {
      randomizeLed();
      //caricamento = caricamento + entropyTX;
      entropyTX++;
      valTasto30_OLD = valTasto30;
    } if ( valTasto9 != valTasto9_OLD) {
      randomizeLed();
      //caricamento = caricamento + entropyTX;
      entropyTX++;
      valTasto9_OLD = valTasto9;
    } if ( valSwitch48 != valSwitch48_OLD) {
      randomizeLed();
      //caricamento = caricamento + entropyTX;
      entropyTX++;
      valSwitch48_OLD = valSwitch48;
    } if ( valSwitch49 != valSwitch49_OLD) {
      randomizeLed();
      //caricamento = caricamento + entropyTX;
      entropyTX++;
      valSwitch49_OLD = valSwitch49;
    } if ( valSwitch50 != valSwitch50_OLD) {
      randomizeLed();
      //caricamento = caricamento + entropyTX;
      entropyTX++;
      valSwitch50_OLD = valSwitch50;
    }

    /*
      if (valSlider1 != valSlider1_OLD) {
      randomizeLed();
      caricamento = caricamento + entropyTX;
      valSlider1_OLD = valSlider1;
      } if (valSlider2 != valSlider2_OLD) {
      //randomizeLed();
      //caricamento = caricamento + entropyTX;
      valSlider2_OLD = valSlider2;
      } if (valSlider3 != valSlider3_OLD) {
      //randomizeLed();
      //caricamento = caricamento + entropyTX;
      valSlider3_OLD = valSlider3;
      } if ( valSlider4 != valSlider4_OLD) {
      //randomizeLed();
      //caricamento = caricamento + entropyTX;
      valSlider4_OLD = valSlider4;
      }
    */

    //avanzamento barra di caricamento LCD
    caricamento = caricamento + 1  + 1 * (entropyTX * 0.01);

    if (caricamento >= caricamentoMax) { //se il caricamento è completato
      lcd.setCursor (0, 2);
      lcd.print("                    ");
      lcd.setCursor (0, 3);
      lcd.print("                    ");
      caricamento = 0;
      mode = 3; //loop
    }
  }

  if (!Serial.available() && mode != 5) { // if chiusura Processing >> wait reconnection
    lcd.clear();
    lcd.backlight();

    lcd.setCursor (0, 0);
    lcd.print("           babel 0.7");
    lcd.setCursor(0, 1);
    lcd.print("attesa riconnessione"); // Attende la presenza di una connessione seriale usb
    lcd.setCursor(0,2);
    lcd.print("Processing...");
    mode = 5;

    //****************************************************debug
    lcd.setCursor (0, 0);
    lcd.print(mode);
  }

  if (mode == 5) {
    if (Serial.available()) {
      lcd.clear();
      lcd.backlight();
      mode = 1;
    }
  }

  
}
