#pragma once

#include <stdint.h>

#define VNODE_PATH_MAX 256

typedef enum
{
    VNODE_TYPE_FILE,
    VNODE_TYPE_DIR,
    VNODE_TYPE_DEV
} VNodeType_T;

struct VNode;

typedef int (*VNodeRead)(struct VNode* node, uint8_t* buf, int size, int offset);
typedef int (*VNodeWrite)(struct VNode* node, const uint8_t* buf, int size, int offset);

typedef struct VNode
{
    char path[VNODE_PATH_MAX];
    VNodeType_T type;
    struct VNode* parent;
    struct VNode** children;
    int childCount;
    VNodeRead read;
    VNodeWrite write;
    void* data;
} VNode_T;

VNode_T* VNode_Create(const char* path, VNodeType_T type);
void VNode_Destroy(VNode_T* node);