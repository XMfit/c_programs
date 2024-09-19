#include "../include/heap.h"

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


    destroy_heap();
    return 0;
}
