#include <stdio.h>  //for printf
#include <string.h> //for strlen
#include "Typedefs.h"


void removeWhitespace(char* str){
    int moveLength = 0;
    for(int i = 0; ; i++){
        int j = i - moveLength;
        str[j] = str[i];
        if(str[i] == '\b') moveLength += 2;
        else if(str[i] == '\n' || str[i] == '\r' || str[i] == '\0') {str[j] = '\0'; break;}
    }
    moveLength = 0;
    for(int i = 0; ; i++){
        int j = i - moveLength;
        str[j] = str[i];
        if(str[i] == '\n' || str[i] == '\r' || str[i] == '\0') {str[j] = '\0'; break;}
        else if(str[i] == ' ' || str[i] == '\t') moveLength++;        
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

int readFileToDelim(char* buffer, int bufferSize, const char* delims, FILE* file){
    if(ferror(file)) return 0;
    for(int i = 0; i <  bufferSize; i++){
        int buff = (char)fgetc(file);
        buffer[i] = buff;
        if(IsCharacters(buffer[i], delims) || feof(file)){
            buffer[i] = '\0';
            return ++i;
        }
    }
    return bufferSize;
}