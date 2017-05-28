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
#include "Operations.h"
#include "Calculation.h"
#include "Variable.h"
#include "StringHelpers.h"

int main(int argc, char** argv) {
    variable* varRoot = newVariable();
    bool keepLastCalculation = false;
    
    char* buffer = malloc(sizeof(char) * 1024);
    double lastResult = 0;
    while(true){
        WelcomeMessage();
        fgets(buffer, 1024, stdin);
        removeWhitespace(buffer);
        char* varBuffer = NULL;
        if(buffer[0] == '_' && IsCharacters('=', buffer)){
            varBuffer = malloc(sizeof(char) * 100);
            int i = 0;
            while(*buffer != '=') varBuffer[i++] = *(++buffer);
            varBuffer[i - 1] = '\0';
            ++buffer;
        }
        
        calculation* currentCalculation = parse(buffer, varRoot, lastResult);        
        double lastResult = calculate(currentCalculation, varRoot);
        
        if(varBuffer)   addVariable(varRoot, varBuffer, currentCalculation);
        else            deleteCalculation(currentCalculation);
        
        printf("Result:\t%g\n", lastResult);
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