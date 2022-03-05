#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

// void encode(struct ppm_pixel** graph_matrix, const char* msg, 
//   int max_char, int w, int h, char* input_name);

int main(int argc, char** argv) {
  if (argc != 2 || argv[1] == NULL) {                                                               
    printf("Invalid runtime arguments. Check number of inputs.");                                         
    exit(1);                                                                     
  }  

  int w;
  int h;
  int product;
  int max_char; // char msg[1024];
  unsigned int mask = 0x01; // 0b0000000000000001
  char* input_name = argv[1];
  char* output_name = malloc(strlen(argv[1]) + 8);
  int name_len = strlen(input_name) - 4;

  struct ppm_pixel** graph_matrix = NULL;
  graph_matrix = read_ppm(argv[1], &w, &h); // call read_ppm                                         
  if (graph_matrix == NULL) {                                                    
    printf("Read encoded ppm file failed\n");                                    
    exit(1);                                                                     
  }
  
  strncpy(output_name, input_name, name_len); // add postfix for output filename
  output_name[name_len] = '\0';
  strcat(output_name, "-encoded.ppm");             

  max_char = (w * h * 3) / 8; // max number of characters that can be encoded 

  printf("Reading %s with %d and height %d\n", input_name, w, h);                                                                                          
                                                                                                                                 
  char* secret_msg = malloc(sizeof(char) * max_char);                                                  
  if (secret_msg == NULL) {                                                             
    printf("memory allocation failed. Exiting");                                 
    exit(1);                                                                     
  }    
  printf("Max number of characters that can be encoded in the image wihtout the terminating character: %d\n", max_char - 1);
  printf("writing %s\n", output_name);


  printf("Enter a phrase: ");
  fgets(secret_msg, max_char, stdin);
  int secret_msg_len = strlen(secret_msg);

  char* secret_msg_bi = malloc(sizeof(char) * (w * h * 3 + 1));
  if ( secret_msg_bi == NULL) {
    printf("memory allocation failed. Exiting");                                 
    exit(1);
  }

  int bi_index = 0;
  for (int i = 0; i < secret_msg_len; i++) {
    for (int j = 7; j >= 0; j--) {
      product = mask & (1 << secret_msg[i]);
      if (product == 1) { 
        secret_msg_bi[bi_index] = '1'; 
      }
      else { 
        secret_msg_bi[bi_index] = '0';  
      }
      bi_index++;
    }
  }
  secret_msg_bi[bi_index] = '\0'; // add terminating character at the end
  printf("secretmsg in binary: %s", secret_msg_bi);
  int num = 0;
  while (bi_index > 0) {
    for (int j = 0; j < h ; j++) {                                                 
      for (int k = 0; k < w; k++) {
        for (int l = 0; l < 3; l++) {
          if (secret_msg_bi[num] == '1') { // encode '1'
            if (((mask & graph_matrix[j][k].colors[l]) == '0') && 
              graph_matrix[j][k].colors[l] < 255) {
              graph_matrix[j][k].colors[l] += 1;
            }
            else {
              graph_matrix[j][k].colors[l] -= 1;
            }
          }
          else { // encode '0'
            if ((mask & graph_matrix[j][k].colors[l]) == '1') {
              graph_matrix[j][k].colors[l] -= 1;
            }
            else {
              continue;
            }
          } 
        }
        bi_index--;
        num++;  
      }
    }  
  }

  write_ppm(output_name, graph_matrix, w, h);

  // free allocated memory
  for (int i = 0; i < h; i++) {
    free(graph_matrix[i]);
    graph_matrix[i] = NULL;
  }
  free(graph_matrix);
  graph_matrix = NULL;
  free(output_name);
  output_name = NULL;
  free(secret_msg);
  secret_msg = NULL;
  free(secret_msg_bi);
  secret_msg_bi = NULL;
  return 0;
}
