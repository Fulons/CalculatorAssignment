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

