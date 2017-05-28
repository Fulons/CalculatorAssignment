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
#include "StringHelpers.h"

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
    if(!node || !name) return NULL;
    if(*name == '\0') return node->calc;
    else return findCalculation(node->vars[*name - 'a'], &name[1]);
}

void printVariable(variable* var, char* varName, bool printChildren, bool printCalc, FILE* file){
    if(var->calc) {
        if(!printCalc) fprintf(file, "%s = %g\n", varName, var->calc->value);
        else fprintf(file, "%s = ", varName);
        if(printCalc) printCalculation(var->calc, file);
    }    
    if(printChildren){
        int nameLength = strlen(varName);        
        for(int i = 0; i < 26; i++){
            if(var->vars[i]){
                varName[nameLength] = (char)i + 'a';
                varName[nameLength + 1] = '\0';
                printVariable(var->vars[i], varName, printChildren, printCalc, file);
            }
        }
    }
}

char* checkForVariablAsssignment(char* str){
    char* varBuffer = NULL;
    if(str[0] == '_' && IsCharacters('=', str)){
        varBuffer = malloc(sizeof(char) * 100);
        int i = 0;
        while(*str != '=') varBuffer[i++] = *(++str);
        varBuffer[i - 1] = '\0';
        ++str;
    }
    return varBuffer;
}