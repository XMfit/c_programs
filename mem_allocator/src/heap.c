#include "../include/heap.h"

void *heap = NULL;

int init_heap() {
    heap = mmap(NULL, INIT_HEAP_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    if (heap == MAP_FAILED) {
        perror("mmap failed during heap initalization");
        return -1;
    }

    // Split our heap into blocks
    struct block_metadata *initial_block = (struct block_metadata *)heap;   // 1 big free block
    initial_block->size = INIT_HEAP_SIZE - BLOCK_SIZE;  // INIT_HEAP_SIZE - BLOCK_SIZE = available space for user data
    initial_block->free = 1;    // list all that user space as free
    initial_block->next = NULL; // next block is null

    return 0;
}

int destroy_heap() {
    if(munmap(heap, INIT_HEAP_SIZE)) {
        perror("munmap");
        return -1;
    }
    return 0;
}

size_t align_size(size_t size) {
    // size + BLOCK_SIZE - 1 ensures size of a multiple if it wasnt already
    // bitwise AND clears lower bit that represents remainder, rounding up to a multiple
    return (size + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);
}

void *my_malloc(size_t size) {
    struct block_metadata *block = heap;
    struct block_metadata *last = NULL;

    if (size <= 0) {
        return NULL;
    }

    size = align_size(size);    // Align size

    // terminates upon finding suitable block or end of memory block list
    while (block && !(block->free && block->size >= size)) {
        last = block;
        block = block->next;
    }

    if (!block) {   // valid block not found
        fprintf(stderr, "Error: Unable to allocate memory block of size %ld\n", size);
        return NULL;
    }

    // Future block splitting?

    // Mark as no longer being free, and return
    block->free = 0;
    return (char *)block + BLOCK_SIZE;

}

void my_free(void *ptr) {
    if (!ptr) {
        return;
    }

    struct block_metadata *block = (struct block_metadata *)((char *)ptr - BLOCK_SIZE);
    block->free = 1;

    // Future free split blocks

}
