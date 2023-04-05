#include "../include/file.h"
#include "../include/kernel.h"
#include "../include/module.h"
#include "../include/libc.h"

int sceLibcModule;


// Memory allocation functions
void* (*malloc)(size_t size);
void  (*free)(void* ptr);
void* (*calloc)(size_t num, size_t size);
void* (*realloc)(void* ptr, size_t size);
void* (*memalign)(size_t boundary, size_t size);
void* (*memset)(void* destination, int value, size_t num);
void* (*memcpy)(void* destination, const void* source, size_t num);
int (*memcmp)(const void* s1, const void* s2, size_t n);
void* (*memmove)(void* dst, const void* src, size_t len);
errno_t(*memmove_s)(void* dest, rsize_t destsz, const void* src, rsize_t count);
errno_t(*memcpy_s)(void* _Destination, size_t _DestinationSize, const void* _Source, size_t count);
errno_t(*memset_s)(void* dest, size_t dest_size, int value, size_t count);

// String manipulation functions
char* (*strcpy)(char* destination, const char* source);
char* (*strncpy)(char* destination, const char* source, size_t num);
errno_t* (*strncpy_s)(char* restrict dest, rsize_t destsz, const char* restrict src, rsize_t count);
char* (*strcat)(char* dest, const char* src);
char* (*strncat)(char* dest, const char* src, size_t n);
size_t(*strlen)(const char* s);
int (*strcmp)(const char* s1, const char* s2);
int (*strncmp)(const char* s1, const char* s2, size_t n);
int (*sprintf)(char* str, const char* format, ...);
int (*snprintf)(char* str, size_t size, const char* format, ...);
int (*snprintf_s)(char* restrict buffer, rsize_t bufsz, const char* restrict format, ...);
int (*sscanf)(const char* str, const char* format, ...);
int (*strtol)(const char* s1, char** s2, int base);
char* (*strtok)(char* str, const char* delimiters);
char* (*strchr)(const char* s, int c);
char* (*strrchr)(const char* s, int c);
char* (*strstr)(const char* _String, const char* _SubString);
char* (*strdup)(const char* _String);
char* (*strtok)(char* str, const char* sep);
char* (*index)(const char* s, int c);
char* (*rindex)(const char* s, int c);
int (*isdigit)(int c);
int (*atoi)(const char* s);
double (*atof)(const char* s);
size_t(*strlcpy)(char* dst, const char* src, size_t size);
char* (*strerror)(int errnum);
void* (*_Getpctype)();
unsigned long (*_Stoul)(const char*, char**, int);
void (*bcopy)(const void* s1, void* s2, size_t n);
int (*vsnprintf)(char* str, size_t size, const char* format, va_list ap);

// Mathematical functions
double (*ceil)(double x);

// Standard I/O functions
FILE* (*fopen)(const char* filename, const char* mode);
size_t(*fread)(void* ptr, size_t size, size_t count, FILE* stream);
size_t(*fwrite)(const void* ptr, size_t size, size_t count, FILE* stream);
int (*fseek)(FILE* stream, long int offset, int origin);
long int (*ftell)(FILE* stream);
int (*fclose)(FILE* stream);
int (*fprintf)(FILE* stream, const char* format, ...);

// Time functions
char* (*asctime)(const struct tm* tm);
char* (*asctime_r)(const struct tm* tm, char* buf);
char* (*ctime)(const time_t* timep);
char* (*ctime_r)(const time_t* timep, char* buf);
time_t(*time)(time_t* tloc);
struct tm* (*gmtime)(const time_t* timep);
struct tm* (*gmtime_s)(const time_t* timep, struct tm* result);
struct tm* (*localtime)(const time_t* timep);
struct tm* (*localtime_r)(const time_t* timep, struct tm* result);
time_t(*mktime)(struct tm* tm);

// Directory functions
DIR* (*opendir)(const char* filename);
struct dirent* (*readdir)(DIR* dirp);
int (*readdir_r)(DIR* dirp, struct dirent* entry, struct dirent** result);
long (*telldir)(const DIR* dirp);
void (*seekdir)(DIR* dirp, long loc);
void (*rewinddir)(DIR* dirp);
int (*closedir)(DIR* dirp);
int (*dirfd)(DIR* dirp);
char* (*getprogname)();

// Random number generator functions
void (*srand)(unsigned int seed);
int (*rand)(void);

