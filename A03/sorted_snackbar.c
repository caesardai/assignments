//---------------------------------------------------------------------
// sorted_snackbar.c
// CS223 - Spring 2022
// Ask the user for a list of snacks and store them in alphabetical order
// Name: Caesar Dai
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct snack
{
    char name[32];
    int quantity;
    float cost;
    struct snack* next;
};

// Insert a new node to a list (implemented as a linked list).
// The new node should store the given properties
// Param snacks: the first item in the list (NULL if empty)
// Param name: the snack name (max length is 32 characters)
// Param quantity: the snack quantity
// Param cost: the snack cost
// Returns the first item in the list
struct snack *insert_sorted(struct snack *snacks, 
    const char *name, float cost, int quantity);

// Delete (e.g. free) all nodes in the given list of snacks
// Param snacks: the first node in the list (NULL if empty)
void clear(struct snack *snacks);

int main() {
    int num_of_snack;
    int index = 1;
    char name[32];
    int quantity;
    float cost;
    struct snack *snack_menu = NULL;

    printf("Enter a number of snakcs: ");
    scanf(" %d", &num_of_snack);

    for (int i = 0; i < num_of_snack; i++) {
        printf("Enter a name: ");
        scanf(" %s", name);
        printf("Enter a cost: ");
        scanf(" %f", &cost);
        printf("Enter a quantity: ");
        scanf(" %d", &quantity);

        snack_menu = insert_sorted(snack_menu, name, cost, quantity);
    }

    printf("Welcome to Caesar's Sorted Snack Bar!\n");
    struct snack *item = snack_menu;
    while (snack_menu != NULL) {
        printf("%d) %s      cost: $%.2f   quantity: %d\n", index, 
            snack_menu->name, snack_menu->cost, snack_menu->quantity);
        index++;
        snack_menu = snack_menu->next;
    }
    clear(item);
    return 0;
}

struct snack *insert_sorted(struct snack *snacks,
                            const char *name, float cost, int quantity) {
    struct snack *new = malloc(sizeof(struct snack));
    if (new == NULL) {
        printf("memory allocation failed. Exiting");
        exit(1);
    }

    // construct entered object: snack
    strcpy(new->name, name);
    new->cost = cost;
    new->quantity = quantity;
    new->next = NULL;

    struct snack *prev = NULL;
    struct snack *curr = snacks;

    if (snacks == NULL) {
        return new;
    }
    int name_comp = strcmp(new->name, curr->name);
    // printf("strncmp number: %d", name_comp); // testing
    // keep on moving while ranked lower
    while (curr != NULL) {
      if (name_comp > 0) {
          prev = curr;
          curr = curr->next;
      }else {
          break;
      }
    }
    new->next = curr;
    if (prev == NULL){
      return new;
    }

    prev->next = new;

    return snacks;
}

void clear(struct snack* snacks) {
  struct snack* curr = snacks;
  struct snack* next;
  while (curr != NULL) {
    next = curr->next;
    free(curr);
    curr = next;
  }
}
//void clear(struct snack* snacks) {                                               
//  struct snack* curr = snacks;                                                   
//  while (curr != NULL) {                                                         
//    struct snack* next = curr->next;                                             
//    free(curr);                                                                  
//    curr = next;                                                                 
//  }                                                                              
//  snacks = NULL;                                                                 
//}       

// sample code:
// // insert at beginning
// if (head == NULL || strcmp(new->name, curr->name) < 0) {
//     new->next = head;
//     head = new;
// }
// // insert in the middle
// else {
//     while (curr != NULL && strcmp(new->name, curr->name) > 0) {
//         curr = curr->next;
//     }
//     new->next = curr->next;
//     curr->next = new;
// }
// return head;
