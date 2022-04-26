#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>
#include "read_ppm.h"

struct thread_data {
  int id;
  int size;
  float xmin;
  float xmax;
  float ymin;
  float ymax;
  int maxIterations;
  int row_s, row_t, col_s, col_t;
  struct ppm_pixel* palette;
  struct ppm_pixel** graph_matrix;
};

void* compute_image(void* args);

int main(int argc, char* argv[]) {
  int size = 2000;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;
  double timer;
  struct timeval tstart, tend;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> "
        "-b <ymin> -t <ymax> -p <numProcesses>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your code here
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

  // alocate image memory
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
  pthread_t tid[4];
  struct thread_data data[4];

  gettimeofday(&tstart, NULL);
  for (int i = 0; i < numProcesses; i++) {
    int half = size / 2;
    int row_s, row_t, col_s, col_t;

    if (i == 0) { // first quadrant
      row_s = 0;
      row_t = half;
      col_s = 0;
      col_t = half;
    } if (i == 1) { // second quadrant
      row_s = 0;
      row_t = half;
      col_s = half;
      col_t = size;
    } if (i == 2) { // third quadrant
      row_s = half;
      row_t = size;
      col_s = 0;
      col_t = half;
    } if (i == 3) { // forth quadrant
      row_s = half;
      row_t = size;
      col_s = half;
      col_t = size;
    }
    data[i].size = size;
    data[i].row_s = row_s;
    data[i].row_t = row_t;
    data[i].col_s = col_s;
    data[i].col_t = col_t;
    data[i].xmin = xmin;
    data[i].xmax = xmax;
    data[i].ymin = ymin;
    data[i].ymax = ymax;
    data[i].maxIterations = maxIterations;
    data[i].palette = palette;
    data[i].graph_matrix = graph_matrix;
    pthread_create(&tid[i], NULL, compute_image, (void*) &data[i]);
  }

  for (int i = 0; i < numProcesses; i++) {
    pthread_join(tid[i], NULL);
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
  

  // free memory
  free(palette);
  palette = NULL;

  for (int i = 0; i < size; i++) {
    free(graph_matrix[i]);
    graph_matrix[i] = NULL;
  }
  free(graph_matrix);
  graph_matrix = NULL;

  return 0;
}

void *compute_image(void* args) {
  int iter;
  float xfrac, yfrac, x0, y0, x, y, xtmp;

  struct thread_data* data = (struct thread_data *) args;

  printf("Thread %d) sub-image block: cols (%d, %d) to rows (%d, %d)\n",
      data->id, data->col_s, data->col_t, data->row_s, data->row_t);

  for (int r = data->row_s; r < data->row_t; r++) {
    for (int c = data->col_s; c < data->col_t; c++) {
      xfrac = (float) r /data->size;
      yfrac = (float) c /data->size;
      x0 = data->xmin + xfrac * (data->xmax - data->xmin);
      y0 = data->ymin + yfrac * (data->ymax - data->ymin);

      x = 0;
      y = 0;
      iter = 0;
      while (iter < data->maxIterations && x*x + y*y < 2*2) {
        xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        iter++;
      }
      if (iter < data->maxIterations) { //escaped
        data->graph_matrix[c][r].red = data->palette[iter].red;
        data->graph_matrix[c][r].green = data->palette[iter].green;
        data->graph_matrix[c][r].blue = data->palette[iter].blue;
      } 
      else { // else all black
        data->graph_matrix[c][r].red = 0;
        data->graph_matrix[c][r].green = 0;
        data->graph_matrix[c][r].blue = 0; 
      }
    }
  }
  printf("Thread %d) finished\n", data->id);
  return (void *) NULL;
}
