
#include <Arduino.h>
#include "serial_funcs.h"
#include "packets.h"
#include "bytebuffer.h"
#include "list.h"
#include "packet_types.h"
#include "debug.h"

static const int MAX_PACKETS = 16;
static const byte START = 10;
static const byte END = 13;

struct poll_packet_t{
  int rate;
  long lastPoll;
  void *packet;
  pkt_poll_func func;
};

static long readRate;
static long lastPacketRead;
static long currTime;
static byte currPacketID = -1;
static boolean isNextByteID = false;
static bytebuffer_t bytebuffer = {0, 64, (byte*)malloc(bytebuffer.cap)};

/**
* Hook to write packet fifteen to the serial stream
*/ 
static void writePacket15(void *p)
{
  packet15_t packet = *((packet15_t*)p);
  int i;
  int len = packet.len;
  char *msg = packet.msg;
  serialWriteInt16(packet.len);
  for(i = 0; i < len; i++)
  {
    serialWriteByte(msg[i]); 
  }
}

/**
* Array of Process Packet Function Pointers
*/
static boolean (*processPacketFuncs[MAX_PACKETS])(bytebuffer_t*);

/**
* Array of Write Packet Function Pointers
* Note this is very type unsafe!! 
* Relies on user to cast that void pointer correctly.
*/
static void (*writePacketFuncs[MAX_PACKETS])(void*);

/**
* Linked List of Poll Packet Function Structs
*/
static list_t pollPacketFuncs = {0, 0, 0};

/**
* Initiate the byte buffer and 
* register the debug packet write hook
*/
void initPacketHandler(int rate)
{
  readRate = rate;
  lastPacketRead = 0;
  bytebuffer.buffer = (byte*)malloc(bytebuffer.cap);
  #ifdef DEBUG
    regWritePacketFunc(15, writePacket15);
  #endif
}

/**
* Register a packet write hook
*/
void regWritePacketFunc(int id, void (*f)(void*))
{
  if(id < 0 || id >= MAX_PACKETS || writePacketFuncs[id] != 0)
    return;  
  writePacketFuncs[id] = f;  
}

/**
* Rgister a packet poll hook
*/
void regPollPacketFunc(int rate, void *p, void (*f)(void*))
{
   poll_packet_t *poll = (poll_packet_t*)malloc(sizeof(poll_packet_t));
   poll->rate = rate;
   poll->func = f;
   poll->packet = p;
   poll->lastPoll = 0L;
   listAdd(&pollPacketFuncs, poll);
}

/**
* Writes a packet to the serial stream
*/
void writePacket(byte id, void *d)
{
  if(id < 0 || id >= MAX_PACKETS || writePacketFuncs[id] == 0)
    return;
  Serial.write(START);
  Serial.write(id);
  writePacketFuncs[id](d);
  Serial.write(END);
  DEBUG_PRINTF("PACKET (%d) WRITTEN", (int)id);
}

void pollPacketHandler()
{
  
  /**
  * Poll packets that need polling
  */
  poll_packet_t *poll;
  node_t *node = pollPacketFuncs.root;
  while(poll != 0)
  {
    currTime = millis();
    poll = (poll_packet_t*)node->datum;
    if(currTime - poll->lastPoll >= poll->rate)
    {
      poll->func(poll->packet); 
      poll->lastPoll = currTime;
    } 
    node = node->next;
  }
  
  /**
  * Handle Incoming data
  */
  while(Serial.available() > 0)
  {
    byte b = Serial.read();
    if(b == START)
    {
      currPacketID = -1;
      isNextByteID = true;
    }
    else if(b == END)
    {
      if(currPacketID == -1)
        continue;
      if(currPacketID < 0 || currPacketID >= MAX_PACKETS || processPacketFuncs[currPacketID] == 0)
      {
        //Invalid packet ID...
        currPacketID = -1;
        continue;
      }
      if(processPacketFuncs[currPacketID](&bytebuffer))
      {
        DEBUG_PRINTF("PACKET (%d) RECEIVED", (int)currPacketID); 
      }
      else
      {
        DEBUG_PRINTF("PACKET (%d) CORRUPTED", (int)currPacketID);   
      }
      currPacketID = -1;
    }
    else if(isNextByteID)
    {
      currPacketID = b;
      isNextByteID = false;
    }
    else
    {
      if(currPacketID != -1)
      {
        byteBufferPut(&bytebuffer, b);
      }
      else
      {
        /**
        * Corrupted stream, losing data, packet cannot be determined...
        */
      }
    }
  }
}

/**
* Register a packet process hook
*/
void regProcessPacketFunc(int i, boolean (*f)(bytebuffer_t*))
{
  processPacketFuncs[i] = f;
}

