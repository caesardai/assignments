#include <stdio.h>

int main() {
  int sum = 0;
  char bString[9];

  printf("Enter 8 bits: ");
  scanf(" %s", bString);
  
  for (int i = 0; i < 8; i++) {
    if (bString[i] == '1') {
      int deci = 1;
      int pwr = 7 - i;
      for (int j = 0; j < pwr; j++) {
        deci = deci * 2;
      }
      sum += deci;
    }
  }
  char ascii = sum;
  printf("Your character is: %c\n", ascii);
  return 0;
}
