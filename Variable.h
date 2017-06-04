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
};

Variable* NewVariable();
void DeleteVariable(Variable* var);
void AddVariable(Variable* node, const char* name, Calculation* calc);
Calculation* FindCalculation(Variable* node, const char* name);
void PrintVariable(Variable* var, char* varName, bool printChildren, bool printCalc, FILE* file, bool printToSaveFile);
char* CheckForVariableAsssignment(char* str);
void CalcTrie(Variable* var, Variable* varRoot);
bool CheckForSelfContainingVariable(Calculation* calc, ConstStringArray* n, Variable* varRoot);

#ifdef __cplusplus
}
#endif

#endif /* VARIABLE_H */

