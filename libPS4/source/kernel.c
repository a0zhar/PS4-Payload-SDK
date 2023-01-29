#include "module.h"
#include "syscall.h"
#include "kernel.h"

int libKernelHandle;

int** __stack_chk_guard;
void (*__stack_chk_fail)(void);
int* (*__error)();

int (*sceKernelError)(int);

int (*sceKernelLoadStartModule)(const char* name, size_t argc, const void* argv, unsigned int flags, int, int);

int (*sceKernelAllocateDirectMemory)(off_t searchStart, off_t searchEnd, size_t length, size_t alignment, int type, off_t* physicalAddressDestination);
int (*sceKernelMapDirectMemory)(void** addr, size_t length, int protection, int flags, off_t start, size_t alignment);
size_t(*sceKernelGetDirectMemorySize)();

int (*sceKernelStat)(const char* path, void* buf);
int (*sceKernelOpen)(const char* path, int flags, int mode);
int (*sceKernelRead)(int fd, void* buf, size_t nbyte);
int (*sceKernelLseek)(int fd, off_t offset, int whence);
int (*sceKernelClose)(int fd);

unsigned int (*sceKernelSleep)(unsigned int seconds);
int (*sceKernelUsleep)(unsigned int microseconds);
int (*sceKernelGettimeofday)(SceKernelTimeval* tp);
uint64_t(*sceKernelGetProcessTime)(void);
int (*sceKernelGetCurrentCpu)(void);

int (*sysctl)(int* name, unsigned int namelen, char* oldval, size_t* oldlen, char* newval, size_t newlen);
int (*sysctlbyname)(char* name, char* oldval, size_t* oldlen, char* newval, size_t newlen);
int (*sysarch)(int type, void* arg);
int (*execve)(char* path, char* argv[], char* envp[]);

void* (*pthread_self)();
int (*pthread_setaffinity_np)(void* one, long unsigned int two, void* three);

int (*sceKernelCreateEqueue)(SceKernelEqueue* eq, const char* name);
int (*sceKernelDeleteEqueue)(SceKernelEqueue eq);
int (*sceKernelAddUserEvent)(SceKernelEqueue eq, int id);
int (*sceKernelAddReadEvent)(SceKernelEqueue eq, int fd, size_t size, void* udata);

int (*getuid)();
int (*getgid)();
int (*getpid)();

int (*setuid)(int uid);
int (*setgid)(int gid);
int (*setreuid)(int ruid, int euid);
int (*setregid)(int rgid, int egid);

int (*sceKernelSendNotificationRequest)(int device, SceNotificationRequest* req, size_t size, int blocking);
const char* (*sceKernelGetFsSandboxRandomWord)();
int (*sceKernelGetSystemSwVersion)(SceFwInfo* fw_info);
uint32_t(*sceKernelGetCpuTemperature)(uint32_t*);

SYSCALL(kill, 37);
SYSCALL(ioctl, 54);
SYSCALL(kexec, 11);

void initKernel(void) {
  if (libKernelHandle) return;
  

  __error = NULL;

  if (loadModule("libkernel.sprx", &libKernelHandle) && 
      loadModule("libkernel_web.sprx", &libKernelHandle)) {
      loadModule("libkernel_sys.sprx", &libKernelHandle);
  }

  resolveFunction(libKernelHandle, __stack_chk_guard);
  resolveFunction(libKernelHandle, __stack_chk_fail);
  resolveFunction(libKernelHandle, __error);

  resolveFunction(libKernelHandle, sceKernelError);

  resolveFunction(libKernelHandle, sceKernelLoadStartModule);

  resolveFunction(libKernelHandle, sceKernelAllocateDirectMemory);
  resolveFunction(libKernelHandle, sceKernelMapDirectMemory);
  resolveFunction(libKernelHandle, sceKernelGetDirectMemorySize);

  resolveFunction(libKernelHandle, sceKernelStat);
  resolveFunction(libKernelHandle, sceKernelOpen);
  resolveFunction(libKernelHandle, sceKernelRead);
  resolveFunction(libKernelHandle, sceKernelLseek);
  resolveFunction(libKernelHandle, sceKernelClose);

  resolveFunction(libKernelHandle, sceKernelSleep);
  resolveFunction(libKernelHandle, sceKernelUsleep);
  resolveFunction(libKernelHandle, sceKernelGettimeofday);
  resolveFunction(libKernelHandle, sceKernelGetProcessTime);
  resolveFunction(libKernelHandle, sceKernelGetCurrentCpu);

  resolveFunction(libKernelHandle, sysctl);
  resolveFunction(libKernelHandle, sysctlbyname);
  resolveFunction(libKernelHandle, sysarch);
  resolveFunction(libKernelHandle, execve);

  resolveFunction(libKernelHandle, pthread_self);
  resolveFunction(libKernelHandle, pthread_setaffinity_np);

  resolveFunction(libKernelHandle, sceKernelCreateEqueue);
  resolveFunction(libKernelHandle, sceKernelDeleteEqueue);
  resolveFunction(libKernelHandle, sceKernelAddUserEvent);
  resolveFunction(libKernelHandle, sceKernelAddReadEvent);

  resolveFunction(libKernelHandle, getuid);
  resolveFunction(libKernelHandle, getgid);
  resolveFunction(libKernelHandle, getpid);

  resolveFunction(libKernelHandle, setuid);
  resolveFunction(libKernelHandle, setgid);
  resolveFunction(libKernelHandle, setreuid);
  resolveFunction(libKernelHandle, setregid);

  resolveFunction(libKernelHandle, sceKernelSendNotificationRequest);
  resolveFunction(libKernelHandle, sceKernelGetFsSandboxRandomWord);
  resolveFunction(libKernelHandle, sceKernelGetSystemSwVersion);
  resolveFunction(libKernelHandle, sceKernelGetCpuTemperature);
}

void unloadLibKernelHandle() {
  if (libKernelHandle != 0) {
    unloadModule(libKernelHandle);
  }
}