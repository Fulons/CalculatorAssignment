#include <stdio.h>  //for printf
#include <string.h> //for strlen
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

bool askUserYesOrNo(char* str){
    while(true){
        printf("%s (Y/N):", str);
        char c;
        scanf(" %c", &c);
        if(c == 'Y' || c == 'y') return true;
        else if(c == 'N' || c == 'n') return false;
    }
}

void toLowerCase(char* str){
    int length = strlen(str);
    for (int i = 0; i < length; i++){
        if(str[i] >= 'A' && str[i] <= 'Z')
            str[i] = str[i] - 'A' + 'a';
    }
}