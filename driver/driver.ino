

/**
* Toggle this on/off to enable/disable debugging.
*/
#define DEBUG


#include "timestamp.h"
#include "serial_funcs.h"
#include "packets.h"
#include "packet_types.h"
#include "FreqPeriodCounter.h"
#include "debug.h"


#define DATA_RATE 57600
#define PACKET_READ_RATE 500
#define WHEEL_DIAMETER 24
#define ENCODER_NOTCHES 10


/**
* Arduino pins used
*/
const byte BUTTON_PIN = 4; 
const byte TEMP_PIN = 3;

/**
* RGB led pins
*/
const byte R_PIN = 13;
const byte G_PIN = 10;
const byte B_PIN = 11;

/**
* Frequency inputs and interrupt
*/
const byte ENGINE_PIN = 3;
const byte ENGINE_INTERRUPT = 1;
const byte WHEEL_PIN = 2;
const byte WHEEL_INTERRUPT = 0;

const float WHEEL_CIRCUM = WHEEL_DIAMETER * PI;
const byte NUM_NOTCHES = ENCODER_NOTCHES;
const float MILES_PER_INCH = 0.000015783f;

/**
* Initialize frequency / period counters
*/
FreqPeriodCounter ENGINE(ENGINE_PIN, micros, 0);
FreqPeriodCounter WHEEL(WHEEL_PIN, micros, 0);

/**
* Packet references...
*/
packet0_t packet0; //Wheel Speed / Engine Speed Packet
packet1_t packet1; //Temperature Packet
packet2_t packet2; //Test Client Side Packet


static unsigned long lastEngineHertz = 0;
static unsigned long lastWheelHertz = 0;
static int lastTemp = 0;
static unsigned long engineHertz = 0;
static unsigned long wheelHertz = 0;
static int temp;
static long lastPacket2Read;

void EngineCount()
{
  ENGINE.poll();
}

void WheelCount()
{
  WHEEL.poll();
}

/**
* Update packet zero based on sensor data
* Push the updated packet to the client
*/ 
void pollPacket0(void *p)
{
   
  engineHertz = ENGINE.hertz();
  wheelHertz = WHEEL.hertz();
  
  /**
  * Only send a packet if the values have changes
  * TODO - Possibly implement a threshold here for both values
  */
  if(engineHertz == lastEngineHertz && wheelHertz == lastWheelHertz)
    return;
  
  lastWheelHertz = wheelHertz;  
  lastEngineHertz = engineHertz;
    
  packet0.engineSpeed = (float)(100 * 60 * engineHertz / NUM_NOTCHES);
  packet0.engineSpeed = packet0.engineSpeed / 100;
  
  packet0.wheelSpeed = (float)(100 * wheelHertz * WHEEL_CIRCUM * MILES_PER_INCH * 3600 / NUM_NOTCHES);
  packet0.wheelSpeed = packet0.wheelSpeed / 100; 
  
  pollTimeStamp(&packet0.timestamp);
  
  writePacket(0, &packet0);

}

/**
* Update packet one based on sensor data
* Push the updated packet to the client
*/
void pollPacket1(void *p)
{
  
  temp = analogRead(TEMP_PIN);
  
  /**
  * Only send a packet if the value has changes
  * TODO - Possibly implement a threshold here for the temperature value
  */
  if(temp == lastTemp)
    return;
  
  lastTemp = temp;
  packet1.temp = analogRead(temp);
  pollTimeStamp(&packet1.timestamp);
  writePacket(1, &packet1);

}

/**
* Hook to write packet zero to serial stream
*/ 
void writePacket0(void *p)
{
  packet0_t packet = *((packet0_t*)p);
  serialWriteFloat32(packet.wheelSpeed);
  serialWriteFloat32(packet.engineSpeed);
  pollTimeStamp(&packet.timestamp);
  serialWriteTimeStamp(&packet.timestamp);
}

/**
* Hook to write packet one to the serial stream
*/ 
void writePacket1(void *p)
{
  packet1_t packet = *((packet1_t*)p);
  serialWriteInt16(packet.temp);
  pollTimeStamp(&packet.timestamp);
  serialWriteTimeStamp(&packet.timestamp);
}

/**
* Hook to write packet two to serial the stream
*/ 
void writePacket2(void *p)
{
  packet2_t packet = *((packet2_t*)p);
  serialWriteByte(packet.flag);
  pollTimeStamp(&packet.timestamp);
  serialWriteTimeStamp(&packet.timestamp);
}

/**
* Hook to process packet 2
*/
boolean processPacket2(bytebuffer_t *b)
{
  
  bytebuffer_t buffer = *b;
  
  /**
  * If the buffer length does not equal 
  * the expected length of the packet,
  * return false.
  */ 
  if(buffer.mark != 12) 
    return false;
  
  /**
  * Rewind the buffer for forwards analysis
  */
  rewind(buffer);
  byte* bytes = buffer.buffer;
  
  /**
  * Update the contents of packet two
  */
  int index = 0;
  if(!decodeByte(bytes, &index, &packet2.flag))
    return false;
  if(!decodeTimeStamp(bytes, &index, &packet2.timestamp))
    return false;
  
  /**
  * If somehow this new packet two is 
  * older than the last packet two received,
  * ignore it! (possible TODO here :P)
  */
  if(packet2.timestamp.clock - lastPacket2Read < 0)
    return false;
  
  lastPacket2Read = packet2.timestamp.clock;
  
  /**
  * Handle new values
  */
  switch(packet2.flag)
  {
  case 9:
  case 6:
  case 7:
    digitalWrite(packet2.flag + 4, LOW);
    break;
  case 13:
  case 10:
  case 11:
    digitalWrite(packet2.flag, HIGH);
    break;
  }
    
  /**
  * Return to sender :D
  */
  pollTimeStamp(&packet2.timestamp);   
  writePacket(2, &packet2); 
  
  return true;

}

void setup()
{
  
  /**
  * Open the serial stream.
  */ 
  Serial.begin(DATA_RATE);
  
  /**
  * Set output pins (red, green, & blue LED)
  */ 
  pinMode(R_PIN, OUTPUT);
  digitalWrite(R_PIN, LOW);
  pinMode(G_PIN, OUTPUT);
  digitalWrite(G_PIN, LOW);
  pinMode(B_PIN, OUTPUT);
  digitalWrite(B_PIN, LOW);
  
  /**
  * Set inputs (button, speed signal, rpm signal, & temperature sensor)
  */
  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(BUTTON_PIN, HIGH);
  
  /**
  * Initialize rpm and speed signal interrupts
  */
  attachInterrupt(ENGINE_INTERRUPT, EngineCount, CHANGE);
  attachInterrupt(WHEEL_INTERRUPT, WheelCount, CHANGE);
  
  /**
  * This is required to initiate a few behind
  * the scenes packet reading mechanisms.
  */ 
  initPacketHandler(PACKET_READ_RATE);
  
  /**
  * Set up Packet Info
  * (Must be done carefully!)
  * (Uses void pointers -> not type safe)
  */
  regWritePacketFunc(0, writePacket0);
  regWritePacketFunc(1, writePacket1);
  regWritePacketFunc(2, writePacket2);
  regPollPacketFunc(25, &packet0, pollPacket0);
  regPollPacketFunc(250, &packet1, pollPacket1);
  regProcessPacketFunc(2, processPacket2);
  /**
  * End 
  */
  
}

void loop()
{
  
  pollPacketHandler();
  
  delay(25);
  
}
