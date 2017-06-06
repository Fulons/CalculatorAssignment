#include <stdlib.h> //for malloc
#include <stdio.h>  //for printf
#include <string.h> //for strlen
#include "Variable.h"
#include "Calculation.h"
#include "StringHelpers.h"
#include "GeneralHelperFunctions.h"

Variable* NewVariable(){
    Variable* var = MyMalloc(1, Variable);
    for(int i = 0; i < 26; i++){
        var->vars[i] = NULL;
    }
    var->calc = NULL;
    var->isSelfContaining = false;
    return var;
}

void DeleteVariable(Variable* var){
    if(!var) return;
    for(int i = 0; i < VARIABLE_TRIE_WIDTH; i++)
        if(var->vars[i]){
            DeleteVariable(var->vars[i]);
            var->vars[i] = NULL;
        }
    if(var->calc) {
        DeleteCalculation(var->calc);
        var->calc = NULL;
    }
    MyFree(var, Variable);
}

bool AddVariable(Variable* node, const char* name, Calculation* calc, bool selfContaining){
    if(*name == '\0'){          //This indicates that the correct location has been found
        if(node->calc){         //If variable already exist ask the user if it should be replaced with the new one or not
            if(AskUserYesOrNo("You sure you want to replace this variable?")){
                node->calc = calc;
                node->isSelfContaining = selfContaining;
                return true;
            }
        }
        else {
            node->calc = calc;
            node->isSelfContaining = selfContaining;
        }
        return false;
    }
    if(node->vars[*name - 'a'] == NULL) node->vars[*name - 'a'] = NewVariable();    //Allocate if needed
    return AddVariable(node->vars[*name - 'a'], &name[1], calc, selfContaining);           //Continue traversing the trie
}

Calculation* FindCalculation(Variable* node, const char* name){
    if(!node || !name) return NULL;
    if(*name == '\0') return node->calc;
    else return FindCalculation(node->vars[*name - 'a'], &name[1]);
}

//Recursive function that prints the stored variables in the trie
void PrintVariable(Variable* var, char* varName, bool printChildren, bool printCalc, FILE* file, bool printToSaveFile){
    if(var->calc) {
        if(var->isSelfContaining && !printToSaveFile) { errorPrint("SELF CONTAINING: %s\n", varName); }
        else if(!printCalc) fprintf(file, "_%s = %g\n", varName, var->calc->value);
        else {
            if(printToSaveFile) fprintf(file, "C");
            fprintf(file, "_%s = ", varName);
            PrintCalculation(var->calc, file, !printToSaveFile);
        }        
    }    
    if(printChildren){
        int nameLength = strlen(varName);               //This should be the same as the current depth of the trie
        for(int i = 0; i < 26; i++){
            if(var->vars[i]){
                varName[nameLength] = (char)i + 'a';    //As the name of the variable is not stored with the calculation it needs to be compiled while traversing the trie
                varName[nameLength + 1] = '\0';
                PrintVariable(var->vars[i], varName, printChildren, printCalc, file, printToSaveFile);
            }
        }
    }
}

void CalcTrie(Variable* var, Variable* varRoot){
    if(var->calc && !var->isSelfContaining) Calculate(var->calc, varRoot);
    for(int i = 0; i < VARIABLE_TRIE_WIDTH; i++){
        if(var->vars[i]) CalcTrie(var->vars[i], varRoot);
    }
}

void CheckTrieVariablesForSelfContainingVariables(Variable* var, Variable* varRoot, string* varName){
    if(varName == NULL) varName = CreateString();
    if(var->calc){
        ConstStringArray* arr = CreateConstStringArray(varName->str);
        if(CheckForSelfContainingVariable(var->calc, arr, varRoot))
            var->isSelfContaining = true;
        else var->isSelfContaining = false;
        DeleteArrayOfstring(arr);
    }
    for(int i = 0; i < VARIABLE_TRIE_WIDTH; i++){
        if(var->vars[i]) {
            CheckTrieVariablesForSelfContainingVariables(var->vars[i], varRoot, PushChar(varName, i + 'a'));
            PopLastChar(varName);            
        }
    }
    if(varName->size == 0) DeleteString(varName);
}