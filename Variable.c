/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <stdlib.h> //for malloc
#include <stdio.h>  //for printf
#include <string.h> //for strlen
#include "Variable.h"
#include "Calculation.h"


variable* newVariable(){
    variable* var = malloc(sizeof(variable));
    for(int i = 0; i < 26; i++){
        var->vars[i] = NULL;
    }
    var->calc = NULL;
    return var;
}

void addVariable(variable* node, const char* name, calculation* calc){
    if(*name == '\0'){ node->calc = calc; return; } //TODO: need to check if node->calc is already set and ask user to confirm overwriting
    if(node->vars[*name - 'a'] == NULL) node->vars[*name - 'a'] = newVariable();
    addVariable(node->vars[*name - 'a'], &name[1], calc);
}

calculation* findCalculation(variable* node, const char* name){
    if(!name) return NULL;
    if(*name == '\0') return node->calc;
    else return findCalculation(node->vars[*name - 'a'], &name[1]);
}

void printVariable(variable* var, char* varName, bool printChildren, bool printCalc){
    printf("%s = %g\n", varName, var->calc->value);
    if(printCalculation) printCalculation(var->calc);
    if(printChildren){
        int nameLength = strlen(varName);        
        for(int i = 0; i < 26; i++){
            if(var->vars[i]){
                varName[nameLength] = (char)i;
                varName[nameLength + 1] = '\0';
                printVariable(var->vars[i], varName, printChildren, printCalc);
            }
        }
    }
}