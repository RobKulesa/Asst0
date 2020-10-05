#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef enum {WORD, DECIMAL, OCTAL, HEX, FLOAT, OPERATOR, GARBAGE} Token;

// begin prototypes
int isoctal(char digit);
void printToken(Token token, char* str, int begIndex, int endIndex);
int* findToken(char* str, int index);
char* idOperator(char* operator);
// end prototypes

/* function isoctal(input character)
 * determines if an input character is a valid octal character (0-7)
 * returns 1 if true, 0 if false.
 */ 
/*
 * function idOperator(input string)
 * Purpose: given a string that is guaranteed to be an operator, determine which operator is represented by the string
 * Algorithm: Checks by string length first, and then by the first character. If it is a multicharacter operator, it will determine which operator it is
 * given whatever the first character of the multicharacter operator is
 * Return: returns the english name for the operator as a char* (string)
 */
char* idOperator(char* operator){
    int size = strlen(operator);
    char c = operator[0];
    switch(size){
        case 1: 
            switch(c){
                case '(': return "left parenthesis";
                case ')': return "right parenthesis";
                case '[': return "left bracket";
                case ']': return "right bracket";
                case '.': return "structure member";
                case ',': return "comma";
                case '^': return "bitwise XOR";
                case '!': return "negate";
                case '~': return "1s complement";
                case '|': return "bitwise OR";
                case '+': return "addition";
                case '/': return "division";
                case '?': return "conditional true";
                case ':': return "conditional false";
                case '<': return "less than test";
                case '>': return "greater than test";
                case '=': return "assignment";
                case '&': return "AND/address operator";
                case '-': return "minus/subtract operator";
                case '*': return "multiply/dereference operator";
            }
            break;
        case 2:
            switch(c){
                case '-': 
                    if (operator[1] == '>')
                        return "structure pointer";
                    else if(operator[1] == '-')
                        return "decrement";
                    else
                        return "minus equals";
                    break;
                case '>': 
                    if(operator[1] == '>')
                        return "shift right";
                    else
                        return "greater than or equal test";
                    break;
                case '<': 
                    if(operator[1] == '<')
                        return "shift left";
                    else
                        return "less than or equal test";
                    break;
                case '+': 
                    if(operator[1] == '+')
                        return "increment";
                    else
                        return "plus equals";
                    break;
                case '|': 
                    if(operator[1] == '|')
                        return "logical OR";
                    else
                        return "bitwise OR equals";
                    break;
                case '&': 
                    if(operator[1] == '&')
                        return "logical AND";
                    else
                        return "bitwise AND equals";
                    break;
                case '=': return "equality test";
                case '!': return "inequality test";
                case '*': return "times equals";
                case '/': return "divide equals";
                case '%': return "mod equals";
                case '^': return "bitwise XOR equals";
            }
            break;
        case 3: 
            switch(c){
                case '>': return "shift right equals";
                case '<': return "shift left equals";
            }
            break;
        case 6:
            return "sizeof";
        default:
            return "error";
    }
    return "error";
}
/* function isoctal(input character)
 * determines if an input character is a valid octal character (0-7)
 * returns 1 if true, 0 if false.
 */ 
int isoctal(char digit) {
    if(digit >= '0' && digit <= '7') return 1;
    return 0;
}
/* function printToken(token type, input string, start and end indices of token)
 * prints the tokens as a substring of original input string with formatting according to token type
 */ 
