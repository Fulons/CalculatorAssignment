/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <assert.h> //for assert
#include <stdlib.h> //for malloc
#include <stdio.h>  //for printf
#include "Calculation.h"
#include "StringHelpers.h"
#include "Operations.h"
#include "Variable.h"

enum operator{
    OP_ADDITION,
    OP_SUBTRACTION,
    OP_MULTIPLICATION,
    OP_DIVISION,
    OP_ROOT,
    OP_POWER,
    OP_NOOP,
    OP_EXTERNAL_CALCULATION
};

int getPresedenceInt(int op){
    switch(op){
        case OP_ADDITION:
        case OP_SUBTRACTION:
            return 0;
        case OP_MULTIPLICATION:
        case OP_DIVISION:
            return 1;
        case OP_ROOT:
        case OP_POWER:
            return 2;
        case OP_NOOP:
            return -1;
    }
}

bool hasHigherPrecedence(int op, int opcomp){                                   //returns true if op has higher precedence than opcomp    
    assert((getPresedenceInt(op) != -1) && (getPresedenceInt(opcomp) != -1));
    return op > opcomp;
}

int translateOperator(char op){
    switch(op){
        case '+': return OP_ADDITION;
        case '-': return OP_SUBTRACTION;
        case '*':
        case 'x':
        case 'X': return OP_MULTIPLICATION;
        case '/': return OP_DIVISION;
        case '^': return OP_POWER;
        case 'V':
        case 'v': return OP_ROOT;
    }
}


calculation* newCalculation(calculation* parent){
    calculation* calc = malloc(sizeof(calculation));
    calc->operand1 = NULL;
    calc->operand2 = NULL;
    calc->op = OP_NOOP;
    calc->parent = parent;
    calc->operand1Set = false;
    return calc;
}

void deleteCalculation(calculation* calc){
    
}
#define MAX_VAR_NAME_SIZE 20
calculation* parse(char* str, variable* varRoot, double lastResult){
    calculation* currentCalculation = newCalculation(NULL);
    calculation* root = currentCalculation;
    bool parsing = true;
    while(parsing){
        if(IsCharacters(*str, "0123456789.")){
            if(currentCalculation->operand1Set){
                currentCalculation->operand2 = newCalculation(currentCalculation);
                currentCalculation->operand2->value = strtod(str, &str);
            }
            else{
                currentCalculation->operand1 = newCalculation(currentCalculation);
                currentCalculation->operand1->value = strtod(str, &str);
                currentCalculation->operand1Set = true;
            }
        }
        else if(IsCharacters(*str, "+-*/^v")){
            if(currentCalculation->op == OP_NOOP){
                currentCalculation->op = translateOperator(*str);
                ++str;
            }
            else{
                if(hasHigherPrecedence(translateOperator(*str), currentCalculation->op)){
                    currentCalculation->operand2->operand1 = newCalculation(currentCalculation->operand2);
                    currentCalculation->operand2->operand1->value = currentCalculation->operand2->value;
                    currentCalculation->operand2->value = 0;
                    currentCalculation->operand2->operand1Set = true;
                    currentCalculation = currentCalculation->operand2;
                }
                else{
                    if(currentCalculation->parent == NULL){
                        currentCalculation->parent = newCalculation(NULL);
                        currentCalculation->parent->operand1 = currentCalculation;
                        currentCalculation = currentCalculation->parent;
                        currentCalculation->operand1Set = true;
                        root = currentCalculation;
                    }
                    else if(currentCalculation->parent->operand1 == currentCalculation){
                        currentCalculation->parent->operand1 = newCalculation(NULL);
                        currentCalculation->parent->operand1->operand1 = currentCalculation;
                        currentCalculation->parent->operand1->parent = currentCalculation->parent;
                        currentCalculation->parent = currentCalculation->parent->operand1;
                        currentCalculation = currentCalculation->parent->operand1;
                    }
                    else{
                        currentCalculation->parent->operand2 = newCalculation(NULL);
                        currentCalculation->parent->operand2->operand1 = currentCalculation;
                        currentCalculation->parent->operand2->parent = currentCalculation->parent;
                        currentCalculation->parent = currentCalculation->parent->operand2;
                        currentCalculation = currentCalculation->parent->operand2;
                    }
                }
            }
        }
        else if (*str == '('){
            if(currentCalculation->operand1Set){
                currentCalculation->operand2 = newCalculation(currentCalculation);
                currentCalculation = currentCalculation->operand2;
            }
            else{
                currentCalculation->operand1 = newCalculation(currentCalculation);
                currentCalculation = currentCalculation->operand1;
                currentCalculation->operand1Set = true;
            }
            ++str;
        }
        else if (*str == ')'){
            currentCalculation = currentCalculation->parent;
            ++str;
        }
        else if (*str == '_'){
            if(currentCalculation->operand1Set){
                currentCalculation->operand2 = newCalculation(currentCalculation);
                currentCalculation->operand2->op = OP_EXTERNAL_CALCULATION;
                char* varName = malloc(sizeof(char) * (MAX_VAR_NAME_SIZE + 1));
                int i = 0;
                do{ varName[i++] = *(++str);}
                while(*str != '_');
                varName[i - 1] = '\0';
                currentCalculation->operand2->externalCalculationName = varName;
                
                ++str;
            }
            else{
                currentCalculation->operand1 = newCalculation(currentCalculation);
                currentCalculation->operand1->op = OP_EXTERNAL_CALCULATION;
                char* varName = malloc(sizeof(char) * (MAX_VAR_NAME_SIZE + 1));
                int i = 0;
                do{ varName[i++] = *(++str);}
                while(*str != '_');
                varName[i - 1] = '\0';
                currentCalculation->operand1->externalCalculationName = varName;
                
                ++str;
                currentCalculation->operand1Set = true;
            }
        }
        else if(*str == 'L' || *str == 'l'){
            if(currentCalculation->operand1Set){
                currentCalculation->operand2 = newCalculation(currentCalculation);
                currentCalculation->operand2->value = lastResult;
            }
            else{
                currentCalculation->operand1 = newCalculation(currentCalculation);
                currentCalculation->operand1->value = lastResult;
                currentCalculation->operand1Set = true;
            }
        }
        else if(*str == '\0') parsing = false;
    }
    printf("Done parsing\n");
    return root;
}

