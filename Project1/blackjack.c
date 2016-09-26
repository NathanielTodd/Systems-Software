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
	int value = (rand() % 13) + 2;
	/*testing for face card id*/
	if(value==11 || value==12 || value==13){
		value=10;
	}
	else if(value==14){
		value=11;
	}

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
* printhands function: prints out the contents of each players hands
*/
void printhands(){
	printf("\nDealer:\n%d ",d.hand[0]);
	int i;
	for(i=1;i<d.size; i++){
			printf("+ %d ",d.hand[i]);
		}
		printf("= %d\n\nYou: \n%d ",d.total,p.hand[0]);
		for(i=1;i<p.size; i++){
			printf("+ %d ",p.hand[i]);
		}
		printf("= %d\n\n",p.total);
}

int main(int arg, char **arv){

	/*seeding the random number generator*/
	srand((unsigned int)time(NULL));
	d.size = 0; d.total = 0;
	p.size = 0; p.total = 0;

	/*dealing first four cards*/
	deal(0); deal(0);
	deal(1); deal(1);

	/**Displaying initial data*/
	printf("\nDealer:\n? + %d\n\n",d.hand[1]);
	printf("You:\n%d + %d = %d\n\n",p.hand[0],p.hand[1],p.total);

	/*starting game logic*/
	int play = 1;
	char input[10];
	while(play==1){

		printf("Would you like to \"hit\" or \"stand\"? ");
		scanf("%s",input);

		/*deciding who hits*/
		if(strcmp(input,"hit")==0){
			deal(1);
			if(d.total<17 && p.total<22){
				deal(0);
			}
		}
		else if(strcmp(input,"stand")==0){
			if(d.total<17){
				deal(0);
			}
		}
		else{
			printf("Choice not recognized. Try again.\n");
		}

		/* deciding who wins*/
		if(d.total>21){
			printf("\nThe Dealer Busted. You Win!");
			printhands();
			play = 0;
		}
		else if(p.total>21){
			printf("\nYou Busted. The Dealer Wins!");
			printhands();
			play = 0;
		}
		else if(strcmp(input,"stand")==0 && d.total>16){
			if(d.total>p.total){
				printf("\nYou both chose stand. The Dealer Wins!");
				printhands();
				play = 0;
			}
			else if(p.total>d.total){
				printf("\nYou both chose stand. You Win!");
				printhands();
				play = 0;
			}
			else if(p.total==d.total){
				printf("\nYou both chose stand. It's a Push!");
				printhands();
				play = 0;
			}
		}
		else{
			printf("Dealer:\n? + %d\n\n",d.hand[1]);
			printf("You:\n%d + %d = %d\n\n",p.total-p.hand[p.size-1],p.hand[p.size-1],p.total);
		}		
	}
}