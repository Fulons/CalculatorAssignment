/* 
 * File:   main.c
 * Author: Fulons
 *
 * Created on 19 May 2017, 09:36
 */

#include <stdio.h>      //for:  printf, fgets, stdin
#include <stdlib.h>     //for:  EXIT_SUCCESS, malloc, strtod
#include <string.h>     //for:  strlen

#include "Operations.h"
#include "Calculation.h"
#include "Variable.h"
#include "StringHelpers.h"

#define INPUT_BUFFER_SIZE 1024
#define DEFAULT_SAVED_VARIABLE_FILENAME "SavedCalculations.txt"


//Returns true if the input has been handled by the the preprocess
bool PreProcess(char* buffer, Variable* varRoot){
    RemoveWhitespace(buffer);                   //Remove all whitespace    
    if(buffer[0] == '\0') return true;             //In some cases stdin will have some remainning whitespace this will resolve that
    ToLowerCase(buffer);
    if(!IsCharacters(*buffer, "q+-*x/^v0123456789._(l?p")){      //Check if first char in buffer is a legit
        printf("Unexpected character at beginning of string: %c\nPlease see ReadMe.txt for usage or try again.\n", *buffer);
        return true;
    }
    if(buffer[0] == 'q') return false;                             //Quits the main loop if user entered q //Might want to prompt user if they are sure
    else if(buffer[0] == '\0') return true;                    //In some cases fgets fill buffer with only whitespace
    else if(buffer[0] == '?') { displayHelp(); return true; }  //Display help if user enters ?
    else if(buffer[0] == 'p'){                              //Prints out all the variables currently in memory if user enters p
        char nameBuffer[VARIABLE_MAX_NAME_LENGTH];
        nameBuffer[0] = '\0';
        PrintVariable(varRoot, nameBuffer, true, true, stdout, false);
        return true;
    }
    return false;
}

int main(int argc, char** argv) {
    Variable* varRoot = NewVariable();                                  //Holds the root to the trie of variables
    char* buffer = (char*)malloc(sizeof(char) * INPUT_BUFFER_SIZE);     //Buffer to store user input
    double lastResult = 0;                                              //Holds result of last calculation
    welcomeMessage();                                                   //Display welcome message
    if(AskUserYesOrNo("Do you want to load variables from last session?")){
        ParseFile(DEFAULT_SAVED_VARIABLE_FILENAME, varRoot);            //Load last saved variables
    }

    while(true){
        fgets(buffer, INPUT_BUFFER_SIZE, stdin);    //Get input from user
        if(PreProcess(buffer, varRoot)) continue;
        else if(buffer[0] == 'q') break;    //Quits the main loop if user entered q //Might want to prompt user if they are sure
        //TODO: check for potential bug?
        char* varName = CheckForVariableAsssignment(buffer);                        //Extract varName if present
        Calculation* currentCalculation;
        if(varName){
            currentCalculation = Parse(&buffer[(strlen(varName) + 2)], lastResult); //Parse the string excluding the underscore, varName and equals symbol
            if(!currentCalculation) continue;            
            bool selfContaining = CheckForSelfContainingVariable(currentCalculation, CreateArrayOfStrings(varName), varRoot);
            AddVariable(varRoot, varName, currentCalculation, selfContaining);                      //Add the variable to the trie
            free(varName);  //choosing not to set varName to NULL as it needs to be checked a few lines down
            if(selfContaining) {
                CheckTrieVariablesForSelfContainingVariables(varRoot, varRoot, CreateString());
                continue;
            }
        }
        else currentCalculation = Parse(&buffer[0], lastResult);                    //No variable, just parse the string
        if(!currentCalculation) continue;
        lastResult = Calculate(currentCalculation, varRoot);                        //Calculate the calculation
        if(!varName) DeleteCalculation(currentCalculation);                         //Calculation is not stored so free the memory. Even though the memory varName points to is freed it will not be NULL
        printf("Result:\t%g\n", lastResult);                                        //Display the result
    }
    
    if(AskUserYesOrNo("Do you want to save your variables?")){                      //Save variables to file if user wish to
        FILE* file = fopen(DEFAULT_SAVED_VARIABLE_FILENAME, "w");
        char NameBuffer[VARIABLE_MAX_NAME_LENGTH];
        NameBuffer[0] = '\0';
        PrintVariable(varRoot, NameBuffer, true, true, file, true);
    }
    free(buffer);
    DeleteVariable(varRoot);
    return (EXIT_SUCCESS);
}

//1. Document Analysis
//2. Design Analysis
//3. Unit Testing
//4. Regression Testing
//5. Beta Testing
//6. Penetration Testing
//7. Load Testing
//8. UX Testing