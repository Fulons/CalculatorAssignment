#include <stdlib.h> //for malloc
#include <stdio.h>  //for printf
#include <string.h> //for strlen
#include "Variable.h"
#include "Calculation.h"
#include "StringHelpers.h"

variable* newVariable(){
    variable* var = (variable*)malloc(sizeof(variable));
    for(int i = 0; i < 26; i++){
        var->vars[i] = NULL;
    }
    var->calc = NULL;
    return var;
}

void deleteVariable(variable* var){
    if(!var) return;
    for(int i = 0; i < VARIABLE_TRIE_WIDTH; i++)
        if(var->vars[i]){
            deleteVariable(var->vars[i]);
        }
    if(var->calc) free(var->calc);
    free(var);
}

void addVariable(variable* node, const char* name, calculation* calc){
    if(*name == '\0'){
        if(node->calc){
            if(askUserYesOrNo("You sure you want to replace this variable?"))
                node->calc = calc;
        }
        else node->calc = calc;
        return;
    } //TODO: need to check if node->calc is already set and ask user to confirm overwriting
    if(node->vars[*name - 'a'] == NULL) node->vars[*name - 'a'] = newVariable();
    addVariable(node->vars[*name - 'a'], &name[1], calc);
}

calculation* findCalculation(variable* node, const char* name){
    if(!node || !name) return NULL;
    if(*name == '\0') return node->calc;
    else return findCalculation(node->vars[*name - 'a'], &name[1]);
}

void printVariable(variable* var, char* varName, bool printChildren, bool printCalc, FILE* file, bool printToSaveFile){
    if(var->calc) {
        if(!printCalc) fprintf(file, "_%s = %g\n", varName, var->calc->value);
        else {
            if(printToSaveFile) fprintf(file, "C");
            fprintf(file, "_%s = ", varName);
            printCalculation(var->calc, file, !printToSaveFile);
        }
    }    
    if(printChildren){
        int nameLength = strlen(varName);        
        for(int i = 0; i < 26; i++){
            if(var->vars[i]){
                varName[nameLength] = (char)i + 'a';
                varName[nameLength + 1] = '\0';
                printVariable(var->vars[i], varName, printChildren, printCalc, file, printToSaveFile);
            }
        }
    }
}

char* checkForVariableAsssignment(char* str){
    char* varBuffer = NULL;
    if(str[0] == '_' && IsCharacters('=', str)){
        varBuffer = (char*)malloc(sizeof(char) * VARIABLE_MAX_NAME_LENGTH);
        int i = 0;
        while(*str != '=') {            
            varBuffer[i++] = *(++str);
            if((varBuffer[i - 1] < 'a' || varBuffer[i - 1] > 'z') && varBuffer[i - 1] != '='){ free(varBuffer); return NULL;}
        }
        varBuffer[i - 1] = '\0';
        ++str;
    }
    return varBuffer;
}

void calcTrie(variable* var, variable* varRoot){
    if(var->calc) calculate(var->calc, varRoot);
    for(int i = 0; i < VARIABLE_TRIE_WIDTH; i++){
        if(var->vars[i]) calcTrie(var->vars[i], varRoot);
    }
}