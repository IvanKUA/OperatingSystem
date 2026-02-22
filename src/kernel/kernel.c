#include "kernel.h"
#include <hal/hal.h>
#include <vfs/vfs.h>
#include <arch/i686/vga.h>
#include <mem/memory.h>
#include <mem/malloc.h>

void init_kernel()
{
    memset(&__bss_start, 0, (&__end) - (&__bss_start));

    VGA_clrscr();
    init_heap();
    HAL_Init();
    VFS_Init();
}

void init_heap()
{
    malloc_init(&__end, &__end + 0x100000);
}

void __attribute__((section(".entry"))) kmain(uint8_t bootDrive)
{
    init_kernel();

    for(;;);
}