#include <stdio.h>
#include <stdlib.h>

struct tiff{
	unsigned short id;
	unsigned short type;
	unsigned int datanum;
	unsigned int offset;
};

struct tiff tag;

int etag(unsigned short t){
	if(t==0xa002 || t==0xa003 || t==0x8827 || t==0x829a || t==0x829d || t==0x920a || tag.id==0x010f || tag.id==0x0110 || tag.id==0x9003){
		return 1;
	}
	else{
		return 0;
	}
}

void tagread(struct tiff *t, char tb[12]){
	t->id = ((tb[0]&0xff) | ((tb[1]&0xff) << 8));
	t->type = (tb[2]&0xff) | ((tb[3]&0xff) << 8);
	t->datanum = (tb[4]&0xff) | ((tb[5]&0xff) << 8) | ((tb[6]&0xff) << 16) | ((tb[7]&0xff) << 24);
	t->offset = (tb[8]&0xff) | ((tb[9]&0xff) << 8) | ((tb[10]&0xff) << 16) | ((tb[11]&0xff) << 24);
}

void tagprints(struct tiff *t, FILE *s){

	char data[t->datanum];
	fread(data,sizeof(char),t->datanum,s);
	if(t->id==0x010f) {
		printf("Manufacturer: \t%s\n",data);
	}
	else if(t->id==0x0110){
		printf("Camera Model: \t%s\n",data);
	}
	else if(t->id==0x9003){
		printf("Date Taken: \t%s\n",data);
	}
}

void tagprint32(struct tiff *t){
	
	if(t->id==0xa002) {
		printf("Width: \t\t%u pixels\n",t->offset);
	}
	else if(t->id==0xa003){
		printf("Height: \t%u pixels\n",t->offset);
	}
	if(t->id==0x8827) {
		printf("ISO: \t\tISO %d\n",t->offset);
	}
}

void tagprintf(struct tiff *t, FILE *s){


	unsigned char data[8];
	fread(data,sizeof(char),8,s);
	int data1 = (data[0]&0xff) | ((data[1]&0xff) << 8) | ((data[2]&0xff) << 16) | ((data[3]&0xff) << 24);
	int data2 = (data[4]&0xff) | ((data[5]&0xff) << 8) | ((data[6]&0xff) << 16) | ((data[7]&0xff) << 24);
	if(t->id==0x829a) {
		printf("Exposure: \t%d/%d second\n",data1,data2);
	}
	else if(t->id==0x829d){
		printf("F-stop: \tf/%.2lf\n",(double)data1/data2);
	}
	else if(t->id==0x920a){
		printf("Focal Length: \t%d mm\n",data1/data2);
	}
}

int main(int arg, char **argv){

	FILE *rfile;		//file open jpeg with
	char verify[20];	//buffer to read file into
	char tagb[12];		//buffer to read tags
	short count;		//number of TIFF tags that exist, ID for tiff tags
	int i;				//for loop counter
	int p;				//position to keep track of file locaion
	
	//making sure the file exists
	if((rfile = fopen(argv[1],"rb"))==NULL){
		printf("File not found");
		return 1;
	}	
	
	//reading the first 20 bytes of the file into the buffer
	fread(verify,sizeof(char),20,rfile);

	//Verifying that the the file is APP1 and little endian
	if((verify[2] & 0xff)!=0xff || (verify[3] & 0xff)!=0xe1){
		printf("APP tag: %x%x\n",verify[2] & 0xff, verify[3] & 0xff);
		printf("File is not APP1, terminating\n");
		return 0;
	}
	else{
		printf("\nFile is APP1.");
	}
	if((verify[12] & 0xff)!=0x49 || (verify[13] & 0xff)!=0x49){
		printf("\nEndian tag: %x%x\n",verify[12] & 0xff, verify[13] & 0xff);
		printf("File is not little endian, terminating.\n");
		return 0;
	}
	else{
		printf(" File is little endian.\n");
		printf("-------------------------------------------------------\n\n");
	}

	//reading in the count to see how many tiff tags there are.
	fread(tagb,sizeof(char),2,rfile);
	count = ((tagb[0]&0xff) | ((tagb[1]&0xff) << 8));

	//looping through the tags
	for(i=0;i<count;i++){
		//reading and storing the next tiff tag
		fread(tagb,sizeof(char),12,rfile);
		tagread(&tag,tagb);
		//determining if the tag is useful to us
		if(etag(tag.id)==1) {
			p = ftell(rfile);
			fseek(rfile,tag.offset+12,SEEK_SET);
			//printing the tag
			switch(tag.type){
				case 2:
					tagprints(&tag,rfile);
					break;
				case 3:
				case 4:
					tagprint32(&tag);
					break;
				case 5:
					tagprintf(&tag,rfile);
					break;
			}
			fseek(rfile,p,SEEK_SET);
		}
		else if(tag.id==0x8769){
			//goint to exif subblock
			fseek(rfile,tag.offset+12,SEEK_SET);
			//resetting count and i to loop through the sub block
			fread(tagb,sizeof(char),2,rfile);
			count = ((tagb[0]&0xff) | ((tagb[1]&0xff) << 8));
			i = 0;
		}
	}	
}