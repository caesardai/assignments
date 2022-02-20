#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

// assignming intensity to corresponding symbols
void ascii_symbols(double intensity);

int main(int argc, char** argv) {
  // check command line input
  if (argc != 2) {
    printf("fail to read the ppm file");
    exit(1);
  }

  int w;
  int h;
  double intensity;
  char* input = argv[1]; 
  struct ppm_pixel** graph_matrix = NULL;

  // call read_ppm and print according to intensity
  graph_matrix = read_ppm(input, &w, &h);
  
  printf("Reading %s with width %d and height %d\n", input, w, h);
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      intensity = (graph_matrix[i][j].red + graph_matrix[i][j].green + graph_matrix[i][j].blue) / 3;
      ascii_symbols(intensity);
    }
    printf("\n");
  }

  // free allocated memory 
  // arrays
  for (int i = 0; i < h; i++) {
    free(graph_matrix[i]);
    graph_matrix[i] = NULL;
  }
  // arrray of pointers
  free(graph_matrix);
  graph_matrix = NULL;
  return 0;
}

void ascii_symbols(double intensity) {
  if (intensity >=0 && intensity <= 25) {
    printf("@");
  }
  else if (intensity >= 26 && intensity <= 50) {
    printf("#");
  }
  else if (intensity >= 51 && intensity <= 75) {
    printf("%%");
  }
  else if (intensity >= 76 && intensity <= 100) {
    printf("*");
  }
  else if (intensity >= 101 && intensity <= 125) {
    printf("o");
  }
  else if (intensity >= 126 && intensity <= 150) {
    printf(";");
  }
  else if (intensity >= 151 && intensity <= 175) {
    printf(":");
  }
  else if (intensity >= 176 && intensity <= 200) {
    printf(",");
  }
  else if (intensity >= 201 && intensity <= 225) {
    printf(".");
  }
  else {
    printf(" ");
  }
}
