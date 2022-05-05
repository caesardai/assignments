#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int matching(int word_len, char input, int *correct_guesses, 
  char game_progress[], char word[]);

int main() {
  char input;
  char word[33]; 
  char game_progress[33]; // 32 + 1 for terminating character
  int num_words, word_len, rand_choice, game_len;

  FILE* infile = fopen("words1.txt", "r");
  if (infile == NULL) {
    printf("Unable to open file. Exiting\n");
    exit(1);
  }

  // read in first line as an integer
  fgets(word, 33, infile); 
  num_words = atoi(word); // string to number

  // randomly pick word
  srand(time(0));
  rand_choice = rand() % num_words + 1;
  for (int i = 0; i < rand_choice; i++) {
    fgets(word, 33, infile);
  }
  
  word_len = strlen(word);
  word[word_len - 1] = '\0';
  
  for (int i = 0; i < (word_len - 1); i++) {
    game_progress[i] = '_';
  }
  game_progress[word_len-1] = '\0';
  game_len = strlen(game_progress);

  // print statement
  printf("Welcome to Word Guess!\n");
  int match = 0;
  int round_index = 0;
  int correct_guesses = 0;
  while(correct_guesses != (word_len - 1)) {
    match = 0; // reset
    round_index++;
    printf("\nTurn: %d\n", round_index);
    for (int i = 0; i < game_len; i++) {
      printf("%c ", game_progress[i]);
    }
    printf("\n");
    printf("Guess a character: ");
    scanf(" %c", &input);

    matching(word_len, input, &correct_guesses, game_progress, word);
  }

  for (int i = 0; i < game_len; i++) {
    printf("%c ", game_progress[i]);
  }

  printf("\nYou won in %d turns!\n", round_index);
  fclose(infile);
  return 0;
}

int matching(int word_len, char input, int *correct_guesses, 
  char game_progress[], char word[]) {
  int found = 0;
  int num_inputs;
  num_inputs = *correct_guesses;
  for (int i = 0; i < word_len; i++) {
    if (word[i] == input) {
      game_progress[i] = input;
      num_inputs++;
      found = 1;
    }
  }
  if (found != 1) {
      printf("Sorry, letter %c not found!\n", input);
    }
  *correct_guesses = num_inputs;
  return num_inputs;
}

