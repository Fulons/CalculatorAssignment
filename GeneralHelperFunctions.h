/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GeneralHelperFunctions.h
 * Author: Fulons
 *
 * Created on 05 June 2017, 03:19
 */

#ifndef GENERALHELPERFUNCTIONS_H
#define GENERALHELPERFUNCTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

void ProcessBuffer(char* buffer, double* lastResult, Variable* varRoot, bool checkSelfContaining, bool printVarName, bool printCalc);
    
//This checks a string if it has variable assignment and returns the name of the
//variable or NULL if not found or any error was encountered
//It also assures that the variable name is valid (only containing letters a-z)
char* CheckForVariableAsssignment(char* str);

//Returns true if the input has been handled by the the preprocess
//Just a helper function to the main loop to make it look less messy
bool PreProcess(char* buffer, Variable* varRoot);

//Prints "str (Y/N):" and wait for user to enter y/n, if input is invalid it will ask again until a valid input is given
//Returns true for y/Y and false for n/N
bool AskUserYesOrNo(char* str);

//Prints welcome message to console
void WelcomeMessage();

//Prints help message to console
void DisplayHelp();

#ifdef __cplusplus
}
#endif

#endif /* GENERALHELPERFUNCTIONS_H */

