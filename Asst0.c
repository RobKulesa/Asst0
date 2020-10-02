#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef enum {WORD, DECIMAL, OCTAL, HEX, FLOAT, OPERATOR, GARBAGE} Token;

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

int isoctal(char digit) {
    if(digit >= '0' && digit <= '7') return 1;
    return 0;
}
int printSubString(Token token, char* str, int begIndex, int endIndex) {
    //Print out the Token type

    char substring[endIndex - begIndex + 2]; 
    int i;
    for(i = begIndex; i <= endIndex; ++i) {
        substring[i-begIndex] = str[i];
    }
    substring[endIndex-begIndex + 1] = '\0';
    char* operatorName;

    switch(token){
        case WORD: printf("word: "); break;
        case DECIMAL: printf("decimal: "); break;
        case OCTAL: printf("octal: "); break;
        case HEX: printf("hex: "); break;
        case FLOAT: printf("float: "); break;
        case OPERATOR:
            operatorName = idOperator(substring);
            printf("%s: ", operatorName);
            break;
        case GARBAGE:
            printf("Garbage: ");
            break;
        default:
            printf("TOKEN ID ERROR: ");
            break;
    }
    
    for(i = begIndex; i <= endIndex; i++) {
        printf("%c", str[i]);
    }
    printf("\n");
    return 0;
}

