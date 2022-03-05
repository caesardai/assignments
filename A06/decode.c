#include <stdio.h>                                                               
#include <stdlib.h>                                                              
#include <string.h>                                                              
#include "read_ppm.h"                                                            

int power(int base, int pwr);           

int main(int argc, char** argv) {                                                
  if (argc != 2) {                                                               
    printf("Invalid runtime arguments");                                         
    exit(1);                                                                     
  } 

  if (argv[1] == NULL) {                                                         
    printf("Need an input file!");                                               
    exit(1);                                                                     
  }                                                                              
                                                                                 
  int w, h, product, max_char;                                                                  
  const char* input_filename = argv[1];                                          
  struct ppm_pixel** graph_matrix = NULL;                                        
                                                                                                                      
  graph_matrix = read_ppm(argv[1], &w, &h); // call read_ppm                                         
  if (graph_matrix == NULL) {                                                    
    printf("Read encoded ppm file failed\n");                                    
    exit(1);                                                                     
  }                                                                              
                                                                                 
  max_char = (w * h * 3) / 8; // calculate max encoded character                 
  printf("Reading %s with %d and height %d\n", input_filename, w, h);            
  printf("Max number of characters in the image: %d\n", max_char);                                                                              
                                                                                 
  // extract message                                                             
  char* msg = malloc(max_char);                                                  
  if (msg == NULL) {                                                             
    printf("memory allocation failed. Exiting");                                 
    exit(1);                                                                     
  }                                                                              
                                                                                 
  unsigned int mask = 0x1; // 0b0000000000000001                                 
                                                                                 
  // take out the number while calculating for the sum                                                                                 
  int pwr = 7, sum = 0, index = 0, counter = 0;  

  for (int i = 0; i < h ; i++) {                                                 
    for ( int j = 0; j < w; j++) {                                               
      for (int k = 0 ; k < 3; k++) {                                             
        product = mask & graph_matrix[i][j].colors[k]; // extract the last num for each

        if (product == 1) {  
          // printf("1");                                                                                                                                             
          sum += power(2, pwr);                                              
        }                                                                      
        counter++;
        pwr--;                                                                 

        if (counter == 8) {
          msg[index] = (char) sum;
          index++;                                                                                                                      
          pwr = 7;                                                                   
          sum = 0;                                                                   
          counter = 0;   
        }                                                                                                                                                                                                 
      }                                                                          
    }                                                                                                                                                                                                            
  }                                                                                                                                                  
  printf(" %s", msg);                                                             

  // free allocated memory                                                       
  for (int o = 0; o < h; o++) {                                                  
    free(graph_matrix[o]);                                                       
    graph_matrix[o] = NULL;                                                      
  }                                                                              
  free(graph_matrix);                                                            
  graph_matrix = NULL;                                                           
                                                                                 
  free(msg);                                                                     
  msg = NULL;                                                                    
  return 0;                                                                                                                                                
}                      

int power(int base, int pwr) {
  int val = 1;
  while (pwr > 0) {
    val = val * base;
    pwr--; 
  }
  return val;
}
