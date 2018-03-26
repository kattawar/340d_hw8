#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
//
//
// comment here
//
//
//
//
/*
The mygrep utility is a tool that can be used for searching a piece of
text for occurrences of a certain pattern. The text to be searched must
be read in from STDIN and the pattern to be read will be stored inside
a file. The term pattern means any type of regular expression that
matches the following constraints. It can contain any of the printable
characters from 32-126 ASCII values inclusive and some of the characters
have special properties. A period character will act as a wildcard
and match any of the other characters. A left brace is used to open
a character set and the character set is open until it is closed
with a right brace. All characters inside the character set are valid
for that position in the pattern. [abc][abc] Would match any two
character strings if they contain an a, b, or c in both positions.
Character sets can also be complemented by the use of a ^ at the beginning.
[^abc] would match all of the characters that are not a, b , or c. The last
special character that can be used inside the character set is the range
operator, or the dash. When the dash is used inside a character set it
represents all of the characters from the start character to the end character,
and if a backwards range is given it will result in undefined behavior.
[a-z] would match all of the lower case letters in the English alphabet.
If the pattern inside the pattern file being supplied to mygrep breaks any
of the rules described above undefined behavior may occur and no results
are guaranteed. In order to supply the file to mygrep the –f flag is also
needed which is similar to grep and just tells our program to read the
pattern from a file instead of from the shell, however patterns supplied
in the shell are not supported by mygrep.

Example usage:

Cat Adventures_of_sherlock_holmes | ./mygrep –f my_pattern.txt

In this case we are searching the text of Sherlock Holmes and checking
for all matches to the pattern inside my_pattern.txt. If my_pattern
contains something like Sherl[^abc]ck then we would get a result that
contained all the lines that have the name Sherlock on them and possibly a few others.

Implementation Details:
The mygrep utility is built using a two-stage process in the broad sense.
First we must take the pattern inside the pattern file and turn that into
something the computer can work with and second we must apply this pattern
to the text we are supplied in an algorithmic fashion. We first start by
reading in character by character from the pattern file and if we hit a
regular character between 32-126 that isn’t a special character then a
slot in a 2d matrix with corresponding value to that ascii character
of width 256 is flipped to a 1 to signal that for the first character
rule that one character is acceptable. If a special character like a
period is read then we flip all the bits to a 1 to accept any type
of character. After we read a character we fill in the necessary
bit and then bump the row count one to move down to the next character
rule. We keep doing this unless we are inside a character set. If we
are in a character set there is a bunch of logic around all the
special operators in order to make those behave differently than the
other characters. Once the characters are all read in we are left with
a 2d matrix that has a number of rows equal to the length of the characters
that should be matched. So for the example usage above that supplied Sherl[^abc]ck
there would be a total of 8 rows in the 2d matrix. After we get this
matrix we can now tart to read the text from STDIN and apply the pattern
to see if we get any matches. We start reading STDIN character by character
and if we match a character to the first rule of our 2d matrix then we
open up another loop to read character by character and loop over
the rules now. If we loop over the rules successfully with each new
char read in then we know we have read in a string that matches our
pattern so we will increase our count of matches. If a rule is ever
not matched the inner while loop will stop and return reading char by
char until the first rule is met again. This entire time regardless of
a match or not a buffer is kept to store the most current line being read,
which is built up character by character as we read STDIN. If we ever get a
match we know we need to print out this buffer because the line contains a
match. The loop will continue to run until there is no more input from STDIN.

 */

// // comment here
// //

// char *readFile(char *fname)
// {
//   FILE *file = fopen(fname, "r");
//   char *code;
//   size_t n = 0;
//   int c;

//   if (file == NULL)
//       return NULL; //could not open file

//   // needed ~65500 elems, added some extra room for other potential characters just in case...
//   code = calloc(70000, sizeof(char));

//   while ((c = fgetc(file)) != EOF)
//   {
//       //printf("%c", c);
//       code[n++] = (char) c;
//   }

//   //add a null character so EOF terminates properly
//   code[n] = '\0';

//   return code;
// }

// void print_map(int rules[1024][256], int num_rows){

//   printf("MAP IS: \n");
//   for(int i = 0; i < num_rows; i++){
//     for(int j = 0; j < 256; j++){
//       if(rules[i][j] == 1){
//         printf("%c", j);
//       }
//     }
//     printf("\n");
//   }
// }

