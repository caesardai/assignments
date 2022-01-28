#include <stdio.h>
#include <string.h>

// define a new type: struct snackS
	
struct snackS {
	int id;
	char name[64];
	float cost;
	int quantity;
};

// define function used in main 
void checkPayment(struct snackS snack_menu[], float* money, int* item_number);


int main() {
	float money;
  int item_number;
  struct snackS snack_menu[3];
	struct snackS snack0, snack1, snack2;

	printf("Welcome to my Snack Bar.\nHow much money do you have? ");
	scanf("%g", &money);

  // snack1 information
  snack_menu[0].id = 0;
	strcpy(snack_menu[0].name, "Coco Puffs");
	snack_menu[0].cost = 1.50;
	snack_menu[0].quantity = 4;	
	
	// snack2 information
  snack_menu[1].id = 1;                                                          
  strcpy(snack_menu[1].name, "Manchego cheese");                                      
  snack_menu[1].cost = 15.50;                                                     
  snack_menu[1].quantity = 6;

	// snack3 information
  snack_menu[2].id = 2;                                                          
  strcpy(snack_menu[2].name, "Magic bean");                                      
  snack_menu[2].cost = 6;                                                     
  snack_menu[2].quantity = 0;
    
	// print snack menu to customer
	printf("%d) %s   cost: $%g   quantity: %d\n", 
			snack_menu[0].id, snack_menu[0].name, snack_menu[0].cost, snack_menu[0].quantity);
	printf("%d) %s   cost: $%g   quantity: %d\n", 
      snack_menu[1].id, snack_menu[1].name, snack_menu[1].cost, snack_menu[1].quantity);
	printf("%d) %s   cost: $%g   quantity: %d\n",
      snack_menu[2].id, snack_menu[2].name, snack_menu[2].cost, snack_menu[2].quantity);	

  // question for what snack is the person purchasing
	printf("What snack would you like to buy? [0, 1, 2]");
	scanf("%d", &item_number);

  checkPayment(snack_menu, &money, &item_number);
	return 0;
}


// action after payment amount is received
void checkPayment(struct snackS snack_menu[], float* money, int* item_number) {   
  float budget = *money;                                                         
  int target = *item_number;                                                     
  float change;                                                                  
  
  // if we don't have enough money to purchase the snack                                                            
  if (budget < snack_menu[target].cost) {                                        
    printf("You can't afford it!\n");                                              
  }       

  // if there isn't any desired snack left                                                                       
  else if (snack_menu[target].quantity == 0) {                                   
    printf("Sorry we are out of %s\n", snack_menu[target].name);                   
  }                                       

  // amount of money left after purchasing the desired snack                                       
  else {                                                                         
    snack_menu[target].quantity--;                                               
    change = budget - snack_menu[target].cost;                                   
    printf("You bought %s\nYou have $%g left\n", snack_menu[target].name, change); 
  }                                                                                                                  
}         











