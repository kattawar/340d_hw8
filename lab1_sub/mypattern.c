#include <stdio.h>
#include <stdlib.h>


char *readFile(char *fname)
{
  FILE *file = fopen(fname, "r");
  char *code;
  size_t n = 0;
  int c;

  if (file == NULL)
      return NULL; //could not open file

  // needed ~65500 elems, added some extra room for other potential characters just in case...
  code = calloc(70000, sizeof(char));

  while ((c = fgetc(file)) != EOF)
  {
      //printf("%c", c);
      code[n++] = (char) c;
  }

  //add a null character so EOF terminates properly
  code[n] = '\0';

  return code;
}

void print_map(int rules[1024][256], int num_rows){

  printf("MAP IS: \n");
  for(int i = 0; i < num_rows; i++){
    for(int j = 0; j < 256; j++){
      if(rules[i][j] == 1){
        printf("%c", j);
      }
    }
    printf("\n");
  }
}

void parse_pattern(int rows, int cols, int rules[rows][cols], char *file)
{
  int num_args;
  int num_env;
  //int rules[1024][256];
  int length_of_regex = 0;
  int num_rows = 0;
  int set_start = 0;
  int set_end = 0;
  int set_open = 0;
  int c_index = 0;


  // if (argc > 1){
  //   for (num_args = 1; num_args < argc; num_args++){
  //         printf("%d %s\n", num_args, argv[num_args]);
  //   }
  // }

  // Sanitize the input from command line
  char regex[200];
  int reg_index = 0;


  char *pattern = readFile(file);
  while(*pattern){
    regex[reg_index] = *pattern;
    *pattern++;
    reg_index++;
  }
  regex[reg_index]= '\0';

  //printf("REGEX IS: %s", regex);


  //loop over regex chars
  for(int i = 0; i < sizeof(regex)/sizeof(char); i++){
    char k = regex[i];
    if(k > 31){
      if( k == '.'){
        // these char are supported by the . operator
        for(int g = 32; g < 127; g++){
          rules[c_index][g] = 1;
        }
        num_rows++;
      }
      if( k == '^'){
        // all newline characters
        for(int g = 9; g < 14; g++){
          rules[c_index][g] = 1;
        }
        num_rows++;
      }
      if( k == '\\'){
        //printf("SETTING CIND: %d", c_index);
        rules[c_index][regex[i+1]] = 1;
        num_rows++;
        c_index++;
        i++;
        continue;
      }

      int is_complimnent = 0;
      int rows_to_flop[39];

      //open SET
      if(set_open == 0 && k == '['){
        set_open = 1;
        int j = i+1;
        char c = regex[j];

        if(c == '^'){
          is_complimnent = 1;
        }


        while(c != ']'){
          //if hit RANGE
          if(regex[j+1] == '-'){
            //printf("IN SETRANGE\n");
            for(int q = regex[j]; q < (regex[j+2] + 1); q++){
              //printf("SETTING: %c\n",q);
              rules[c_index][q] = 1;
            }

            j = j+3;
            c = regex[j];
          }
          else{
            // regular IN SET
            //printf("INSET:%c\n", regex[j]);
            rules[c_index][regex[j]] = 1;
            j++;
            c = regex[j];
          }
        }
        set_open = 0;
        i = j;
        num_rows++;
      }// close SET
      else{
        //printf("SETTING CIND: %d", c_index);
        //printf("REGULAR CHAR:%c\n", regex[i]);
        rules[c_index][regex[i]] = 1;
        num_rows++;
      }


      if(is_complimnent){
        for(int s = 32; s < 127; s++){
          if(rules[c_index][s] == 1){
            rules[c_index][s] = 0;
          }
          else{
            rules[c_index][s] = 1;
          }
        }
      }


      is_complimnent = 0;
      c_index++;
    }

    // bail out regex is done
    if(regex[i] == 0){
      //printf("\nIMDONE\n");
      i = sizeof(regex)/sizeof(char);
    }
  }

  //printf("\nROWS IS : %d\n", num_rows);
  //printf("%d\n", rules[2][99]);
  //print_map(rules, num_rows);



  // DEBUGGER MAP
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
}

