/* 
 * File:   StringHelpers.h
 * Author: Fulons
 *
 * Created on 28 May 2017, 01:44
 */

#ifndef STRINGHELPERS_H
#define STRINGHELPERS_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DEBUG
#define debugPrint(a, ...) printf("DEBUG: "); printf(a, ##__VA_ARGS__)
#else
#define debugPrint(a)
#endif

#define errorPrint(a) printf("Error: "); printf(a)

////////////////Used for error searching for self containing variables///////////////
typedef struct{
    const char* str;
}CString;

typedef struct{
    CString* array;
    int numNames;
}ConstStringArray;

void PushName(ConstStringArray* n, const char* str);
void PopLastString(ConstStringArray* n);
bool FindName(ConstStringArray* n, const char* str);
ConstStringArray* CreateArrayOfStrings(const char* firstStr);
void DeleteArrayOfstring(ConstStringArray* n);


typedef struct{
    char* str;
    int size;
}string;

string* PushChar(string* str, char c);
string* PopLastChar(string* str);
string* CreateString();
void DeleteString(string* str);
/////////////////////////////////////////////////////////////////////////////////////

void RemoveWhitespace(char* str);
bool IsCharacters(char c, const char* str);
bool AskUserYesOrNo(char* str);
void ToLowerCase(char* str);
int ReadFileToDelim(char* buffer, int bufferSize, const char* delims, FILE* file);

#ifdef __cplusplus
}
#endif

#endif /* STRINGHELPERS_H */

