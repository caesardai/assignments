#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// randomly generate AI moves
char AImove() {
  char AI;
  int num = (rand() % 3);
  if (num == 0) {
    AI == 'r';
    printf("rock\n");
  }
  else if (num == 1) {
    AI == 'p';
    printf("paper\n"); 
  }
  else {
    AI == 's';
    printf("scissors\n"); 
  }
}


// compare player and AI results
int match(char player, char AI) {
  // when player and AI make the same move
  if (player == AI) {
    return -1;
  }

  if (player == 'r' && AI == 'p') {
    return 0;
  }
  else if (player == 'p' && AI == 'r') {
    return 1;
  }

  if (player == 'r' && AI == 's') {
    return 1;
  }
  else if (player == 's' && AI == 'r') {
    return 0;
  }

  if (player == 'p' && AI == 's') {
    return 0;
  }
  else if (player == 's' && AI == 'p') {
    return 1;
  }
}


int main() {
  char player, AI;
  int result, rounds, scoreAI = 0, scorePlayer = 0;
  srand(time(0));

  //  printf("Welcome to Rock, Paper, Scissors!\nHow many rounds do you want to play?\n");
  //  scanf("%d", &rounds);


  //  for (int i = 0; i <= rounds; i++) {

  printf("Which do you choose? rock, paper, or scissors?\n");
  scanf("%c", &player);

  int num = (rand() % 3);                                                      
  if (num == 0) {                                                              
    AI == 'r';                                                                 
    //printf("rock\n");                                                          
  }                                                                            
  else if (num == 1) {                                                         
    AI == 'p';                                                                 
    //printf("paper\n");                                                         
  }                                                                            
  else {                                                                       
    AI == 's';                                                                 
    //printf("scissors\n");                                                      
  }        

  result = match(player, AI);

  if (result == -1) {
    printf("AI chose %c\n", AI);
    printf("AI score: %d, Player score: %d\n", scoreAI, scorePlayer);
  }

  else if (result == 1) {
    scorePlayer++;
    printf("AI chose %c\n", AI);                                                 
    printf("AI score: %d, Player score: %d\n", scoreAI, scorePlayer);  
  }

  else {
    scoreAI++;                                                                 
    printf("AI chose %c\n", AI);                                                 
    printf("AI score: %d, Player score: %d\n", scoreAI, scorePlayer);  
  }
  //}
}

//    if (player != 'r' || player != 'p' || player != 's') {
//      printf("AI chose %s", AI);                                           
//      printf("You entered an invalid choice: %c", player);             
//      printf("AI score: %d, Player score: %d", scoreAI, scorePlayer);  
