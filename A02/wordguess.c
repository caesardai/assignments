#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

int main()
{

  int turn = 0;
  int num_of_words;
  char buffer[1024];
  char word[32];
  char guess;
  bool match;
  bool guess_complete;

  // declare 'FILE *' variable
  FILE *infile;
  //FILE *outfile;

  srand(time(0));

  // opening the file
  infile = fopen("words.txt", "r");
  if (infile == NULL) {
    printf("Error: unable to open file %s\n", "words.txt");
    exit(1);
  }

  if (fgets(buffer, 1024, infile)) {
    // pointer to where the char array is stored n max number to be read
    // infile where the characters are read from
    sscanf(buffer, "%d", &num_of_words);
  }

  int rand_num = rand() % num_of_words + 1;

  for (int i = 0; i < rand_num; i++) {
    if (fgets(buffer, 1024, infile)) {
      sscanf(buffer, "%s", word);
    }
  }

  // declare a character pointer that points to the heap and allocate memory space
  // for characters(1 byte) of twice the length of the chosen random word
  char* chosen_rand_word = malloc(sizeof(char) * strlen(word) * 2);
  if (chosen_rand_word == NULL) {
    printf("Cannot allocate memory. Exiting\n");
    exit(1);
  }

  for (int i = 0; i < strlen(word)*2; i++) {
    if (i % 2 == 0) {
          strcpy(&chosen_rand_word[i], "_");
        }
        else {
          strcpy(&chosen_rand_word[i], " ");
        }
  }

  // funtion starts
  printf("Welcome to Word Guess!\n");

  while (!guess_complete) // while guess isn't complete, continue
  {
    turn++;

    // print statement
    printf("Turn: %d\n", turn);
    printf("%s\n", chosen_rand_word);
    printf("Guess a character: ");
    scanf(" %c", &guess);

    // check whether the guess matches
    for (int i = 0; i < strlen(word); i++){
      if (word[i] == guess) {
        match = true;
        chosen_rand_word[i*2] = guess;
      }
    }

      if (!match) {
        printf("Sorry, %c not found\n", guess);
      }

      guess_complete = true;

      for (int i = 0; i < strlen(word); i++) {
        if (chosen_rand_word[i*2] == '_') { // if there are still '_', then check is not complete
          guess_complete = false; 
        }
      }
    }

    printf("%s\n", chosen_rand_word);
    printf("You have won in %d turns. \n", turn);

    free(chosen_rand_word);
    chosen_rand_word = NULL;

    return 0;
}
