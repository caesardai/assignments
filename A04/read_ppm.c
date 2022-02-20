#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

// TODO: Implement this function
// Feel free to change the function signature if you prefer to implement an 
// array of arrays
struct ppm_pixel** read_ppm(const char* filename, int* w, int* h) {
  
  FILE* infile = NULL;
  unsigned char r, g, b;
  //char filetype[10];
  char buffer[1024]; // allocate 1024 for potential use
  struct ppm_pixel** pixel = NULL;

  infile = fopen(filename, "r");
  if (infile == NULL) { // error message if can't open file
    printf("Cannot open file. Exiting.\n");
    return NULL;
  }

  // check whether it's ASCII
  fgets(buffer, 1024, infile);
  // if (strcmp(buffer, 'P3') != 0) {
  //   printf("Incorrect file type. Need to be in ASCII");
  //   return NULL;
  // }

  // read in comment or skip any other kinds of blank lines
  while (buffer[0] == '#' || buffer[0] == ' ' || 
    buffer[0] == '\n' || buffer[0] == '\r') {
    fgets(buffer, 1024, infile);
  }

  // read in width and height
  fgets(buffer, 1024, infile);
  sscanf(buffer, "%d %d", w, h);
  
  // assume the maximum color value will always be correct
  fgets(buffer, 1024, infile);

  // allocate memory for 2D array of arrays
  pixel = malloc(sizeof(struct ppm_pixel*) * *h);
  if (pixel == NULL) {
    printf("allocate new memory failed. Exit.\n");
    return NULL;
  }

  for (int i = 0; i < *h; i++) {
    pixel[i] = malloc(sizeof(struct ppm_pixel) * *w);
    if (pixel[i] == NULL) {
      printf("allocate new memory failed. Exit.\n");
      return NULL;
    }
  }

  // fill the 2D array
  for (int i = 0; i < *h; i++) {
    for (int j = 0; j < *w; j++) {
      fscanf(infile, " %hhu %hhu %hhu", &r, &g, &b);
      pixel[i][j].red = r;
      pixel[i][j].green = g;
      pixel[i][j].blue = b;
    }
  } 

  fclose(infile);
  return pixel;
}
