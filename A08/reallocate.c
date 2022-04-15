#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "rand.h"

int main (int argc, char* argv[]) {
  void *init = sbrk(0);
  printf("The initial top of the heap is %p.\n", init);
  for (int i = 0; i < 10; i++) {
    void *test_allocate = malloc(sizeof(char) * 100);
    if  (test_allocate == NULL) {
      printf("Memory allocation failed. Exiting.");
      exit(1);
    }
    free(test_allocate);
    test_allocate = NULL;
  }

  void *current = sbrk(0);
  printf("The current top of the heap is %p.\n", current);
  
  int heap_size = (int)(current - init);
  printf("Incresed by %d (0x%d) bytes.\n", heap_size, heap_size);
  return 0 ;
}
