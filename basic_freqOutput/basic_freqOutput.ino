#include <FreqPeriodCounter.h>

const byte RPM_PIN = 2;
const byte RPM_INTERRUPT = 0;

FreqPeriodCounter RPM(RPM_PIN, micros, 0);

void setup(){
  Serial.begin(57600);
  
  attachInterrupt(RPM_INTERRUPT, rpmCount, CHANGE);
}

void loop(){
  if (RPM.ready()){
    // Read capacitive fuel level
    //long fuelIndicator = fuelVal.capSenseRaw(200);
    // Read temperature
    //temperatureVal = analogRead(TEMP_PIN);
    //Transmit data packet
    Serial.print(RPM.hertz());
    Serial.print(" "); 
    Serial.println();
  }
  
}

void rpmCount(){
  RPM.poll();
}
