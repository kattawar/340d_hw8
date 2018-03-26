#include <stdio.h>
#include <stdlib.h>
//#include <string.h>

// comment here
// int num_rows_valid(int rules[1024][256]){
//   int num_rows = 0;

//   for(int i = 0; i < 256; i++){
//     int found_value = 0;
//     for(int j = 0; j < 256; j++){
//       if(rules[i][j] == 1){
//         found_value = 1;
//       }
//     }

//     if(found_value == 1){
//       num_rows++;
//     }
//   }

//   return num_rows;
// }


int start_matching(int rules[1024][256]){
  //printf("STARTING MATCH WITH MAP\n");
  // for(int i = 0; i < 256; i++){
  //   for(int j = 0; j < 256; j++){
  //     if(rules[i][j] == 1){
  //       printf("1");
  //     }
  //     else if(rules[i][j] == 0){
  //       printf("0");
  //     }
  //   }
  //   printf("\n");
  // }


  char text_buf[1000000];
  int cp_index;
  char g;
  while((g = getchar()) != EOF){
    text_buf[cp_index] = g;
    cp_index++;
  }

  char *text = &text_buf[0];
  int count = 0;
  int num_rows = num_rows_valid(rules);
  char hold_line[10000];
  char* hold_line2 = (char*) malloc(10000 * sizeof(char));
  *hold_line2 = '\0';
  int line_index = 0;
  int line_match = 0;
  int first_hk = 0;

  //printf("FOUND RULES ROWS: %d\n", num_rows);
  //char *restore_text = text;
  int checked_chars = 0;

  while(*text){
    //printf("RESETTING COUNTERS\n");
    //printf("CHECKING %c\n", *text);
    int i = 0;
    int matches = 1;
    char *temp_text = text;


    while(i < num_rows && *temp_text){

      //printf("CHECKING i: %d, CHECKING char: %c\n", i, *temp_text);
      //printf("%d\n", rules[1]['M']);
      //
      //printf("CHECK IS: %d\n", (rules[i][*temp_text]));
      if(rules[i][*temp_text] != 1){
        //printf("NO MATCH, i: %d, char: %c\n", i, *temp_text);
        matches = 0;
      }

      *temp_text++;
      i++;
    }

    if(matches == 1){
      line_match = 1;
      count++;
    }

    if(*text != '\n'){
      hold_line[line_index] = *text;
    }
    else if(*text == '\n'){
      //printf("PR LINE: ");
      if(line_match == 1){
        hold_line[line_index] = '\0';
        //printf("INDEX: %d\n", first_hk);
        printf("%s\n", &hold_line[first_hk]);
      }

      line_match = 0;
      line_index = 0;

      //weird i dont know why i need this... fixes my output for first line
      if(first_hk == 0){
        first_hk = 1;
      }
    }

    //printf("%c", *text);

    *text++;
    checked_chars++;
    line_index++;
  }
  //printf("MATCHED: %d\n", count);
  //printf("LOOKED AT # CHARS: %d\n", checked_chars);
  return count;
}




