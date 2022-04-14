#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include "read_ppm.h"

int main(int argc, char* argv[]) {
  int size = 2000;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  //int basered, basegreen, baseblue;
  struct timeval tstart, tend;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your work here
  // generate pallet
  struct ppm_pixel* palette = malloc(sizeof(struct ppm_pixel) * maxIterations);
  if (palette == NULL) {
    printf("Memory allocation failed. Exiting.\n");
    exit(1);
  }

  srand(time(0));
  for (int i = 0; i < maxIterations; i++) {
    palette[i].red = rand() % 255;
    palette[i].green = rand() % 255;
    palette[i].blue = rand() % 255;
  }

  // IMAGE 2D array 
  struct ppm_pixel** graph_matrix = malloc(sizeof(struct ppm_pixel*) * size);
  if (graph_matrix == NULL) {
    printf("Memory allocation failed. Exiting.\n");
    exit(1);
  }
  for (int i = 0; i < size; i++) {
    graph_matrix[i] = malloc(sizeof(struct ppm_pixel) * size);
    if (graph_matrix[i] == NULL) {
      printf("Memory allocation failed. Exiting.\n");
      exit(1);
    }
  }

  // compute image
  //float res;
  int iter;
  double timer;
  float xfrac, yfrac, x0, y0, x, y, xtmp;

  gettimeofday(&tstart, NULL);
  for (int row = 0; row < size; row++) {
    for (int col = 0; col < size; col++) {
      xfrac = (float)row /size;
      yfrac = (float)col /size;
      x0 = xmin + xfrac * (xmax - xmin);
      y0 = ymin + yfrac * (ymax - ymin);

      x = 0;
      y = 0;
      iter = 0;
      while (iter < maxIterations && x*x + y*y < 2*2) {
        xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        iter++;
      }
      if (iter < maxIterations) { //escaped
        graph_matrix[col][row].red = palette[iter].red;
        graph_matrix[col][row].green = palette[iter].green;
        graph_matrix[col][row].blue = palette[iter].blue;
      } 
      else { // else all black
        graph_matrix[col][row].red = 0;
        graph_matrix[col][row].green = 0;
        graph_matrix[col][row].blue = 0; 
      }
    }
  }
  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Computed mandelbrot set (%dx%d) in %.6f seconds\n", size, size, timer);

  char output_name[128];
  sprintf(output_name, "mandelbrot-%d-%.10ld.ppm", size, time(0));
  int name_len = strlen(output_name);
  output_name[name_len] = '\0';

  // write file
  write_ppm(output_name, graph_matrix, size, size);


  // free palette
  free(palette);
  palette = NULL;
  // free image
  for (int i = 0; i < size; i++) {
    free(graph_matrix[i]);
    graph_matrix[i] = NULL;
  }
  free(graph_matrix);
  graph_matrix = NULL;
  
  return 0;
}