// void parse_pattern(int rows, int cols, int rules[rows][cols])
// {
//   int num_args;
//   int num_env;
//   //int rules[1024][256];
//   int length_of_regex = 0;
//   int num_rows = 0;
//   int set_start = 0;
//   int set_end = 0;
//   int set_open = 0;
//   int c_index = 0;


//   // if (argc > 1){
//   //   for (num_args = 1; num_args < argc; num_args++){
//   //         printf("%d %s\n", num_args, argv[num_args]);
//   //   }
//   // }

//   // Sanitize the input from command line
//   char regex[200];
//   int reg_index = 0;


//   char *pattern = readFile("test_pattern.txt");
//   while(*pattern){
//     regex[reg_index] = *pattern;
//     *pattern++;
//     reg_index++;
//   }
//   regex[reg_index]= '\0';

//   //printf("REGEX IS: %s", regex);


//   //loop over regex chars
//   for(int i = 0; i < sizeof(regex)/sizeof(char); i++){
//     char k = regex[i];
//     if(k > 31){
//       if( k == '.'){
//         // these char are supported by the . operator
//         for(int g = 32; g < 127; g++){
//           rules[c_index][g] = 1;
//         }
//         num_rows++;
//       }
//       if( k == '^'){
//         // all newline characters
//         for(int g = 9; g < 14; g++){
//           rules[c_index][g] = 1;
//         }
//         num_rows++;
//       }
//       if( k == '\\'){
//         //printf("SETTING CIND: %d", c_index);
//         rules[c_index][regex[i+1]] = 1;
//         num_rows++;
//         c_index++;
//         i++;
//         continue;
//       }

//       int is_complimnent = 0;
//       int rows_to_flop[39];

//       //open SET
//       if(set_open == 0 && k == '['){
//         set_open = 1;
//         int j = i+1;
//         char c = regex[j];

//         if(c == '^'){
//           is_complimnent = 1;
//         }


//         while(c != ']'){
//           //if hit RANGE
//           if(regex[j+1] == '-'){
//             //printf("IN SETRANGE\n");
//             for(int q = regex[j]; q < (regex[j+2] + 1); q++){
//               //printf("SETTING: %c\n",q);
//               rules[c_index][q] = 1;
//             }

//             j = j+3;
//             c = regex[j];
//           }
//           else{
//             // regular IN SET
//             //printf("INSET:%c\n", regex[j]);
//             rules[c_index][regex[j]] = 1;
//             j++;
//             c = regex[j];
//           }
//         }
//         set_open = 0;
//         i = j;
//         num_rows++;
//       }// close SET
//       else{
//         //printf("SETTING CIND: %d", c_index);
//         //printf("REGULAR CHAR:%c\n", regex[i]);
//         rules[c_index][regex[i]] = 1;
//         num_rows++;
//       }


//       if(is_complimnent){
//         for(int s = 32; s < 127; s++){
//           if(rules[c_index][s] == 1){
//             rules[c_index][s] = 0;
//           }
//           else{
//             rules[c_index][s] = 1;
//           }
//         }
//       }


//       is_complimnent = 0;
//       c_index++;
//     }

//     // bail out regex is done
//     if(regex[i] == 0){
//       //printf("\nIMDONE\n");
//       i = sizeof(regex)/sizeof(char);
//     }
//   }

//   //printf("\nROWS IS : %d\n", num_rows);
//   //printf("%d\n", rules[2][99]);
//   //print_map(rules, num_rows);



//   // DEBUGGER MAP
//   // for(int i = 0; i < 256; i++){
//   //   for(int j = 0; j < 256; j++){
//   //     if(rules[i][j] == 1){
//   //       printf("1");
//   //     }
//   //     else if(rules[i][j] == 0){
//   //       printf("0");
//   //     }
//   //   }
//   //   printf("\n");
//   // }
// }


//////////////////////////////////////////
//////////////////////////////////////////
//////////////////////////////////////////
//////////////////////////////////////////
/// MYFILTER BELOW



// char *readFile2(char *fname)
// {
//     FILE *file = fopen(fname, "r");
//     char *code;
//     size_t n = 0;
//     int c;

//     if (file == NULL)
//         return NULL; //could not open file

//     // needed ~65500 elems, added some extra room for other potential characters just in case...
//     code = calloc(70000, sizeof(char));

//     while ((c = fgetc(file)) != EOF)
//     {
//         //printf("%c", c);
//         code[n++] = (char) c;
//     }

//     //add a null character so EOF terminates properly
//     code[n] = '\0';

//     return code;
// }

