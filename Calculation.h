/* 
 * File:   Calculation.h
 * Author: Fulons
 *
 * Created on 28 May 2017, 01:38
 */

#ifndef CALCULATION_H
#define CALCULATION_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "Typedefs.h"

#define VARIABLE_MAX_NAME_LENGTH 21

typedef enum operator{
    OP_ADDITION,
    OP_SUBTRACTION,
    OP_MULTIPLICATION,
    OP_DIVISION,
    OP_ROOT,
    OP_POWER,
    OP_NOOP,
    OP_EXTERNAL_CALCULATION
} operator;

//A binary tree data structure
struct calculation{
    operator op;            //Holds an enum value that determine the type of operation //The enum is found in Calculation.c
    double value;           //The calculated value
    calculation* operand1;
    calculation* operand2;
    calculation* parent;
    bool operand1Set;       //A helper when parsing the calculation string
    bool inBracket;         //A helper when parsing and printing the calculation string
    char* externalCalculationName;  //If op is external this will store the name of the variable it is stored in
};

calculation* newCalculation(calculation* parent);   //Allocates memory and initialise
void deleteCalculation(calculation* calc);          //Frees the memory and the memory of its children

void parseFile(const char* filePath, variable* varRoot);    //Parses a file of calculations
calculation* parse(char* str, double lastResult);           //Parses a string into a calculation tree
double calculate(calculation* calc, variable* node);        //Do the actual calculation of the tree

void printCalculation(calculation* calc, FILE* file, bool printResult); //To print to a loadable file printResult must be false

#ifdef __cplusplus
}
#endif

#endif /* CALCULATION_H */

