#include <stdio.h>
#include <stdlib.h>

struct tiff{
	unsigned short id;
	unsigned short type;
	unsigned int datanum;
	unsigned int offset;
};

struct tiff tag;

int main(int arg, char **arv){

	FILE *rfile;		//file open jpeg with
	char verify[20];	//buffer to read file into
	char tagb[12];		//buffer to read tags
	short count;		//number of TIFF tags that exist, ID for tiff tags
	int i;				//for loop counter
	
	//making sure the file exists
	if((rfile = fopen("test.jpg","rb"))==NULL){
		printf("File not found");
		return 1;
	}	
	
	//reading the first 20 bytes of the file into the buffer
	fread(verify,sizeof(char),20,rfile);

	//Verifying that the the file is APP1
	if((verify[2] & 0xff)!=0xff || (verify[3] & 0xff)!=0xe1){
		printf("APP tag: %x%x\n\n",verify[2] & 0xff, verify[3] & 0xe1);
		printf("File is not APP1, terminating");
		return 1;
	}
	else{
		printf("File is APP1.\n\n");
	}

	fread(tagb,sizeof(char),2,rfile);
	count = tagb[0] | tagb[1] << 8;

	for(i=0;i<count;i++){
		fread(tagb,sizeof(char),12,rfile);

		tag.id = (tagb[0] | (tagb[1] << 8));
		tag.type = tagb[2] | (tagb[3] << 8);
		tag.datanum = tagb[4] | (tagb[5] << 8) | (tagb[6] << 16) | (tagb[7] << 24);
		tag.offset = tagb[8] | (tagb[9] << 8) | (tagb[10] << 16) | (tagb[11] << 24);


		if(tag.id==0x010f) {
			printf("found it 1! %x\n\n",tag.id);
		}
		else if(tag.id==0x0110){
			printf("found it 2! %x\n\n",tag.id);
		}
		else if(tag.id==0x8769){
			printf("found it 3! %x\n\n",tag.id);
		}
		else{
			printf("some random shit %x\n\n",tag.id);
		}			
	}
	printf("done!");
}