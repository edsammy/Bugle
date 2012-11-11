
#ifndef _SERIAL_FUNCS_H_
#define _SERIAL_FUNCS_H_

#include <Arduino.h>

boolean decodeByte(byte *b, int *s, byte *d);
boolean decodeFloat32(byte *b, int *s, float *d);
boolean decodeInt16(byte *b, int *s, int *d);
boolean decodeLong32(byte *b, int *s, long *d);

void serialWriteByte(byte b);
void serialWriteInt16(int i);
void serialWriteInt32(int i);
void serialWriteFloat32(float f);

#endif
