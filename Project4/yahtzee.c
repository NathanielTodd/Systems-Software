#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

int bonus = 0;                                //bonus from upper section
int dice[] = {0,0,0,0,0};                     //array to hold the dice rolls 
int totalscore = 0;                           //user's total score
int upper[6] = {-1,-1,-1,-1,-1,-1};           //scores for upper section
int lower[7] = {-1,-1,-1,-1,-1,-1,-1};        //scores for lower section
int lower_score = 0;                          //lower total
int upper_score = 0;                          //upper total
int dice_driver;

/*
 * partitioning function to make output more readable
 */
void partition(){
  printf("---------------------------------------------------------\n");
}

/*
 * comparison function for qsort
 */
int cmpfunc(const void * a, const void * b){
  return ( *(int*)a - *(int*)b );
}

/*
 * calls read to get random roll form dice_driver
 */
int roll(){
  unsigned char randbuff;
  read(dice_driver,&randbuff,1);
  return randbuff;
}

/*
 * Prints out all the current scores and labeling the corresponding section
 */
void display_score(){
  // Print current results to user and updata scores
  partition();
  printf("NOTE: -1 represents categories you have not chosen yet.\n");
  printf("\nYour score so far is: %d\n",lower_score+upper_score+bonus);
  printf("\nOnes: %d\t\tFours: %d\nTwos: %d\t\tFives: %d\nThrees: %d\t\tSixes: %d\n", upper[0],upper[3],upper[1],upper[4],upper[2],upper[5]);
  printf("Upper section bonus: %d\n\n",bonus);
  printf("Three of a Kind: %d\tFour of a Kind: %d\nSmall Straight: %d\tLarge Straight:%d\nFull House: %d\t\tYahtzee: %d\nChance: %d\n",  lower[0],lower[1],lower[2],lower[3],lower[4],lower[5],lower[6]);
}

/*
 * Check function - gets input from user and ensures it is valid for category selections
 */
int check(int choice){
  char input[20];
  int selection;
  while(1){
    printf("\nSelection? ");
    fgets(input, sizeof(input), stdin);
    selection = atoi(input);
    switch(choice){
      //upper-lower selection
      case 2:
        if(selection==1 || selection ==2){
          return selection;
        }
        else{
          printf("Number %d does not correspond to an upper or lower section",selection);
        }
      //upper
      case 1:
        if(selection < 0 || selection > 6){
          printf("No category #%d in upper categories", selection);
        }
        else if(upper[selection - 1] == -1){
          return selection;
        }
        printf("Upper category #%d already assigned", selection);
        break;
      //lower
      case 0:
          if(selection < 0 || selection > 7){
            printf("No category #%d in lower categories", selection);
          }
          else if(lower[selection - 1] == -1){
            return selection;
          }
          printf("Lower category #%d already assigned", selection);
          break;
      //Error
      default:
          printf("Case Selection Check Error\n");
          break;
    }
  }
}

/*
 * Scores players roll once placed in appropriate category. Uses check() to make category selections.
 */
