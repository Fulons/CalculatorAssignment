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
    
    char* buffer = malloc(sizeof(char) * 1024); //remove ++buffer...will run out of scope...
    double lastResult = 0;
    parseFile("C:/Users/Fulons/Desktop/Calculator/Calculations.txt", varRoot);
    while(true){
        WelcomeMessage();
        fgets(buffer, 1024, stdin);
        removeWhitespace(buffer);
        char* varBuffer = checkForVariablAsssignment(buffer);
        
        calculation* currentCalculation = parse(buffer, lastResult);
        lastResult = calculate(currentCalculation, varRoot);
        
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