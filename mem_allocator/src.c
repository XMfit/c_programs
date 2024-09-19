#define _GNU_SOURCE
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define INIT_HEAP_SIZE (1 << 25)                              // Size of heap 2MB
#define BLOCK_SIZE sizeof(struct block_metadata)              // Size of a block

/* Pointer for heap */
void *heap = NULL;

/* Memory block */
struct block_metadata {
    size_t size;                    // size of block
    int free;                       // flag for if its free or not
    struct block_metadata *next;    // points to next block
};

/* Functions to manage heap structure */
int init_heap();
int destroy_heap();

/* Expected functions to aid in malloc() and free()*/
size_t align_size(size_t size);

/* Functions for dynamic allocation */
void *my_malloc(size_t size);
void my_free(void *ptr);

int main() {

    init_heap();

    printf("Location %p\n", (void *)heap);

    int *ptr1 = (int *)my_malloc(sizeof(int));
    *ptr1 = 1;
    printf("Location %p Value: %d\n", (void *)ptr1, *ptr1);
    my_free(ptr1);

    char *ptr2 = (char *)my_malloc(sizeof(char));
    *ptr2 = 'A';
    printf("Location %p Value: %c\n", (void *)ptr2, *ptr2);
    my_free(ptr2);

    char *str = (char *)my_malloc(sizeof(char) * 2);
    str[0] = 'H';
    str[1] = 'i';
    printf("Location %p Value: %s\n", (void *)str, str);
    my_free(str);

    void *ptr4 = my_malloc(1 << 25);
    void *ptr5 = my_malloc(sizeof(char) + 2);

    destroy_heap();
    return 0;
}

int init_heap() {
    heap = mmap(NULL, INIT_HEAP_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    if (heap == MAP_FAILED) {
        perror("mmap failed");
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
    // bitwise and clears lower bit that represents remainder, rounding up to a multiple
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
        perror("Unable to allocate block\n");
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
