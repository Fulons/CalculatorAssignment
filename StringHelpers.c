#include <stdio.h>  //for printf
#include <string.h> //for strlen
#include <stdlib.h> //for free, malloc, realloc
#include "Typedefs.h"
#include "StringHelpers.h"
#include "GeneralHelperFunctions.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PushName(ConstStringArray* n, const char* str){
    ++n->numNames;
    n->array = MyRealloc(n->array, n->numNames, ConstString);
    n->array[n->numNames - 1].str = str;
}

void PopLastString(ConstStringArray* n){
    --n->numNames;
    n->array = MyRealloc(n->array, n->numNames, ConstString);
}

bool FindName(ConstStringArray* n, const char* str){
    for(int i = 0; i < n->numNames; i++)
        if(StringCompare(n->array[i].str, str) == 0) return true;
    return false;
}

ConstStringArray* CreateConstStringArray(const char* firstStr){
    ConstStringArray* ret = MyMalloc(1, ConstStringArray);
    ret->array = MyMalloc(1, ConstString);
    ret->array[0].str = firstStr;
    ret->numNames = 1;
    return ret;
}

void DeleteArrayOfstring(ConstStringArray* n){
    if(n->array){
        MyFree(n->array, ConstString);
    }
    n->numNames = 0;
    MyFree(n, ConstStringArray);
}

ConstStringArray* GetUniqueList(ConstStringArray* n){
    ConstStringArray* ret = CreateConstStringArray("none");
    for(int i = 0; i < n->numNames; i++){
        if(!FindName(ret, n->array[i].str)){
            PushName(ret, n->array[i].str);
        }
    }
    return ret;
}

int FindNumberOfInstances(ConstStringArray* n, const char* str){
    int num = 0;
    for(int i = 0; i < n->numNames; i++)
        if(StringCompare(n->array[i].str, str) == 0) ++num;
    return num;
}

ConstStringArray* Merge(ConstStringArray* n, ConstStringArray* n2){
    for(int i = 0; i < n2->numNames; ++i)
        PushName(n, n2->array[i].str);
    return n;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

string* PushChar(string* str, char c){
    ++str->size;
    str->str = MyRealloc(str->str, str->size + 1, char);
    str->str[str->size - 1] = c;
    str->str[str->size] = '\0';
    return str;
}
string* PopLastChar(string* str){
    --str->size;
    str->str = MyRealloc(str->str, str->size + 1, char);
    str->str[str->size] = '\0';
    return str;
}

string* CreateString(){    
    string* str = MyMalloc(1, string);
    str->str = MyMalloc(1, char);
    str->size = 0;
    str->str[str->size] = '\0';
    return str;
}

void DeleteString(string* str){
    if(str){
        if(str->str) { MyFree(str->str, char); }
        MyFree(str, string);
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

int StringCompare(const char* str, const char* str2){
    if((strcmp(str, str2) == 0) && (strlen(str) == strlen(str2))) return 0;
    return 1;
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