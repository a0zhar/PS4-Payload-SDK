#include "kernel.h"
#include "libc.h"
#include "module.h"
#include "syscall.h"

#include "sysutil.h"

int sysUtilHandle;
int libSceSystemService;
int libSceUserService;

int (*sceSysUtilSendSystemNotificationWithText)(int messageType, char* message);
int (*sceSystemServiceLaunchWebBrowser)(const char* uri, void*);
int (*sceUserServiceInitialize)(void*);
int (*sceUserServiceGetLoginUserIdList)(SceUserServiceLoginUserIdList*);
int (*sceUserServiceGetUserName)(int32_t userId, char* userName, const size_t size);
int (*sceUserServiceGetInitialUser)(int32_t*);
int (*sceUserServiceTerminate)();
int (*sceKernelReboot)();

void initSysUtil(void) {
  if (!sysUtilHandle) {
    sysUtilHandle = sceKernelLoadStartModule("/system/common/lib/libSceSysUtil.sprx", 0, 0, 0, NULL, NULL);

    getFunctionAddressByName(sysUtilHandle, "sceSysUtilSendSystemNotificationWithText", &sceSysUtilSendSystemNotificationWithText);
  }

  if (!libSceSystemService) {
    libSceSystemService = sceKernelLoadStartModule(
      "/system/common/lib/libSceSystemService.sprx", 0, 0, 0, NULL, NULL
    );
    getFunctionAddressByName(
      libSceSystemService, "sceSystemServiceLaunchWebBrowser", &sceSystemServiceLaunchWebBrowser
    );
  }
}

void initUserService(void) {
  if (libSceUserService) return;


  libSceUserService = sceKernelLoadStartModule(
    "/system/common/lib/libSceUserService.sprx", 0, 0, 0, NULL, NULL
  );

  getFunctionAddressByName(libSceUserService, "sceUserServiceInitialize", &sceUserServiceInitialize);
  getFunctionAddressByName(libSceUserService, "sceUserServiceGetInitialUser", &sceUserServiceGetInitialUser);
  getFunctionAddressByName(libSceUserService, "sceUserServiceGetLoginUserIdList", &sceUserServiceGetLoginUserIdList);
  getFunctionAddressByName(libSceUserService, "sceUserServiceGetUserName", &sceUserServiceGetUserName);
  getFunctionAddressByName(libSceUserService, "sceUserServiceTerminate", &sceUserServiceTerminate);
}

void openBrowser(char* uri) {
  sceSystemServiceLaunchWebBrowser(uri, NULL);
}

int getUserIDList(SceUserServiceLoginUserIdList* userIdList) {
  initUserService();
  if (sceUserServiceInitialize(NULL) == 0) {
    if (sceUserServiceGetLoginUserIdList(userIdList) == 0) {
      sceUserServiceTerminate();
      return 0;
    }
  }
  return -1;
}
int32_t getUserID() {
  SceUserServiceLoginUserIdList userIdList;
  getUserIDList(&userIdList);
  for (int i = 0; i < 1; i++) {
    if (userIdList.userId[i] != -1) {
      return userIdList.userId[i];
    }
  }
  return -1;
}

char* getUserName(int32_t userId) {
  char username[SCE_USER_SERVICE_MAX_USER_NAME_LENGTH + 1];
  memset(username, 0, sizeof(username));
  initUserService();
  if (sceUserServiceInitialize(NULL) == 0) {
    if (sceUserServiceGetUserName(userId, username, sizeof(username)) == 0) {
      sceUserServiceTerminate();
      char* retval = calloc(1, SCE_USER_SERVICE_MAX_USER_NAME_LENGTH + 1);
      if (retval == NULL) {
        return NULL;
      }
      strcpy(retval, username);
      return retval;
    }
  }
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

void shutdown() {
  int evf = syscall(540, "SceSysCoreReboot");
  syscall(546, evf, 0x4000, 0);
  syscall(541, evf);
  syscall(37, 1, 30);
}

void reboot() {
  getFunctionAddressByName(
    sceKernelLoadStartModule("/system/common/lib/libkernel.sprx", NULL, NULL, NULL, NULL, NULL),
    "sceKernelReboot",
    &sceKernelReboot
  );
  sceKernelReboot();
}

void sendNotification(char* icon, const char* format) {
  SceNotificationRequest noti_buffer = {
    .type = 0,
    .unk3 = 0,
    .use_icon_image_uri = 1,
    .target_id = -1
  };

  strcpy(noti_buffer.uri, icon == 0 ? "cxml://psnotification/tex_morpheus_trophy_gold" : icon);
  strcpy(noti_buffer.message, format);
  sceKernelSendNotificationRequest(0, &noti_buffer, sizeof(noti_buffer), 0);
}
