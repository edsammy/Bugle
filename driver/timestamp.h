
#ifndef _TIMESTAMP_H_
#define _TIMESTAMP_H_
  
#include <Arduino.h>

struct timestamp_t{
  byte hrs;
  byte mins;
  byte secs;
  int ms;
  long clock;
};

boolean decodeTimeStamp(byte *b, int *s, timestamp_t *t);
void serialWriteTimeStamp(timestamp_t *t);
void pollTimeStamp(timestamp_t *t);

#endif
