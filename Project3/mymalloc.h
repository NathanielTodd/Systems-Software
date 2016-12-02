#ifndef MYMALLOC_H_INCLUDED
#define MYMALLOC_H_INCLUDED
struct header;
void part_mem(int index);
void *my_buddy_malloc(int size);
void coalesc(struct header *cur);
void my_free(void *ptr);
void dump_heap();
#endif