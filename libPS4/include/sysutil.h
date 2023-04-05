#pragma once

#ifndef SYSUTIL_H
#define SYSUTIL_H

#include "./debug.h"
#include "./kernel.h"
#include "./libc.h"
#include "./network.h"
#include "./types.h"

#define SCE_USER_SERVICE_MAX_LOGIN_USERS 4
#define SCE_USER_SERVICE_MAX_USER_NAME_LENGTH 16
#define openBrowser(uri) sceSystemServiceLaunchWebBrowser(uri, NULL);

extern int (*sceSysUtilSendSystemNotificationWithText)(int messageType, char* message);

typedef struct SceUserServiceLoginUserIdList {
  int32_t userId[SCE_USER_SERVICE_MAX_LOGIN_USERS];
} SceUserServiceLoginUserIdList;

void initSysUtil(void);
int getUserIDList(SceUserServiceLoginUserIdList* userIdList);
int32_t getUserID();
char* getUserName(int32_t userId);
int32_t getInitialUser();
void reboot();
void shutdown();

void sendNotification(const char* icon, const char* format, ...);
void unloadSysUtil();

#endif
