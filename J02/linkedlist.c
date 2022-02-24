#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct cake {
  float cost;
  char flavor[16];
  struct cake* next;
};

struct cake *cheapestCake(struct cake *cakes, const char *flavor, float cost) {
    struct cake *new = malloc(sizeof(struct cake));
    new = cakes;
    
    if (new == NULL) {
        printf("memory allocation failed. Exiting");
        exit(1);
    }
    // struct cake* least = new;
    // while (new != NULL && new->next != NULL) {
    //     if (new->cost < least->cost) {
    //       least = new;
    //     }
    //     new = new->next;
    // }
    // cakes = new;
    // return cakes;
  if (cakes != NULL) {
    while (new != NULL) {
      if (new->cost > cakes->next->cost) {
        new = cakes->next;  
      }
       break;
    }
  }
  return cakes;
}
/*    struct cake *prev = NULL;
    struct cake *curr = cakes;

    if (cakes == NULL) {
        return new;
    }

    // keep on moving while ranked lower
    while (curr != NULL) {
      if (new->cost < curr->cost) {
          prev = curr;
          curr = curr->next;
      }
      else {
          break;
      }
    }

    new->next = curr;
    if (prev == NULL){
      return new;
    }

    prev->next = new;*/
   


struct cake makeCake(const char* flavor, float cost) {
  struct cake new;
  strncpy(new.flavor, flavor, 16);
  new.cost = cost;
  new.next = NULL;
  return new;
}

// todo: implement cheapestCake

int main() {
  float cost;
  char flavor[16];
  

  struct cake cake1 = makeCake("red velvet", 2.00);
  struct cake cake2 = makeCake("chocolate", 1.75);
  struct cake cake3 = makeCake("mocha", 3.50);
  struct cake cake4 = makeCake("vanilla", 1.00);
  
  // todo: define 4th cake

  cake1.next = &cake2;
  cake2.next = &cake3;
  cake3.next = &cake4;
  // draw stack and heap here
  // struct cake* cake_rank = &cake1;
  struct cake* cake_rank = cheapestCake(&cake1, flavor, cost);

  int index = 0;
  while (cake_rank->next != NULL) {
    printf("%d) name: %s   cost: $%.2f\n", index, cake_rank->flavor, cake_rank->cost);
    index++;
    cake_rank = cake_rank->next;
  } 

  // todo: call cheapestCake
  
  
  printf("The cheapest cake is %s", cake_rank->flavor);
  
  
  while (cake_rank != NULL) {
      cake_rank = cake_rank->next;
      free(cake_rank);
      cake_rank = NULL;
  }
  return 0;
  
}
