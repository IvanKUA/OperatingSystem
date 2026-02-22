#include "vfs.h"
#include <mem/malloc.h>
#include <string.h>

#define PATH_MAX_LEN 256

VFS_T* VFS_Init(void)
{
    VFS_T* vfs = (VFS_T*)kmalloc(sizeof(VFS_T));
    if (!vfs) return NULL;

    vfs->mountCount = 0;
    return vfs;
}

int VFS_Mount(VFS_T* vfs, const char* label, VNode_T* root)
{
    if (!vfs || !label || !root)
        return -1;

    if (vfs->mountCount >= VFS_MAX_MOUNTS)
        return -1;

    VFSMount_T* m = &vfs->mounts[vfs->mountCount];

    strncpy(m->label, label, VFS_MOUNT_LABEL_MAX - 1);
    m->label[VFS_MOUNT_LABEL_MAX - 1] = '\0';
    m->root = root;

    vfs->mountCount++;
    return 0;
}

int VFS_AddChild(VNode_T* parent, VNode_T* child)
{
    if (!parent || !child)
        return -1;

    VNode_T** newChildren = (VNode_T**)krealloc(
        parent->children,
        sizeof(VNode_T*) * (parent->childCount + 1)
    );

    if (!newChildren)
        return -1;

    parent->children = newChildren;
    parent->children[parent->childCount] = child;
    parent->childCount++;

    child->parent = parent;
    return 0;
}

static VNode_T* VFS_FindChild(VNode_T* parent, const char* name)
{
    if (!parent || !name)
        return NULL;

    for (int i = 0; i < parent->childCount; i++)
    {
        VNode_T* child = parent->children[i];

        const char* childName = strchr(child->path, '/');
        if (childName)
            childName++;
        else
            childName = child->path;

        if (strcmp(childName, name) == 0)
            return child;
    }

    return NULL;
}

VNode_T* VFS_Lookup(VFS_T* vfs, const char* path)
{
    if (!vfs || !path || path[0] != '/')
        return NULL;

    if (vfs->mountCount == 0)
        return NULL;

    VNode_T* current = vfs->mounts[0].root;

    if (strcmp(path, "/") == 0)
        return current;

    char temp[PATH_MAX_LEN];
    strncpy(temp, path, PATH_MAX_LEN - 1);
    temp[PATH_MAX_LEN - 1] = '\0';

    char* token = strtok(temp, "/");

    while (token)
    {
        current = VFS_FindChild(current, token);
        if (!current)
            return NULL;

        token = strtok(NULL, "/");
    }

    return current;
}