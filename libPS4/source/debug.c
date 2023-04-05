#include "../include/debug.h"



int initSocket(const char* ip, int port) {
  struct sockaddr_in server = { 0 };
  server.sin_family = AF_INET;
  sceNetInetPton(AF_INET, ip, &server.sin_addr);
  server.sin_port = sceNetHtons(port);

  int sock = sceNetSocket("debug", AF_INET, SOCK_STREAM, 0);
  if (sock <= 0) return 0;

  int connection_status = sceNetConnect(sock, (struct sockaddr*)&server, sizeof(server));
  if (connection_status != 0) {
    sceNetSocketClose(sock);
    return 0;
  }
  return sock;
}
void SocketPrintf(const char* format, ...) {
  va_list args;
  va_start(args, format);

  // Determine the required buffer size for the formatted message
  int bufferSize = vsnprintf(NULL, 0, format, args) + 1;

  // Allocate the buffer on the stack if possible, or on the heap if it's too large
  char* msgData = NULL;
  if (bufferSize <= 1024) {
    char stackBuffer[1024];
    msgData = stackBuffer;
  } else {
    msgData = malloc(bufferSize);
  }

  if (msgData != NULL) {
    // Format the message into the buffer
    vsnprintf(msgData, bufferSize, format, args);
    va_end(args);

    // Send the message over the network
    int DEBUG_SOCK = initSocket(DEBUG_IP, DEBUG_PORT);
    sceNetSend(DEBUG_SOCK, msgData, bufferSize - 1, 0);
    sceNetSocketClose(DEBUG_SOCK);

    // Deallocate the buffer if it was allocated on the heap
    if (bufferSize > 1024) {
      free(msgData);
    }
  } else {
    va_end(args);
  }
}
