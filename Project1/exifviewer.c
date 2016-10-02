#include <stdio.h>
#include <stdlib.h>

/*
* header struct
* struct for storing the necessary information from the header
*/
struct header{
	unsigned short app;
	unsigned short end;
	unsigned int estring;
};

/*
* struct for storing each tiff tag
*/
struct tiff{
	unsigned short id;
	unsigned short type;
	unsigned int datanum;
	unsigned int offset;
};

/*
* function etag - function that evaluates the tag id
* param t - unsigned short that is the tag id
* return int - either a 1 or 0. 1 if the id is a tag we're looking for. 0 otherwise.
*/
int etag(unsigned short t){
	if(t==0xa002 || t==0xa003 || t==0x8827 || t==0x829a || t==0x829d || t==0x920a || t==0x010f || t==0x0110 || t==0x9003){
		return 1;
	}
	else{
		return 0;
	}
}

/*
* function tagread - read the data from the buffer into the struct
* param *t - pointer to the tag struct to store the info
* param tb[12] - buffer to extract the data from
*/
void tagread(struct tiff *t, char tb[12]){
	t->id = ((tb[0]&0xff) | ((tb[1]&0xff) << 8));
	t->type = (tb[2]&0xff) | ((tb[3]&0xff) << 8);
	t->datanum = (tb[4]&0xff) | ((tb[5]&0xff) << 8) | ((tb[6]&0xff) << 16) | ((tb[7]&0xff) << 24);
	t->offset = (tb[8]&0xff) | ((tb[9]&0xff) << 8) | ((tb[10]&0xff) << 16) | ((tb[11]&0xff) << 24);
}

/*
* function tagreadh - read the data from the header buffer into the header struct
* param *h - pointer to the header struct to store the info
* param v[12] - buffer to extract the data from
*/
void tagreadh(struct header *h, char v[20]){
	h->app = ((v[2] & 0xff) << 8) | (v[3] & 0xff);
	h->end = ((v[12] & 0xff) << 8) | (v[13] & 0xff);
	h->estring = (v[9]&0xff) | ((v[8]&0xff) << 8) | ((v[7]&0xff) << 16) | ((v[6]&0xff) << 24);
}

/*
* function tagprints - prints out data of tag type 2
* param *t - pointer to the tag struct of tag that will be printed
* param *s - file to read the data from
*/
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

/*
* function tagprint34 - prints out data of tag type 3 & 4
* param *t - pointer to the tag struct of tag that will be printed
*/
void tagprint34(struct tiff *t){
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

/*
* function tagprintf - prints out data of tag type 5
* param *t - pointer to the tag struct of tag that will be printed
* param *s - file to read the data from
*/
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

	FILE *rfile;							//file open jpeg with
	int p;									//p - position variable to keep track of reading locaion
	short i; short count;					//i - counter for for loops, count - number of tags in file	
	char tagb[12];	struct tiff tag;		//buffer & struct to read & store tiff tags
	char headb[20]; struct header head;		//buffer & struct to read & verifying information from header
	

	//making sure the file exists
	if((rfile = fopen(argv[1],"rb"))==NULL){
		printf("File not found, terminating.\n\n");
		return 1;
	}	
	
	//reading the first 20 bytes of the file into the buffer and filling struct
	fread(headb,sizeof(char),20,rfile);
	tagreadh(&head,headb);

	//Verifying that the the file is APP1 and little endian
	if(head.app!=0xffe1){
		printf("File is not APP1, terminating\n");
		return 0;
	}
	else{
		printf("\nFile is APP1.");
	}

	//Verifying Exif String
	if(head.estring!=0x45786966){
		printf(" Exif String not located, terminating.\n");
		return 0;
	}
	else{
		printf(" Exif String Verified.");
	}

	//Verifying Endian
	if(head.end!=0x4949){
		printf(" File is not little endian, terminating.\n");
		return 0;
	}
	else{
		printf(" File is little endian.\n");
		printf("-----------------------------------------------------------\n\n");
	}

	//reading in the count to see how many tiff tags there are.
	fread(tagb,sizeof(char),2,rfile);
	count = ((tagb[0]&0xff) | ((tagb[1]&0xff) << 8));

	//looping through the tags
	for(i=0;i<count;i++){
		//reading and storing the next tiff tag
		fread(tagb,sizeof(char),12,rfile);
		tagread(&tag,tagb);
		//seeking to data and printing if the tag is one we need
		if(etag(tag.id)==1) {
			p = ftell(rfile);
			fseek(rfile,tag.offset+12,SEEK_SET);
			switch(tag.type){
				case 2:
					tagprints(&tag,rfile);
					break;
				case 3:
				case 4:
					tagprint34(&tag);
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