// int check_row(int rules[1024][256], int row_num, char c){
//   if(rules[row_num][c] == 1 ){
//     return 1;
//   }

//   return 0;
// }

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


// int start_matching(int rules[1024][256]){
//   //printf("STARTING MATCH WITH MAP\n");
//   // for(int i = 0; i < 256; i++){
//   //   for(int j = 0; j < 256; j++){
//   //     if(rules[i][j] == 1){
//   //       printf("1");
//   //     }
//   //     else if(rules[i][j] == 0){
//   //       printf("0");
//   //     }
//   //   }
//   //   printf("\n");
//   // }


//   char text_buf[1000000];
//   int cp_index;
//   char g;
//   while((g = getchar()) != EOF){
//     text_buf[cp_index] = g;
//     cp_index++;
//   }

//   char *text = &text_buf[0];
//   int count = 0;
//   int num_rows = num_rows_valid(rules);
//   char hold_line[10000];
//   int line_index = 0;
//   int line_match = 0;

//   //printf("FOUND RULES ROWS: %d\n", num_rows);
//   //char *restore_text = text;
//   int checked_chars = 0;

//   while(*text){
//     //printf("RESETTING COUNTERS\n");
//     int i = 0;
//     int matches = 1;
//     char *temp_text = text;

//     while(i < num_rows && *temp_text){
//       //printf("CHECKING i: %d, CHECKING char: %c\n", i, *temp_text);
//       //printf("%d\n", rules[1]['M']);
//       //
//       //printf("CHECK IS: %d\n", (rules[i][*temp_text]));
//       if(rules[i][*temp_text] != 1){
//         //printf("NO MATCH, i: %d, char: %c\n", i, *temp_text);
//         matches = 0;
//       }

//       *temp_text++;
//       i++;
//     }

//     if(matches == 1){
//       line_match = 1;
//       count++;
//     }

//     if(*text != '\n'){
//       hold_line[line_index] = *text;
//     }
//     else if(*text == '\n'){
//       //printf("PR LINE: ");
//       if(line_match == 1){
//         hold_line[line_index] = '\0';
//         printf("%s\n", &hold_line[1]);
//       }

//       line_match = 0;
//       line_index = 0;
//     }

//     //printf("%c", *text);

//     *text++;
//     checked_chars++;
//     line_index++;
//   }
//   //printf("MATCHED: %d\n", count);
//   //printf("LOOKED AT # CHARS: %d\n", checked_chars);
//   return count;
// }

// void search_text(int rows, int cols, int rules[rows][cols]){
//   int found_matches = start_matching(rules);





// }







// int main (int argc, char *argv[], char *envp[])
// {
//   int num_args;
//   int num_env;
//   int rules[1024][256];

//   if (argc > 1)
//   {
//     for (num_args = 1; num_args < argc; num_args++)
//     {
//       //printf("%lu %s\n", strlen(argv[num_args]), argv[num_args]);
//       printf("%s\n", argv[num_args]);
//     }
//   }


//   // rebuild the rules matrix from the input
//   char *mr = readFile("1.txt");
//   int row_num = 0;
//   int col_num = 0;
//   while(*mr){
//     col_num = 0;
//     while(*mr != '\n'){
//       if(*mr == '1'){
//         rules[row_num][col_num] = 1;
//         //printf("%c", *mr);
//       }
//       else{
//         rules[row_num][col_num] = 0;
//       }

//       col_num++;
//       *mr++;
//     }
//     //printf("\n");
//     row_num++;
//     *mr++;
//   }

//   // DEBUGGER, prints out the rules table up to 256 rows
//   // for(int i = 0; i < 256; i++){
//   //   for(int j = 0; j < 256; j++){
//   //     if(rules[i][j] == 1){
//   //       printf("1");
//   //     }
//   //     else if(rules[i][j] == 0){
//   //       printf("0");
//   //     }
//   //   }
//   //   printf("\n");
//   // }
//   //
//   //

//   int found_match = start_matching(rules);




//   exit(argc);
// }

















// PROJECT
////////////////////////////
////////////////////////////
////////////////////////////
////////////////////////////
int main (int argc, char *argv[], char *envp[])
{
  int num_args;
  int num_env;

  if (argc > 1)
  {
    for (num_args = 1; num_args < argc; num_args++)
      {
        //printf("%s , %d\n", argv[num_args], num_args);
      }
  }

  int rules[1024][256];

  parse_pattern(1024, 256, rules, argv[2]);

  int result = start_matching(rules);




  exit(argc);
}
