#include <stdio.h>
#include <stdlib.h>



struct tag{
	short id;
	short type;
	int datanum;
	int offset;
};

int main(int arg, char **arv){

	FILE *rfile;
	int nread;
	char verify[20];
	
	if((rfile = fopen("test.jpg","rb"))==NULL){
		printf("File not found");
		return 1;
	}	
	
	nread = fread(verify,sizeof(char),20,rfile);

	// printf("\n\n");
	// int i;
	// for(i=0;i<nread;i++){
	// 	char ch = verify[i];
	// 	printf("%x", ch & 0xff);
	// 	if((i+1)%2==0 && i!=0){
	// 		printf("\n\n");
	// 	}
	// }

	if(verify[2]!=0xff || verify[3]!=0xe1){
		printf("APP tag: %x%x\n\n",verify[2] & 0xff, verify[3] & 0xe1);
		printf("File is not APP1, terminating");
		return 1;
	}
}