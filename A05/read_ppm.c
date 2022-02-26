#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "read_ppm.h"

// TODO: Implement this function
// Feel free to change the function signature if you prefer to implement an 
// array of arrays
struct ppm_pixel** read_ppm(const char* filename, int* w, int* h) {
  FILE* infile = NULL;
  char buffer[1024]; // allocate 1024 for potential use
  // unsigned char r, g, b;
  struct ppm_pixel** pixel = NULL;
  struct ppm_pixel curr_pixel; 

  infile = fopen(filename, "rb");
  if (infile == NULL || !infile) { // error message if can't open file
    printf("Fail to open file.\n");
    return NULL;
  }

  // ppm file type
  fgets(buffer, 1024, infile);
  fgets(buffer, 1024, infile);
  // read in comment or skip any other kinds of blank lines
  while (buffer[0] == '#' || buffer[0] == ' ' || 
    buffer[0] == '\n' || buffer[0] == '\r') { 
    fgets(buffer, 1024, infile);
  }

  // read in width and height
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

  for (int i = 0; i < *h; i++) {
    for (int j = 0; j < *w; j++) {
      fread(&curr_pixel, 3, 1, infile);
        pixel[i][j] = curr_pixel;
    }
  }

  fread(pixel, sizeof(struct ppm_pixel), (*w * *h), infile);
  // close file to restore memory
  fclose(infile);
  return pixel;
}

// TODO: Implement this function
// Feel free to change the function signature if you prefer to implement an 
// array of arrays
extern void write_ppm(const char* filename, struct ppm_pixel** pxs, int w, int h) {
  FILE* outfile = fopen(filename, "wb");
  if (outfile == NULL || !outfile) { // error message if can't open file
    printf("Fail to open file.\n");
    exit(1);
  }
  
  char dim[64];

  // header
  fwrite("P6\n", 1, 3, outfile);
  sprintf(dim, "%d", w);
  fwrite(dim, 1, strlen(dim), outfile);
  fwrite(" ", 1, 1, outfile);
  sprintf(dim, "%d\n", h);
  fwrite(dim, 1, strlen(dim), outfile);
  fwrite("255\n", 1, 4, outfile);

  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      fwrite(pxs[i][j].colors, 1, 3, outfile);
    }
  }
  fclose(outfile);
}

