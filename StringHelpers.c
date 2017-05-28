/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "Typedefs.h"

void removeWhitespace(char* str){
    int moveLength = 0;
    for(int i = 0; ; i++){
        int j = i - moveLength;
        str[j] = str[i];
        if(str[i] == '\n') {str[j] = '\0'; break;}
        if(str[i] == ' ' || str[i] == '\t') moveLength++;
    }
}

bool IsCharacters(char c, const char* str){
	for (int i = 0;; i++){
            if (str[i] == '\0') return false;
            if (c == str[i]) return true;
		
	}
}