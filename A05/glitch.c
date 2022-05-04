// name: Caesar Dai
// added terminating character

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("Invalid runtime arguments");
    exit(1);
  }

  int w;
  int h;
  char* input_name = argv[1];
  char* output_name = malloc(strlen(argv[1]) + 8);
  int name_len = strlen(input_name) - 4;

  struct ppm_pixel** graph_matrix = NULL;

  // call read_ppm
  graph_matrix = read_ppm(argv[1], &w, &h);
  if (graph_matrix == NULL) {
    printf("failed to create graph matrix\n");
    exit(1);
  }
  
  
  // construct output file
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      graph_matrix[i][j].red = graph_matrix[i][j].red << (rand() % 2); 
      graph_matrix[i][j].green = graph_matrix[i][j].green << (rand() % 2); 
      graph_matrix[i][j].blue = graph_matrix[i][j].blue << (rand() % 2); 
    }
  }

  strncpy(output_name, input_name, name_len);
  output_name[name_len] = '\0';
  strcat(output_name, "-glitch.ppm");

  printf("Reading %s with width %d and height %d\n", input_name, w, h);
  printf("Writing file %s\n", output_name);

  write_ppm(output_name, graph_matrix, w, h);
  
  free(output_name);
  output_name = NULL;

  // free allocated memory
  for (int i = 0; i < h; i++) {
    free(graph_matrix[i]);
    graph_matrix[i] = NULL;
  }
  // array of pointers
  free(graph_matrix);
  graph_matrix = NULL;
  
  return 0;
}

