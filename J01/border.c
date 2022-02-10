#include <stdio.h>
#include <string.h>

int main() {
  char word[64];
  char ch;
  printf("Please enter a word: ");
  scanf(" %s", word);
  printf("Please enter a symbol: ");
  scanf(" %c", &ch);

  for (int i = 0; i < strlen(word) + 4; i++) {
    printf("%c", ch);
  }

  printf("\n%c %s %c\n", ch, word, ch);
  
  for (int i = 0; i < strlen(word) + 4; i ++) {
    printf("%c", ch);
  }
  printf("\n");
  return 0;
}
