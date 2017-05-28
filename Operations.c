/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <math.h>
#include <stdio.h>

float multiplication(float x, float y){
    return x * y;
}

float division(float x, float y){
    return x / y;
}
    
float addition(float x, float y){
    return x + y;
}
    
float subtraction(float x, float y){
    return x - y;
}
    
float power(float x, float p){
    return pow(x, p);
}
    
float root(float x, float root){
    return pow(x, 1.0f / root);
}

void WelcomeMessage(){
    printf("*********************************************\n");
    printf("*            FULONS PRESENTS                *\n");
    printf("*        SUPER DUPER CALCULATOR             *\n");
    printf("*                                           *\n");
    printf("* * * * * * * * * * * * * * * * * * * * * * *\n");
    printf("*     by: Fulons K Solbakken                *\n");
    printf("*********************************************\n");
}