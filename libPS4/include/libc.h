#pragma once

#ifndef LIBC_H
#define LIBC_H

#include "./types.h"
extern int sceLibcModule;


#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

extern void* (*malloc)(size_t _Size);
extern void  (*free)(void* _Block);
extern void* (*calloc)(size_t _Count, size_t _Size);
extern void* (*realloc)(void* _Block, size_t _Size);
extern void* (*memalign)(size_t _Boundary, size_t _Size);
extern void* (*memset)(void* _Destination, int _Val, size_t _Size);
extern void* (*memcpy)(void* _Destination, const void* _Source, size_t _Size);
extern int   (*memcmp)(const void* _Buffer1, const void* _Buffer2, size_t _Size);
extern void* (*memmove)(void* _Destination, const void* _Source, size_t _Size);
extern errno_t(*memmove_s)(void* _Destination, rsize_t _DestinationSize, const void* _Source, rsize_t _SourceSize);
extern errno_t(*memset_s)(void* dest, size_t dest_size, int value, size_t count);


extern char* (*strcpy)(char* _Destination, const char* _Source);
extern char* (*strncpy)(char* _Destination, const char* _Source, size_t _Count);
extern errno_t* (*strncpy_s)(char* restrict _Destination, rsize_t _SizeInBytes, const char* restrict _Source, rsize_t _MaxCount);
extern char* (*strcat)(char* _Destination, const char* _Source);
extern char* (*strncat)(char* _Destination, const char* _Source, size_t _Count);
extern size_t(*strlen)(const char* _String);
extern int (*strcmp)(const char* _String1, const char* _String2);
extern int (*strncmp)(const char* _String1, const char* _String2, size_t _MaxCount);
extern int (*sprintf)(char* _Buffer, const char* _Format, ...);
extern int (*snprintf)(char* _Buffer, size_t _BufferCount, const char* _Format, ...);
extern int (*snprintf_s)(char* restrict buffer, rsize_t bufsz, const char* restrict format, ...);
extern int (*sscanf)(const char* _Buffer, const char* _Format, ...);
extern int (*strtol)(const char* _String, char** _EndPtr, int _Radix);
extern char* (*strtok)(char* _String, const char* _Delimiter);
extern char* (*strchr)(const char* _String, int _Ch);
extern char* (*strrchr)(const char* _String, int _Ch);
extern char* (*strstr)(const char* _String, const char* _SubString);
extern char* (*strdup)(const char* _String);
extern char* (*strtok)(char* _String, const char* _Delimiter);
extern char* (*index)(const char* s, int c);
extern char* (*rindex)(const char* s, int c);
extern char* (*rindex)(const char* s, int c);

extern int (*isdigit)(int c);
extern int (*atoi)(const char* s);
extern double (*atof)(const char* s);
extern size_t(*strlcpy)(char* dst, const char* src, size_t _Size);
extern char* (*strerror)(int errnum);
extern void* (*_Getpctype)();
extern unsigned long (*_Stoul)(const char*, char**, int);
extern void (*bcopy)(const void* s1, void* s2, size_t n);
extern double (*ceil)(double x);
extern int (*vsnprintf)(char* str, size_t size, const char* format, va_list ap);

extern void (*srand)(unsigned int seed);
extern int (*rand)(void);

extern char* (*asctime)(const struct tm* tm);
extern char* (*asctime_r)(const struct tm* tm, char* buf);
extern char* (*ctime)(const time_t* timep);
extern char* (*ctime_r)(const time_t* timep, char* buf);
extern time_t(*time)(time_t* tloc);
extern struct tm* (*gmtime)(const time_t* timep);
extern struct tm* (*gmtime_s)(const time_t* timep, struct tm* result);
extern struct tm* (*localtime)(const time_t* timep);
extern struct tm* (*localtime_r)(const time_t* timep, struct tm* result);
extern time_t(*mktime)(struct tm* tm);

extern DIR* (*opendir)(const char* filename);
extern struct dirent* (*readdir)(DIR* dirp);
extern int (*readdir_r)(DIR* dirp, struct dirent* entry, struct dirent** result);
extern long (*telldir)(const DIR* dirp);
extern void (*seekdir)(DIR* dirp, long loc);
extern void (*rewinddir)(DIR* dirp);
extern int (*closedir)(DIR* dirp);
extern int (*dirfd)(DIR* dirp);
extern char* (*getprogname)();

extern FILE* (*fopen)(const char* filename, const char* mode);
extern size_t(*fread)(void* ptr, size_t _Size, size_t count, FILE* stream);
extern size_t(*fwrite)(const void* ptr, size_t _Size, size_t count, FILE* stream);
extern int (*fseek)(FILE* stream, long int offset, int origin);
extern long int (*ftell)(FILE* stream);
extern int (*fclose)(FILE* stream);
extern int (*fprintf)(FILE* stream, const char* format, ...);

void initLibc(void);
void unloadLibc();


#endif
