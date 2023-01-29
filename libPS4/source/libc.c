#include "file.h"
#include "kernel.h"
#include "module.h"

#include "libc.h"

int libc;

void* (*malloc)(size_t size);
void (*free)(void* ptr);
void* (*calloc)(size_t num, size_t size);
void* (*realloc)(void* ptr, size_t size);
void* (*memalign)(size_t boundary, size_t size);
void* (*memset)(void* destination, int value, size_t num);
void* (*memcpy)(void* destination, const void* source, size_t num);
int (*memcmp)(const void* s1, const void* s2, size_t n);
void* (*memmove)(void* dst, const void* src, size_t len);
errno_t(*memmove_s)(void* dest, rsize_t destsz, const void* src, rsize_t count);

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
char* (*strstr)(char* str1, char* str2);
char* (*strdup)(const char* s);
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

double (*ceil)(double x);

int (*vsnprintf)(char *str, size_t size, const char *format, va_list ap);
void (*srand)(unsigned int seed);
int (*rand)(void);

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

DIR* (*opendir)(const char* filename);
struct dirent* (*readdir)(DIR* dirp);
int (*readdir_r)(DIR* dirp, struct dirent* entry, struct dirent** result);
long (*telldir)(const DIR* dirp);
void (*seekdir)(DIR* dirp, long loc);
void (*rewinddir)(DIR* dirp);
int (*closedir)(DIR* dirp);
int (*dirfd)(DIR* dirp);
char* (*getprogname)();

FILE* (*fopen)(const char* filename, const char* mode);
size_t(*fread)(void* ptr, size_t size, size_t count, FILE* stream);
size_t(*fwrite)(const void* ptr, size_t size, size_t count, FILE* stream);
int (*fseek)(FILE* stream, long int offset, int origin);
long int (*ftell)(FILE* stream);
int (*fclose)(FILE* stream);
int (*fprintf)(FILE* stream, const char* format, ...);

int memset_s(void* dest, rsize_t destsz, int ch, rsize_t count) {
  if ((dest == NULL) || (destsz > RSIZE_MAX) || (count > RSIZE_MAX) || (count > destsz)) {
    if ((dest != NULL) && !(destsz > RSIZE_MAX)) {
      for (rsize_t i = 0; i < destsz; ++i) {
        ((volatile unsigned char*)dest)[i] = ch;
      }
    }
    return 1;
  } else {
    for (rsize_t i = 0; i < count; ++i) {
      ((volatile unsigned char*)dest)[i] = ch;
    }
    return 0;
  }
}

void initLibc(void) {
  if (libc) { return; }

  libc = sceKernelLoadStartModule("libSceLibcInternal.sprx", 0, 0, 0, NULL, NULL);

  getFunctionAddressByName(libc, "malloc", &malloc);
  getFunctionAddressByName(libc, "free", &free);
  getFunctionAddressByName(libc, "calloc", &calloc);
  getFunctionAddressByName(libc, "realloc", &realloc);
  getFunctionAddressByName(libc, "memalign", &memalign);
  getFunctionAddressByName(libc, "memset", &memset);
  getFunctionAddressByName(libc, "memcpy", &memcpy);
  getFunctionAddressByName(libc, "memcmp", &memcmp);
  getFunctionAddressByName(libc, "memmove", &memmove);
  getFunctionAddressByName(libc, "memmove_s", &memmove_s);

  getFunctionAddressByName(libc, "strcpy", &strcpy);
  getFunctionAddressByName(libc, "strncpy", &strncpy);
  getFunctionAddressByName(libc, "strncpy_s", &strncpy_s);
  getFunctionAddressByName(libc, "strcat", &strcat);
  getFunctionAddressByName(libc, "strncat", &strncat);
  getFunctionAddressByName(libc, "strlen", &strlen);
  getFunctionAddressByName(libc, "strcmp", &strcmp);
  getFunctionAddressByName(libc, "strncmp", &strncmp);
  getFunctionAddressByName(libc, "sprintf", &sprintf);
  getFunctionAddressByName(libc, "snprintf", &snprintf);
  getFunctionAddressByName(libc, "snprintf_s", &snprintf_s);
  getFunctionAddressByName(libc, "sscanf", &sscanf);
  getFunctionAddressByName(libc, "strtol", &strtol);
  getFunctionAddressByName(libc, "strtok", &strtok);
  getFunctionAddressByName(libc, "strchr", &strchr);
  getFunctionAddressByName(libc, "strrchr", &strrchr);
  getFunctionAddressByName(libc, "strstr", &strstr);
  getFunctionAddressByName(libc, "strdup", &strdup);
  getFunctionAddressByName(libc, "strtok", &strtok);
  resolveFunction(libc, vsnprintf);
  // --------
  getFunctionAddressByName(libc, "index", &index);
  getFunctionAddressByName(libc, "rindex", &rindex);
  getFunctionAddressByName(libc, "isdigit", &isdigit);
  getFunctionAddressByName(libc, "atoi", &atoi);
  getFunctionAddressByName(libc, "atof", &atof);
  getFunctionAddressByName(libc, "strlcpy", &strlcpy);
  getFunctionAddressByName(libc, "strerror", &strerror);
  // --------
  getFunctionAddressByName(libc, "_Getpctype", &_Getpctype);
  getFunctionAddressByName(libc, "_Stoul", &_Stoul);
  getFunctionAddressByName(libc, "bcopy", &bcopy);
  getFunctionAddressByName(libc, "ceil", &ceil);
  // --------
  getFunctionAddressByName(libc, "srand", &srand);
  getFunctionAddressByName(libc, "rand", &rand);
  // --------
  getFunctionAddressByName(libc, "asctime", &asctime);
  getFunctionAddressByName(libc, "asctime_r", &asctime_r);
  getFunctionAddressByName(libc, "ctime", &ctime);
  getFunctionAddressByName(libc, "ctime_r", &ctime_r);
  getFunctionAddressByName(libc, "time", &time);
  getFunctionAddressByName(libc, "gmtime", &gmtime);
  getFunctionAddressByName(libc, "gmtime_s", &gmtime_s);
  getFunctionAddressByName(libc, "localtime", &localtime);
  getFunctionAddressByName(libc, "localtime_r", &localtime_r);
  getFunctionAddressByName(libc, "mktime", &mktime);
  // --------
  getFunctionAddressByName(libc, "opendir", &opendir);
  getFunctionAddressByName(libc, "readdir", &readdir);
  getFunctionAddressByName(libc, "readdir_r", &readdir_r);
  getFunctionAddressByName(libc, "telldir", &telldir);
  getFunctionAddressByName(libc, "seekdir", &seekdir);
  getFunctionAddressByName(libc, "rewinddir", &rewinddir);
  getFunctionAddressByName(libc, "closedir", &closedir);
  getFunctionAddressByName(libc, "dirfd", &dirfd);
  getFunctionAddressByName(libc, "getprogname", &getprogname);
  getFunctionAddressByName(libc, "fopen", &fopen);
  getFunctionAddressByName(libc, "fread", &fread);
  getFunctionAddressByName(libc, "fwrite", &fwrite);
  getFunctionAddressByName(libc, "fseek", &fseek);
  getFunctionAddressByName(libc, "ftell", &ftell);
  getFunctionAddressByName(libc, "fclose", &fclose);
  getFunctionAddressByName(libc, "fprintf", &fprintf);
}



void unloadLibc() {
  if (libc != 0) {
    // print feedback here i guess
    unloadModule(libc);
  }
}