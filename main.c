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


int main(int argc, char** argv) {
    variable* varRoot = newVariable();                                  //Holds the root to the trie of variables
    char* buffer = (char*)malloc(sizeof(char) * INPUT_BUFFER_SIZE);     //Buffer to store user input
    double lastResult = 0;                                              //Holds result of last calculation
    welcomeMessage();                                                   //Display welcome message
    if(askUserYesOrNo("Do you want to load variables from last session?")){
        parseFile(DEFAULT_SAVED_VARIABLE_FILENAME, varRoot);            //Load last saved variables
    }

    while(true){
        fgets(buffer, INPUT_BUFFER_SIZE, stdin);    //Get input from user
        removeWhitespace(buffer);                   //Remove all whitespace
        if(buffer[0] == '\0') continue;             //In some cases stdin will have some remainning whitespace this will resolve that
        toLowerCase(buffer);
        if(!IsCharacters(*buffer, "q+-*x/^v0123456789._(l?p")){      //Check if first char in buffer is a legit
            printf("Unexpected character at beginning of string: %c\nPlease see ReadMe.txt for usage or try again.\n", *buffer);
            continue;
        }
        if(buffer[0] == 'q') break;                             //Quits the main loop if user entered q //Might want to prompt user if they are sure
        else if(buffer[0] == '\0') continue;                    //In some cases fgets fill buffer with only whitespace
        else if(buffer[0] == '?') { displayHelp(); continue; }  //Display help if user enters ?
        else if(buffer[0] == 'p'){                              //Prints out all the variables currently in memory if user enters p
            char nameBuffer[VARIABLE_MAX_NAME_LENGTH];
            nameBuffer[0] = '\0';
            printVariable(varRoot, nameBuffer, true, true, stdout, false);
            continue;
        }
        //TODO: check for potential bug?
        char* varName = checkForVariableAsssignment(buffer);                        //Extract varName if present
        calculation* currentCalculation;
        if(varName){
            currentCalculation = parse(&buffer[(strlen(varName) + 2)], lastResult); //Parse the string excluding the underscore, varName and equals symbol
            if(!currentCalculation) continue;
            addVariable(varRoot, varName, currentCalculation);                      //Add the variable to the trie
            free(varName);  //choosing not to set varName to NULL as it needs to be checked a few lines down
        }
        else currentCalculation = parse(&buffer[0], lastResult);                    //No variable, just parse the string
        if(!currentCalculation) continue;
        lastResult = calculate(currentCalculation, varRoot);                        //Calculate the calculation
        if(!varName) deleteCalculation(currentCalculation);                         //Calculation is not stored so free the memory. Even though the memory varName points to is freed it will not be NULL
        printf("Result:\t%g\n", lastResult);                                        //Display the result
    }
    
    if(askUserYesOrNo("Do you want to save your variables?")){                      //Save variables to file if user wish to
        FILE* file = fopen(DEFAULT_SAVED_VARIABLE_FILENAME, "w");
        char NameBuffer[VARIABLE_MAX_NAME_LENGTH];
        NameBuffer[0] = '\0';
        printVariable(varRoot, NameBuffer, true, true, file, true);
    }
    free(buffer);
    deleteVariable(varRoot);
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