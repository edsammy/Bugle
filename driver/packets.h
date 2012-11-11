
#ifndef _PACKETS_H_
#define _PACKETS_H_

#include <Arduino.h>
#include "timestamp.h"
#include "bytebuffer.h"

/**
* Packet 15 is a debugging packet.
*/
struct packet15_t{
  int len;
  char msg[33];
};

typedef void (*pkt_poll_func)(void*);
typedef void (*pkt_write_func)(void*);
typedef boolean (*pkt_process_func)(bytebuffer_t*); 

void pollPacketHandler();

void regProcessPacketFunc(int id, pkt_process_func f);
void regWritePacketFunc(int id, pkt_write_func f);
void regPollPacketFunc(int rate, void *packet, pkt_poll_func f);

void writePacket(byte id, void *d);

void initPacketHandler(int readRate);

#endif
