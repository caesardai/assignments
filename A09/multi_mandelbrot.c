#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <string.h>
#include "read_ppm.h"

void compute_image(struct ppm_pixel* palette, struct ppm_pixel** graph_matrix, int size, float xmin, 
  float xmax, float ymin, float ymax, int maxIterations, int row_s, int row_t,int col_s, int col_t) {

  int iter;
  float xfrac, yfrac, x0, y0, x, y, xtmp;

  for (int row = row_s; row < row_t; row++) {
    for (int col = col_s; col < col_t; col++) {
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
}

int main(int argc, char* argv[]) {
  int size = 400;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  double timer;
  int maxIterations = 1000;
  int numProcesses = 4;
  struct timeval tstart, tend;
  int shmid_outer;
  int *shmid_inner;
  struct ppm_pixel** graph_matrix = NULL;
  struct ppm_pixel* palette = NULL;
  
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
  // shared image memory
    // OUTER Layer
  shmid_outer = shmget(IPC_PRIVATE, sizeof(struct ppm_pixel*) * size, 0644 | IPC_CREAT);
  if (shmid_outer == -1) {
    perror("Can't initialize outer shared1 memory. Exiting.\n");
    exit(1);
  }
  graph_matrix = shmat(shmid_outer, NULL, 0);
  if (graph_matrix == (void*) - 1) {
    perror("Can't access outer shared2 memory. Exiting.\n");
    exit(1);
  }
    // INNER Layer
  shmid_inner = malloc(sizeof(int) * size);
  if (shmid_inner == NULL) {
    printf("Memory allocation failed. Exiting.\n");
    exit(1);
  }
  for (int i = 0; i < size; i++) {
    shmid_inner[i] = shmget(IPC_PRIVATE, sizeof(struct ppm_pixel) * size, 0644 | IPC_CREAT);
    if (shmid_inner[i] == -1) {
      perror("Can't initialize shared inner1 memory. Exiting.\n");
      exit(1);
    }
    graph_matrix[i] = shmat(shmid_inner[i], NULL, 0);
    if (graph_matrix[i] == (void*) - 1) {
      perror("Can't access shared inner2 memory. Exiting.\n");
      exit(1);
    }
  }

  // generate palette
  palette = malloc(sizeof(struct ppm_pixel) * maxIterations);
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

  // compute image with 4 child processes
  gettimeofday(&tstart, NULL);
  for (int i = 0; i < numProcesses; i++) {
    int pid = fork(); // creating new child process
    int row_s, row_t, col_s, col_t;
    int half = size / 2;

    if (i == 0) {
      row_s = 0;
      row_t = half;
      col_s = 0;
      col_t = half;
    } else if (i == 1) {
      row_s = 0;
      row_t = half;
      col_s = half;
      col_t = size;
    } else if (i == 2) {
      row_s = half;
      row_t = size;
      col_s = 0;
      col_t = half;
    } else if (i == 3) {
      row_s = half;
      row_t = size;
      col_s = half;
      col_t = size;
    }
    if (pid == 0) {
      compute_image(palette, graph_matrix, half, xmin, xmax, ymin, ymax, maxIterations, 
         row_s, row_t, col_s, col_t);
      exit(0);
    }
    else {
      printf("Launched child process: %d\n", pid);
      printf("%d) Sub-image block: cols (%d, %d) to rows (%d,%d)\n", pid, col_s, col_t, row_s, row_t);
    }
  }

  // wait
  for (int i = 0; i < numProcesses; i++) {
    int status;
    int pid = wait(&status);
    printf("Child process complete: %d\n", pid);
  }
    
  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Computed mandelbrot set (%dx%d) in %.6f seconds\n", size, size, timer);

  char output_name[128];
  sprintf(output_name, "multi-mandelbrot-%d-%.10ld.ppm", size, time(0));
  int name_len = strlen(output_name);
  output_name[name_len] = '\0';

  // write file
  write_ppm(output_name, graph_matrix, size, size);

  // detach shared memory
    // inner
  for (int i = 0; i < size; i++) {
    if (shmdt(graph_matrix[i]) == -1) {
      perror("Failed to detach from inner shared2 memory. Exiting.\n");
      exit(1);
    }
    if (shmctl(shmid_inner[i], IPC_RMID, 0) == -1){
      perror("Failed to delete inner shared1 memory. Exiting.\n");
      exit(1);
    }
  }
    // outer
  if (shmdt(graph_matrix) == -1) {
    perror("Failed to detach from outer shared2 memory. Exiting.\n");
    exit(1);
  }
  if (shmctl(shmid_outer, IPC_RMID, 0) == -1) {
    perror("Failed to detach outer shared1 memory. Exiting.\n");
    exit(1);
  }
  free(shmid_inner);
  shmid_inner = NULL;
  free(palette);
  palette = NULL;

  return 0;
}
