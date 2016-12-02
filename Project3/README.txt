Compilation of submitted code: gcc mymalloc.c mallocdrv.c -o test -lm

int main(){
	void *test = my_buddy_malloc(8);
 	dump_heap();
 	my_free(test);
 	dump_heap();
 	return 0;
}

Above is the very simplistic program that I used to initialy test my implementation of malloc and free. I simply inserted this into mymalloc.c and compiled it alone. By dumping the heap during and after the function calls it helped me debug my code. Once I got it working with this main, I moved to mallocdrv to test more complicated cases. Again, I inserted dump_heap() around function calls within mallocdrv and within the functions themselves to understand where the functions were causing Segmentation faults and why.
