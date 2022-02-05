#include <stdio.h>
#include <stdlib.h>

int main()
{
  char *word;
  int *count;
  
  // allocate memory
  word = malloc(sizeof(char) * 32); // allocate an array of 32 chars on the heap
                                    // after 'word' stores the base address of
                                    //an array of 32 contiguous char storage locations in heap memory
  if (word)
  {
    printf("malloc is successful for allocating space for word\n");
  }
  else
  {
    printf("malloc failed for allocating space for word\n");
  }

  count = malloc(sizeof(int)); //allocate heap memory for storing an int
  if (count)
  {
    printf("malloc is successful for allocating space for count\n");
  }
  else
  {
    printf("malloc failed for allocating space for count\n");
  }

  printf("Enter a word: ");
  scanf("%s", word);
  printf("Enter a count: ");
  scanf("%d", count);

  for (int i = 0; i < *count; i++)
  {
    printf("%s", word);
  }

  // freeing memory
  free(word);
  word = NULL;

  free(count);
  count = NULL;
  return 0;
}
