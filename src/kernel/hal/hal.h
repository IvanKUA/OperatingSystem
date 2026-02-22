#pragma once

#include "vfs/vfs.h"

void HAL_Init();
VFS_T* HAL_GetVFS();

void print(const char* str);