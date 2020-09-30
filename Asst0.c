#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef enum {WORD, DECIMAL, OCTAL, HEX, FLOAT, OPERATOR} Token;

// begin prototypes
int isoctal(char digit);
int printSubString(Token token, char* str, int begIndex, int endIndex);
int* findToken(char* str, int index);
// end prototypes

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


int isoctal(char digit) {
    if(digit >= '0' && digit <= '7') return 1;
    return 0;
}
int printSubString(Token token, char* str, int begIndex, int endIndex) {
    //Print out the Token type
    switch(token){
        case WORD: printf("word: "); break;
        case DECIMAL: printf("decimal: "); break;
        case OCTAL: printf("octal: "); break;
        case HEX: printf("hex: "); break;
        case FLOAT: printf("float: "); break;
        case OPERATOR:
            printf("operator: ");
            //char* operatorName;
            // TODO: Add findoperatorname to print type of operator (lots of switch case statements prolly)
            //printf("%s: ", findOperatorName(str[begIndex]));
            //free(operatorName);
            break;
    }
    int i;
    for(i = begIndex; i <= endIndex; i++) {
        printf("%c", str[i]);
    }
    printf("\n");
    return 0;
}
/*
 * When do we know what kind of token the string is?
 *  - WORD: If the first character is alphabetic, we know it is either a word, or the sizeof operator
 *  - 
 */
int* findToken(char* str, int index) {
    int *array = malloc(2*sizeof(int)); // malloc space for 2 ints to be returned [token type, last index of token]
    char curr = str[index]; // current character being assessed
    if(isalpha(curr)) { // if token is alphabetic character
        array[0] = WORD; // pre-emptively assign token type to WORD
        int i = index; // at the end of the while loop, i will have the last index of the WORD token
        while(isalnum(curr) && !(isspace(str[i+1]) || str[i+1] == '\0')) curr = str[++i];
        array[1] = i; //assign [,last index of token] to i
        char *wordToken = malloc((i - index + 2)*sizeof(char)); // move below lines into modular helper function?
        wordToken[strlen(wordToken) - 1] = '\0';                // basically just checks if string is equal to "sizeof" and sets to operator if true
        memcpy(wordToken, &str[index], (i - index + 1)*sizeof(char));
        if(strcmp(wordToken, "sizeof") == 0) {
            array[0] = OPERATOR;
        }
    }
    else if(isdigit(curr)) {
        array[0] = DECIMAL; // pre-emptively assign token type to DECIMAL
        if(str[index] == '0') {
            if((str[index + 1] == 'x' || str[index + 1] == 'X') && isxdigit(str[index + 2])) { // hexadecimal number
                array[0] = HEX; // assign token type to HEX
                int i = index + 2;
                while(isxdigit(str[i+1]) && !(isspace(str[i+1]) || str[i+1] == '\0')) ++i;
                array[1] = i;     
            }
            else if(isoctal(str[index+1])) {
                array[0] = OCTAL; // assume octal unless we find a non-octal digit in string
                int i = index + 1;
                while(isdigit(str[i]) && !(isspace(str[i+1]) || str[i+1] == '\0')) {
                    if(!isoctal(str[i])) array[0] = DECIMAL;
                    ++i;
                }    
                array[1] = i;
            }
            else { // decimal numbers that begin with 0
                int i = index;
                while(isdigit(str[i]) && !(isspace(str[i+1]) || str[i+1] == '\0')) ++i;
                array[1] = i;
            }
        }
    }
    return array;
}

int main(int argc, char **argv) {
    int length = strlen(argv[1]);
    int i = 0;
    for(i = 0; i < length; ++i) {
        char curr = argv[1][i];
        while(isspace(curr)) {
            curr = argv[1][++i]; // iterate until we find a non-whitespace character
        }
        if(!(i < length)) return 0;    
        int *array = findToken(argv[1], i); // call find token to get pointer to array that stores [token type, last index of token]
        printSubString(array[0], argv[1], i, array[1]); // call printSubString to output our tokens
        i = array[1];
        
    }

    // test cases from pdf
    // char test1[] = "123stuff";
    // char test2[] = "123 stuff";
    // char test3[] = "array[xyz ] += pi 3.14159e-10";
    
    return 0;
}