void printToken(Token token, char* str, int begIndex, int endIndex) {
    if(token == GARBAGE) return;
    char substring[endIndex - begIndex + 2]; 
    int i;
    for(i = begIndex; i <= endIndex; ++i) {
        substring[i-begIndex] = str[i];
    }
    substring[endIndex-begIndex + 1] = '\0';
    char* operatorName;
    switch(token){
        case WORD: printf("word: \""); break;
        case DECIMAL: printf("decimal integer: \""); break;
        case OCTAL: printf("octal integer: \""); break;
        case HEX: printf("hex integer: \""); break;
        case FLOAT: printf("floating point: \""); break;
        case OPERATOR:
            operatorName = idOperator(substring);
            printf("%s: \"", operatorName);
            break;
        case GARBAGE:
            break;
        default:
            printf("TOKEN ID ERROR: ");
            break;
    }
    for(i = begIndex; i <= endIndex; i++) {
        printf("%c", str[i]);
    }
    printf("%s", "\"");
    printf("\n");
    return;
}
/* findToken(input string, starting index of token)
 * function that iterates through the input string starting from the starting index and greedily determines last index of token/token type
 * returns length 2 int array [token type, last index of token]
*/
int* findToken(char* str, int index) {
    int *array = malloc(2*sizeof(int)); // malloc space for 2 ints to be returned [token type, last index of token]
    char curr = str[index]; // current character being assessed
    /*
     * WORD token
     */
    if(isalpha(curr)) { //! If the first character of token is alphabetic, it must be a word token
        array[0] = WORD; // pre-emptively assign token type to WORD
        int i = index; // at the end of the while loop, i will have the last index of the WORD token
        while(isalnum(curr) && !(isspace(str[i+1]) || str[i+1] == '\0')) curr = str[++i];
        if(!isalnum(curr)) i--;
        array[1] = i; // assign [,last index of token] to i
        char *wordToken = malloc((i - index + 2)*sizeof(char)); // basically just checks if string is equal to "sizeof" and sets to operator if true
        wordToken[strlen(wordToken) - 1] = '\0';                
        memcpy(wordToken, &str[index], (i - index + 1)*sizeof(char));
        if(strcmp(wordToken, "sizeof") == 0) {
            array[0] = OPERATOR;
        }
        return array;
    }
    /*
     * DECIMAL, HEXADECIMAL, OR OCTAL
     */
    else if(isdigit(curr)) {//! if the first character of a token is numeric, it must be a Decimal, Hexadecimal, Octal, or float number.
        array[0] = DECIMAL; // pre-emptively assign token type to DECIMAL
        if(str[index] == '0') {
            //If the first two characters are 0x or 0X, the token MUST be a hex integer
            if((str[index + 1] == 'x' || str[index + 1] == 'X') && isxdigit(str[index + 2])) { // hexadecimal number
                array[0] = HEX; // assign token type to HEX
                int i = index + 2;
                while(isxdigit(str[i+1]) && !(isspace(str[i+1]) || str[i+1] == '\0')) ++i;
                array[1] = i;
                return array;     
            } else if(isdigit(str[index+1])) { //! The first digit is 0 and the next character is numeric
                //If the first character is 0 and the second character is numeric, than it is either OCTAL or DECIMAL
                array[0] = OCTAL; // assume octal unless we find a non-octal digit in string
                //int i = index + 1;
                int i = index;
                while(isdigit(str[i]) && !(isspace(str[i+1]) || str[i+1] == '\0')) { //Checks for digits that would make it not octal
                    if(!isoctal(str[i])) 
                        array[0] = DECIMAL; //pre-emptively assigns token type to DECIMAL
                    ++i;
                }
                if(isspace(str[i+1]) || str[i+1] == '\0') {
                    if(isdigit(str[i]) && !isoctal(str[i])) 
                        array[0] = DECIMAL;
                }
                //If we are dealing with an octal or a decimal (not float), we want to end the function as soon as possible
                if(str[i] != '.' || (str[i]== '.' && (str[i+1] == '\0' || !isdigit(str[i+1]))) ) { 
                    if(!isdigit(str[i])) --i;
                    array[1] = i;
                    return array;
                } else { //If the first non-numeric character found is '.', then the entire token we are dealing with is a float, and we must figure out where it terminates
                    array[0] = FLOAT;
                    ++i; //Currently, str[i] points to the '.' character. We need to increment this.
                    while(isdigit(str[i]) && !(isspace(str[i+1]) || str[i+1] == '\0')) ++i; //Iterates until non-numeric value is found
                    /* 1. If the non-numeric character found while iterating through the string is not 'e' or 'E', the token CANNOT be a float
                     * 2. If the string terminates after the 'E' or 'e' is found, it cannot be a float
                     * 3. Let # represent any digit character, if the token does not follows pattern e#, e-#, or e+#, then the token cannot be a float
                     */
                    if((str[i] != 'e' && str[i]!='E') || str[i+1] == '\0'|| !(isdigit(str[i+1]) || (str[i+2]!= '\0' && isdigit(str[i+2]) && (str[i+1] == '+' || str[i+1] == '-')))  ) { 
                        if(!isdigit(str[i])) --i;
                        array[1] = i;
                        return array;
                    }
                    else{
                        if(str[i+1] == '+' || str[i+1] == '-') i+=1; // If we do detect 'e', '-', and a digit following, skip to the index of the digit
                        do{
                            ++i;
                        }
                        while(isdigit(str[i+1]) && !(isspace(str[i+1]) || str[i+1] == '\0'));
                        array[1] = i;
                        return array;
                    }
                    array[1] = i;
                    return array;
                }    
            } else { //! The first number is 0, and the next character is not a digit
                if(str[index+1] == '\0' || str[index+1] != '.'|| (str[index+1] == '.' && (str[index+2] == '\0' || !isdigit(str[index+2]))) ) {
                    array[0] = OCTAL;
                    array[1] = index;
                    return array;
                } else {
                    int i = index + 1;
                    array[0] = FLOAT;
                    ++i; //Currently, str[i] points to the '.' character. We need to increment this.
                    while(isdigit(str[i]) && !(isspace(str[i+1]) || str[i+1] == '\0')){ //Iterates until non-numeric value is found
                        ++i;
                    }
                    /*
                     * 1. If the non-numeric character found while iterating through the string is not 'e' or 'E', the token CANNOT be a float
                     * 2. If the string terminates after the 'E' or 'e' is found, it cannot be a float
                     * 3. Let # represent any digit character, if the token does not follows pattern e#, e-#, or e+#, then the token cannot be a float
                     */
                    if((str[i] != 'e' && str[i]!='E')|| str[i+1] == '\0'|| !(isdigit(str[i+1]) || (str[i+2]!= '\0' && isdigit(str[i+2]) && (str[i+1] == '+' || str[i+1] == '-')))  )
                    {   
                        if(!isdigit(str[i]))
                            --i;
                        array[1] = i;
                        return array;
                    }
                    else{
                        if(str[i+1] == '+' || str[i+1] == '-')
                            i+=1; // If we do detect 'e', '-', and a digit following, skip to the index of the digit
                        do{
                            ++i;
                        }
                        while(isdigit(str[i+1]) && !(isspace(str[i+1]) || str[i+1] == '\0'));
                        array[1] = i;
                        return array;
                    }
                }
            }
        } else { // THE FIRST CHARACTER IS NOT 0
            array[0] = DECIMAL;
            int i = index;
            do{
                ++i;
            }
            while(isdigit(str[i]) && !(isspace(str[i+1]) || str[i+1] == '\0'));
            if(str[i] != '.' || (str[i]== '.' && (str[i+1] == '\0' || !isdigit(str[i+1])) )) {    
                if(!isdigit(str[i])) --i;
                array[1] = i;
                return array;
            }else{ //If the first non-numeric character found is '.', then the entire token we are dealing with is a float, and we must figure out where it terminates
                    array[0] = FLOAT;
                    ++i; //Currently, str[i] points to the '.' character. We need to increment this.
                    while(isdigit(str[i]) && !(isspace(str[i+1]) || str[i+1] == '\0')){ //Iterates until non-numeric value is found
                        ++i;
                    }
                    /*
                     * 1. If the non-numeric character found while iterating through the string is not 'e' or 'E', the token CANNOT be a float
                     * 2. If the string terminates after the 'E' or 'e' is found, it cannot be a float
                     * 3. Let # represent any digit character, if the token does not follows patterns e#, e-#, or e+#, then the token cannot be a float
                     */
                    if((str[i] != 'e' && str[i]!='E')|| str[i+1] == '\0'|| !(isdigit(str[i+1]) || (str[i+2]!= '\0' && isdigit(str[i+2]) && (str[i+1] == '+' || str[i+1] == '-')))  )
                    {   
                        if(!isdigit(str[i]))
                            --i;
                        array[1] = i;
                        return array;
                    }
                    /*
                     * If the float is found to follow patterns e#, e-#, or e+#: The program will iterate until a non-numeric character is found.
                     * The non-numeric character will terminate the token
                     */
                    else{
                        if(str[i+1] == '+' || str[i+1] == '-')
                            i+=1; // If we do detect 'e', '-', and a digit following, skip to the index of the digit
                        do{
                            ++i;
                        }while(isdigit(str[i+1]) && !(isspace(str[i+1]) || str[i+1] == '\0'));
                        array[1] = i;
                        return array;
                    }
                }
        }
    } else {
        array[0] = OPERATOR; // assume token is an operator
        /* Looks at the current character. If it matches a valid c operator, it will skip to that case
         * Certain c operators are multicharacter, thus for certain characters, 
         * it will parse the next couple characters to find a multicharacter operator
         */
        switch(curr){
            case '(': array[1] = index; return array;
            case ')': array[1] = index; return array;
            case '[': array[1] = index; return array;
            case ']': array[1] = index; return array;
            case '.': array[1] = index; return array;
            case ',': array[1] = index; return array;
            case '~': array[1] = index; return array;
            case '?': array[1] = index; return array;
            case ':': array[1] = index; return array;
            case '%':                                               
                if(str[index+1] == '\0' || str[index+1] != '='){ //! Really weird, but if you look at the RefCard.txt, there is no dedicated opeator for %. There's only %=. Weird.
                    array[0] = GARBAGE;
                    array[1] = index;
                    return array;
                }else{
                    array[1] = index+1;
                    return array;
                }
            case '=':
                if(str[index+1] == '\0' || str[index+1] != '=')
                    array[1] = index;
                else
                    array[1] = index+1;
                return array;
            case '!':
                if(str[index+1] == '\0' || str[index+1] != '=')
                    array[1] = index;
                else
                    array[1] = index+1;
                return array;
            case '*':
                if(str[index+1] == '\0' || str[index+1] != '=')
                    array[1] = index;
                else
                    array[1] = index+1;
                return array;
            case '/':                                            
                if(str[index+1] == '\0' || str[index+1] != '=')
                    array[1] = index;
                else
                    array[1] = index+1;
                return array;
            case '^':                   
                if(str[index+1] == '\0' || str[index+1] != '=')
                    array[1] = index;
                else
                    array[1] = index+1;
                return array;
            case '-':
                if(str[index+1] == '\0')
                    array[1] = index;
                else if(str[index+1] == '>' || str[index+1] == '-' || str[index+1] == '=')
                    array[1] = index+1;
                else
                    array[1] = index;
                return array;
            case '+':
                if(str[index+1] == '\0')
                    array[1] = index;
                else if(str[index+1] == '+' || str[index+1] == '=')
                    array[1] = index+1;
                else
                    array[1] = index;
                return array;  
            case '|':
                if(str[index+1] == '\0')
                    array[1] = index;
                else if(str[index+1] == '|' || str[index+1] == '=')
                    array[1] = index+1;
                else
                    array[1] = index;
                return array;
            case '&':
                if(str[index+1] == '\0')
                    array[1] = index;
                else if(str[index+1] == '&' || str[index+1] == '=')
                    array[1] = index+1;
                else
                    array[1] = index;
                return array;
            case '>': 
                if(str[index+1] == '\0')
                    array[1] = index;
                else if(str[index+1] == '>' || str[index+1] == '='){
                    array[1] = index+1;
                    if(str[index+1] == '>' && str[index+2] != '\0' && str[index+2] == '=')
                        array[1]++;
                } else
                    array[1] = index;
                return array;
            case '<': 
                if(str[index+1] == '\0')
                    array[1] = index;
                else if(str[index+1] == '<' || str[index+1] == '='){
                    array[1] = index+1;
                    if(str[index+1] == '<' && str[index+2] != '\0' && str[index+2] == '=')
                        array[1]++;
                } else
                    array[1] = index;
                return array;
            default:
                //If the character does not match a C operator, it is classified as garbage
                array[0] = GARBAGE;
                array[1] = index;
                return array;
        }
    }
    // any input not classified above already is garbage
    array[0] = GARBAGE;
    array[1] = index;
    return array;
}
/* function main(# of arguments, argument vector)
 * iterates through white space & comments from input string
 * when we find the start of a token, we call findToken to find the token type & last index of token
 * after findToken returns, we call printToken to print the token we found in the input string.
*/
int main(int argc, char **argv) {
    if(argv[1] == NULL) return 0;
    int length = strlen(argv[1]);
    int i = 0;
    //Loop Counter Bug Catcher
    int deBug = 0;
    for(i = 0; i < length && deBug < length; ++i) {
        char curr = argv[1][i];
        while(isspace(curr)) {
            curr = argv[1][++i]; // iterate until we find a non-whitespace character
        }
        if(i + 1 < length && curr == '/' && argv[1][i+1] == '/') {
            while(curr != '\n' && i < length) {
                curr = argv[1][++i];
            } 
        }
        if(i + 1 < length && curr == '/' && argv[1][i+1] == '*') {
            while(!(curr == '/' && argv[1][i-1] == '*') && i < length) {
                curr = argv[1][++i];
            }
            ++i;
        } 
        if(!(i < length)) return 0;    
        int *array = findToken(argv[1], i); // call find token to get pointer to array that stores [token type, last index of token]
        printToken(array[0], argv[1], i, array[1]); // call printToken to output our tokens
        i = array[1];
        free(array);
        deBug++;
    }
    return 0;
}

