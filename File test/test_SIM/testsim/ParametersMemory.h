#ifndef ParametersMemory_h
#define ParametersMemory_h
#include <malloc.h>
// Total SRAM and Flash sizes for STM32F103RCT6 (in bytes)
#define TOTAL_SRAM 64 * 1024    // 64 KB
#define TOTAL_FLASH 512 * 1024  // 512 KB
extern "C" char *sbrk(int i);

// Linker symbols
// extern char _end;             // End of static variables (BSS)
// extern char _estack;          // Top of RAM (stack start)
// extern char _Min_Stack_Size;  // Minimum stack size from linker script
// static char *ramend = &_estack;
// static char *minSP = (char *)(ramend - &_Min_Stack_Size);
// static char *heapstart = &_end;

static char *ramstart = (char *)0x20000000;  // Địa chỉ bắt đầu RAM của STM32F1
static char *ramend = (char *)0x20000000 + TOTAL_SRAM;

class ParametersMemory {
public:


  uint32_t getCurrentHeapEnd() {
    // Lấy địa chỉ hiện tại của heap (tương đương __brkval)
    char *heap_end = (char *)sbrk(0);
    // Nếu chưa malloc lần nào, sbrk(0) trả về cuối bss
    return (uint32_t)heap_end;
  }
  // Calculate free RAM (between heap and stack + free heap blocks)
  uint32_t getFreeRam() {
    // char *heapend = (char *)sbrk(0);
    // char *stack_ptr;
    // __asm volatile("mov %0, sp"
    //                : "=r"(stack_ptr));   // Get current stack pointer
    // if (stack_ptr <= heapend) return 0;  // Stack/heap collision
    // struct mallinfo mi = mallinfo();
    // char *sp = (stack_ptr < minSP) ? stack_ptr : minSP;
    // return (sp - heapend) + mi.fordblks;
    char top;
    return &top - reinterpret_cast<char *>(sbrk(0));
  }

  // Calculate stack usage (from _estack to current stack pointer)
  uint32_t getStackUsage() {
    // char *stack_ptr;
    // __asm volatile("mov %0, sp"
    //                : "=r"(stack_ptr));
    // if (stack_ptr > ramend) return 0;  // Invalid stack pointer
    // return ramend - stack_ptr;
    char top;
    return ramend - &top;
  }

  // Calculate heap usage (from heap start to heap end)
  uint32_t getHeapUsage() {
    // char *heapend = (char *)sbrk(0);
    // if (heapend < heapstart) return 0;  // No heap usage
    // return heapend - heapstart;
    char *heapend = reinterpret_cast<char *>(sbrk(0));
    uint32_t used = heapend - ramstart;  // Ước lượng: trừ luôn static
    return (used > 0) ? used : 0;
  }
  //free block in heap
  // uint32_t freeMemoryInHeap() {
  //   // return mallinfo().fordblks;
  //   char top;  // Địa chỉ stack hiện tại
  //   uint32_t heap_end = getCurrentHeapEnd();
  //   uint32_t heap_start = (uint32_t)&__bss_end;
  //   uint32_t current_heap_top = (heap_end <= heap_start) ? heap_start : heap_end;
  //   return (uint32_t)(&top - (char *)current_heap_top);
  // }
  // Estimate free Flash (placeholder; actual value depends on binary size)
  uint32_t getFreeFlash() {
    // Note: This is an estimate. Actual Flash usage depends on the binary size.
    // You can get the program size from the build output (e.g., .bin file size).
    // For demonstration, assume program size is provided (e.g., 50 KB).
    const uint32_t program_size = 50 * 1024;  // Replace with actual size from build
    if (program_size > TOTAL_FLASH) return 0;
    return TOTAL_FLASH - program_size;
  }

  // Get total SRAM for reference
  uint32_t getTotalSram() {
    return TOTAL_SRAM;
  }

  // Get total Flash for reference
  uint32_t getTotalFlash() {
    return TOTAL_FLASH;
  }
};
#endif