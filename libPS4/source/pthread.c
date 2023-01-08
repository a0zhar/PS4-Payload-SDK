#include "kernel.h"
#include "module.h"

#include "pthread.h"

int libPthread;

int (*scePthreadCreate)(ScePthread *thread, const ScePthreadAttr *attr, void *(*entry)(void *), void *arg, const char *name);
void (*scePthreadExit)(void *value);
int (*scePthreadDetach)(ScePthread thread);
int (*scePthreadJoin)(ScePthread thread, void **value_ptr);
void (*scePthreadYield)(void);
ScePthread (*scePthreadSelf)(void);
int (*scePthreadCancel)(ScePthread thread);

int (*scePthreadMutexInit)(ScePthreadMutex *mutex, const ScePthreadMutexattr *attr, const char *name);
int (*scePthreadMutexDestroy)(ScePthreadMutex *mutex);
int (*scePthreadMutexLock)(ScePthreadMutex *mutex);
int (*scePthreadMutexTrylock)(ScePthreadMutex *mutex);
int (*scePthreadMutexTimedlock)(ScePthreadMutex *mutex, SceKernelUseconds usec);
int (*scePthreadMutexUnlock)(ScePthreadMutex *mutex);

void initPthread(void) {
  if (libPthread) {
    return;
  }

  resolveFunction(libKernelHandle, scePthreadCreate);
  resolveFunction(libKernelHandle, scePthreadExit);
  resolveFunction(libKernelHandle, scePthreadDetach);
  resolveFunction(libKernelHandle, scePthreadJoin);
  resolveFunction(libKernelHandle, scePthreadYield);
  resolveFunction(libKernelHandle, scePthreadSelf);
  resolveFunction(libKernelHandle, scePthreadCancel);

  resolveFunction(libKernelHandle, scePthreadMutexInit);
  resolveFunction(libKernelHandle, scePthreadMutexDestroy);
  resolveFunction(libKernelHandle, scePthreadMutexLock);
  resolveFunction(libKernelHandle, scePthreadMutexTrylock);
  resolveFunction(libKernelHandle, scePthreadMutexTimedlock);
  resolveFunction(libKernelHandle, scePthreadMutexUnlock);

  libPthread = 1;
}
