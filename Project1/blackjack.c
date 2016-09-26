#include <stdio.h>
#include <stdlib.h>

/*
* cards struct: struct that contains info for card hands
*/
struct cards{
    int hand[10];
    int total;
    int size;
};

/*p*/
struct cards p;
/*dealer*/
struct cards d;

/*
* deal function: deals card to player
* param person: 1 = player 0 = dealer
*/
void deal(int person){

	/* drawing card*/
	int value = (rand() % 10) + 2;

	/* switching to correct player*/
	switch(person){
		case 1:
			/* testing for ace*/
			if((value==11) && (p.total+11>21)){
				value = 1;
			}
			else if((value==11) && (p.total+11<=21)){
				value = 11;
			}

			p.hand[p.size] = value;
			p.total = p.total + value;
			p.size = p.size + 1;
			break;
		case 0:
			/*testing for ace*/
			if((value==11) && (d.total+11>21)){
				value = 1;
			}
			else if((value==11) && (d.total+11<=21)){
				value = 11;
			}

			d.hand[d.size] = value;
			d.total = d.total + value;
			d.size = d.size + 1;
			break;
		default:
			printf("Player ID invalid\n");
			break;
	}
	 
}

int main(int arg, char **arv){

	/*seeding the random number generator*/
	srand((unsigned int)time(NULL));
	d.size = 0; d.total = 0;
	p.size = 0; p.total = 0;

	/*dealing first four cards*/
	deal(0); deal(0);
	deal(1); deal(1);

	/*displaying initial game to user*/
	printf("Dealer:\n? + %d = %d\n\n",d.hand[1],d.total);
	printf("You:\n%d + %d = %d\n\n",p.hand[0],p.hand[1],p.total);

	int play = 1;
	char input[10];
	while(play==1){

		printf("Would you like to \"hit\" or \"stand\"? ");
		scanf("%s",input);
		printf("%s\n",input);
	}
}