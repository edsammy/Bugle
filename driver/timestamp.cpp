
#include "timestamp.h"
#include "serial_funcs.h"

void serialWriteTimeStamp(timestamp_t *t)
{
  timestamp_t time = *t;
  serialWriteByte(time.hrs);
  serialWriteByte(time.mins);
  serialWriteByte(time.secs);
  serialWriteInt16(time.ms);
}

void pollTimeStamp(timestamp_t *t)
{
  unsigned long curr_time = millis();
  timestamp_t stamp = *t;
  stamp.hrs = (byte)(curr_time / 3600000L);
  curr_time -= stamp.hrs * 3600000L;
  stamp.mins = (byte)(curr_time/ 60000L);
  curr_time -= stamp.mins * 60000L;
  stamp.secs = (byte)(curr_time / 1000L);
  curr_time -= stamp.secs * 1000L;
  stamp.ms = (int)curr_time;
  stamp.clock = curr_time;
}

static long timeStampToMillis(timestamp_t *t)
{
  timestamp_t stamp = *t;
  return (long)stamp.ms + (long)stamp.secs * 1000L + (long)stamp.mins * 60000L + (long)stamp.hrs * 3600000L;
}

boolean decodeTimeStamp(byte* b, int *s, timestamp_t *t)
{
  if(!decodeByte(b, s, &t->hrs))
    return false;
  if(!decodeByte(b, s, &t->mins))
    return false;
  if(!decodeByte(b, s, &t->secs))
    return false;
  if(!decodeInt16(b, s, &t->ms))
    return false;  
  t->clock = timeStampToMillis(t);
}
