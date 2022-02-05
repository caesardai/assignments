#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct snack {
  char name[32];
  float cost;
  int quantity;
};

int main()
{
  char name[32]; // array is always a pointer
  float cost;
  int quantity;
  int count;

  printf("Enter a number of snacks: \n");
  scanf("%d", &count);

  struct snack *dynamic_menu = malloc(sizeof(struct snack) * count);
  // declaring a pointer to the heap asking for amount of memory: struct snack(32 + 4 + 4) * count(4)
  // ^ *dynamic_menu is a pointer to the heap
  if (dynamic_menu == NULL)
  {
    printf("malloc failed!");
    exit(1);
  }
  else
  {
    for (int i = 0; i < count; i++)
    {
      struct snack input_item; 
      printf("Enter a name: \n");
      scanf("%s", name); // already a pointer so don't need to tell which address to store
      printf("Enter a cost: \n");
      scanf("%g", &cost); // not a pointer so need to know the address
      printf("Enter a quantity: \n");
      scanf("%d", &quantity); // not a pointer so need to know the address

      strcpy(input_item.name, name);
      input_item.cost = cost;
      input_item.quantity = quantity;
      dynamic_menu[i] = input_item;
    }
  }

  printf("PLEASE put in more snack for Haverford COOP!\n");
  for (int i = 0; i < count; i++)
  {
    printf("%d) %s   cost: $%.2f   quantity: %d\n", i, dynamic_menu[i].name,
           dynamic_menu[i].cost, dynamic_menu[i].quantity);
  }

  free(dynamic_menu);
  return 0;
}
