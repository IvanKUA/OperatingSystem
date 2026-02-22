#pragma once

#include "vnode.h"
#include <stdint.h>

#define VFS_MAX_MOUNTS 10
#define VFS_MOUNT_LABEL_MAX 32

typedef struct VFSMount
{
    char label[VFS_MOUNT_LABEL_MAX];
    VNode_T* root;
} VFSMount_T;

typedef struct VFS
{
    VFSMount_T mounts[VFS_MAX_MOUNTS];
    int mountCount;
} VFS_T;

void VFS_Init();
VFS_T* VFS_Get();
int VFS_Mount(VFS_T* vfs, const char* label, VNode_T* root);
VNode_T* VFS_Lookup(VFS_T* vfs, const char* path);
int VFS_AddChild(VNode_T* parent, VNode_T* child);