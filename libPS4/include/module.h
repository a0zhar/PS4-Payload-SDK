#pragma once

#ifndef MODULE_H
#define MODULE_H

#include "./types.h"

#define resolveFunction(module, name) getFunctionByName(module, #name, &name)

typedef struct MODULEINFO_T {
  size_t size;           // 0x0
  char name[32];         // 0x8
  char padding1[0xe0];   // 0x28
  void* codeBase;        // 0x108
  unsigned int codeSize; // 0x110
  void* dataBase;        // 0x118
  unsigned int dataSize; // 0x120
  char padding2[0x3c];   // 0x124
} moduleInfo;

extern int (*sceSysmoduleLoadModule)(int id);
int getFunctionByName(int moduleID, char* name, void* destination);
int getLoadedModules(int* destination, int max, int* count);
int getModuleInfo(int loadedModuleID,  moduleInfo* destination);
int loadModule(const char* name, int* idDestination);
int unloadModule(int id);
void initModule(void);

#endif
