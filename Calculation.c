#include <assert.h>         //for assert
#include <stdlib.h>         //for malloc
#include <stdio.h>          //for printf
#include <string.h>         //for strlen
#include "Calculation.h"
#include "StringHelpers.h"
#include "Operations.h"
#include "Variable.h"
#include "StringHelpers.h"  //For debugPrint

int getPresedenceInt(operator op){
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

//returns true if op has higher precedence than opcomp
bool hasHigherPrecedence(operator op, operator opcomp){
    assert((getPresedenceInt(op) != -1) && (getPresedenceInt(opcomp) != -1));
    return (getPresedenceInt(op) > getPresedenceInt(opcomp));
}

//Translate char into operation enum value
operator translateOperator(char op){     
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
    calculation* calc = (calculation*)malloc(sizeof(calculation));
    calc->operand1 = NULL;
    calc->operand2 = NULL;
    calc->op = OP_NOOP;
    calc->parent = parent;
    calc->operand1Set = false;
    calc->inBracket = false;
    return calc;
}

//TODO: Free memory of externalCalculationName string????
void deleteCalculation(calculation* calc){
    if(calc->operand1) free(calc->operand1);
    if(calc->operand2) free(calc->operand2);
    if(calc->externalCalculationName) free(calc->externalCalculationName);
    free(calc);
}

calculation* parse(char* str, double lastResult){
    calculation* currentCalculation = newCalculation(NULL);
    calculation* root = currentCalculation; //The root might change while parsing
    bool parsing = true;
    while(parsing){
        if(IsCharacters(*str, "0123456789.")){      //Numerical operand, rely on newCalculation setting operand operation to OP_NOOP
            if(currentCalculation->operand1Set){    //TODO: ErrorCheck: operator has been set??
                currentCalculation->operand2 = newCalculation(currentCalculation);
                currentCalculation->operand2->value = strtod(str, &str);
            }
            else{
                currentCalculation->operand1 = newCalculation(currentCalculation);
                currentCalculation->operand1->value = strtod(str, &str);
                currentCalculation->operand1Set = true;
            }
        }
        else if(*str == 'l'){
            if(currentCalculation->operand1Set){
                currentCalculation->operand2 = newCalculation(currentCalculation);
                currentCalculation->operand2->value = lastResult;
                ++str;
            }
            else{
                currentCalculation->operand1 = newCalculation(currentCalculation);
                currentCalculation->operand1->value = lastResult;
                currentCalculation->operand1Set = true;
                ++str;
            }
        }
        else if(IsCharacters(*str, "+-*/^v")){      //Operation 
                                                    //TODO: ErrorCheck: First operand set or is this first part of equation so lastValue should be used?
            if(currentCalculation->op == OP_NOOP){
                currentCalculation->op = translateOperator(*str);
                ++str;
            }
            else{   //TODO: ErrorCheck: Make sure Operand2 has been set?
                    //TODO: These following cases are very similar and should be refactored
                if(hasHigherPrecedence(translateOperator(*str), currentCalculation->op)){
                    calculation* newCalc = newCalculation(currentCalculation);
                    newCalc->operand1 = currentCalculation->operand2;
                    newCalc->operand1Set = true;                    
                    currentCalculation->operand2 = newCalc;
                    newCalc->operand1->parent = newCalc;                    
                    currentCalculation = newCalc;
                }
                else{
                    if(currentCalculation->parent == NULL){                        
                        calculation* newCalc = newCalculation(NULL);
                        newCalc->operand1 = currentCalculation;
                        newCalc->operand1Set = true;                        
                        currentCalculation->parent = newCalc;
                        currentCalculation = newCalc;                        
                        root = currentCalculation;
                    }
                    else if(currentCalculation->parent->operand1 == currentCalculation){    //TODO: research if this case will ever happen
                        calculation* newCalc = newCalculation(currentCalculation->parent);
                        newCalc->operand1 = currentCalculation;
                        newCalc->operand1Set = true;
                        newCalc->parent->operand1 = newCalc;
                        currentCalculation->parent = newCalc;
                        currentCalculation = newCalc;
                    }
                    else{
                        calculation* newCalc = newCalculation(currentCalculation->parent);
                        newCalc->operand1 = currentCalculation;
                        newCalc->operand1Set = true;                        
                        newCalc->inBracket = currentCalculation->inBracket;
                        currentCalculation->inBracket = false;
                        newCalc->parent->operand2 = newCalc;
                        currentCalculation->parent = newCalc;
                        currentCalculation = newCalc;
                    }
                }
            }
        }
        else if (*str == '('){  //TODO: Set operator to * or handle it as an error if operation is not set
            if(currentCalculation->operand1Set){
                if(currentCalculation->op == OP_NOOP) currentCalculation->op = OP_MULTIPLICATION;   //if no operator before parenthesis set operator to multiplication
                currentCalculation->operand2 = newCalculation(currentCalculation);
                currentCalculation = currentCalculation->operand2;
                currentCalculation->inBracket = true;
            }
            else{
                currentCalculation->operand1 = newCalculation(currentCalculation);
                currentCalculation->operand1Set = true;
                currentCalculation = currentCalculation->operand1;
                currentCalculation->inBracket = true;
            }
            ++str;
        }
        else if (*str == ')'){  //TODO: Set operator to * or handle it as an error if operation is not set
            while(!currentCalculation->inBracket) currentCalculation = currentCalculation->parent;
            currentCalculation = currentCalculation->parent;
            ++str;
        }
        else if (*str == '_'){
            if(currentCalculation->operand1Set){
                currentCalculation->operand2 = newCalculation(currentCalculation);
                currentCalculation->operand2->op = OP_EXTERNAL_CALCULATION;
                char* varName = (char*)malloc(sizeof(char) * (VARIABLE_MAX_NAME_LENGTH));
                int i = 0;
                do{
                    varName[i++] = *(++str);
                    if(varName[i - 1] == '\0'){
                        errorPrint("Unexpected symbol in string. Enter ? for help.\n");
                        deleteCalculation(root);
                        return NULL;
                    }
                }
                while(*str != '_');
                varName[i - 1] = '\0';
                currentCalculation->operand2->externalCalculationName = varName;
                
                ++str;
            }
            else{
                currentCalculation->operand1 = newCalculation(currentCalculation);
                currentCalculation->operand1->op = OP_EXTERNAL_CALCULATION;
                char* varName = (char*)malloc(sizeof(char) * (VARIABLE_MAX_NAME_LENGTH + 1));
                int i = 0;
                do{
                    varName[i++] = *(++str);
                    if(varName[i - 1] == '\0'){
                        errorPrint("Unexpected symbol in string. Enter ? for help.\n");
                        deleteCalculation(root);
                        return NULL;
                    }
                }
                while(*str != '_');
                varName[i - 1] = '\0';
                currentCalculation->operand1->externalCalculationName = varName;
                
                ++str;
                currentCalculation->operand1Set = true;
            }
        }        
        else if(*str == '\0' || *str == '\r') parsing = false;
        else { errorPrint("Unexpected symbol in string. Enter ? for help.\n"); deleteCalculation(root); return NULL; }    //Free memory and return NULL to indicate error
    }
    debugPrint("Done parsing\n");
    return root;
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
                printf("ERROR: Could not find variable:\t%s\nValue replaced by 0!\n", calc->externalCalculationName);
                calc->value = 0;
            }
            else calc->value = calculate(externalCalc, node);
        }
        case OP_NOOP:{
            if(calc->operand1) calc->value = calculate(calc->operand1, node);
        }
    }
    return calc->value;
}

