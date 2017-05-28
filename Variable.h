/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

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
    

struct variable{
    variable* vars[26];
    calculation* calc;
};

variable* newVariable();
void addVariable(variable* node, const char* name, calculation* calc);
calculation* findCalculation(variable* node, const char* name);
void printVariable(variable* var, char* varName, bool printChildren, bool printCalc, FILE* file);
char* checkForVariablAsssignment(char* str);

#ifdef __cplusplus
}
#endif

#endif /* VARIABLE_H */

