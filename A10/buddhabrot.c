#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include "read_ppm.h"

pthread_mutex_t mutex;
pthread_barrier_t barrier;

// step 1
void *determine_membership(void* args);
// step 2
void *compute_visited_counts(void* args);
// step 3
void *compute_colors(void* args); 
// start routine
void * thread_function(void * args);

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
  int* max_count;
  int** membership_log;
  int** v_count;
};

int main(int argc, char* argv[]) {
  double timer;
  int size = 400;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int max_count = 0;
  int numProcesses = 4;
  int maxIterations = 1000;
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
  printf("Generating buddhabrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your code here
  // initialize barrier and mutex
  pthread_barrier_init(&barrier, NULL, 4);
  pthread_mutex_init(&mutex, NULL);

  // allocate 2D array to store image
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

  // allocate 2D array to keep track of points in mandelbrot set
  int** membership_log = malloc(sizeof(int*) * size);
  if (graph_matrix == NULL) {
    printf("Memory allocation failed. Exiting.\n");
    exit(1);
  }
  for (int i = 0; i < size; i++) {
    membership_log[i] = malloc(sizeof(int) * size);
    if (membership_log[i] == NULL) {
    printf("Memory allocation failed. Exiting.\n");
    exit(1);
    }
  }

  // allocate 2D array to keep track of visited counts
  int** v_count = malloc(sizeof(int*) * size);
  if (v_count == NULL) {
    printf("Memory allocation failed. Exiting.\n");
    exit(1);
  }
  for (int i = 0; i < size; i++) {
    v_count[i] = malloc(sizeof(int) * size);
    if (v_count[i] == NULL) {
    printf("Memory allocation failed. Exiting.\n");
    exit(1);
    }
  }
  // initialize all to zero
  pthread_t tid[4];
  struct thread_data data[4];

  for (int r = 0; r < size; r++) {
    for (int c = 0; c < size; c++) {
      v_count[r][c] = 0;
    }
  }

  // compute image
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
    data[i].id = i;
    data[i].size = size;
    data[i].row_s = row_s;
    data[i].row_t = row_t;
    data[i].col_s = col_s;
    data[i].col_t = col_t;
    data[i].xmin = xmin;
    data[i].xmax = xmax;
    data[i].ymin = ymin;
    data[i].ymax = ymax;
    data[i].max_count = &max_count;
    data[i].maxIterations = maxIterations;
    data[i].graph_matrix = graph_matrix;
    data[i].membership_log = membership_log;
    data[i].v_count = v_count;

    pthread_create(&tid[i], NULL, thread_function, (void*) &data[i]);
    
  }

  for (int i = 0; i < numProcesses; i++) {
    pthread_join(tid[i], NULL);
  }
  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Computed buddhabrot set (%dx%d) in %g seconds\n", size, size, timer);

  char output_name[128];
  sprintf(output_name, "buddhbrot-%d-%.10ld.ppm", size, time(0));
  int name_len = strlen(output_name);
  output_name[name_len] = '\0';

  // write file
  write_ppm(output_name, graph_matrix, size, size);

  // free memory
  for (int i = 0; i < size; i++) {
    free(graph_matrix[i]);
    graph_matrix[i] = NULL;
    free(membership_log[i]);
    membership_log[i] = NULL;
    free(v_count[i]);
    v_count[i] = NULL;
  }
  free(graph_matrix);
  graph_matrix = NULL;
  free(membership_log);
  membership_log = NULL;
  free(v_count);
  v_count = NULL;

  // delete mutex and barrier
  pthread_mutex_destroy(&mutex);
  pthread_barrier_destroy(&barrier);
  return 0;
}


     /////////////////
///////  FUNCTIONS  ///////
    /////////////////

// step 1
void *determine_membership(void* args) {
  int iter;
  float xfrac, yfrac, x0, y0, x, y, xtmp;
  struct thread_data* data = (struct thread_data *) args;

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

      if (iter < data->maxIterations) {
        data->membership_log[r][c] = 0;
      } else {
        data->membership_log[r][c] = 1;
      }
    }
  }
  return (void *) NULL;
}

// step 2
void *compute_visited_counts(void* args) {
  int yrow, xcol;
  float xfrac, yfrac, x0, y0, x, y, xtmp;
  struct thread_data* data = (struct thread_data *) args;

  for (int r = data->row_s; r < data->row_t; r++) {
    for (int c = data->col_s; c < data->col_t; c++) {
      if (data->membership_log[r][c] == 1) {
        continue;
      } else {
        xfrac = (float) r /data->size;
        yfrac = (float) c /data->size;
        x0 = data->xmin + xfrac * (data->xmax - data->xmin);
        y0 = data->ymin + yfrac * (data->ymax - data->ymin);

        x = 0;
        y = 0;
        while (x*x + y*y < 2*2) {
          xtmp = x*x - y*y + x0;
          y = 2*x*y + y0;
          x = xtmp;

          yrow = round(data->size * (y - data->ymin) / (data->ymax - data->ymin));
          xcol = round(data->size * (x - data->xmin) / (data->xmax - data->xmin));
          if (yrow < 0 || yrow >= data->size) { // out of range
            continue; 
          }
          if (xcol < 0 || xcol >= data->size) { // out of range
            continue; 
          }
          
	        pthread_mutex_lock(&mutex);
          data->v_count[yrow][xcol]++;
          // update max count
          if (data->v_count[yrow][xcol] >  *(data->max_count)) {
             *(data->max_count) = data->v_count[yrow][xcol];
          }
          pthread_mutex_unlock(&mutex);
	      }
      }
    }
  }
  return (void *) NULL;
}

// step 3
void *compute_colors(void* args) {
  float gamma = 0.681;
  float factor = 1.0 / gamma;
  struct thread_data* data = (struct thread_data *) args;

  for (int r = data->row_s; r < data->row_t; r++) {
    for (int c = data->col_s; c < data->col_t; c++) {
      float value = 0;
      if (data->v_count[r][c] > 0) {
	      value = log(data->v_count[r][c]) / log(*(data->max_count));
        value = pow(value, factor);
      }
      data->graph_matrix[r][c].red = value * 255;
      data->graph_matrix[r][c].green = value * 255;
      data->graph_matrix[r][c].blue = value * 255;
    }
  }
  return (void *) NULL;
}

void * thread_function(void * args) {
  struct thread_data* data = (struct thread_data *) args;
  printf("Thread %d) sub-image block: cols (%d, %d) to rows (%d, %d)\n",
      data->id, data->col_s, data->col_t, data->row_s, data->row_t);
  // step 1
  void *determine_membership(void* args);
  // step 2
  void *compute_visited_counts(void* args);
  // step 3
  void *compute_colors(void* args); 
  printf("Thread %d) finished\n", data->id);
  return (void *) NULL;
}
