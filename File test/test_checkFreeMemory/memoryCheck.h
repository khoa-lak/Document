#ifndef memoryCheck_h
#define memoryCheck_h
#include <ArduinoJson.h>
#include <malloc.h>

extern "C" char *sbrk(int i);
/* Use linker definition */
extern char _end;
extern char _sdata;
extern char _estack;
extern char _Min_Stack_Size;

static char *ramstart = &_sdata;
static char *ramend = &_estack;
static char *minSP = (char*)(ramend - &_Min_Stack_Size);

#define NUM_BLOCKS 2
#define BLOCK_SIZE 4
class FreeMemory {
  public:
    uint32_t numFreeMemory()
    {
      char *heapend = (char*)sbrk(0);
      char * stack_ptr = (char*)__get_MSP();
      struct mallinfo mi = mallinfo();
      return (((stack_ptr < minSP) ? stack_ptr : minSP) - heapend + mi.fordblks);
    }
};
#endif
