#include "sockcomm.h"
#include "network.h"

int _initConnection(const char* ip, int port) {
    struct sockaddr_in server = { 0 };
    server.sin_family = AF_INET;
    sceNetInetPton(AF_INET, ip, &server.sin_addr);
    server.sin_port = sceNetHtons(port);

    int sock = sceNetSocket("debug", AF_INET, SOCK_STREAM, 0);
    if (sock <= 0) return INVALID_SCE_SOCKET;

    int connection_status = sceNetConnect(sock, (struct sockaddr*)&server, sizeof(server));
    if (connection_status != 0) {
        sceNetSocketClose(sock);
        return INVALID_SCE_SOCKET;
    }
    return sock;
}
