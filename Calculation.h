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
#include "StringHelpers.h"

#define VARIABLE_MAX_NAME_LENGTH 21

typedef enum operator{
    OP_ADDITION,
    OP_SUBTRACTION,
    OP_MULTIPLICATION,
    OP_DIVISION,
    OP_ROOT,
    OP_POWER,
    OP_NOOP,                    //Default value, means the calculation have a constant value
    OP_EXTERNAL_CALCULATION     //This indicates an external calculation stored in a variable.
} operator;

//A binary tree data structure
struct Calculation{
    operator op;            //Holds an enum value that determine the type of operation
    double value;           //The calculated value
    Calculation* operand1;
    Calculation* operand2;
    Calculation* parent;
    bool operand1Set;       //A helper when parsing the calculation string. TODO: Might not be needed as operand1 should be NULL if not set
    bool inBracket;         //A helper when parsing and printing the calculation string
    char* externalCalculationName;  //If op is external this will store the name of the variable it is stored in
};

Calculation* NewCalculation(Calculation* parent);   //Allocates memory and initialise a calculation struct
void DeleteCalculation(Calculation* calc);          //Frees the memory and the memory of its children

void ParseFile(const char* filePath, Variable* varRoot);    //Parses a file of calculations
Calculation* Parse(char* str, double lastResult);           //Parses a string containing a single calculation into a calculation tree
double Calculate(Calculation* calc, Variable* node);        //Do the actual calculation of a single calculation tree

void PrintCalculation(Calculation* calc, FILE* file, bool printResult); //To print to a loadable file printResult must be false

//Recursive function that check for self containing variables in a calculation.
//To avoid infinite loops
bool CheckForSelfContainingVariable(Calculation* calc, ConstStringArray* n, Variable* varRoot);

#ifdef __cplusplus
}
#endif

#endif /* CALCULATION_H */