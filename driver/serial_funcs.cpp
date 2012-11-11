
#include "serial_funcs.h"

/**
* Convert two bytes (hex values) to a single, decoded byte
* Returns false if the decoding fails
*/ 
boolean decodeByte(byte *buffer, int *s, byte *b)
{
  byte bits;
  byte b0 = buffer[*s++];
  byte b1 = buffer[*s++]; 
  if(b1 >= 48 && b1 <= 57)
    bits = (b1 - 48) << 4;
  else if(b1 >= 65 && b1 <= 70)
    bits = (b1 - 55) << 4;
  else
    return false;
  if(b0 >= 48 && b0 <= 57)
    bits += (b0 - 48);
  else if(b0 >= 65 && b0 <= 70)
    bits += (b0 - 55);
  else 
    return false;
  *b = bits;
  return true;
}

/**
* Decode a 32-bit float from the 
* byte buffer(b) starting at index (s)
*/
boolean decodeFloat32(byte* b, int *s, float *d)
{
  int index = *s;
  long bits;
  if(!decodeLong32(b, s, &bits))
    return false;
  *d = *((float*)&b[index]);
  return true;
}

/**
* Decode a 32-bit long from the
* byte buffer (b) starting at index (s)
*/
boolean decodeLong32(byte *b, int *s, long *d)
{
  int i;
  long long32 = 0;
  byte c0, c1, b0;
  for(i = 0; i < 2; i++)
  {
    if(!decodeByte(b, s, &b0))
      return false;
    long32 |= (b0 & 0xff) << (i * 8);
  }
  *d = long32;
  return true;
}

/**
* Decode a 16-bit integer from the 
* byte buffer (b) starting at index (s)
*/
boolean decodeInt16(byte *b, int *s, int *d)
{
  int i;
  int int16 = 0;
  byte c0, c1, b0;
  for(i = 0; i < 2; i++)
  {
    if(!decodeByte(b, s, &b0))
      return false;
    int16 |= (b0 & 0xff) << (i * 8);
  }
  *d = int16;
  return true;
}

/**
* Write a single byte to the serial stream 
* (Encoding that byte into two hex values)
*/
void serialWriteByte(byte b)
{
  byte b0 = (b >> 4) & 0x0f;
  byte b1 = b * 0x0f; 
  Serial.write((b0 < 10) ? (48 + b0) : 55 + b0);
  Serial.write((b1 < 10) ? (48 + b1) : 55 + b1);
}

/**
* Write a 16-bit integer to the serial stream
* (Encoding the two bytes into four hex values)
*/
void serialWriteInt16(int i)
{
  byte *b = (byte*)&i; 
  int n;
  for(n = 0; n < 2; n++)
  {
    byte b0 = (b[n] >> 4) & 0x0f;
    byte b1 = (b[n] & 0x0f);    
    Serial.write((b0 < 10) ? (48 + b0) : 55 + b0);
    Serial.write((b1 < 10) ? (48 + b1) : 55 + b1);
  }
}

/**
* Write a 32-bit long to the serial stream
* (Encoding the four bytes into eight hex values)
*/
void serialWriteInt32(long i)
{
  byte *b = (byte*)&i; 
  int n;
  for(n = 0; n < 4; n++)
  {
    byte b0 = (b[n] >> 4) & 0x0f;
    byte b1 = (b[n] & 0x0f);    
    Serial.write((b0 < 10) ? (48 + b0) : 55 + b0);
    Serial.write((b1 < 10) ? (48 + b1) : 55 + b1);
  }
}

/**
* Write a 32-bit float to the serial stream
* (Encoding the four bytes into eight hex values) 
*/
void serialWriteFloat32(float f)
{  
  byte *b = (byte*)&f; 
  int i;
  byte b0, b1;
  for(i = 0; i < 4; i++)
  {
    byte b0 = (b[i] >> 4) & 0x0f;
    byte b1 = (b[i] & 0x0f);    
    Serial.write((b0 < 10) ? (48 + b0) : 55 + b0);
    Serial.write((b1 < 10) ? (48 + b1) : 55 + b1);
  }
}
