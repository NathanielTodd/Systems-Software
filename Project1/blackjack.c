#include <stdio.h>
#include <stdlib.h>

/*
* cards struct - struct that contains info for card hands
*/
struct cards{
    int hand[10];
    int total;
    int size;
};

/*player*/
struct cards p;
/*dealer*/
struct cards d;

/*
* function deal - deals card to player
* param person - 1 = player 0 = dealer
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

/*
* function hprint - prints out players hands
*/
void hprint(){
	int i;
	//printing out the dealer
	printf("\n\nThe Dealer: \n%d",d.hand[0]);
	for(i=1;i<d.size;i++){
		printf(" + %d",d.hand[i]);
	}
	printf(" = %d",d.total);
	//printing out the player
	printf("\n\nYou: \n%d",p.hand[0]);
	for(i=1;i<p.size;i++){
		printf(" + %d",p.hand[i]);
	}
	printf(" = %d\n\n",p.total);
}


int main(int arg, char **arv){

	//play again label
	again: 
	printf("\nStarting Game: ");
	printf("\n-----------------------------------------------------------");

	/*seeding the random number generator*/
	srand((unsigned int)time(NULL));
	d.size = 0; d.total = 0;
	p.size = 0; p.total = 0;

	/*dealing first four cards*/
	deal(0); deal(0);
	deal(1); deal(1);

	/*displaying initial game to user*/
	printf("\nDealer:\n? + %d\n\n",d.hand[1]);
	printf("You:\n%d + %d = %d\n\n",p.hand[0],p.hand[1],p.total);

	int play = 1;
	char input[10];
	while(play==1){
		//try again label for bad input
		tryagain:
		printf("Would you like to \"hit\" or \"stand\"? ");
		scanf("%s",input);
		printf("*****************************************");
		//dealing player
		if(strcmp(input,"hit")==0){
			deal(1);
		}
		else if(strcmp(input,"stand")==0){}
		else{
			printf("\"%s\" not recognized. Try again.\n",input);
			goto tryagain;
		}
		//seeing if player busted
		if(p.total>21){
			printf("\nYou Busted! Dealer Won!");
			hprint();
			play = 0;
		}
		//dealing dealer
		if(d.total<17){
			deal(0);
			if(d.total>21){
				printf("\nDealer Busted! You Won!");
				hprint();
				play = 0;
			}
		}
		//both players chose stand, no one busted. Deciding who wins
		else if(strcmp(input,"stand")==0){
			printf("\nBoth Players chose Stand.\n");
			if(d.total==p.total){
				printf("\nIt's a push!");
				hprint();
				play = 0;
			}
			else if(d.total>p.total){
				printf("\nThe Dealer Won!");
				hprint();
				play = 0;
			}
			else if(p.total>d.total){
				printf("\nYou Won!");
				hprint();
				play = 0;
			}
		}

		/*displaying new info to user*/
		if(play==1){
			printf("\nDealer:\n? + %d\n\n",d.hand[1]);
			printf("You:\n%d + %d = %d\n\n",p.total-p.hand[p.size-1],p.hand[p.size-1],p.total);
		}
		else{
			//label to repeat for bad input
			badinput:
			printf("Play again (y/n):");
			scanf("%s",input);
			//dealing player
			if(strcmp(input,"y")==0){
				printf("\n");
				goto again;
			}
			else if(strcmp(input,"n")==0){
				printf("Goodbye.\n");
			}
			else{
				printf("\"%s\" not recognized. Try again.\n",input);
				goto badinput;
			}
		}
	}
}