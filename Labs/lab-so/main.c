#include <stdio.h>
#include <dlfcn.h>

int main(){

	void *handle;
	void (*my_str_copy)(char *,char *);
	char *error;
	handle = dlopen("mystr.so", RTLD_LAZY);
	if(!handle){
		printf("%s\n", dlerror());
		exit(1);
	}
	dlerror();
	my_str_copy = dlsym(handle, "my_strcpy");
	if((error = dlerror()) != NULL) {
		printf("%s\n", error);
		exit(1);
	}
	
	char dest[100];
	char src[] = "Hello World!";

	my_str_copy(dest,src);

	printf("%s\n",dest);
	dlclose(handle);
	return 0;
}
