#include "vnode.h"
#include <mem/malloc.h>
#include <string.h>

VNode_T* VNode_Create(const char *path, VNodeType_T type)
{
    VNode_T* node = (VNode_T*)kmalloc(sizeof(VNode_T));
    if (!node) return NULL;

    strncpy(node->path, path, VNODE_PATH_MAX - 1);
    node->path[VNODE_PATH_MAX - 1] = '\0';

    node->type = type;

    node->parent = NULL;
    node->children = NULL;
    node->childCount = 0;

    node->read = NULL;
    node->write = NULL;

    node->data = NULL;

    return node;
}

void VNode_Destroy(VNode_T* node)
{
    if (!node) return;

    if (node->children)
        kfree(node->children);

    kfree(node);
}