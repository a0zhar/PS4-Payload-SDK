#include "../include/kernel.h"
#include "../include/module.h"
#include "../include/file.h"
#include "../include/network.h"

int (*sceNetSocket)(const char*, int, int, int);
int (*sceNetSocketClose)(int);
int (*sceNetConnect)(int, struct sockaddr*, int);
int (*sceNetSend)(int, const void*, size_t, int);
int (*sceNetBind)(int, struct sockaddr*, int);
int (*sceNetListen)(int, int);
int (*sceNetAccept)(int, struct sockaddr*, unsigned int*);
int (*sceNetRecv)(int, void*, size_t, int);
int (*sceNetSocketAbort)(int, int);

int (*sceNetGetsockname)(int, struct sockaddr*, unsigned int*);
int (*sceNetGetsockopt)(int s, int level, int optname, void* restrict optval, socklen_t* restrict optlen);
int (*sceNetSetsockopt)(int s, int level, int optname, const void* optval, socklen_t optlen);

const char (*sceNetInetNtop)(int af, const void* src, char* dst, int size);
int (*sceNetInetPton)(int af, const char* src, void* dst);

uint64_t(*sceNetHtonll)(uint64_t host64);
uint32_t(*sceNetHtonl)(uint32_t host32);
uint16_t(*sceNetHtons)(uint16_t host16);
uint64_t(*sceNetNtohll)(uint64_t net64);
uint32_t(*sceNetNtohl)(uint32_t net32);
uint16_t(*sceNetNtohs)(uint16_t net16);

int (*sceNetCtlInit)(void);
void (*sceNetCtlTerm)(void);
int (*sceNetCtlGetInfo)(int code, SceNetCtlInfo* info);

void initNetwork(void) {
  int libNet = sceKernelLoadStartModule("libSceNet.sprx", 0, NULL, 0, 0, 0);

  getFunctionAddressByName(libNet, "sceNetSocket", &sceNetSocket);
  getFunctionAddressByName(libNet, "sceNetSocketClose", &sceNetSocketClose);
  getFunctionAddressByName(libNet, "sceNetConnect", &sceNetConnect);
  getFunctionAddressByName(libNet, "sceNetSend", &sceNetSend);
  getFunctionAddressByName(libNet, "sceNetBind", &sceNetBind);
  getFunctionAddressByName(libNet, "sceNetListen", &sceNetListen);
  getFunctionAddressByName(libNet, "sceNetAccept", &sceNetAccept);
  getFunctionAddressByName(libNet, "sceNetRecv", &sceNetRecv);
  getFunctionAddressByName(libNet, "sceNetSocketAbort", &sceNetSocketAbort);

  getFunctionAddressByName(libNet, "sceNetGetsockname", &sceNetGetsockname);
  getFunctionAddressByName(libNet, "sceNetGetsockopt", &sceNetGetsockopt);
  getFunctionAddressByName(libNet, "sceNetSetsockopt", &sceNetSetsockopt);

  getFunctionAddressByName(libNet, "sceNetInetNtop", &sceNetInetNtop);
  getFunctionAddressByName(libNet, "sceNetInetPton", &sceNetInetPton);

  getFunctionAddressByName(libNet, "sceNetHtonll", &sceNetHtonll);
  getFunctionAddressByName(libNet, "sceNetHtonl", &sceNetHtonl);
  getFunctionAddressByName(libNet, "sceNetHtons", &sceNetHtons);
  getFunctionAddressByName(libNet, "sceNetNtohll", &sceNetNtohll);
  getFunctionAddressByName(libNet, "sceNetNtohl", &sceNetNtohl);
  getFunctionAddressByName(libNet, "sceNetNtohs", &sceNetNtohs);

  int libNetCtl = sceKernelLoadStartModule("libSceNetCtl.sprx", 0, NULL, 0, 0, 0);
  getFunctionAddressByName(libNetCtl, "sceNetCtlInit", &sceNetCtlInit);
  getFunctionAddressByName(libNetCtl, "sceNetCtlTerm", &sceNetCtlTerm);
  getFunctionAddressByName(libNetCtl, "sceNetCtlGetInfo", &sceNetCtlGetInfo);
  /*RESOLVE(libNet, sceNetSocket);
  RESOLVE(libNet, sceNetSocketClose);
  RESOLVE(libNet, sceNetConnect);
  RESOLVE(libNet, sceNetSend);
  RESOLVE(libNet, sceNetBind);
  RESOLVE(libNet, sceNetListen);
  RESOLVE(libNet, sceNetAccept);
  RESOLVE(libNet, sceNetRecv);
  RESOLVE(libNet, sceNetSocketAbort);
  RESOLVE(libNet, sceNetGetsockname);
  RESOLVE(libNet, sceNetGetsockopt);
  RESOLVE(libNet, sceNetSetsockopt);
  RESOLVE(libNet, sceNetInetNtop);
  RESOLVE(libNet, sceNetInetPton);
  RESOLVE(libNet, sceNetHtonll);
  RESOLVE(libNet, sceNetHtonl);
  RESOLVE(libNet, sceNetHtons);
  RESOLVE(libNet, sceNetNtohll);
  RESOLVE(libNet, sceNetNtohl);
  RESOLVE(libNet, sceNetNtohs);
  RESOLVE(libNetCtl, sceNetCtlInit);
  RESOLVE(libNetCtl, sceNetCtlTerm);
  RESOLVE(libNetCtl, sceNetCtlGetInfo);
  */
}


int SckConnect(char* hostIP, int hostPort) {
  int sock = sceNetSocket("psocket", AF_INET, SOCK_STREAM, 0);

  struct in_addr ip_addr;
  sceNetInetPton(AF_INET, hostIP, &ip_addr);

  struct sockaddr_in sk = {
    .sin_len = sizeof(sk),
    .sin_family = AF_INET,
    .sin_addr = ip_addr,
    .sin_port = sceNetHtons(hostPort)
  };
  //sk.sin_len = sizeof(sk);
  //sk.sin_family = AF_INET;
  //sk.sin_addr = ip_addr;
  //sk.sin_port = sceNetHtons(hostPort);
  memset(sk.sin_zero, 0, sizeof(sk.sin_zero));
  sceNetConnect(sock, (struct sockaddr*)&sk, sizeof(sk));
  return sock;
}

void SckClose(int socket) {
  sceNetSocketClose(socket);
}

void SckSend(int socket, char* sdata, int length) {
  sceNetSend(socket, sdata, length, 0);
}

char* SckRecv(int socket) {
  char rbuf[4096], * retval = malloc(sizeof(char) * 1);
  int plen, length = 0, i;
  while ((plen = sceNetRecv(socket, rbuf, sizeof(rbuf), 0)) > 0) {
    retval = (char*)realloc(retval, sizeof(char) * (length + plen) + 1);
    for (i = 0; i < plen; i++) {
      retval[length] = rbuf[i];
      length++;
    }
    memset(rbuf, 0, sizeof rbuf);
  }
  return retval;
}

void SckRecvf(int socket, char* destfile) {
  char rbuf[4096];
  int plen, fid = open(destfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
  while ((plen = sceNetRecv(socket, rbuf, sizeof(rbuf), 0)) > 0) {
    write(fid, rbuf, plen);
    memset(rbuf, 0, sizeof rbuf);
  }
  close(fid);
}
