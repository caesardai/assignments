#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "rand.h"

int main (int argc, char* argv[]) {
  void *init = sbrk(0);
  for (int i = 0; i < 10; i++) {
    void *test_allocate = malloc(sizeof(char) * 10);
    if  (test_allocate == NULL) {
      printf("Memory allocation failed. Exiting.");
      exit(1);
    }
    free(test_allocate);
    test_allocate = NULL;
  }

  void *current = sbrk(0);
  int heap_size = (int)(current - init);
  printf("The initial top of the heap is %p.\n", init);
  printf("The current top of the heap is %p.\n", current);
  printf("We have allocated %d bytes.\n", heap_size);
  return 0 ;
}
