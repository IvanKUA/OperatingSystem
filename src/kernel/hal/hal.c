#include "hal.h"
#include <arch/i686/vga.h>
#include <arch/i686/gdt.h>
#include <string.h>

void HAL_Init()
{
    i686_GDT_Init();
}