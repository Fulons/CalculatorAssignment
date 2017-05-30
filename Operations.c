#include <math.h>
#include <stdio.h>

float multiplication(float x, float y){
    return x * y;
}

float division(float x, float y){
    return x / y;
}
    
float addition(float x, float y){
    return x + y;
}
    
float subtraction(float x, float y){
    return x - y;
}
    
float power(float x, float p){
    return pow(x, p);
}
    
float root(float x, float root){
    return pow(x, 1.0f / root);
}

void welcomeMessage(){
    printf("**********************************************\n");
    printf("*            FULONS PRESENTS                  *\n");
    printf("*        SUPER DUPER CALCULATOR               *\n");
    printf("*                                             *\n");
    printf("* * * * * * * * * * * * * * * * * * * * * * * *\n");
    printf("*              by: Fulons                     *\n");
    printf("***********************************************\n");
    printf("*Type ? for basic usage or see the ReadMe.txt *\n");
    printf("*ReadMe.txtfile for more in depth instructions*\n");
    printf("***********************************************\n");
}

void displayHelp(){
    printf("                     CALCULATOR USAGE                              \n");
    printf("                                                                   \n");
    printf("|Operation       |   Symbol  | Example usage | Example return     |\n");
    printf("|----------------|-----------|---------------|--------------------|\n");
    printf("|Addition        |   +       |   5 + 3       |   8                |\n");
    printf("|Subtraction     |   -       |   5 - 6       |  -1                |\n");
    printf("|Multiplication  |   * x X   |   3 * 5       |  15                |\n");
    printf("|Division        |   /       |   6 / 2       |   3                |\n");
    printf("|Power           |   ^       |   2 ^ 3       |   8                |\n");
    printf("|Root            |   v V     |   16 v 2      |   4                |\n");
    printf("|----------------|---------------------------|--------------------|\n");
    printf("|Brackets        |   ( )     |   5 * (3 + 2) |  25                |\n");
    printf("|----------------|---------------------------|--------------------|\n");
    printf("|Create variable | _varName= | _myVar = 5 + 2| myVar = 5 + 2 = 7  |\n");
    printf("|Use variable    | _varName_ | 5 + _myVar_   |  12                |\n");
    printf("|----------------|-----------|---------------|--------------------|\n");
    printf("|Use last result |   l L     |   5 + l       |  17                |\n");
    printf("|----------------|-----------|---------------|--------------------|\n");
    printf("|Show help       |   ?       |   ?           |Displays this table |\n");
    printf("|----------------|-----------|---------------|--------------------|\n");
    printf("|Print variables |   p       |   p           |Display list of vars|\n");
    printf("|----------------|-----------|---------------|--------------------|\n");
}