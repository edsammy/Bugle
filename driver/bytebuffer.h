#ifndef _BYTEBUFFER_H_
#define _BYTEBUFFER_H_

#include <Arduino.h>

#define rewind(buffer) (buffer).mark = 0

struct bytebuffer_t{
  int mark;
  int cap;
  byte* buffer;  
};

void byteBufferPut(bytebuffer_t *buffer, byte b);

#endif
