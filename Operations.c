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
    
double PowerPower(double x, double p){
    return pow(x, p);
}
    
double RootRoot(double x, double root){
    return pow(x, 1.0f / root);
}