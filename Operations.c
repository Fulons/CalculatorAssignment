#include <math.h>
#include <stdio.h>

double multiplication(double x, double y){
    return x * y;
}

double division(double x, double y){
    return x / y;
}
    
double addition(double x, double y){
    return x + y;
}
    
double subtraction(double x, double y){
    return x - y;
}
    
double power(double x, double p){
    return pow(x, p);
}
    
double root(double x, double root){
    return pow(x, 1.0f / root);
}