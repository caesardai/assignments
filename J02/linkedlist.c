#include <stdio.h>
#include <string.h>

struct cake {
  float cost;
  char flavor[16];
  struct cake* next;
};

struct cake makeCake(const char* flavor, float cost) {
  struct cake newCake;
  strncpy(newCake.flavor, flavor, 16);
  newCake.cost = cost;
  newCake.next = NULL;
  return newCake;
}

// todo: implement cheapestCake
void cheapestCake(struct cake* head) {
  struct cake* next = head;
  struct cake* cheapest = NULL;
  float curr_min = next->cost; // set the price of current cake as min
  while (next != NULL) { // while not NULL continue to compare
    // printf("cake: %s price: %.2f\n", next->flavor, next->cost);
    if (next->cost < curr_min) {
      curr_min = next->cost;
      cheapest = next;
    }
    next = next->next;
  }
  printf("The cheapest cake is %s, which cost $%.2f.\n", cheapest->flavor, cheapest->cost);
}

int main() {
  struct cake cake1 = makeCake("red velvet", 2.00);
  struct cake cake2 = makeCake("chocolate", 1.75);
  struct cake cake3 = makeCake("mocha", 3.50);
  struct cake cake4 = makeCake("vanilla", 5.00);

  // todo: define 4th cake

  cake1.next = &cake2;
  cake2.next = &cake3;
  cake3.next = &cake4;
  // draw stack and heap here

  cheapestCake(&cake1);

  // todo: call cheapestCake
}
