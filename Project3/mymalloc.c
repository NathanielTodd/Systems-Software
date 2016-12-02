#include <sys/types.h>
#include <sys/mman.h>
#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <math.h>
#include "mymalloc.h"
#define MAX_MEM 1<<30

//list: array of struct pointers for free space
//indexing: memsize = 2^(i+5)
struct header *list[26];

static void * base = NULL;

//Struct for header of each memory segment.
//The char at the beginning of the segment will always be present
//But the struct pointers will only be used in free memory
//When returning a pointer to allocated memory you can return the pointer
//of the struct plus sizeof(char)
struct header{
	unsigned char state;
	struct header *prev;
	struct header *next;
};

/** @brief partitions memory to create appropriate sized block
 *  @param index, the size of the block needed
 */
void part_mem(int index){

	//if no block is available to split, recurse to next size and to find another to split
	if(list[index-5]==NULL){
		part_mem(index+1);
	}
	//to get the proper incrementing, void pointers are initially used.
	void *start = list[index-5];
	void *begin = start;
	void *half = (start+(1<<(index-1)));
	//Once the proper locations are found, I set header structs to the location
	struct header *old = start;
	struct header *new1 = begin;
	struct header *new2 = half;
	//removing the split block from the list
	if(old->next==NULL){
		list[index-5] = NULL;
	}
	else{
		list[index-5] = old->next;
		list[index-5]->prev = NULL;
	}
	//adding the new blocks to the list
	new1->state = (index-1);
	new1->state = (0<<7 | new1->state);
	new1->prev = NULL;
	new1->next = new2;
	list[index-6] = new1;
	new2->state = (index-1);
	new2->state = (0<<7 | new2->state);
	new2->prev = new1;
	new2->next = NULL;
}

/** @brief allocates block of size 2^n and returns pointer to usable memory
 *	2^n>=(size + sizeof(char)) block is larger or equal the size of the header plus the size the user requested
 *  @param int, the size of the block needed
 *  @return void *, location of allocated memory
 */
void *my_buddy_malloc(int size){

	void * ret = NULL;			//return pointer
	struct header *cur = NULL;
	int index;					//index for pointer list
	int i;

	//checking to make sure the size is within program capability
	if(size<1 || size>(1<<30)){
		return NULL;
	}
	
	//initializing the 1 GB space if it has not been mapped yet.
	if(base==NULL){
		base = mmap(NULL, MAX_MEM, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON, 0, 0);
		cur = base;
		cur->state = 30;
		cur->state = (0<<7 | cur->state);
		cur->prev = NULL;
		cur->next = NULL;
		list[25] = cur;
  	}
	assert(base!=NULL);

	//seeing what size space we need
	//log2(size+sizeof(char)) finds the index
	//ceil rounds up to the nearest hole number to ensure we have enough space
	index = (int)ceil(log2((double)(size+sizeof(char))));
	//setting the lowest block size
	if(index<5){
		index = 5;
	}

	//Checking free memory
	if(list[index-5]==NULL){
		//validating there is enough memory left
		for(i = index;i<32;i++){
			if(list[i-5]!=NULL){
				break;
			}
			else if(i==31){
				return NULL;
			}
		}
		//creating memory blocks of appropriate size
		part_mem(index+1);
	}

	//grabbing block of appropriate size and modifying its header
	cur = list[index-5];
	cur->state = (cur->state|(1<<7));
	//removing from list
	if(cur->next!=NULL){
		list[index-5] = cur->next;
		cur->next->prev = NULL;
	}
	else{
		list[index-5] = NULL;
	}
	cur->next = NULL;
	cur->prev = NULL;

	ret = ((void *)cur + sizeof(char));
	return ret;
}

/** @brief takes a freed block to coalesc if possible
 *  @param struct header *, header of block that has been freed
 */
void coalesc(struct header *cur){
	//finding buddy
	void *ptr = (void *)cur;
	struct header *buddy = ((ptr-base) ^ (1<<((cur->state)&127))) + base;
	int size = (cur->state)&127;
	//checking to see if coalescing is possible
	//if states are equal, it means buddy and cur are the same size and unoccupied.
	if(cur->state==buddy->state){
		//removing buddy current free list
		if(buddy->prev==NULL){
			list[size-5] = buddy->next;
			if(buddy->next!=NULL){
				(buddy->next)->prev = NULL;
			}	
			
		}
		else{
			(buddy->prev)->next = buddy->next;
			if(buddy->next!=NULL){
				(buddy->next)->prev = buddy->prev;
			}			
		}
		//setting cur to the first occuring of the buddies
		if(buddy<cur){
			cur = buddy;
		}
		//increasing size
		size = size+1;
		cur->state = size;
		cur->state = (0<<7 | cur->state);
		//recursing to check for more coalescing possibilities
		coalesc(cur);		
	}
	else{
		//if no coalescing is possible adding final block to list
		if(list[size-5]==NULL){
			list[size-5] = cur;
			cur->prev = NULL;
			cur->next = NULL;
		}
		else{
			struct header *set = list[size-5];
			while(set->next!=NULL){set=set->next;}
			set->next = cur;
			cur->prev = set;
			cur->next = NULL;
		}
	}
}

/** @brief takes a block of memory and frees it
 *  @param void *, block to be freed
 */
void my_free(void *ptr){
   assert(base!=NULL);
   ptr = ptr - sizeof(char);
   struct header *cur = ptr;
   cur->state = (cur->state & ~(1<<7)); //setting occupancy bit to 0
   coalesc(cur);
}

/** @brief Dump the contents of the heap.
 *
 *  Traverse the heap starting from the head of the list and print
 *  each block.  While traversing, check the integrity of the heap
 *  through various assertions.
 *
 *  @return Void.
 */
void dump_heap(){
  struct header *cur;
  int i;
  for(i=0; i<26;i++){
	  printf("%d->",i+5);
	  for(cur = list[i]; cur != NULL; cur = cur->next) {
	    printf("[%d:%d:%d]->", ((cur->state>>7)&1), (char*)cur - (char*)base, 1<<((cur->state)&127));
	    if(cur->next != NULL && cur->prev!=NULL) assert(cur->next->prev == cur); // if not last block, check that forward/backward links are consistent
	  }
	  printf("NULL\n");
	}
}