
#include "bytebuffer.h"

/**
* Puts byte (b) on top of the byte buffer
* If the buffer is out of space, 
* reallocate the buffer to a bigger size
*/
void byteBufferPut(bytebuffer_t *buffer, byte b)
{
  if(buffer->mark + 1 >= buffer->cap)
  {
    buffer->cap = 2 * buffer->cap;
    buffer->buffer = (byte*)realloc(buffer->buffer, buffer->cap);
  }
  buffer->buffer[buffer->mark++] = b;
}

/**
* Puts the (n) bytes in (b) on top of
* the buffer. If the buffer is out of space,
* reallocate the buffer to a bigger size
*/
void byteBufferPut(bytebuffer_t *buffer, byte *b, int n)
{
  if(buffer->mark + n >= buffer->cap)
  {
    while(buffer->cap <= buffer->mark + n)
    {
      buffer->cap = 2 * buffer->cap;
    }
    buffer->buffer = (byte*)realloc(buffer->buffer, buffer->cap);
  }
  int i;
  int mark = buffer->mark;
  byte* bytes = buffer->buffer;
  for(i = 0; i < n; i++)
  {
    bytes[mark + i] = b[i];
  }
}
