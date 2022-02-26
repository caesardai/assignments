#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

  fread(pixel, sizeof(struct ppm_pixel), *w * *h, infile);
  // close file to restore memory
  fclose(infile);
  return pixel;
}

// TODO: Implement this function
// Feel free to change the function signature if you prefer to implement an 
// array of arrays
extern void write_ppm(const char* filename, struct ppm_pixel** pxs, int w, int h) {
  FILE *outfile;
  outfile = fopen(filename, "wb");
  if (outfile == NULL || !outfile) { // error message if can't open file
    printf("Fail to open file.\n");
    exit(1);
  }
  
  srand(time(NULL));
  
  //print statement
  fprintf(outfile, "P6\n Comments\n %d %d\n %d\n", w, h, 255);
  struct ppm_pixel curr_pixel;
  
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      curr_pixel = pxs[i][j];
      curr_pixel.red = curr_pixel.red << (rand() % 5); 
      curr_pixel.green = curr_pixel.green << (rand() % 3); 
      curr_pixel.blue = curr_pixel.blue << (rand() % 7); 
      fwrite(curr_pixel.colors, 3, 1, outfile);
    }
  }

  fclose(outfile);
}
