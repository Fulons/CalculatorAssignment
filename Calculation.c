#include <assert.h>         //for assert
#include <stdlib.h>         //for malloc
#include <stdio.h>          //for printf
#include <string.h>         //for strlen
#include "Calculation.h"
#include "StringHelpers.h"
#include "Operations.h"
#include "Variable.h"
#include "StringHelpers.h"  //For debugPrint

//Helper function for hasHigherPrecedence function
int GetPresedenceInt(operator op){
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
bool HasHigherPrecedence(operator op, operator opcomp){
    assert((GetPresedenceInt(op) != -1) && (GetPresedenceInt(opcomp) != -1));
    return (GetPresedenceInt(op) > GetPresedenceInt(opcomp));
}

//Translate char into operation enum value
operator TranslateOperator(char op){     
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
        default: debugPrint("Is not an operator: "); printf("%c", op); return OP_NOOP;
    }
}

//Allocates and initiates a new calculation
Calculation* NewCalculation(Calculation* parent){
    Calculation* calc = (Calculation*)malloc(sizeof(Calculation));
    calc->operand1 = NULL;
    calc->operand2 = NULL;
    calc->op = OP_NOOP;     //This is relied upon
    calc->parent = parent;
    calc->operand1Set = false;
    calc->inBracket = false;
    return calc;
}

//Recursively frees the memory of the calculation and all its children
void DeleteCalculation(Calculation* calc){
    if(calc->operand1) free(calc->operand1);
    if(calc->operand2) free(calc->operand2);
    if(calc->externalCalculationName) free(calc->externalCalculationName);
    free(calc);
}

