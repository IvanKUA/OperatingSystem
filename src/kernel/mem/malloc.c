#include "malloc.h"
#include "memory.h"

typedef struct BlockHeader
{
    size_t size;
    struct BlockHeader* next;
    int free;
} BlockHeader_T;

#define BLOCK_SIZE sizeof(BlockHeader_T)
#define ALIGN4(x) (((x) + 3) & ~0x3)

static BlockHeader_T* heap_start = NULL;

void malloc_init(void* start, void* end)
{
    heap_start = (BlockHeader_T*)start;

    heap_start->size = (uintptr_t)end - (uintptr_t)start - BLOCK_SIZE;
    heap_start->next = NULL;
    heap_start->free = 1;
}

static void merge_free_blocks()
{
    BlockHeader_T* current = heap_start;

    while (current && current->next)
    {
        if (current->free && current->next->free)
        {
            current->size += BLOCK_SIZE + current->next->size;
            current->next = current->next->next;
        }
        else
        {
            current = current->next;
        }
    }
}

void* kmalloc(size_t size)
{
    if (size == 0)
        return NULL;

    size = ALIGN4(size);

    BlockHeader_T* current = heap_start;

    while (current)
    {
        if (current->free && current->size >= size)
        {
            if (current->size > size + BLOCK_SIZE)
            {
                BlockHeader_T* new_block =
                    (BlockHeader_T*)((uint8_t*)current + BLOCK_SIZE + size);

                new_block->size = current->size - size - BLOCK_SIZE;
                new_block->next = current->next;
                new_block->free = 1;

                current->next = new_block;
                current->size = size;
            }

            current->free = 0;

            return (void*)(current + 1);
        }

        current = current->next;
    }

    return NULL;
}

void* krealloc(void* ptr, size_t size)
{
    if (ptr == NULL)
        return kmalloc(size);

    if (size == 0)
    {
        kfree(&ptr);
        return NULL;
    }

    BlockHeader_T* block = ((BlockHeader_T*)ptr) - 1;

    if (block->size >= size)
        return ptr;

    void* new_ptr = kmalloc(size);
    if (!new_ptr)
        return NULL;

    memcpy(new_ptr, ptr, block->size);

    kfree(&ptr);

    return new_ptr;
}

void kfree(void* ptr)
{
    if (!ptr)
        return;

    BlockHeader_T* block = ((BlockHeader_T*)(ptr)) - 1;
    block->free = 1;

    merge_free_blocks();

    ptr = NULL;
}