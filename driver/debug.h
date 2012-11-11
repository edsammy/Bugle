
#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <Arduino.h>
#include "packets.h"
#include "packet_types.h"

void newDbgPkt(char *s);
void newDbgPktF(char *s, ...);

#ifdef DEBUG

#define DEBUG_PRINT(x) newDbgPkt((x))
#define DEBUG_PRINTF(x, ...) newDbgPktF((x), __VA_ARGS__) 

#else

#define DEBUG_PRINT(x)
#define DEBUG_PRINTF(x, ...)

#endif

#endif

