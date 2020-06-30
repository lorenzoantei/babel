//SERIALE
import processing.serial.*;
Serial myPort;                       // The serial port
int[] serialInArray = new int[1];    // Where we'll put what we receive
int serialCount = 0;                 // A count of how many bytes we receive
boolean firstContact = false;        // Whether we've heard from the microcontroller


//CAMERA
import processing.video.*;
Capture cam;
static int numeroCamera = 1; //1 per Windows, 0 per RPI

//ALTRE VARIABILI
int entropyRX;
int dimTesto;
int mode = 0;

void setup() {
  //CAMERA
  println("connessione con camera");
  String[] cameras = Capture.list();
  if (cameras == null) {
    println("Failed to retrieve the list of available cameras, will try the default...");
    cam = new Capture(this, 640, 480);
  } else if (cameras.length == 0) {
    println("There are no cameras available for capture.");
    exit();
  } else {
    println("Available cameras:");
    printArray(cameras);
    cam = new Capture(this, cameras[numeroCamera]);
    cam.start();
  }
  //SERIALE
  // Print a list of the serial ports for debugging purposes
  // if using Processing 2.1 or later, use Serial.printArray()
  println(Serial.list());

  // I know that the first port in the serial list on my Mac is always my FTDI
  // adaptor, so I open Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 9600);

  //FINESTRA
  background(0);
  size(720, 480); //RPI res
  //fullScreen();

  //ALTRO
  noStroke();
  mode = 1;
}

void draw() {

  if (entropyRX == 0 && mode != 2) {
    dimTesto = 0;
    background (0);
    mode = 2;
  }

  if (mode == 2) {
    textSize(100);

    //TITOLO
    textSize(40);
    textAlign(CORNER);
    for (int i=0; i < 15; i++) { //fixare i al minimo utile
      fill(random(0, 255));
      for (int j=0; j < 2; j++) {
        text(j, random(0, width), random(height/2-100, height/2+100));
      }
    }
    fill(0);
    textSize(100);
    textAlign(CENTER);
    text("babel", width/2+random(-height*0.01, +height*0.01), height/2+25+random(-height*0.01, +height*0.01));

    if (entropyRX >0 && entropyRX < 150) {
      mode = 3;
    }
  }

  if (mode == 3) {
    if (cam.available() == true) {
      cam.read();
      image(cam, 0, 0, width, height);
      if (dimTesto < 200) {
        dimTesto++;
        textSize(dimTesto);
      }

      fill(random(0, 255));
      for (int i = 0; i < entropyRX; i++) {
        text(int (random(0, 9)), random(-width/2, width+width/2), random(-height/2, height+height/2));
      }
    }

    if (entropyRX > 150) {
      mode = 4;
    }
  }

  if (mode == 4) {

    if (dimTesto < 200) {
      dimTesto++;
      textSize(dimTesto);
    }

    for (int i = 0; i < entropyRX; i++) {
      fill(random(0, 255));
      text(int (random(0, 9)), random(-width/2, width+width/2), random(-height/2, height+height/2));
    }
  }
}

void serialEvent(Serial myPort) {
  // read a byte from the serial port:
  int inByte = myPort.read();
  // if this is the first byte received, and it's an A, clear the serial
  // buffer and note that you've had first contact from the microcontroller.
  // Otherwise, add the incoming byte to the array:
  if (firstContact == false) {
    if (inByte == 'A') {
      myPort.clear();          // clear the serial port buffer
      firstContact = true;     // you've had first contact from the microcontroller
      myPort.write('A');       // ask for more
    }
  } else {
    // Add the latest byte from the serial port to array:
    serialInArray[serialCount] = inByte;
    serialCount++;

    // If we have 3 bytes:
    if (serialCount > 0 ) {
      entropyRX = serialInArray[0];

      // print the values (for debugging purposes only):
      println(entropyRX);

      // Send a capital A to request new sensor readings:
      myPort.write('A');
      // Reset serialCount:
      serialCount = 0;
    }
  }
}
