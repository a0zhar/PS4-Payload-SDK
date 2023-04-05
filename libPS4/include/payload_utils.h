#pragma once

#ifndef PAYLOAD_UTILS_H
#define PAYLOAD_UTILS_H

#include "./types.h"
struct auditinfo_addr {
  char useless[184];
};

struct ucred {
  uint32_t useless1;
  uint32_t cr_uid;
  uint32_t cr_ruid;
  uint32_t useless2;
  uint32_t useless3;
  uint32_t cr_rgid;
  uint32_t useless4;
  void *useless5;
  void *useless6;
  void *cr_prison;
  void *useless7;
  uint32_t useless8;
  void *useless9[2];
  void *useless10;
  struct auditinfo_addr useless11;
  uint32_t *cr_groups;
  uint32_t useless12;
};

struct filedesc {
  void *useless1[3];
  void *fd_rdir;
  void *fd_jdir;
};

struct proc {
  char useless[64];
  struct ucred *p_ucred;
  struct filedesc *p_fd;
};

struct thread {
  void *useless;
  struct proc *td_proc;
};

struct kpayload_kbase_info {
  uint16_t fw_version;
  uint64_t uaddr;
};

struct kpayload_kbase_args {
  void *syscall_handler;
  struct kpayload_kbase_info *kpayload_kbase_info;
};

typedef struct kpayload_dump_info_t {
  uint16_t fw_version;
  uint64_t kaddr;
  uint64_t uaddr;
  size_t size;
} kernPLDumpInfo;

struct kpayload_dump_args {
  void *syscall_handler;
  kernPLDumpInfo *kpayload_dump_info;
};

struct kpayload_firmware_info {
  uint16_t fw_version;
};

struct kpayload_firmware_args {
  void *syscall_handler;
  struct kpayload_firmware_info *kpayload_firmware_info;
};

struct kpayload_kclock_info {
  uint16_t fw_version;
  uint64_t set_time;
};

struct kpayload_kclock_args {
  void *syscall_handler;
  struct kpayload_kclock_info *kpayload_kclock_info;
};

struct kpayload_target_id_info {
  uint16_t fw_version;
  uint8_t spoof;
};

struct kpayload_target_id_args {
  void *syscall_handler;
  struct kpayload_target_id_info *kpayload_target_id_info;
};

#define X86_CR0_WP (1 << 16)

static uint64_t __readmsr(unsigned long register_num);
static uint64_t readCr0(void);
void writeCr0(uint64_t cr0);

#define copyout_macro(x)                                                    \
  kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K##x##_XFAST_SYSCALL]; \
  copyout = (void *)(kernel_base + K##x##_COPYOUT);

#define jailbreak_macro(x)                                                  \
  kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K##x##_XFAST_SYSCALL]; \
  kernel_ptr = (uint8_t *)kernel_base;                                      \
  prison0 = (void **)&kernel_ptr[K##x##_PRISON_0];                          \
  rootvnode = (void **)&kernel_ptr[K##x##_ROOTVNODE];

#define mmap_macro(x)                                                       \
  kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K##x##_XFAST_SYSCALL]; \
  kernel_ptr = (uint8_t *)kernel_base;                                      \
  mmap_patch_1 = &kernel_ptr[K##x##_MMAP_SELF_1];                           \
  mmap_patch_2 = &kernel_ptr[K##x##_MMAP_SELF_2];                           \
  mmap_patch_3 = &kernel_ptr[K##x##_MMAP_SELF_3];

#define aslr_macro(x)                                                       \
  kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K##x##_XFAST_SYSCALL]; \
  kernel_ptr = (uint8_t *)kernel_base;                                      \
  aslr_patch = &kernel_ptr[K##x##_DISABLE_ASLR];

#define kclock_macro(x)                                                                   \
  kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K##x##_XFAST_SYSCALL];               \
  if (atoi(#x) > 407) {                                                                   \
    sceSblSrtcClearTimeDifference = (void *)(kernel_base + K##x##_CLEAR_TIME_DIFFERENCE); \
    sceSblSrtcClearTimeDifference(15);                                                    \
  }                                                                                       \
  sceSblSrtcSetTime = (void *)(kernel_base + K##x##_SET_TIME);

#define enable_browser_macro(x)                                             \
  kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K##x##_XFAST_SYSCALL]; \
  sceRegMgrSetInt = (void *)(kernel_base + K##x##_REG_MGR_SET_INT);

#define tid_macro(x)                                                        \
  kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K##x##_XFAST_SYSCALL]; \
  kernel_ptr = (uint8_t *)kernel_base;                                      \
  tid_patch = &kernel_ptr[K##x##_TARGET_ID];

#define icc_nvs_write_macro(x)                                              \
  kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K##x##_XFAST_SYSCALL]; \
  icc_nvs_write = (void *)(kernel_base + K##x##_ICC_NVS_WRITE);

#define npdrm_macro(x)                                                      \
  kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K##x##_XFAST_SYSCALL]; \
  kernel_ptr = (uint8_t *)kernel_base;                                      \
  npdrm_open = &kernel_ptr[K##x##_NPDRM_OPEN];                              \
  npdrm_close = &kernel_ptr[K##x##_NPDRM_CLOSE];                            \
  npdrm_ioctl = &kernel_ptr[K##x##_NPDRM_IOCTL];

#define lazyCase(id, macro) case id: macro(id); break;

#define build_kpayload(id, macro)                                \
switch (id) {                                                    \
  lazyCase(350, macro);lazyCase(355, macro);lazyCase(370, macro);\
  lazyCase(400, macro);lazyCase(401, macro);lazyCase(405, macro);\
  lazyCase(406, macro);lazyCase(407, macro);lazyCase(450, macro);\
  lazyCase(455, macro);lazyCase(470, macro);lazyCase(471, macro);\
  lazyCase(472, macro);lazyCase(473, macro);lazyCase(474, macro);\
  lazyCase(500, macro);lazyCase(501, macro);lazyCase(503, macro);\
  lazyCase(505, macro);lazyCase(507, macro);lazyCase(550, macro);\
  lazyCase(553, macro);lazyCase(555, macro);lazyCase(556, macro);\
  lazyCase(600, macro);lazyCase(602, macro);lazyCase(620, macro);\
  lazyCase(650, macro);lazyCase(651, macro);lazyCase(670, macro);\
  lazyCase(671, macro);lazyCase(672, macro);lazyCase(700, macro);\
  lazyCase(701, macro);lazyCase(702, macro);lazyCase(750, macro);\
  lazyCase(751, macro);lazyCase(755, macro);lazyCase(800, macro);\
  lazyCase(801, macro);lazyCase(803, macro);lazyCase(850, macro);\
  lazyCase(852, macro);lazyCase(900, macro);lazyCase(903, macro);\
  lazyCase(904, macro);lazyCase(950, macro);lazyCase(951, macro);\
  lazyCase(960, macro);lazyCase(1000, macro);lazyCase(1001, macro);\
  default: SocketPrintf("Unsupported firmware"); return -1; break;\
}

int is_fw_spoofed();
int is_jailbroken();
uint16_t get_firmware();
int get_firmware_string(char *fw_string);
uint64_t get_kernel_base();
int get_memory_dump(uint64_t kaddr, uint64_t *dump, size_t size);
int jailbreak();
int mmap_patch();
int disable_aslr();
int kernel_clock(uint64_t value);
int enable_browser();
int spoof_target_id(uint8_t id);
int enable_perm_uart();
int exit_idu();
int npdrm_patch();

#endif
