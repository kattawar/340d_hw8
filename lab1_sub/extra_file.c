#include <stdio.h>
#include <stdlib.h>


// comment here
int num_rows_valid(int rules[1024][256]){
  int num_rows = 0;

  for(int i = 0; i < 256; i++){
    int found_value = 0;
    for(int j = 0; j < 256; j++){
      if(rules[i][j] == 1){
        found_value = 1;
      }
    }

    if(found_value == 1){
      num_rows++;
    }
  }

  return num_rows;
}
