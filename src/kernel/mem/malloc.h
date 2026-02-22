#pragma once

#include <stddef.h>
#include <stdint.h>

void malloc_init(void* start, void* end);

void* kmalloc(size_t size);
void* krealloc(void* ptr, size_t size);
void kfree(void* ptr);