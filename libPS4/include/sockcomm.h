#pragma once
#define INVALID_SCE_SOCKET -1

#ifdef IS_DEBUGGING
int _initConnection(const char* ip, int port);

// This Simple Macro can be used like following e.g: 
// EasySckPrint("hello world my age is %i", 12);
#define EasySckPrint(format, ...) {                              \
    int length = snprintf(NULL, 0, format, ##__VA_ARGS__) + 1;   \
    if (length != 0) {                                           \
        char* msg = calloc(1, length);                           \
        if (msg != NULL) {                                       \
            snprintf(msg, length, format, ##__VA_ARGS__);        \
            int sock = _initConnection(SERVER_IP, SERVER_PORT);  \
            sceNetSend(sock, msg, length, 0);                    \
            sceNetSocketClose(sock);                             \
            free(msg);                                           \
        }                                                        \
    }                                                            \
}                      
#endif                                         