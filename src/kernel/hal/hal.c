#include "hal.h"
#include <arch/i686/vga.h>
#include <arch/i686/gdt.h>
#include <string.h>

static VFS_T* vfs;

VNode_T* stdoutNode;

int Stdout_Write(struct VNode* node, const uint8_t* buf, int size, int offset);

void HAL_Init()
{
    i686_GDT_Init();

    vfs = VFS_Init();
    VNode_T* root = VNode_Create("/", VNODE_TYPE_DIR);

    stdoutNode = VNode_Create("stdout", VNODE_TYPE_FILE);
    stdoutNode->write = Stdout_Write;

    VFS_AddChild(root, stdoutNode);
    VFS_Mount(vfs, "", root);
}

int Stdout_Write(struct VNode* node, const uint8_t* buf, int size, int offset)
{
    for (int i = 0; i < size; i++)
    {
        VGA_putc(buf[i]);
    }

    return size;
}

VFS_T* HAL_GetVFS()
{
    return vfs;
}