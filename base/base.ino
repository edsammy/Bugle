// Frequency capture and conversion library
#include <FreqPeriodCounter.h>

// Arduino pins used
const byte BUTTON_PIN = 4; 
const byte TEMP_PIN = 3;
// RGB led pins
const byte R_PIN = 13;
const byte G_PIN = 10;
const byte B_PIN = 11;

// Frequency inputs and interrupt
const byte ENGINE_PIN = 3;
const byte ENGINE_INTERRUPT = 1;
const byte WHEEL_PIN = 2;
const byte WHEEL_INTERRUPT = 0;

// Wheel constants
const byte d = 24;
const float C = d * PI;

// Number of notches on encoder wheel
const byte n = 10;

// miles per inch
const float MILESperINCH = 0.000015783;

// Analog readings
int inByte;
//int temperatureVal;


// Initialize distress signal state
boolean distressSignal = false;

// Initialize frequency capture and conversion for  and rpm readings
FreqPeriodCounter ENGINE(ENGINE_PIN, micros, 0);
FreqPeriodCounter WHEEL(WHEEL_PIN, micros, 0);

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
  attachInterrupt(ENGINE_INTERRUPT, EngineCount, CHANGE);
  attachInterrupt(WHEEL_INTERRUPT, WheelCount, CHANGE);
  
  // Wait for initial contact from PC
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
  if (ENGINE.ready()){
    //Transmit data packet format: (RPM, SPEED)
    Serial.print(
    ((ENGINE.hertz()) / n) * 60
    );
    Serial.print(", "); 
    Serial.print(
    ((WHEEL.hertz()) / n) * C * MILESperINCH * 3600
    );    
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

void EngineCount(){
  ENGINE.poll();
}

void WheelCount(){
  WHEEL.poll();
}

