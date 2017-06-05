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

//Helper defines to print out various messages
#ifdef DEBUG
#define debugPrint(a, ...) printf("DEBUG: "); printf(a, ##__VA_ARGS__)
#else
#define debugPrint(a, ...)
#endif
#define errorPrint(a, ...) printf("ERROR: "); printf(a, ##__VA_ARGS__)
#define infoPrint(a, ...) printf("INFO: "); printf(a, ##__VA_ARGS__)

//ConstString, ConstStringArray and string is mostly used as an aid to checking for self containing variables
    
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
//A const string handle for ConstStringArray
typedef struct{
    const char* str;
}ConstString;

//Simple array struct to store const char*
typedef struct{
    ConstString* array;
    int numNames;
}ConstStringArray;

void PushName(ConstStringArray* n, const char* str);            //Adds str to the end of the array
void PopLastString(ConstStringArray* n);                        //Removes last entry in the array
bool FindName(ConstStringArray* n, const char* str);            //Returns true if name is in array
ConstStringArray* CreateConstStringArray(const char* firstStr);   //Allocates and initialises the array
void DeleteArrayOfstring(ConstStringArray* n);                  //Frees the memory of the array
ConstStringArray* GetUniqueList(ConstStringArray* n);
int FindNumberOfInstances(ConstStringArray* n, const char* str);
ConstStringArray* Merge(ConstStringArray* n, ConstStringArray* n2);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Simple string struct
typedef struct{
    char* str;
    int size;
}string;

string* PushChar(string* str, char c);      //Adds a char to end of string
string* PopLastChar(string* str);           //Removes last char from string
string* CreateString();                     //Allocates a new string
void DeleteString(string* str);             //Frees the memory of the string

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void RemoveWhitespace(char* str);           //Removes ' ' and '\t' from the string in addition to sort out '\b'. Only up to '\n', '\r' or '\0' and sets it to '\0'.
bool IsCharacters(char c, const char* str); //Returns true if c exists in str
void ToLowerCase(char* str);
int StringCompare(const char* str, const char* str2);

//Reads file until one of the characters in delim is found or until buffer is full.
//Returns number of chars read
int ReadFileToDelim(char* buffer, int bufferSize, const char* delims, FILE* file);  

#ifdef __cplusplus
}
#endif

#endif /* STRINGHELPERS_H */

