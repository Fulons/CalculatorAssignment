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
#include "GeneralHelperFunctions.h"

#define INPUT_BUFFER_SIZE 1024
#define DEFAULT_SAVED_VARIABLE_FILENAME "SavedCalculations.txt"

int main(int argc, char** argv) {
    MEMORY_DEBUG_INIT
    Variable* varRoot = NewVariable();                                  //Holds the root to the trie of variables
    char* buffer = MyMalloc(INPUT_BUFFER_SIZE, char);                   //Buffer to store user input
    double lastResult = 0;                                              //Holds result of last calculation
    WelcomeMessage();                                                   //Display welcome message
    if(AskUserYesOrNo("Do you want to load variables from last session?")){
        ParseFile(DEFAULT_SAVED_VARIABLE_FILENAME, varRoot);            //Load last saved variables
    }

    while(true){
        fgets(buffer, INPUT_BUFFER_SIZE, stdin);    //Get input from user
        if(PreProcess(buffer, varRoot)) continue;
        else if(buffer[0] == 'q') break;    //Quits the main loop if user entered q //Might want to prompt user if they are sure
        ProcessBuffer(buffer, &lastResult, varRoot, true, false, true);
    }
    
    if(AskUserYesOrNo("Do you want to save your variables?")){                      //Save variables to file if user wish to
        FILE* file = fopen(DEFAULT_SAVED_VARIABLE_FILENAME, "w");
        if(!file) { errorPrint("Could not open save file: %s", DEFAULT_SAVED_VARIABLE_FILENAME); }
        char NameBuffer[VARIABLE_MAX_NAME_LENGTH];
        NameBuffer[0] = '\0';
        PrintVariable(varRoot, NameBuffer, true, true, file, true);
        fclose(file);
    }
    MyFree(buffer, char);
    DeleteVariable(varRoot);
    MEMORY_DEBUG_PRINT_ALLOCATIONS
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