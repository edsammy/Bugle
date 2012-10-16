// Capacitive sensing library
//#include <CapSense.h>
// Frequency capture and conversion library
#include <FreqPeriodCounter.h>

// Initialize capacitive sensing for fuel sensor (1M resistor between pins 7 & 8)
//CapSense fuelVal = CapSense(7, 8);

// Arduino pins used
#define BUTTON_PIN 4 
#define TEMP_PIN 3
// RGB led pins
#define R_PIN 13
#define G_PIN 10
#define B_PIN 11

// How many data inputs to capture and transmit
//#define ARRAY_SIZE 3

// Analog readings
int inByte;
//int temperatureVal;
//long fuelIndicator = 0;

// Initialize distress signal state
boolean distressSignal = false;

const byte RPM_PIN = 3;
const byte SPEED_PIN = 2;
const byte RPM_INTERRUPT = 1;
const byte SPEED_INTERRUPT = 0;

// Initialize frequency capture and conversion for speed and rpm readings
FreqPeriodCounter RPM(RPM_PIN, micros, 0);
FreqPeriodCounter SPEED(SPEED_PIN, micros, 0);

void setup(){
  // Set outputs (red, green, & blue LED)
  pinMode(R_PIN, OUTPUT);
  digitalWrite(R_PIN, LOW);
  pinMode(G_PIN, OUTPUT);
  digitalWrite(G_PIN, LOW);
  pinMode(B_PIN, OUTPUT);
  digitalWrite(B_PIN, LOW);
  
  // Set inputs (button, speed signal, rpm signal, & temperature sensor)
  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(BUTTON_PIN, HIGH);
  //pinMode(TEMP_PIN, INPUT);
  
  Serial.begin(57600);
  
  // Initialize rpm and speed signal interrupt
  attachInterrupt(RPM_INTERRUPT, rpmCount, CHANGE);
  attachInterrupt(SPEED_INTERRUPT, speedCount, CHANGE);
  
  // Wait for initial contact from Pier
  intercom();
}

void loop(){
  // Check for intercom message, if available read it.
  if (Serial.available() > 0){
    intercom();
  }
  
  // Check for distress signal, if engaged stop sending data and wait for response
  if (distressSignal == true){
    while (Serial.available() <= 0){
    }
    if (Serial.available() > 0){
      distressSignal = false;
      return;
    }
  }
  if (RPM.ready()){
    // Read capacitive fuel level
    //long fuelIndicator = fuelVal.capSenseRaw(200);
    // Read temperature
    //temperatureVal = analogRead(TEMP_PIN);
    //Transmit data packet format: (RPM SPEED)
    Serial.print(RPM.hertz());
    Serial.print(" "); 
    Serial.print(SPEED.hertz());    
    Serial.println();
  } 
}

// Communication methods
void intercom(){
  // Handshaking method
  // If no serial communication received, wait for one
  while (Serial.available() <= 0){
  }
  // If serial communication received, read it
  if (Serial.available() > 0){
    int inByte = Serial.read();
    
    // Basic communication using RGB LED
    // Only G and B LED wired
    if (inByte == 'a'){
      ledCommunication(R_PIN);
    }
    else if (inByte == 'b'){
      ledCommunication(G_PIN);
    }
    else if (inByte == 'c'){
      ledCommunication(B_PIN);
    }
  }
}

//// Interrupts all data transmission when distress signal is engaged
//void distress(){
//  if (distressSignal == false){
//    distressSignal = true;
//    digitalWrite(R_PIN, HIGH);
//    Serial.println("!"); 
//  }
//}

void ledCommunication(int ledPin){
  int ledState = digitalRead(ledPin);
  if (ledState == HIGH){
    digitalWrite(ledPin, LOW);
  }
  else if (ledState == LOW){
    digitalWrite(ledPin, HIGH);
  }
}

void rpmCount(){
  RPM.poll();
}

void speedCount(){
  SPEED.poll();
}