int* findToken(char* str, int index) {
    int *array = malloc(2*sizeof(int)); // malloc space for 2 ints to be returned [token type, last index of token]
    char curr = str[index]; // current character being assessed
    if(isalpha(curr)) { // if token is alphabetic character
        array[0] = WORD; // pre-emptively assign token type to WORD
        int i = index; // at the end of the while loop, i will have the last index of the WORD token
        while(isalnum(curr) && !(isspace(str[i+1]) || str[i+1] == '\0')) curr = str[++i];
        if(!isalnum(curr)) //! Changed because while I was
            i--;
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
                return array;     
            }
            else if(isdigit(str[index+1])) { //! The first digit is 0 and the next character is numeric
                array[0] = OCTAL; // assume octal unless we find a non-octal digit in string
                //int i = index + 1;
                int i = index;
                
                while(isdigit(str[i]) && !(isspace(str[i+1]) || str[i+1] == '\0')) { //Checks for digits that would make it not octal
                    if(!isoctal(str[i])) 
                        array[0] = DECIMAL; //pre-emptively assigns token type to DECIMAL
                    ++i;
                }
                if(isspace(str[i+1]) || str[i+1] == '\0'){
                    if(!isoctal(str[i])) 
                        array[0] = DECIMAL;
                }
                
            

                //If we are dealing with an octal or a decimal (not float), we want to end the function as soon as possible
                if(str[i] != '.' || (str[i]== '.' && (str[i+1] == '\0' || !isdigit(str[i+1])) )) { 
                    if(!isdigit(str[i]))
                    --i;
                    array[1] = i;
                    return array;
                }
                else{ //If the first non-numeric character found is '.', then the entire token we are dealing with is a float, and we must figure out where it terminates
                    array[0] = FLOAT;
                    ++i; //Currently, str[i] points to the '.' character. We need to increment this.
                    while(isdigit(str[i]) && !(isspace(str[i+1]) || str[i+1] == '\0')){ //Iterates until non-numeric value is found
                        ++i;
                    }
                    

                    //TODO - Fix this conditional
                                                                                //THIS IS JUST ONE BIG CONDITIONAL LOL
                    if(str[i] != 'e' || str[i+1] == '\0'|| !(isdigit(str[i+1]) || (str[i+2]!= '\0' && isdigit(str[i+2]) && (str[i+1] == '+' || str[i+1] == '-')))  )//If the first non-numeric character found is not the letter e or if the string terminates too early, terminate token
                    {   
                        
                        if(!isdigit(str[i]))
                            --i;
                        array[1] = i;
                        return array;
                    }
                    else{
                        //TODO - Fix this stuff too
                        


                        if(str[i+1] == '+' || str[i+1] == '-')
                            i+=1; // If we do detect 'e', '-', and a digit following, skip to the index of the digit
                        do{
                            ++i;
                        }while(isdigit(str[i+1]) && !(isspace(str[i+1]) || str[i+1] == '\0'));
                        
                        
                        array[1] = i;
                        return array;
                    }
                }    
            } else{ //! The first number is 0, and the next character is not a digit
                if(str[index+1] == '\0' || str[index+1] != '.'||(str[index+1] == '.' && (str[index+2] == '\0' || !isdigit(str[index+2])))){
                    array[0] = DECIMAL;
                    array[1] = index;
                }else{
                    int i = index + 1;
                    array[0] = FLOAT;
                    ++i; //Currently, str[i] points to the '.' character. We need to increment this.
                    while(isdigit(str[i]) && !(isspace(str[i+1]) || str[i+1] == '\0')){ //Iterates until non-numeric value is found
                        ++i;
                    }
                    
                    //TODO - Fix this conditional
                    if(str[i] != 'e' || str[i+1] == '\0'|| !(isdigit(str[i+1]) || (str[i+2]!= '\0' && isdigit(str[i+2]) && (str[i+1] == '+' || str[i+1] == '-')))  )//If the first non-numeric character found is not the letter e or if the string terminates too early, terminate token
                    {   
                        if(!isdigit(str[i]))
                            --i;
                        array[1] = i;
                        return array;
                    }
                    else{
                        //TODO - Fix this stuff too
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
        } else {//! THE FIRST CHARACTER IS NOT 0
            array[0] = DECIMAL;
            int i = index;
            do{
                ++i;
            }while(isdigit(str[i]) && !(isspace(str[i+1]) || str[i+1] == '\0'));

            if(str[i] != '.' || (str[i]== '.' && (str[i+1] == '\0' || !isdigit(str[i+1])) )) {    
                if(!isdigit(str[i]))
                    --i;
                array[1] = i;
                return array;
            }else{ //If the first non-numeric character found is '.', then the entire token we are dealing with is a float, and we must figure out where it terminates
                    array[0] = FLOAT;
                    ++i; //Currently, str[i] points to the '.' character. We need to increment this.
                    while(isdigit(str[i]) && !(isspace(str[i+1]) || str[i+1] == '\0')){ //Iterates until non-numeric value is found
                        ++i;
                    }
                    
                    //TODO fix this conditional
                    if(str[i] != 'e' || str[i+1] == '\0'|| !(isdigit(str[i+1]) || (str[i+2]!= '\0' && isdigit(str[i+2]) && (str[i+1] == '+' || str[i+1] == '-')))  )//If the first non-numeric character found is not the letter e or if the string terminates too early, terminate token
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
                        }while(isdigit(str[i+1]) && !(isspace(str[i+1]) || str[i+1] == '\0'));
                        
                        
                        array[1] = i;
                        return array;
                    }
                }

            
        }
    }
    // at this point in code, token is either an operator or garbage
    
    //iterate through start of token until 
    // <<=garbage 
    else{
        array[0] = OPERATOR; // assume token is an operator
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
            case '/':                                               //! Code needs to be different to account for comment extra credit
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
                array[0] = GARBAGE;
                array[1] = index;
                return array;
        }
    }
    
    


    return array;
}

int main(int argc, char **argv) {
    int length = strlen(argv[1]);
    int i = 0;
    //Loop Counter Bug Catcher
    int deBug = 0;
    //to recognize single line comments, look for // followed by anything + '\n'
    //to recognize multi line comments, look for /* followed by anything + */
    for(i = 0; i < length && deBug < length; ++i) {
        char curr = argv[1][i];
        while(isspace(curr)) {
            curr = argv[1][++i]; // iterate until we find a non-whitespace character
        }
        
/*         if(i + 1 < length && curr == '/' && argv[1][i+1] == '/') {
            while(curr != '\n' && curr != '\r') {
                curr = argv[1][++i];
            } 
        }
        if(i + 1 < length && curr == '/' && argv[1][i+1] == '*') {
            while(curr != '*' && argv[1][i-1] != '/') {
                curr = argv[1][++i];
            }
        } */
        
        if(!(i < length)) return 0;    
        int *array = findToken(argv[1], i); // call find token to get pointer to array that stores [token type, last index of token]
        printSubString(array[0], argv[1], i, array[1]); // call printSubString to output our tokens
        
        i = array[1];
        free(array);
        deBug++;
    }
    //printf("\n%d\n", i);
    return 0;
}

