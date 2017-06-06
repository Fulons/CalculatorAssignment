/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Typedefs.h
 * Author: Fulons
 *
 * Created on 28 May 2017, 01:47
 */

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { false = 0, true = 1 } bool;

//To enable these two structs to contain a pointer of their own type they must be declared like this
typedef struct Variable Variable;
typedef struct Calculation Calculation; 

#ifdef __cplusplus
}
#endif

#endif /* TYPEDEFS_H */

