/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: 172030
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

int main(int argc, char** argv) {
    variable* varRoot = newVariable();
    bool keepLastCalculation = false;
    
    char* buffer = malloc(sizeof(char) * 1024); //remove ++buffer...will run out of scope...
    double lastResult = 0;                      //Holds result of last calculation
    welcomeMessage();                           //Display welcome message
    if(askUserYesOrNo("Do you want to save your variables?")){
        parseFile("SavedCalculations.txt", varRoot);  //Load last saved variables
    }

    while(true){
        calculation* currentCalculation;

        fgets(buffer, 1024, stdin);             //Get input from user
        removeWhitespace(buffer);               //Remove all whitespace
        if(buffer[0] == 'q') break;
        else if(buffer[0] == '?') displayHelp();
        else if(!IsCharacters(*buffer, "q+-*xX/^vV0123456789._(Ll?")){
            printf("Unexpected character at beginning of string: %c\nPlease see ReadMe.txt for usage or try again.", *buffer);
        }
        
        char* varName = checkForVariableAsssignment(buffer); //Extract varName if present
        if(varName){
            currentCalculation = parse(&buffer[(strlen(varName) + 2)], lastResult); //Parse the string excluding the underscore, varName and equals symbol
            addVariable(varRoot, varName, currentCalculation);                      //Add the variable to the trie
        }
        else currentCalculation = parse(&buffer[1], lastResult);    //No variable, just parse the string
        lastResult = calculate(currentCalculation, varRoot);        //Calculate the calculation
        if(!varName) {
            deleteCalculation(currentCalculation);  //Calculation is not stored so free the memory
            currentCalculation = NULL;
        }
        printf("Result:\t%g\n", lastResult);        //Display the result
    }
    if(askUserYesOrNo("Do you want to save your variables?")){
        FILE* file = fopen("SavedCalculations.txt", "w");
        char NameBuffer[21];
        NameBuffer[0] = '\0';
        printVariable(varRoot, NameBuffer, true, true, file, true);
    }
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