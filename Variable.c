#include <stdlib.h> //for malloc
#include <stdio.h>  //for printf
#include <string.h> //for strlen
#include "Variable.h"
#include "Calculation.h"
#include "StringHelpers.h"

//Allocates and initialises a new variable
Variable* NewVariable(){
    Variable* var = (Variable*)malloc(sizeof(Variable));
    for(int i = 0; i < 26; i++){
        var->vars[i] = NULL;
    }
    var->calc = NULL;
    return var;
}

//Free the memory of the variable and all its children
void DeleteVariable(Variable* var){
    if(!var) return;
    for(int i = 0; i < VARIABLE_TRIE_WIDTH; i++)
        if(var->vars[i]){
            DeleteVariable(var->vars[i]);
        }
    if(var->calc) free(var->calc);
    free(var);
}

//Recursive function to add a calculation to the variable trie
void AddVariable(Variable* node, const char* name, Calculation* calc){
    if(*name == '\0'){          //This indicates that the correct location has been found
        if(node->calc){         //If variable already exist ask the user if it should be replaced with the new one or not
            if(AskUserYesOrNo("You sure you want to replace this variable?"))
                node->calc = calc;
        }
        else node->calc = calc;
        return;
    }
    if(node->vars[*name - 'a'] == NULL) node->vars[*name - 'a'] = NewVariable();    //Allocate if needed
    AddVariable(node->vars[*name - 'a'], &name[1], calc);                           //Continue traversing the trie
}

//Recursive function that finds a calculation in the trie
Calculation* FindCalculation(Variable* node, const char* name){
    if(!node || !name) return NULL;
    if(*name == '\0') return node->calc;
    else return FindCalculation(node->vars[*name - 'a'], &name[1]);
}

//Recursive function that prints the stored variables in the trie
void PrintVariable(Variable* var, char* varName, bool printChildren, bool printCalc, FILE* file, bool printToSaveFile){
    if(var->calc) {
        if(!printCalc) fprintf(file, "_%s = %g\n", varName, var->calc->value);
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

//This checks a string if it has variable assignment and returns the name of the
//variable or NULL if not found or any error was encountered
//It also assures that the variable name is valid (only containing letters a-z)
char* CheckForVariableAsssignment(char* str){
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

//Traverses the trie and recalculate all variables
void CalcTrie(Variable* var, Variable* varRoot){
    if(var->calc) Calculate(var->calc, varRoot);
    for(int i = 0; i < VARIABLE_TRIE_WIDTH; i++){
        if(var->vars[i]) CalcTrie(var->vars[i], varRoot);
    }
}

//Traverses the trie and check every child for self containing variables
void CheckTrieVariablesForSelfContainingVariables(Variable* var, Variable* varRoot, string* varName){
    if(var->calc) CheckForSelfContainingVariable(var->calc, CreateArrayOfStrings(varName->str), varRoot);
    for(int i = 0; i < VARIABLE_TRIE_WIDTH; i++){
        if(var->vars[i]) {
            CheckTrieVariablesForSelfContainingVariables(var->vars[i], varRoot, PushChar(varName, i - 'a'));
            PopLastChar(varName);
        }
    }
}

//Recursive function that check for self containing variables in a calculation
bool CheckForSelfContainingVariable(Calculation* calc, ConstStringArray* n, Variable* varRoot){
    if(calc->operand1 && CheckForSelfContainingVariable(calc->operand1, n, varRoot)) return true;
    if(calc->operand2 && CheckForSelfContainingVariable(calc->operand2, n, varRoot)) return true;
    if(calc->op == OP_EXTERNAL_CALCULATION){
        if(FindName(n, calc->externalCalculationName)){
            printf("Found self containing variable: %s in %s", calc->externalCalculationName, n->array->str);
            DeleteArrayOfstring(n);
            return true;
        }
        else {
            Calculation* externalCalc = FindCalculation(varRoot, calc->externalCalculationName);
            if(externalCalc) {
                PushName(n, externalCalc->externalCalculationName);
                if(CheckForSelfContainingVariable(externalCalc, n, varRoot)) return true;
                PopLastString(n);
            }
        }
    }
    if(n->numNames == 1) DeleteArrayOfstring(n);
    return false;
}