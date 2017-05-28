/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Calculation.h
 * Author: Fulons
 *
 * Created on 28 May 2017, 01:38
 */

#include "Typedefs.h"
#ifndef CALCULATION_H
#define CALCULATION_H

#ifdef __cplusplus
extern "C" {
#endif

//Predefinition to allow the struct to contain pointers to itself
struct calculation{                     //Structure that will create a tree hierarchical data structure
    int op;
    double value;
    calculation* operand1;
    calculation* operand2;
    calculation* parent;
    bool operand1Set;
    char* externalCalculationName;
};

calculation* newCalculation(calculation* parent);
void deleteCalculation(calculation* calc);

calculation* parse(char* str, variable* varRoot, double lastResult);
double calculate(calculation* calc, variable* node);


#ifdef __cplusplus
}
#endif

#endif /* CALCULATION_H */

