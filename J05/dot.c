#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 100000

struct thread_data {
  int start_index;
  int end_index;
  int* varray;
  int* uarray;
  int* dotprod;
};

void *mularr(void *args) {
  struct thread_data *data = (struct thread_data *) args;
  for (int i = 0; i < SIZE / 4; i++) {
    // data->varray[i] = rand() % 1000 - 500;
    // data->uarray[i] = rand() % 1000 - 500;
    data->dotprod += data->uarray[i] * data->varray[i];
  }
  return (void*) NULL;
}

int main(int argc, char *argv[]) {
  srand(time(0));

  int v[SIZE];
  int u[SIZE];
  int dotproduct = 0;
  int *result[4];

  for (int i = 0; i < SIZE / 4; i++) {
    v[i] = rand() % 1000 - 500;
    u[i] = rand() % 1000 - 500;
    dotproduct += u[i] * v[i];
  }

  printf("Ground truth dot product: %d\n", dotproduct);

  // TODO: Implement your thread solution here
  printf("Test with 4 threads\n");
  pthread_mutex_t mutex;
  pthread_mutex_init(&mutex, NULL);
  pthread_t tid[4];

  struct thread_data ids[4]; // 4 threads
  for (int i = 0; i < 4; i++) {
    ids[i].start_index = i * (SIZE / 4);
    ids[i].start_index = i * (SIZE / 4) + (SIZE / 4);
    ids[i].varray = v;
    ids[i].uarray = u;
    ids[i].dotprod = &dotproduct;
    pthread_create(&tid[i], NULL, mularr, &ids[i]);
  }
  for (int i = 0; i < 4; i++) {
    pthread_join(tid[i], NULL);
  }
  printf("Answer with threads: %d\n", dotproduct);

  return 0;
}


