#pragma once

#include <stdint.h>

extern uint8_t __bss_start;
extern uint8_t __end;

void init_kernel();
void init_heap();

void __attribute__((section(".entry"))) kmain(uint8_t bootDrive);