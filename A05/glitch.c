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
  
  // construct file name for output file
  strncpy(output_name, input_name, name_len);
  strcat(output_name, "-glitch.ppm");

  printf("Reading %s with width %d and height %d\n", input_name, w, h);
  printf("Writing file %s\n", output_name);

  write_ppm(input_name, graph_matrix, w, h);

  // free allocated memory
  free(graph_matrix);
  graph_matrix = NULL;
  free(output_name);
  output_name = NULL;
  return 0;
}
