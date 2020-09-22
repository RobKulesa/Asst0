#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef enum {WORD, DECIMAL, OCTAL, HEX, FLOAT, OPERATOR} Token;

// printSubString function prototype
int printSubString(Token token, char* str, int begIndex, int endIndex);
//char findOperatorName(char* operatorString);
/*
 * 1. Word Token
 *      Ends when ____ is found
 *      - White Space
 *      - Operator
 * 2. Decimal token
 *      Ends when ____ is found
 *      - White Space
 *      - Operator
 *      - Alphabetic Char found
 * 3. Octal Token
 *      Ends when ____ is found
 *      - White Space
 *      - Operator
 *      - Alphabetic Char found
 * 4. Hex Token
 *      Ends when ____ is found
 *      - White Space
 *      - Operator
 *      - Alphabetic Char found that isn't a-f
 * 5. Float
 *      Ends when ____ is found
 *      - White Space
 *      - Operator
 *      - Alphabetic Char Found
 *      - BUT NOT WHEN "e-" is found because this is scientific notation
 * 6. Operator
 *      Ends
 *      - Immediately After
 ******** White Space Characters
 */



int printSubString(Token token, char* str, int begIndex, int endIndex) {

    //Print out the Token type
    switch(token){
        case WORD: printf("word: "); break;
        case DECIMAL: printf("decimal: "); break;
        case OCTAL: printf("octal: "); break;
        case HEX: printf("hex: "); break;
        case FLOAT: printf("float: "); break;
        case OPERATOR:
            //char* operatorName;
            // TODO: Add findoperatorname to print type of operator (lots of switch case statements prolly)
            //printf("%s: ", findOperatorName(str[begIndex]));
            //free(operatorName);
            break;
    }
    int i;
    for(i = begIndex; i < endIndex; i++) {
        printf("%c", str[i]);
    }
    printf("\n");
    return 0;
}

int main(int argc, char **argv) {
    //1. first figure out length of passed string
    int length = strlen(argv[1]);
    int i = 0;
    //2. iterate through characters of string to tokenize
    for(i = 0; i < length; ++i) {
        char curr = argv[1][i]; // define pointer for current character
        // TODO: make sure token isn't sizeof operator
        if(isalpha(curr)) { // if start of token is alphabetical
            int begIndex = i; // mark first index of WORD token
            int counter = 1; // counter for length of WORD token, first one is already alphabetical 
            curr = argv[1][++i]; // move to next char
            while(isalnum(curr)) { // iterate through string until nonalphanumeric character found to find length of WORD token
                curr = argv[1][++i];
                ++counter;
            }
            printSubString(WORD, argv[1], begIndex, begIndex + counter);
        }
    }

        
    // test cases from pdf
    // char test1[] = "123stuff";
    // char test2[] = "123 stuff";
    // char test3[] = "array[xyz ] += pi 3.14159e-10";
    
    return 0;
}

