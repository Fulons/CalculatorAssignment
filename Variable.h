/* 
 * File:   Variable.h
 * Author: Fulons
 *
 * Created on 28 May 2017, 01:45
 */



#ifndef VARIABLE_H
#define VARIABLE_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "Typedefs.h"
#include "StringHelpers.h"
    
#define VARIABLE_TRIE_WIDTH 26
#define VARIABLE_MAX_NAME_LENGTH 21     //Name length will be one less as the last char will be '\0'
    
struct Variable{
    Variable* vars[VARIABLE_TRIE_WIDTH];
    Calculation* calc;
    bool isSelfContaining;
};

Variable* NewVariable();            //Allocates memory and initialises a Variable struct
void DeleteVariable(Variable* var); //Recuresively frees the memory of itself and children

//Recursive function to add a calculation to the variable trie
void AddVariable(Variable* node, const char* name, Calculation* calc, bool selfContaining);
//Recursive function that finds and returns a calculation in the trie or NULL if not found
Calculation* FindCalculation(Variable* node, const char* name);
//Prints variables to file or console
void PrintVariable(Variable* var, char* varName, bool printChildren, bool printCalc, FILE* file, bool printToSaveFile);

//Traverses the trie and recalculate all variables
void CalcTrie(Variable* var, Variable* varRoot);
//Traverses the trie and check every child for self containing variables
void CheckTrieVariablesForSelfContainingVariables(Variable* var, Variable* varRoot, string* varName);


#ifdef __cplusplus
}
#endif

#endif /* VARIABLE_H */

