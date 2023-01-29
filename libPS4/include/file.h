#pragma once

#ifndef FILE_H
#define FILE_H

#include "types.h"


#define PATH_MAX 255
#define MNT_UPDATE 0x0000000000010000ULL /* not real mount, just update */

#define O_RDONLY    0x0000 //Open the file for reading. 
#define O_WRONLY    0x0001 //Open the file for writing. 
#define O_RDWR      0x0002 //Open the file for both reading and writing. 

#define O_NONBLOCK  0x0004 // no delay
#define O_APPEND    0x0008 // set append mode
#define O_CREAT     0x0200 // create if nonexistent
#define O_TRUNC     0x0400 // truncate to zero length
#define O_EXCL      0x0800 // error if already exists

#define O_ASYNC     0x0040 // send signal when data ready 
#define O_DIRECT    0x8000 // direct disk access 
#define O_NOFOLLOW  0x0100 // don't follow links 
#define O_SYNC      0x0010 // synchronous writes 
#define O_DSYNC     0x0080 // synchronous writes of data only 
#define O_RSYNC     0x0400 // synchronous reads 
#define O_NOATIME   0x0400 // don't update access time
#define O_ACCMODE   0x0003

#define O_DIRECTORY 0x10000 // fail if not a directory 
#define O_NOCTTY    0x20000 // don't assign controlling terminal 
#define O_CLOEXEC   0x80000 // set close-on-exec

#define SupremePerms 0777 //rwxrwxrwx   
#define newLinee     "\n" //newline

#define DT_BLK  6  // block device file  
#define DT_CHR  2  // character device file
#define DT_DIR  4  // directory
#define DT_FIFO 1  // FIFO (named pipe) file
#define DT_LNK  10 // symbolic link
#define DT_REG  8  // regular file
#define DT_SOCK 12 // socket file  

struct stat {
  dev_t st_dev;            /* inode's device */
  ino_t st_ino;            /* inode's number */
  mode_t st_mode;          /* inode protection mode */
  nlink_t st_nlink;        /* number of hard links */
  uid_t st_uid;            /* user ID of the file's owner */
  gid_t st_gid;            /* group ID of the file's group */
  dev_t st_rdev;           /* device type */
  struct timespec st_atim; /* time of last access */
  struct timespec st_mtim; /* time of last data modification */
  struct timespec st_ctim; /* time of last file status change */
  off_t st_size;           /* file size, in bytes */
  blkcnt_t st_blocks;      /* blocks allocated for file */
  blksize_t st_blksize;    /* optimal blocksize for I/O */
  fflags_t st_flags;       /* user defined flags for file */
  uint32_t st_gen;         /* file generation number */
  int32_t st_lspare;
  struct timespec st_birthtim; /* time of file creation */
  unsigned int : (8 / 2)* (16 - (int)sizeof(struct timespec));
  unsigned int : (8 / 2)* (16 - (int)sizeof(struct timespec));
};

struct dirent {
  uint32_t d_fileno;
  uint16_t d_reclen;
  uint8_t d_type;
  uint8_t d_namlen;
  char d_name[255 + 1];
};

struct iovec {
  void* iov_base;
  size_t iov_len;
};



int S_ISDIR(mode_t m);
int S_ISCHR(mode_t m);
int S_ISBLK(mode_t m);
int S_ISREG(mode_t m);
int S_ISFIFO(mode_t m);
int S_ISLNK(mode_t m);
int S_ISSOCK(mode_t m);
int S_ISWHT(mode_t m);

ssize_t read(int fd, void* buf, size_t nbyte);
ssize_t write(int fd, const void* buf, size_t count);
int open(const char* path, int flags, int mode);
int close(int fd);
int link(const char* path, const char* link);
int unlink(const char* pathname);
int readlink(const char* path, char* buf, int bufsiz);
int symlink(const char* path, const char* link);
int mount(const char* type, const char* dir, int flags, void* data);
int nmount(struct iovec* iov, uint32_t niov, int flags);
int unmount(const char* dir, int flags);
int fchown(int fd, int uid, int gid);
int fchmod(int fd, int mode);
int rename(const char* oldpath, const char* newpath);
int mkdir(const char* pathname, mode_t mode);
int rmdir(const char* path);
int stat(const char* path, struct stat* sb);
int fstat(int fd, struct stat* sb);
int fstatat(int fd, const char* path, struct stat* buf, int flag);
int lstat(const char* path, struct stat* buf);
int getdents(int fd, char* buf, int count);
off_t lseek(int fildes, off_t offset, int whence);
int getSandboxDirectory(char* destination, int* length);



int symlinkExsist(const char* fname);
int fileExists(const char* fname);
int directoryExists(const char* dname);

int rmtree(const char* path);
int fgetc_pointer(int fp);
int mount_large_fs(const char* device, const char* mountpoint, const char* fstype, const char* mode, unsigned int flags);
void create_iovec(struct iovec** iov, int* iovlen, const char* name, const void* val, size_t len);

// File Functions
void copyFile(char* sourcefile, char* destfile);
int compareFiles(char* fname1, char* fname2);
off_t getFileSize(const char* path);
void touchFile(const char* destfile);

// Directory Functions
void copyDirectory(char* sourcedir, char* destdir);

#endif
