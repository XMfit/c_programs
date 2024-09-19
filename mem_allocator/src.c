#define _GNU_SOURCE
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 512              // Size of a block
#define INIT_HEAP_SIZE (1 << 25)    // 2 MB

/* Structure managing heap */
typedef struct {
    void *base;     // pointer to track where we are in the heap
    size_t size;
} Heap;

Heap heap;

/* Functions to manage heap structure */
Heap create_heap(size_t size);
Heap resize_heap(Heap *heap, size_t new_size);
void destroy_heap(Heap *heap);

int main() {

    heap = create_heap(INIT_HEAP_SIZE);

    printf("Size: %zu bytes\n", heap.size);
    printf("Location %p\n", (void *)heap.base);

    resize_heap(&heap, INIT_HEAP_SIZE * 2);

    printf("Size: %zu bytes\n", heap.size);
    printf("Location %p\n", (void *)heap.base);

    destroy_heap(&heap);

    printf("Size: %zu bytes\n", heap.size);
    printf("Location %p\n", (void *)heap.base);

    return 0;
}

Heap create_heap(size_t size) {
    Heap heap;
    heap.base = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    if (heap.base == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }
    heap.size = size;
    return heap;
}

Heap resize_heap(Heap *heap, size_t new_size) {
    void *new_base = mmap(NULL, new_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (new_base == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

    if (new_size > heap->size) {
        memcpy(new_base, heap->base, heap->size);
    }

    munmap(heap->base, heap->size);

    heap->base = new_base;
    heap->size = new_size;
}

void destroy_heap(Heap *heap) {
    if (heap->base != MAP_FAILED) {
        munmap(heap->base, heap->size);
        heap->base = NULL;
        heap->size = 0;
    }
}