/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Variable.h"
#include "Calculation.h"
#include "GeneralHelperFunctions.h"
#include "Typedefs.h"

#ifdef DEBUG
    int G_numAllocations = 0;
    int G_numDeallocations = 0;
    ConstStringArray* G_allocatedTypes = NULL;
    ConstStringArray* G_deallocatedTypes = NULL;
#endif
    
char* CheckForVariableAsssignment(char* str){
    char* varBuffer = NULL;
    if(str[0] == '_' && IsCharacters('=', str)){
        varBuffer = MyMalloc(VARIABLE_MAX_NAME_LENGTH, char);
        int i = 0;
        while(*str != '=') {            
            varBuffer[i++] = *(++str);
            if((varBuffer[i - 1] < 'a' || varBuffer[i - 1] > 'z') && varBuffer[i - 1] != '='){
                MyFree(varBuffer, char);
                return NULL;
            }
        }
        varBuffer[i - 1] = '\0';
        ++str;
    }
    return varBuffer;
}

bool PreProcess(char* buffer, Variable* varRoot){
    RemoveWhitespace(buffer);
    if(buffer[0] == '\0') return true;          //In some cases stdin will have some remaining whitespace in the buffer this will resolve that
    ToLowerCase(buffer);
    if(!IsCharacters(*buffer, "q+-*x/^v0123456789._(l?p")){      //Check if first char in buffer is a legit
        printf("Unexpected character at beginning of string: %c\nPlease see ReadMe.txt for usage or try again.\n", *buffer);
        return true;
    }
    if(buffer[0] == 'q') return false;                          //Quits the main loop if user entered q //Might want to prompt user if they are sure
    else if(buffer[0] == '?') { DisplayHelp(); return true; }   //Display help if user enters ?
    else if(buffer[0] == 'p'){                                  //Prints out all the variables currently in memory if user enters p
        char nameBuffer[VARIABLE_MAX_NAME_LENGTH];
        nameBuffer[0] = '\0';
        PrintVariable(varRoot, nameBuffer, true, true, stdout, false);
        return true;
    }
    return false;
}

bool AskUserYesOrNo(char* str){
    while(true){
        printf("%s (Y/N):", str);
        char c;
        scanf(" %c", &c);
        if(c == 'Y' || c == 'y') return true;
        else if(c == 'N' || c == 'n') return false;
    }
}

void WelcomeMessage(){
    printf("***********************************************\n");
    printf("*            FULONS PRESENTS                  *\n");
    printf("*        SUPER DUPER CALCULATOR               *\n");
    printf("*                                             *\n");
    printf("* * * * * * * * * * * * * * * * * * * * * * * *\n");
    printf("*              by: Fulons                     *\n");
    printf("***********************************************\n");
    printf("*Type ? for basic usage or see the ReadMe.txt *\n");
    printf("*ReadMe.txtfile for more in depth instructions*\n");
    printf("***********************************************\n");
}

void DisplayHelp(){
    printf("                     CALCULATOR USAGE                              \n");
    printf("                                                                   \n");
    printf("|Operation       |   Symbol  | Example usage | Example return     |\n");
    printf("|----------------|-----------|---------------|--------------------|\n");
    printf("|Addition        |   +       |   5 + 3       |   8                |\n");
    printf("|Subtraction     |   -       |   5 - 6       |  -1                |\n");
    printf("|Multiplication  |   * x X   |   3 * 5       |  15                |\n");
    printf("|Division        |   /       |   6 / 2       |   3                |\n");
    printf("|Power           |   ^       |   2 ^ 3       |   8                |\n");
    printf("|Root            |   v V     |   16 v 2      |   4                |\n");
    printf("|----------------|---------------------------|--------------------|\n");
    printf("|Brackets        |   ( )     |   5 * (3 + 2) |  25                |\n");
    printf("|----------------|---------------------------|--------------------|\n");
    printf("|Create variable | _varName= | _myVar = 5 + 2| myVar = 5 + 2 = 7  |\n");
    printf("|Use variable    | _varName_ | 5 + _myVar_   |  12                |\n");
    printf("|----------------|-----------|---------------|--------------------|\n");
    printf("|Use last result |   l L     |   5 + l       |  17                |\n");
    printf("|----------------|-----------|---------------|--------------------|\n");
    printf("|Show help       |   ?       |   ?           |Displays this table |\n");
    printf("|----------------|-----------|---------------|--------------------|\n");
    printf("|Print variables |   p       |   p           |Display list of vars|\n");
    printf("|----------------|-----------|---------------|--------------------|\n");
}

void ProcessBuffer(char* buffer, double* lastResult, Variable* varRoot, bool checkSelfContaining, bool printVarName, bool printCalc){
    char* varName = CheckForVariableAsssignment(buffer);
    Calculation* calc;
    if(varName){
        calc = Parse(&buffer[strlen(varName) + 2], *lastResult);
        if(!calc) return;
        bool selfContaining = false;
        if(checkSelfContaining || printCalc){
            ConstStringArray* arr = CreateConstStringArray(varName);
            selfContaining = CheckForSelfContainingVariable(calc, arr, varRoot);
            DeleteArrayOfstring(arr);            
        }
        if(AddVariable(varRoot, varName, calc, selfContaining)){
            infoPrint("Recalculating self containing variables...\n");
            CheckTrieVariablesForSelfContainingVariables(varRoot, varRoot, CreateString()); //Rechecks entire trie for updates on selfcontaining variables
        }
        if(printCalc){
            if(!selfContaining){
                *lastResult = Calculate(calc, varRoot);
                printf("%s = ", varName);
                PrintCalculation(calc, stdout, true);
            }
        }
        else if(printVarName) {infoPrint("Loaded variable: %s\n", varName);}
        MyFree(varName, char);
    }
    else{
        calc = Parse(buffer, *lastResult);
        if(calc) {
            *lastResult = Calculate(calc, varRoot);
            PrintCalculation(calc, stdout, true);
            DeleteCalculation(calc);
        }
    }
}