void initLibc(void) {
  if (sceLibcModule) { return; }
  sceLibcModule = sceKernelLoadStartModule("libSceLibcInternal.sprx", 0, 0, 0, 0, 0);
  if (sceLibcModule) {
    getFunctionByName(sceLibcModule, "malloc", &malloc);
    getFunctionByName(sceLibcModule, "free", &free);
    getFunctionByName(sceLibcModule, "calloc", &calloc);
    getFunctionByName(sceLibcModule, "realloc", &realloc);
    getFunctionByName(sceLibcModule, "memalign", &memalign);
    getFunctionByName(sceLibcModule, "memset", &memset);
    getFunctionByName(sceLibcModule, "memset_s", &memset_s);
    getFunctionByName(sceLibcModule, "memcpy", &memcpy);
    getFunctionByName(sceLibcModule, "memcmp", &memcmp);
    getFunctionByName(sceLibcModule, "memmove", &memmove);
    getFunctionByName(sceLibcModule, "memmove_s", &memmove_s);
    getFunctionByName(sceLibcModule, "strcpy", &strcpy);
    getFunctionByName(sceLibcModule, "strncpy", &strncpy);
    getFunctionByName(sceLibcModule, "strncpy_s", &strncpy_s);
    getFunctionByName(sceLibcModule, "strcat", &strcat);
    getFunctionByName(sceLibcModule, "strncat", &strncat);
    getFunctionByName(sceLibcModule, "strlen", &strlen);
    getFunctionByName(sceLibcModule, "strcmp", &strcmp);
    getFunctionByName(sceLibcModule, "strncmp", &strncmp);
    getFunctionByName(sceLibcModule, "sprintf", &sprintf);
    getFunctionByName(sceLibcModule, "snprintf", &snprintf);
    getFunctionByName(sceLibcModule, "snprintf_s", &snprintf_s);
    getFunctionByName(sceLibcModule, "sscanf", &sscanf);
    getFunctionByName(sceLibcModule, "strtol", &strtol);
    getFunctionByName(sceLibcModule, "strtok", &strtok);
    getFunctionByName(sceLibcModule, "strchr", &strchr);
    getFunctionByName(sceLibcModule, "strrchr", &strrchr);
    getFunctionByName(sceLibcModule, "strstr", &strstr);
    getFunctionByName(sceLibcModule, "strdup", &strdup);
    getFunctionByName(sceLibcModule, "strtok", &strtok);
    getFunctionByName(sceLibcModule, "vsnprintf", &vsnprintf);
    getFunctionByName(sceLibcModule, "index", &index);
    getFunctionByName(sceLibcModule, "rindex", &rindex);
    getFunctionByName(sceLibcModule, "isdigit", &isdigit);
    getFunctionByName(sceLibcModule, "atoi", &atoi);
    getFunctionByName(sceLibcModule, "atof", &atof);
    getFunctionByName(sceLibcModule, "strlcpy", &strlcpy);
    getFunctionByName(sceLibcModule, "strerror", &strerror);
    getFunctionByName(sceLibcModule, "_Getpctype", &_Getpctype);
    getFunctionByName(sceLibcModule, "_Stoul", &_Stoul);
    getFunctionByName(sceLibcModule, "bcopy", &bcopy);
    getFunctionByName(sceLibcModule, "ceil", &ceil);
    getFunctionByName(sceLibcModule, "srand", &srand);
    getFunctionByName(sceLibcModule, "rand", &rand);
    getFunctionByName(sceLibcModule, "asctime", &asctime);
    getFunctionByName(sceLibcModule, "asctime_r", &asctime_r);
    getFunctionByName(sceLibcModule, "ctime", &ctime);
    getFunctionByName(sceLibcModule, "ctime_r", &ctime_r);
    getFunctionByName(sceLibcModule, "time", &time);
    getFunctionByName(sceLibcModule, "gmtime", &gmtime);
    getFunctionByName(sceLibcModule, "gmtime_s", &gmtime_s);
    getFunctionByName(sceLibcModule, "localtime", &localtime);
    getFunctionByName(sceLibcModule, "localtime_r", &localtime_r);
    getFunctionByName(sceLibcModule, "mktime", &mktime);
    getFunctionByName(sceLibcModule, "opendir", &opendir);
    getFunctionByName(sceLibcModule, "readdir", &readdir);
    getFunctionByName(sceLibcModule, "readdir_r", &readdir_r);
    getFunctionByName(sceLibcModule, "telldir", &telldir);
    getFunctionByName(sceLibcModule, "seekdir", &seekdir);
    getFunctionByName(sceLibcModule, "rewinddir", &rewinddir);
    getFunctionByName(sceLibcModule, "closedir", &closedir);
    getFunctionByName(sceLibcModule, "dirfd", &dirfd);
    getFunctionByName(sceLibcModule, "getprogname", &getprogname);
    getFunctionByName(sceLibcModule, "fopen", &fopen);
    getFunctionByName(sceLibcModule, "fread", &fread);
    getFunctionByName(sceLibcModule, "fwrite", &fwrite);
    getFunctionByName(sceLibcModule, "fseek", &fseek);
    getFunctionByName(sceLibcModule, "ftell", &ftell);
    getFunctionByName(sceLibcModule, "fclose", &fclose);
    getFunctionByName(sceLibcModule, "fprintf", &fprintf);
  }

}
void unloadLibc() {
  unloadModule(sceLibcModule);
}