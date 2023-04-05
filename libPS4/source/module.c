#include "../include/syscall.h"
#include "../include/kernel.h"
#include "../include/module.h"

int libModule;

int (*sceSysmoduleLoadModule)(int handle);

SYSCALL(getFunctionByName, 591);
SYSCALL(getLoadedModules, 592);

int getModuleInfo(int loadedModuleID, moduleInfo *destination) {
  destination->size = sizeof(*destination);
  return syscall(SYS_DYNLIB_GET_INFO, loadedModuleID, destination);
}

int loadModule(const char *name, int *idDestination) {
  return syscall(
    SYS_DYNLIB_LOAD_PRX,
    name,
    0,
    idDestination,
    0
  );
}


int unloadModule(int handle) {
  return syscall(
    SYS_DYNLIB_UNLOAD_PRX,
    handle, 0, 0
  );
}

void initModule(void) {
  if (!libModule) return;
  libModule = sceKernelLoadStartModule("libSceSysmodule.sprx", 0, 0, 0, NULL, NULL);
  getFunctionByName(libModule, "sceSysmoduleLoadModule", &sceSysmoduleLoadModule);
}

void unloadLibModule() { unloadModule(libModule); }