//This is one of the main functions of the program
//It parses the string into the binary tree of calculations
Calculation* Parse(char* str, double lastResult){
    Calculation* currentCalculation = NewCalculation(NULL);
    Calculation* root = currentCalculation; //The root might change while parsing. See design documentation.
    bool parsing = true;
    while(parsing){
        if(IsCharacters(*str, "0123456789.")){      //Numerical operand, rely on NewCalculation setting operand operation to OP_NOOP
            if(currentCalculation->operand1Set){    //TODO: ErrorCheck: operator has been set??
                currentCalculation->operand2 = NewCalculation(currentCalculation);
                currentCalculation->operand2->value = strtod(str, &str);
            }
            else{
                currentCalculation->operand1 = NewCalculation(currentCalculation);
                currentCalculation->operand1->value = strtod(str, &str);
                currentCalculation->operand1Set = true;
            }
        }
        else if(*str == 'l'){                       //Signals the use of lastResult
            if(currentCalculation->operand1Set){
                currentCalculation->operand2 = NewCalculation(currentCalculation);
                currentCalculation->operand2->value = lastResult;
                ++str;
            }
            else{
                currentCalculation->operand1 = NewCalculation(currentCalculation);
                currentCalculation->operand1->value = lastResult;
                currentCalculation->operand1Set = true;
                ++str;
            }
        }
        else if(IsCharacters(*str, "+-*/^v")){      //Operation 
                                                    //TODO: ErrorCheck: First operand set or is this first part of equation so lastValue should be used?
            if(currentCalculation->op == OP_NOOP){
                assert(currentCalculation->operand1Set);
                currentCalculation->op = TranslateOperator(*str);
                ++str;
            }
            else{   //TODO: ErrorCheck: Make sure Operand2 has been set?
                    //TODO: These following cases are very similar and should be refactored
                if(HasHigherPrecedence(TranslateOperator(*str), currentCalculation->op)){
                    Calculation* newCalc = NewCalculation(currentCalculation);
                    newCalc->operand1 = currentCalculation->operand2;
                    newCalc->operand1Set = true;                    
                    currentCalculation->operand2 = newCalc;
                    newCalc->operand1->parent = newCalc;                    
                    currentCalculation = newCalc;
                }
                else{
                    if(currentCalculation->parent == NULL){                        
                        Calculation* newCalc = NewCalculation(NULL);
                        newCalc->operand1 = currentCalculation;
                        newCalc->operand1Set = true;                        
                        currentCalculation->parent = newCalc;
                        currentCalculation = newCalc;                        
                        root = currentCalculation;
                    }
                    else if(currentCalculation->parent->operand1 == currentCalculation){    //TODO: research if this case will ever happen
                        Calculation* newCalc = NewCalculation(currentCalculation->parent);
                        newCalc->operand1 = currentCalculation;
                        newCalc->operand1Set = true;
                        newCalc->parent->operand1 = newCalc;
                        currentCalculation->parent = newCalc;
                        currentCalculation = newCalc;
                    }
                    else{
                        Calculation* newCalc = NewCalculation(currentCalculation->parent);
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
        else if (*str == '('){
            if(currentCalculation->operand1Set){
                if(currentCalculation->op == OP_NOOP) currentCalculation->op = OP_MULTIPLICATION;   //if no operator before parenthesis set operator to multiplication
                currentCalculation->operand2 = NewCalculation(currentCalculation);
                currentCalculation = currentCalculation->operand2;
                currentCalculation->inBracket = true;
            }
            else{
                currentCalculation->operand1 = NewCalculation(currentCalculation);
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
                currentCalculation->operand2 = NewCalculation(currentCalculation);
                currentCalculation->operand2->op = OP_EXTERNAL_CALCULATION;
                char* varName = (char*)malloc(sizeof(char) * (VARIABLE_MAX_NAME_LENGTH));
                int i = 0;
                do{
                    varName[i++] = *(++str);
                    if(varName[i - 1] == '\0'){
                        errorPrint("Unexpected symbol in string. Enter ? for help.\n");
                        DeleteCalculation(root);
                        return NULL;
                    }
                }
                while(*str != '_');
                varName[i - 1] = '\0';
                currentCalculation->operand2->externalCalculationName = varName;
                
                ++str;
            }
            else{
                currentCalculation->operand1 = NewCalculation(currentCalculation);
                currentCalculation->operand1->op = OP_EXTERNAL_CALCULATION;
                char* varName = (char*)malloc(sizeof(char) * (VARIABLE_MAX_NAME_LENGTH + 1));
                int i = 0;
                do{
                    varName[i++] = *(++str);
                    if(varName[i - 1] == '\0'){
                        errorPrint("Unexpected symbol in string. Enter ? for help.\n");
                        DeleteCalculation(root);
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
        else { errorPrint("Unexpected symbol in string. Enter ? for help.\n"); DeleteCalculation(root); return NULL; }    //Free memory and return NULL to indicate error
    }
    debugPrint("Done parsing\n");
    return root;
}

double Calculate(Calculation* calc, Variable* node){    //recursive
    switch(calc->op){
        case OP_ADDITION:       calc->value = Calculate(calc->operand1, node) + Calculate(calc->operand2, node); break;
        case OP_SUBTRACTION:    calc->value = Calculate(calc->operand1, node) - Calculate(calc->operand2, node); break;
        case OP_MULTIPLICATION: calc->value = Calculate(calc->operand1, node) * Calculate(calc->operand2, node); break;
        case OP_DIVISION:       calc->value = Calculate(calc->operand1, node) / Calculate(calc->operand2, node); break;
        case OP_POWER:          calc->value = power(Calculate(calc->operand1, node), Calculate(calc->operand2, node)); break;
        case OP_ROOT:           calc->value = root(Calculate(calc->operand1, node), Calculate(calc->operand2, node)); break;
        case OP_EXTERNAL_CALCULATION: {
            Calculation* externalCalc = FindCalculation(node, calc->externalCalculationName);
            if(!externalCalc) {
                printf("ERROR: Could not find variable:\t%s\nValue replaced by 0!\n", calc->externalCalculationName);
                calc->value = 0;
            }
            else calc->value = Calculate(externalCalc, node);
        }
        case OP_NOOP:{
            if(calc->operand1) calc->value = Calculate(calc->operand1, node);
        }
    }
    return calc->value;
}

void PrintCalculationRecursive(Calculation* calc, FILE* file, bool printResult){
    if(calc->inBracket) fprintf(file, "(");
    if(calc->operand1) PrintCalculationRecursive(calc->operand1, file, printResult);
    else if(calc->op == OP_EXTERNAL_CALCULATION){
        if(printResult) fprintf(file, "\"%s=%g\"", calc->externalCalculationName, calc->value);
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
    if(calc->operand2) PrintCalculationRecursive(calc->operand2, file, printResult);
    if(calc->inBracket) fprintf(file, ")");
}

void PrintCalculation(Calculation* calc, FILE* file, bool printResult){
    PrintCalculationRecursive(calc, file, printResult);
    if(printResult) fprintf(file, " = %f\n", calc->value);
    else fprintf(file, "\r\n");
}

#define FILE_BUFFER_SIZE 1024

void ParseFile(const char* filePath, Variable* varRoot){
    char* buffer = (char*)malloc(sizeof(char) * FILE_BUFFER_SIZE);
    FILE* file = fopen(filePath, "r");
    if(!file) { printf("Could not find file: %s", filePath); return; }
    
    Calculation* calc = NULL;
    double lastResult = 0;
    while(true){
        if(ReadFileToDelim(buffer, sizeof(char) * FILE_BUFFER_SIZE, "\n\r", file) == 0) break;        
        ToLowerCase(buffer);        
        RemoveWhitespace(buffer);
        if(buffer[0] == 'c'){            
            char* varName = CheckForVariableAsssignment(&buffer[1]);
            if(varName){
                calc = Parse(&buffer[(strlen(varName) + 3)], lastResult);
                if(!calc) { free(varName); continue; }
                AddVariable(varRoot, varName, calc);
                CheckForSelfContainingVariable(calc,CreateArrayOfStrings(varName), varRoot);
                free(varName);
            }
            else {
                calc = Parse(&buffer[1], lastResult);
                if(!calc) { free(varName); continue; }
            }
            //lastResult = Calculate(calc, varRoot);
        }
        if(feof(file) != 0 || buffer[0] == '#') break;
    }//fseek(file, 0, SEEK_END);
    CalcTrie(varRoot, varRoot);
    char name[VARIABLE_MAX_NAME_LENGTH];
    name[0] = '\0';
    PrintVariable(varRoot, name, true, true, stdout, false);
    fclose(file);
    free(buffer);
}

//typedef union{
//    char* c;
//    struct{
//        char* c;
//        int i;
//    };
//} array;
//
//void whohoo(){
//    array arr;
//    arr[2] = 2;???
//}
