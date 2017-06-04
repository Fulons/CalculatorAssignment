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
#define debugPrint(a) printf("DEBUG: "); printf(a)
#else
#define debugPrint(a)
#endif

#define errorPrint(a) printf("Error: "); printf(a)
    
void removeWhitespace(char* str);
bool IsCharacters(char c, const char* str);
bool askUserYesOrNo(char* str);
void toLowerCase(char* str);
int readFileToDelim(char* buffer, int bufferSize, const char* delims, FILE* file);

#ifdef __cplusplus
}
#endif

#endif /* STRINGHELPERS_H */

