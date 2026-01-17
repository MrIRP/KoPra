
#include "fastop.h"

//assembly delay function
static void fastop::delay(int times)
{
  while (times > 0){
    __asm__ __volatile__ ("nop");
    times--;
  }
}