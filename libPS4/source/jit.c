#include "../include/kernel.h"
#include "../include/memory.h"
#include "../include/module.h"
#include "../include/jit.h"

int libJIT;

int (*sceKernelJitCreateSharedMemory)(int flags, size_t size, int protection, int* destinationHandle);
int (*sceKernelJitCreateAliasOfSharedMemory)(int handle, int protection, int* destinationHandle);
int (*sceKernelJitMapSharedMemory)(int handle, int protection, void** destination);

void initJIT(void) {
  if (libJIT) {
    return;
  }

  getFunctionByName(libKernelHandle, "sceKernelJitCreateSharedMemory", &sceKernelJitCreateSharedMemory);
  getFunctionByName(libKernelHandle, "sceKernelJitCreateAliasOfSharedMemory", &sceKernelJitCreateAliasOfSharedMemory);
  getFunctionByName(libKernelHandle, "sceKernelJitMapSharedMemory", &sceKernelJitMapSharedMemory);

  libJIT = 1;
}

void allocateJIT(size_t size, void** executableAddress, void** writableAddress) {
  int executableHandle;
  int writableHandle;

  sceKernelJitCreateSharedMemory(0, size, PROT_CPU_READ | PROT_CPU_WRITE | PROT_CPU_EXEC, &executableHandle);
  sceKernelJitCreateAliasOfSharedMemory(executableHandle, PROT_CPU_READ | PROT_CPU_WRITE, &writableHandle);

  *executableAddress = mmap(NULL, size, PROT_CPU_READ | PROT_CPU_EXEC, MAP_SHARED, executableHandle, 0);

  //sceKernelJitMapSharedMemory(writableHandle, PROT_CPU_READ | PROT_CPU_WRITE, writableAddress);
  *writableAddress = mmap(NULL, size, PROT_CPU_READ | PROT_CPU_WRITE, MAP_PRIVATE | MAP_TYPE, writableHandle, 0);
}