bool askUserYesOrNo(char* str){
    while(true){
        printf("%s (Y/N):", str);
        char c;
        scanf(" %c", &c);
        if(c == 'Y' || c == 'y') return true;
        else if(c == 'N' || c == 'n') return false;
    }
}

double calculate(calculation* calc, variable* node){    //recursive
    switch(calc->op){
        case OP_ADDITION:       calc->value = calculate(calc->operand1, node) + calculate(calc->operand2, node); break;
        case OP_SUBTRACTION:    calc->value = calculate(calc->operand1, node) - calculate(calc->operand2, node); break;
        case OP_MULTIPLICATION: calc->value = calculate(calc->operand1, node) * calculate(calc->operand2, node); break;
        case OP_DIVISION:       calc->value = calculate(calc->operand1, node) / calculate(calc->operand2, node); break;
        case OP_POWER:          calc->value = power(calculate(calc->operand1, node), calculate(calc->operand2, node)); break;
        case OP_ROOT:           calc->value = root(calculate(calc->operand1, node), calculate(calc->operand2, node)); break;
        case OP_EXTERNAL_CALCULATION: {
            calculation* externalCalc = findCalculation(node, calc->externalCalculationName);
            if(!externalCalc) {
                printf("ERROR: Could not find variable:\t%s\nValue replaced by 0!", calc->externalCalculationName);
                calc->value = 0;
            }
            else calc->value = calculate(externalCalc, node);
        }
        case OP_NOOP:{
            assert(calc->operand1);
            assert(!calc->operand2);
            calc->value = calculate(calc->operand1, node);
        }
    }
    return calc->value;
}

void printCalculationRecursive(calculation* calc){
    if(calc->operand1) printCalculation(calc->operand1);
    else if(calc->op == OP_EXTERNAL_CALCULATION) printf("\"%s %g\"", calc->externalCalculationName, calc->value);
    else if(calc->op == OP_NOOP)return;
    else printf("%g", calc->value);
    switch(calc->op){
        case OP_ADDITION:               printf(" + "); break;
        case OP_SUBTRACTION:            printf(" - "); break;
        case OP_MULTIPLICATION:         printf(" * "); break;
        case OP_DIVISION:               printf(" / "); break;
        case OP_POWER:                  printf("^"); break;
        case OP_ROOT:                   printf("v"); break;
    }
    if(calc->operand2) printCalculation(calc->operand2);
}

void printCalculation(calculation* calc){
    printCalculationRecursive(calc);
    printf(" = %f\n", calc->value);
}