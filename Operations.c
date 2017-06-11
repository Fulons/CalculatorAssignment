#include <math.h>
#include <stdio.h>

double Multiplication(double x, double y){
    return x * y;
}

double Division(double x, double y){
    return x / y;
}
    
double Addition(double x, double y){
    return x + y;
}
    
double Subtraction(double x, double y){
    return x - y;
}
    
double Power(double x, double p){
    return pow(x, p);
}
    
double Root(double x, double root){
    return pow(x, 1.0f / root);
}