void printCalculationRecursive(calculation* calc, FILE* file, bool printVarValue){
    if(calc->inBracket) fprintf(file, "(");
    if(calc->operand1) printCalculationRecursive(calc->operand1, file, printVarValue);
    else if(calc->op == OP_EXTERNAL_CALCULATION){
        if(printVarValue) fprintf(file, "\"%s=%g\"", calc->externalCalculationName, calc->value);
        else fprintf(file, "_%s_", calc->externalCalculationName);
    }
    else fprintf(file, "%g", calc->value);
    switch(calc->op){
        case OP_ADDITION:               fprintf(file, " + "); break;
        case OP_SUBTRACTION:            fprintf(file, " - "); break;
        case OP_MULTIPLICATION:         fprintf(file, " * "); break;
        case OP_DIVISION:               fprintf(file, " / "); break;
        case OP_POWER:                  fprintf(file, "^"); break;
        case OP_ROOT:                   fprintf(file, "v"); break;
    }
    if(calc->operand2) printCalculationRecursive(calc->operand2, file, printVarValue);
    if(calc->inBracket) fprintf(file, ")");
}

void printCalculation(calculation* calc, FILE* file, bool printResult){
    printCalculationRecursive(calc, file, printResult);
    if(printResult) fprintf(file, " = %f\n", calc->value);
    else fprintf(file, "\r\n");
}

#define FILE_BUFFER_SIZE 1024

void parseFile(const char* filePath, variable* varRoot){
    char* buffer = (char*)malloc(sizeof(char) * FILE_BUFFER_SIZE);
    FILE* file = fopen(filePath, "r");
    if(!file) { printf("Could not find file: %s", filePath); return; }
    
    calculation* calc = NULL;
    double lastResult = 0;
    while(true){
        if(readFileToDelim(buffer, sizeof(char) * FILE_BUFFER_SIZE, "\n\r", file) == 0) break;        
        toLowerCase(buffer);        
        removeWhitespace(buffer);
        if(buffer[0] == 'c'){            
            char* varName = checkForVariableAsssignment(&buffer[1]);
            if(varName){
                calc = parse(&buffer[(strlen(varName) + 3)], lastResult);
                if(!calc) { free(varName); continue; }
                addVariable(varRoot, varName, calc);
                free(varName);
            }
            else {
                calc = parse(&buffer[1], lastResult);
                if(!calc) { free(varName); continue; }
            }
            //lastResult = calculate(calc, varRoot);
        }
        if(feof(file) != 0 || buffer[0] == '#') break;
    }//fseek(file, 0, SEEK_END);
    calcTrie(varRoot, varRoot);
    char name[VARIABLE_MAX_NAME_LENGTH];
    name[0] = '\0';
    printVariable(varRoot, name, true, true, stdout, false);
    fclose(file);
    free(buffer);
}