
#include "debug.h"

static packet15_t packet15;

static int strlen(char *string)
{
  int length;
  for(length = 0; *string != '\0'; string++)
    length++;
  return length;
}

static void strcopy(char *dst, int len, char *src)
{
  int i = 0;
  while(len-- >= 0)
  {
    if((dst[i] = src[i++]) == '\0')
    {
      return;   
    }
  }
  dst[i] = '\0';
}

void newDbgPkt(char *s)
{
  if((packet15.len = strlen(s)) < 1)
    return;
  strcopy(packet15.msg, 32, s);
  writePacket(5, &packet15);
}

void newDbgPktF(char *s, ...)
{
  
  if(strlen(s) < 1)
    return;
  
  va_list argptr;
  va_start(argptr, s);
  vsnprintf(packet15.msg, 33, s, argptr);
  va_end(argptr);
  
  packet15.len = strlen(packet15.msg);
  
  writePacket(15, &packet15);  

}

