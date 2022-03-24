#include <stdio.h>
#include <string.h>

int main() {
  char phrase[100];
  
  printf("Please enter a phrase: ");
  fgets(phrase, 100, stdin);

  int len = strlen(phrase); // phrase length
  for (int i = 0; i < len; i++) {
    unsigned char ch = phrase[i];
    unsigned char second = 0x02;
    unsigned char third = 0x04;

    unsigned char secondch = ch & second;
    unsigned char thirdch = ch & third;

    if ((secondch >> 1) != (thirdch >> 2)) {
      // sec = (ch & second) ^ 1;
      // thir = (ch & third) ^ 1;
      secondch = secondch << 1;
      if (secondch != thirdch) {
        ch = ch ^ 0x04;
        ch = ch ^ 0x02;
      }
    }
    phrase[i] = ch;
  }
  phrase[len] = '\0';

  printf("scramble: %s\n", phrase);
  return 0;
}
