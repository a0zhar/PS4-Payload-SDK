#pragma once

#ifndef STRINGS_H
#define STRINGS_H

size_t countInstances(const char * str, const char * word);


char* replaceString(const char*fullStr, const char*oldWord, const char*newWord);
char** splitString(const char* str, const char* delim, int* count) ;
char *read_string(int f);
int substring(char *haystack, char *needle);

#endif
