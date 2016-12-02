#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
* function etag - function that takes file and prints out all ASCII characters from 32-126
* param file - receives file from command line by running program with: ./mystrings <filename> 
*/
int main(int argc, char *argv[]){

  char *str = malloc(8 * sizeof(char)); //initial string buffer
  int size = 8;                         //size of string array
  int len = 0;                          //length of current string being read in
  char c;                               //current character being read from file
  FILE *file;                           //file descriptor

  //opening and making sure file exists
  file = fopen(argv[1], "r"); 
  if (file == NULL){
    printf("The file does not exist");
    return 0;
  }
 
  //reading strings from file
  while(feof(file) == 0) {

    //reading characters
    fread(&c, sizeof(c), 1, file);
    if (c > 31 && c < 127){
      str[len] = c;
      len++;
      //resizing array if necessary
      if(len==size){
        size = size * 2;
        str = realloc(str, size * sizeof(char));
      }
      continue;
    }

    str[len] = '\0';
    if (4 <= len){
      printf("%s\n", str);
    }

    len = 0;
    str[0] = '\0';
  }

  free(str);
  fclose(file);
  return 0;
}