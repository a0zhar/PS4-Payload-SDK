#pragma once

#ifndef SYSUTIL_H
#define SYSUTIL_H

#include "debug.h"
#include "kernel.h"
#include "libc.h"
#include "network.h"
#include "types.h"

#define SCE_USER_SERVICE_MAX_LOGIN_USERS 4
#define SCE_USER_SERVICE_MAX_USER_NAME_LENGTH 16

extern int (*sceSysUtilSendSystemNotificationWithText)(int messageType, char* message);

typedef struct SceUserServiceLoginUserIdList {
  int32_t userId[SCE_USER_SERVICE_MAX_LOGIN_USERS];
} SceUserServiceLoginUserIdList;

void initSysUtil(void);
void openBrowser(char* uri);
int getUserIDList(SceUserServiceLoginUserIdList* userIdList);
int32_t getUserID();
char* getUserName(int32_t userId);
int32_t getInitialUser();
void reboot();
void shutdown();

// HUGE shoutout to OSM-Made for removing the need to use the football/soccer icon in the notifications
// https://github.com/OSM-Made/PS4-Notify
void sendNotification(char* icon, const char* format);

void unloadSysUtil();

#endif
