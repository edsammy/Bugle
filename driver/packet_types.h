#ifndef _PACKET_TYPES_H_
#define _PACKET_TYPES_H_

/**
* Packet 0 is an rpm update packet.
*/
struct packet0_t{
  float wheelSpeed;
  float engineSpeed;
  timestamp_t timestamp;
};

/**
* Packet 1 is a temperature update packet.
*/
struct packet1_t{
  int temp;
  timestamp_t timestamp;
};

/**
* Packet 2 is a test client side packet.
*/
struct packet2_t{
  byte flag;
  timestamp_t timestamp;
};

#endif
