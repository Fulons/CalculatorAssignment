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

#include "StringHelpers.h"


#ifdef __cplusplus
extern "C" {
#endif    
#ifdef DEBUG
    extern int G_numAllocations;
    extern int G_numDeallocations;
    extern ConstStringArray* G_allocatedTypes;
    extern ConstStringArray* G_deallocatedTypes;
    #define MEMORY_DEBUG_INIT   G_allocatedTypes = malloc(sizeof(ConstStringArray)); G_allocatedTypes->numNames = 0; G_allocatedTypes->array = malloc(sizeof(ConstString)); \
                                G_deallocatedTypes = malloc(sizeof(ConstStringArray)); G_deallocatedTypes->numNames = 0; G_deallocatedTypes->array = malloc(sizeof(ConstString));
    #define MEMORY_DEBUG_PRINT_ALLOCATIONS  ConstStringArray* uniqueAllocations = GetUniqueList(G_allocatedTypes);\
                                            ConstStringArray* uniqueDeallocations = GetUniqueList(G_deallocatedTypes);\
                                            ConstStringArray* uniqueVariables = GetUniqueList(Merge(uniqueAllocations, uniqueDeallocations));\
                                            printf("Num allocations  : %u\n", G_numAllocations);\
                                            printf("Num deallocations: %u\n", G_numDeallocations);\
                                            for(int i = 0; i < uniqueVariables->numNames; ++i)\
                                                printf("Type: %s\nAllocations  : %u\nDeallocations: %u\n",\
                                                uniqueVariables->array[i].str,\
                                                FindNumberOfInstances(G_allocatedTypes, uniqueVariables->array[i].str),\
                                                FindNumberOfInstances(G_deallocatedTypes, uniqueVariables->array[i].str));\
                                            printf("NOTE: 3 Allocations of constStringArray and constString are\n      used for the purposes of this memory debug!\n");\
                                            free(G_allocatedTypes->array); free(G_allocatedTypes);\
                                            free(G_deallocatedTypes->array); free(G_deallocatedTypes);\
                                            free(uniqueAllocations->array); free(uniqueAllocations);\
                                            free(uniqueDeallocations->array); free(uniqueDeallocations);\
                                            free(uniqueVariables->array); free(uniqueVariables);
    #ifdef MEMORY_DEBUG_FULL    //Does memory allocation and deallocation counting and prints out message every time memory is allocated or deallocated
        #define MyRealloc(ptr, num, type) (type*)realloc(ptr, sizeof(type) * (num)); printf("Reallocated memory.\nType: %s\nSize: %u\n", #type, num * sizeof(type))
        #define MyMalloc(num, type) (type*)malloc(sizeof(type) * (num)); G_numAllocations++; PushName(G_allocatedTypes, #type); printf("Allocated memory.\nType: %s\nSize: %u\n", #type, num * sizeof(type))
        #define MyFree(ptr, type) free(ptr); G_numDeallocations++; PushName(G_deallocatedTypes, #type); printf("Freed memory of type: %s\n", #type)
    #else                       //Does memory allocation and deallocation counting but only prints out  end result        
        #define MyRealloc(ptr, num, type) (type*)realloc(ptr, sizeof(type) * (num))
        #define MyMalloc(num, type) (type*)malloc(sizeof(type) * (num)); G_numAllocations++; PushName(G_allocatedTypes, #type)
        #define MyFree(ptr, type) free(ptr); G_numDeallocations++; PushName(G_deallocatedTypes, #type)
    #endif
#else
    #define MyRealloc(ptr, num, type) (type*)realloc(ptr, sizeof(type) * (num))
    #define MyMalloc(num, type) (type*)malloc(sizeof(type) * (num))
    #define MyFree(ptr, type) free(ptr)
    #define MEMORY_DEBUG_INIT
    #define MEMORY_DEBUG_PRINT_ALLOCATIONS
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

