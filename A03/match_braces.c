//---------------------------------------------------------------------
// match_braces.c 
// CS223 - Spring 2022
// Identify matched braces from a given file
// Name: Caesar
//
#include <stdio.h>
#include <stdlib.h>

struct node {
  char sym;
  int linenum;
  int colnum;
  struct node* next;
};

// Push a new node to a stack (implemented as a linked list). 
// The new node should store the given symbol, line number, and column number
// Param sym: a character symbol, '{' for this program
// Param line: the line number of the symbol
// Param line: the column number of the symbol
// Param top: the top node of the stack (NULL if empty)
// Returns the new top of the stack
struct node* push(char sym, int line, int col, struct node* top) {
  struct node* new_item = malloc(sizeof(struct node));
  new_item->sym = sym;
  new_item->linenum = line;
  new_item->colnum = col;
  new_item->next = NULL;

  if (top == NULL) {
    return new_item;
  }
  else {
    new_item->next = top;
    top = new_item;
    return top;
  }
}

// Pop the top node from a stack (implemented as a linked list) and frees it
// Param top: the top node of the current stack (NULL if empty)
// Returns the new top of the stack
struct node* pop(struct node* top) {
  if (top == NULL) {
    return NULL;
  }
  else {
    struct node* next_item = top->next;
    top = NULL;
    free(top);
    return next_item;
  }
}

// Delete (e.g. free) all nodes in the given stack
// Param top: the top node of the stack (NULL if empty)
void clear(struct node* top) {
  while (top != NULL) {
    struct node* curr = top;
    top = top->next;
    free(curr);
    curr = NULL;
  }
}

// Print all nodes in the given stack (from top to bottom)
// Param top: the top node of the stack (NULL if empty)
void print(struct node* top) {
  printf("All nodes in stack: ");
  while (top != NULL) {
    printf("Symbol: %c, (%d, %d)", top->sym, top->linenum, top->colnum);
    top = top->next;
  }
}

int main(int argc, char* argv[]) { // argc can only have two arguments
  if (argc != 2) {
    printf("Need two arguments: program_name input_file.txt\n"); // usage
    exit(1);
  }

  FILE* infile = NULL;
  infile = fopen(argv[1], "r");
  if (infile == NULL) { // error message if can't open file
    printf("Cannot open file. Exiting.\n");
    exit(1);
  }
  
  int row_num = 1;
  int col_num = 1;
  char character;
  struct node* top = NULL;

   
  while (character != EOF) {
    character = fgetc(infile);
    if (character == '\n') {
      row_num++;
      col_num = 1;
    }
    else {
      col_num++;
      if (character == '{') {
        top = push(character, row_num, col_num, top);
      }
      else if (character == '}') {
        if (top == NULL) {
          printf("Unmatched brace on Line %d and Column %d\n", row_num, col_num);
        }
        else {
          printf("Found matching braces: (%d, %d) -> (%d, %d)\n", 
            top->linenum, top->colnum, row_num, col_num);
            top = pop(top);
        }
      } 
    }
  }

  while (top != NULL) {
    printf("Unmatched brace on Line %d and Column %d\n", top->linenum, top->colnum);
    top = pop(top);
  }

  clear(top);
  fclose(infile);
  return 0;
}
