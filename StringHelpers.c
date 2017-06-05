#include <stdio.h>  //for printf
#include <string.h> //for strlen
#include <stdlib.h> //for free, malloc, realloc
#include "Typedefs.h"
#include "StringHelpers.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PushName(ConstStringArray* n, const char* str){
    realloc(n->array, sizeof(ConstString) * ++n->numNames);
    n->array[n->numNames - 1].str = str;
}

void PopLastString(ConstStringArray* n){
    realloc(n->array, sizeof(ConstString) * --n->numNames);
}

bool FindName(ConstStringArray* n, const char* str){
    for(int i = 0; i < n->numNames; i++)
        if(strcmp(n->array[i].str, str) == 0) return true;
    return false;
}

ConstStringArray* CreateArrayOfStrings(const char* firstStr){
    ConstStringArray* ret = malloc(sizeof(ConstStringArray));
    ret->array = malloc(sizeof(ConstString));
    ret->array[0].str = firstStr;
    ret->numNames = 1;
    return ret;
}

void DeleteArrayOfstring(ConstStringArray* n){
    if(n->array)
        free(n->array);
    n->numNames = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

string* PushChar(string* str, char c){
    str->str = realloc(str->str, sizeof(char) * (++str->size + 1));
    str->str[str->size - 1] = c;
    str->str[str->size] = '\0';
    return str;
}
string* PopLastChar(string* str){
    str->str = realloc(str->str, sizeof(char) * (--str->size + 1));
    str->str[str->size] = '\0';
    return str;
}

string* CreateString(){    
    string* str = malloc(sizeof(string));
    str->str = malloc(sizeof(char));
    str->size = 0;
    return str;
}

void DeleteString(string* str){
    if(str){
        if(str->str) free(str->str);
        free(str);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void RemoveWhitespace(char* str){
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

void ToLowerCase(char* str){
    int length = strlen(str);
    for (int i = 0; i < length; i++){
        if(str[i] >= 'A' && str[i] <= 'Z')
            str[i] = str[i] - 'A' + 'a';
    }
}

int ReadFileToDelim(char* buffer, int bufferSize, const char* delims, FILE* file){
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