void score(){

  int value = 0;                //auxiliary variable
  int selection, i, sum = 0;    //selection holder, counter, sum variables
  int frequency[7];             //holds the frequency of occurance of each possible die value

  printf("\nPlace dice into:\n");
  printf("1) Upper Section\n");
  printf("2) Lower Section\n");

  selection = check(2);

  // Upper categories
  if (selection == 1){
    printf("\nPlace dice into:\n");
    printf("1) Ones\n");
    printf("2) Twos\n");
    printf("3) Threes\n");
    printf("4) Fours\n");
    printf("5) Fives\n");
    printf("6) Sixes\n");

    selection = check(1);

    for (i = 0; i < 5; i++){
      if (dice[i] == selection){
        sum += selection;
      }
    }

    upper[selection - 1] = sum;
    upper_score += sum;

    if(upper_score > 62){
      bonus = 35;
    }
  }
  // Lower categories
  else{
    printf("\nPlace dice into:\n");
    printf("1) Three of a kind\n");
    printf("2) Four of a kind\n");
    printf("3) Small Straight\n");
    printf("4) Large Straight\n");
    printf("5) Full House\n");
    printf("6) Yahtzee\n");
    printf("7) Chance\n");

    selection = check(0);
    switch(selection){
      //three of a kind
      case 1:
        value = 0;
        sum = 0;
        for(i = 0; i < 7; i++){
          frequency[i] = 0;
        }

        //counting how many times each die appears
        for(i = 0; i < 5; i++){
          value = dice[i];
          frequency[value] += 1;  
          sum += value;
        }

        //assigning points
        for(i = 0; i < 7; i++){
          if(frequency[i] > 2){
            lower[0] = sum;
            lower_score += sum;
            break;
          }
          else{
            lower[0]=0;
          }
        }
        break;
      // Four of a kind
      case 2:
        value = 0;
        sum = 0;
        for(i = 0; i < 7; i++){
          frequency[i] = 0;
        }

        //counting how many times each die appears
        for(i = 0; i < 5; i++){
          value = dice[i];
          frequency[value] += 1;
          sum += value;
        }

        //assigning points
        for(i = 0; i < 7; i++){
          if(frequency[i] > 3){
            lower[1] = sum;
            lower_score += sum;
            break;
          }
          else{
            lower[1] = 0;
          }
        }
        break;
      // Small straight
      case 3:
        value = 0;
        //coutning how many die are in a sequence
        for(i=0;i<5;i++){
        	if(dice[i]==(dice[i+1]-1)){
        		value++;
        	}
        }
        if(value>2){
        	lower[2] = 30;
        	lower_score += 30;
        }
        else{
          lower[2] = 0;
        }
        break;
      // Large straight
      case 4:
        if(dice[0] < dice[1] && dice[1] < dice[2] && dice[3] < dice[3] && dice[3] < dice[4] && dice[4] < dice[5]){
          lower[3] = 40;
          lower_score += 40;
        }
        else{
          lower[3] = 0;
        }
        break;
      // Full House
      case 5:
        if(dice[0] == dice[1] && (dice[2] == dice[3]) && (dice[3] == dice[4])){
          lower_score += 25;
          lower[4] = 25;
        }
        else if((dice[0] == dice[1]) && (dice[1] == dice[2]) && dice[3] == dice[4]){
          lower_score += 25;
          lower[4] = 25;
        }
        else{
          lower[4] = 0;
        }
        break;
      // Yahtzee
      case 6:
        if(dice[0] == dice[1] && dice[1] == dice[2] && dice[2] == dice[3] && dice[3] == dice[4] && dice[4] == dice[5]){
          lower_score += 50;
          lower[5] = 50;
        }
        else{
          lower[5] = 0;
        }
        break;
      // Chance
      case 7:
        sum = 0;
        for(i = 0; i < 5; i++){
          sum += dice[i];
        }
        lower[6] = sum;
        lower_score += sum;
        break;
      default:
        printf("Case Selection Error\n");
        break;
    }
  }
}

/*
 * Reroll funciton - prompts user to select dice to reroll and evaluates the input numbers accordingly
 * 0 is the exit number when entered first, otherwise they will be ignored
 */
int reroll(int count){
  int reroll_buf[5] = {0,0,0,0,0};
  int i=0;
  char temp;

  printf("0's will continue without a reroll if entered first, otherwise, they will be ignored.\n");
  printf("Which dice to reroll? ");
  do {
      scanf("%d%c", &reroll_buf[i], &temp);
      i++;
  } while(temp != '\n');
  
  //exiting reroll if 0 is the first entered die
  if(reroll_buf[0]==0){
    return 1;
  }

  //evaluating all entered numbers
  for(i=0;i<5;i++){
    if(reroll_buf[i]>5 || reroll_buf[i]<0){
      printf("%d is not a valid die entry. Discarding.\n",reroll_buf[i]);
    }
    else if(reroll_buf[i]==0){
      continue;
    }
    else{
      dice[reroll_buf[i]-1] = roll();
    }
  }

  //printf roll
  switch(count){
    case 0:
      printf("\nYour second roll:\n");
      for (i = 0; i < 5; i++){
        printf("%d ", dice[i]);
      }
      printf("\n");
      break;
    case 1:
      printf("\nYour third, and final, roll:\n");
      for (i = 0; i < 5; i++){
        printf("%d ", dice[i]);
      }
      printf("\n");
      break;
    default:
      printf("reroll error\n");
      break;
  }
  return 0;
}

int main(){

  int turns, i;
  dice_driver = open("/dev/dice", O_RDONLY); // open dice_driver
  if (dice_driver == NULL){
    printf("Driver Error.\n");
    return 0;
  }

  for (turns = 12; turns >= 0; turns--){
    //getting dice roll to start turn
    partition();
    printf("You rolled:\n");
    for (i = 0; i < 5; i++){
      dice[i] = roll();
      printf("%d ", dice[i]);
    }
    printf("\n");

    //asking for reroll
    for(i = 0; i < 2; i++){
      if(reroll(i)){
        break;
      }
    }

    //sorting dice to make scoring easier
    qsort(dice, 5, sizeof(int), cmpfunc);
    //assigning score
    score();
    display_score();
    partition();
    //displaying turns
    if (turns == 0){
      printf("This is your last turn\n");
    }
    else if (turns == 1){
      printf("You have 1 turn left\n");
    }
    else{
      printf("You have %d turns left\n", turns);
    }
  }
  return 0;
}