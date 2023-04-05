#include "../include/kernel.h"
#include "../include/libc.h"
#include "../include/module.h"
#include "../include/syscall.h"
#include "../include/notifications.h"
#include "../include/sysutil.h"
int sysUtilHandle;
int libSceSystemService;
int libSceUserService;

int (*sceSysUtilSendSystemNotificationWithText)(int messageType, char *message);
int (*sceSystemServiceLaunchWebBrowser)(const char *uri, void *);
int (*sceUserServiceInitialize)(void *);
int (*sceUserServiceGetLoginUserIdList)(SceUserServiceLoginUserIdList *);
int (*sceUserServiceGetUserName)(int32_t userId, char *userName, const size_t size);
int (*sceUserServiceGetInitialUser)(int32_t *);
int (*sceUserServiceTerminate)();
int (*sceKernelReboot)();
#define lazyModuleInit(variableName, modulePath) if(!variableName)\
  variableName = sceKernelLoadStartModule(modulePath, 0,0,0,0,0);


void initSysUtil(void) {

  lazyModuleInit(sysUtilHandle, "/system/common/lib/libSceSysUtil.sprx");
  if (sysUtilHandle) {
    resolveFunction(sysUtilHandle, sceSysUtilSendSystemNotificationWithText);
  }
  lazyModuleInit(libSceSystemService, "/system/common/lib/libSceSystemService.sprx");
  if (libSceSystemService) {
    resolveFunction(libSceSystemService, sceSystemServiceLaunchWebBrowser);
  }
}

void initUserService(void) {
  if (libSceUserService)return;
  libSceUserService = sceKernelLoadStartModule("/system/common/lib/libSceUserService.sprx", 0, 0, 0, NULL, NULL);
  getFunctionByName(libSceUserService, "sceUserServiceInitialize", &sceUserServiceInitialize);
  getFunctionByName(libSceUserService, "sceUserServiceGetInitialUser", &sceUserServiceGetInitialUser);
  getFunctionByName(libSceUserService, "sceUserServiceGetLoginUserIdList", &sceUserServiceGetLoginUserIdList);
  getFunctionByName(libSceUserService, "sceUserServiceGetUserName", &sceUserServiceGetUserName);
  getFunctionByName(libSceUserService, "sceUserServiceTerminate", &sceUserServiceTerminate);
}

int getUserIDList(SceUserServiceLoginUserIdList *userIdList) {
  int ret = sceUserServiceInitialize(NULL);
  if (ret == 0) {
    ret = sceUserServiceGetLoginUserIdList(userIdList);
    sceUserServiceTerminate();
  }
  return ret;
}

int32_t getUserID() {
  SceUserServiceLoginUserIdList userIdList;
  int ret = getUserIDList(&userIdList);
  if (ret != 0) {
    return -1;
  }

  for (int i = 0; i < SCE_USER_SERVICE_MAX_LOGIN_USERS; i++) {
    if (userIdList.userId[i] != -1) {
      return userIdList.userId[i];
    }
  }
  return -1;
}

char *getUserName(int32_t userId) {
  char *retval = malloc(SCE_USER_SERVICE_MAX_USER_NAME_LENGTH + 1);
  if (retval == NULL) {
    return NULL;
  }
  initUserService();
  if (sceUserServiceInitialize(NULL) == 0) {
    char username[SCE_USER_SERVICE_MAX_USER_NAME_LENGTH + 1];
    if (sceUserServiceGetUserName(userId, username, sizeof(username)) == 0) {
      strcpy(retval, username);
      sceUserServiceTerminate();
      return retval;
    }
  }
  free(retval);
  return NULL;
}

int32_t getInitialUser() {
  int32_t userId;
  initUserService();
  if (sceUserServiceInitialize(NULL) == 0) {
    if (sceUserServiceGetInitialUser(&userId) == 0) {
      sceUserServiceTerminate();
      return userId;
    }
  }
  return -1;
}
#define SYS_EVF_CANCEL 546
#define SYS_EVF_CLOSE  541
#define SYS_EVF_OPEN   540

void shutdown() {
  int evf = syscall(SYS_EVF_OPEN, "SceSysCoreReboot");
  syscall(SYS_EVF_CANCEL, evf, 0x4000, 0);
  syscall(SYS_EVF_CLOSE, evf);
  syscall(37, 1, 30);
}

void reboot() {
  int libKern = sceKernelLoadStartModule("/system/common/lib/libkernel.sprx", 0, 0, 0, 0, 0);
  getFunctionByName(libKern, "sceKernelReboot", &sceKernelReboot);
  unloadModule(libKern);
  sceKernelReboot();
}

void sendNotification(const char *icon, const char *format, ...) {
  SceNotificationRequest notif;
  notif.type = N_NotificationRequest;
  notif.unk3 = 0;
  notif.use_icon_image_uri = 1;
  notif.target_id = -1;

  // Set notification Icon
  strcpy(notif.uri, icon);

  // Set Notification Content
  va_list args;
  va_start(args, format);
  vsnprintf(notif.message, sizeof(notif.message) - 1, format, args);
  va_end(args);

  // Send Notification
  sceKernelSendNotificationRequest(0, &notif, sizeof(notif), 0);
}

void unloadSysUtil() {
  unloadModule(libSceSystemService);
  unloadModule(libSceUserService);
  unloadModule(sysUtilHandle);
}