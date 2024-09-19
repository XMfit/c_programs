#define _GNU_SOURCE
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define INIT_HEAP_SIZE (1 << 25)    // 2 MB
#define BLOCK_SIZE 256              // Size of a block

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

/* Functions for dynamic allocation */
void *my_malloc(size_t size);
void *my_free(void *ptr);

int main() {

    init_heap();

    // printf("Location %p\n", (void *)heap);

    int *ptr = (int *)my_malloc(sizeof(int));

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
    struct block_metadata *initial_block = (struct block_metadata *)heap;
    initial_block->size = INIT_HEAP_SIZE - BLOCK_SIZE;
    initial_block->free = 1;
    initial_block->next = NULL;

    return 0;
}

int destroy_heap() {
    if(munmap(heap, INIT_HEAP_SIZE)) {
        perror("munmap");
        return -1;
    }
    return 0;
}

void *my_malloc(size_t size) {
    struct block_metadata *block;

    if (size <= 0) {
        return NULL;
    }

}
