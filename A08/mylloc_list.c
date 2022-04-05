#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct chunk {
  int size;
  int mem_in_use;
  struct chunk *next;
};
struct chunk *flist = NULL;

void *malloc (size_t size) {
  if (size == 0){
    return NULL;
  }
  struct chunk *next = flist;
  struct chunk *prev = NULL;

  while (next != NULL) {
    if (next->size >= size) {
      if (prev != NULL) {
        prev->next = next->next;
      }
      else {
        flist = next->next;
      }
      next->mem_in_use = size;
      return (void*)(next + 1);
    }
    else {
      prev = next;
      next = next->next;
    }
  }

  void *memory = sbrk(size + sizeof(struct chunk));
  if (memory == (void *) -1) {
    return NULL;
  } else {
    struct chunk* cnk = (struct chunk*) memory;
    cnk->size = size;
    cnk->mem_in_use = size;
    return (void*) (cnk + 1);
  }
}

void free(void *memory) {
  if (memory != NULL) {
    struct chunk *cnk = (struct chunk*)((struct chunk*)memory - 1);
    cnk->next = flist;
    flist = cnk;
  }
  return;
}

void fragstats(void* buffers[], int len) {
  int total_free_cnk = 0;
  int total_inUse_cnk = 0;
  int total_unused_mem = 0;
  int l_unused_cnk = 0;
  float ave_unused_cnk;
  int s_unused_cnk = 99999999;
  int l_free_cnk = 0;
  float ave_free_cnk;
  int s_free_cnk = 99999999;

  for (int i = 0; i < len; i++) { // count used block
    if (buffers[i] != NULL) {
      struct chunk *cnk = (struct chunk*)((struct chunk*)buffers[i] - 1);
      total_inUse_cnk++;
      if (cnk->mem_in_use < s_unused_cnk) {
        s_unused_cnk = cnk->mem_in_use;
      }
      else if (cnk->mem_in_use > s_unused_cnk) {
        l_unused_cnk = cnk->mem_in_use;
      }
      total_unused_mem += cnk->mem_in_use;
    } 
  }
  ave_unused_cnk = total_unused_mem / len;

  struct chunk *current = flist;
  while (current != NULL) { // count freed blocks
    total_free_cnk++;
    if (current->size < s_free_cnk) {
      s_free_cnk = current->size;
    }
    else if (current->size > l_free_cnk) {
      l_free_cnk = current->size;
    }
    total_inUse_cnk += current->size;
    current = current->next;
  }
  ave_free_cnk = total_inUse_cnk / total_free_cnk;

  
  int total_block = total_free_cnk + total_inUse_cnk;
  printf("Total blocks: %d, Free: %d, Used: %d\n", total_block, total_free_cnk, total_inUse_cnk);
  printf("Internal unused: total: %d, average: %.2f, smallest: %d, largest: %d\n", total_unused_mem, ave_unused_cnk, s_unused_cnk, l_unused_cnk);
  printf("External unused: total: %d, average: %.2f, smallest: %d, largest: %d\n", total_free_cnk, ave_free_cnk, s_free_cnk, l_free_cnk);
}
