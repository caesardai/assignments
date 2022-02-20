#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "read_ppm.h"

int main() {
  int w;
  int h;
  char* input = "feep-ascii.ppm"; // testing, so manually input file name
  struct ppm_pixel** graph_matrix = NULL;

  // call read_ppm
  graph_matrix = read_ppm(input, &w, &h);
  
  // print statment
  printf("Testing file %s: %d %d\n", input, w, h);
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      printf("(%d,%d,%d) ", 
        graph_matrix[i][j].red, 
        graph_matrix[i][j].green, 
        graph_matrix[i][j].blue);
    }
    printf("\n");
  }

  // freeing alocated memories
  // arrays
  for (int i = 0; i < h; i++) {
    free(graph_matrix[i]);
    graph_matrix[i] = NULL;
  }
  // array of pointers
  free(graph_matrix);
  graph_matrix = NULL;
  return 0;
}
