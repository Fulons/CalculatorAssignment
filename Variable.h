/* 
 * File:   Variable.h
 * Author: Fulons
 *
 * Created on 28 May 2017, 01:45
 */



#ifndef VARIABLE_H
#define VARIABLE_H

#include "Typedefs.h"

#ifdef __cplusplus
extern "C" {
#endif
    
#define VARIABLE_TRIE_WIDTH 26
#define VARIABLE_MAX_NAME_LENGTH 21     //Name length will be one less as the last char will be '\0'
    
struct variable{
    variable* vars[VARIABLE_TRIE_WIDTH];
    calculation* calc;
};

variable* newVariable();
void deleteVariable(variable* var);
void addVariable(variable* node, const char* name, calculation* calc);
calculation* findCalculation(variable* node, const char* name);
void printVariable(variable* var, char* varName, bool printChildren, bool printCalc, FILE* file, bool printToSaveFile);
char* checkForVariableAsssignment(char* str);
void calcTrie(variable* var, variable* varRoot);

#ifdef __cplusplus
}
#endif

#endif /* VARIABLE_H */

