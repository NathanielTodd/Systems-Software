#include <stdio.h>
#include <stdlib.h>

struct Node{
	int grade;
	struct Node *next;
};

int main(int arg, char **arv){
	
	int grade;
	int i = 0;
	int j = 0;
	struct Node *root;
	struct Node *temp;
	struct Node *temp2;

	/*
	*enterting grades
	*/
	while(grade!=-1){
		printf("\nEnter a grade (-1 to terminate): ");
		scanf("%d", &grade);
		if(grade!=-1){ 
			//allocating first node to root so I can maintain a pointer to the beginning
			if(i==0){
				i++;
				root = (struct Node *)malloc(sizeof(struct Node));
				(*root).grade = grade;
				temp = (struct Node *)malloc(sizeof(struct Node));
				(*root).next = temp;
			}
			//building the rest of the link list
			else{
				(*temp).grade = grade;
				temp2 = (struct Node *)malloc(sizeof(struct Node));
				(*temp).next = temp2;
				temp = temp2;
				i++;
			}
		}
	}

	//deconstructing
	printf("\nDeconstructing: \n\n");
	for(j=0;j<i;j++){
		printf("Grade %d: %d\n\n", j+1, (*root).grade);
		temp = (*root).next;
		free(root);
		root = temp;
	}
	printf("Done.\n");
}