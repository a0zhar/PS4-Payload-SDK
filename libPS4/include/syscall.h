#pragma once

#ifndef SYSCALL_H
#define SYSCALL_H

#define SYS_RANDOMIZED_PATH         602
#define SYS_MDBG_SERVICE            601 
#define SYS_DYNLIB_UNLOAD_PRX       595
#define SYS_DYNLIB_LOAD_PRX         594
#define SYS_DYNLIB_GET_INFO         593
#define syscall_dynlib_getlist         592
#define syscall_dynlib_dlsym        591
#define sys_virtual_query           572
#define sys_mmap                    477
#define sys_query_memory_protection 547
#define sys_munlock                 204
#define sys_mlock                   203
#define sys_mprotect                74
#define sys_munmap                  73
#define sys_msync                   65

#define SYSCALL(name, number)        \
  __asm__(".intel_syntax noprefix"); \
  __asm__(".globl " #name "");       \
  __asm__("" #name ":");             \
  __asm__("movq rax, " #number "");  \
  __asm__("jmp syscall_macro");

unsigned long syscall(unsigned long n, ...);

#endif
