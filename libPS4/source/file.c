#include "libc.h"
#include "syscall.h"
#include "file.h"

SYSCALL(read, 3);
SYSCALL(write, 4);
SYSCALL(open, 5);
SYSCALL(close, 6);
SYSCALL(unlink, 10);
SYSCALL(link, 9);
SYSCALL(readlink, 58);
SYSCALL(symlink, 57);
SYSCALL(mount, 21);
SYSCALL(nmount, 378);
SYSCALL(unmount, 22);
SYSCALL(fchown, 123);
SYSCALL(fchmod, 124);
SYSCALL(rename, 128);
SYSCALL(mkdir, 136);
SYSCALL(rmdir, 137);
SYSCALL(stat, 188);
SYSCALL(fstat, 189);
SYSCALL(lstat, 190);
SYSCALL(getdents, 272);
SYSCALL(lseek, 478);
SYSCALL(fstatat, 493);

int getFileSize(const char* path) {
  int fd = open(path, O_RDONLY, 0);
  if (fd != -1) {
    lseek(fd, 0, SEEK_END);
    int fsize = lseek(fd, 0, SEEK_CUR);
    lseek(fd, 0, SEEK_SET);
    close(fd);
    return fsize;
  }
  return 0;
}


int getSandboxDirectory(char* destination, int* length) {
  return syscall(602, 0, destination, length);
}
int fileExists(char* fname) {
  int file = open(fname, O_RDONLY, 0);
  if (file != -1) {
    close(file);
    return 1;
  }
  return 0;
}

int directoryExists(char* dname) {
  DIR* dir = opendir(dname);
  if (dir) {
    closedir(dir);
    return 1;
  }
  return 0;
}

int symlink_exists(const char* fname) {
  struct stat statbuf;
  if (lstat(fname, &statbuf) < 0) {
    return -1;
  }
  if (S_ISLNK(statbuf.st_mode) == 1) {
    return 1;
  } else {
    return 0;
  }
}

void touch_file(char* destfile) {
  int fd = open(destfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
  if (fd != -1) {
    close(fd);
  }
}

void  copyFile(char* sourcefile, char* destfile) {
  // Open source file in read-only mode
  int src = open(sourcefile, O_RDONLY, 0);
  if (src == -1) {
    return; // Return early if source file cannot be opened
  }

  // Open destination file in write-only mode, create if it doesn't exist, and truncate existing content
  int out = open(destfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
  if (out == -1) {
    close(src); // Close source file before returning
    return; // Return early if destination file cannot be opened
  }

  // Allocate a buffer to hold the file content
  size_t buffer_size = 3194304; // 3.19MB Chunks
  char* buffer = calloc(1, buffer_size);
  if (buffer == NULL) {
    close(src); // Close source file before returning
    close(out); // Close destination file before returning
    return; // Return early if buffer cannot be allocated
  }

  // Read file content from source file in chunks and write to destination file
  size_t bytes;
  while ((bytes = read(src, buffer, buffer_size)) > 0) {
    write(out, buffer, bytes);
  }

  // Clean up resources
  free(buffer);
  close(src);
  close(out);
}
void copyDirectory(char* sourcedir, char* destdir) {
  DIR* dir = opendir(sourcedir);
  if (!dir) {
    closedir(dir);
    return;
  }
  struct dirent* dp;
  struct stat info;
  char src_path[1024];
  char dst_path[1024];

  mkdir(destdir, 0777);
  while ((dp = readdir(dir)) != NULL) {
    if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..")) {
      continue;
    } else {
      strcpy(src_path, sourcedir);
      strcat(src_path, "/");
      strcat(src_path, dp->d_name);


      snprintf(src_path, sizeof(src_path), "%s/%s", sourcedir, dp->d_name);
      snprintf(dst_path, sizeof(dst_path), "%s/%s", destdir, dp->d_name);

      if (!stat(src_path, &info)) {
        if (S_ISDIR(info.st_mode)) {
          copyDirectory(src_path, dst_path);
        } else if (S_ISREG(info.st_mode)) {
          copyFile(src_path, dst_path);
        }
      }
    }
  }
  closedir(dir);
}

int _DebugLog(const char* logName, char* msg) {
  if (logName == NULL || msg == NULL)
    return 0;
  int dstFile = open(
    logName,
    fileExists(logName) ? O_APPEND : O_CREAT | O_WRONLY,
    SupremePerms
  );

  write(dstFile, msg, sizeof(msg));
  write(dstFile, newLinee, strlen(newLinee));

  close(dstFile);
  return 1;
}
int compareFiles(char* file1, char* file2) {
  if (file1 == NULL || file2 == NULL)
    return 0;

  char* file1Content = 0;
  char* file2Content = 0;
  int result = 0;
  int fd1 = open(file1, O_RDONLY, 0);
  if (fd1 != NULL) {
    long file1Size = getFileSize(file1);
    long file2Size = getFileSize(file2);
    if (file1Size == file2Size) {
      file1Content = calloc(1, file1Size + 1);
      if (file1Content != NULL) {
        read(fd1, file1Content, sizeof(file1Content) - 1);
        int fd2 = open(file2, O_RDONLY, 0);
        if (fd2 != NULL) {
          file2Content = calloc(1, file2Size + 1);
          read(fd2, file2Content, file2Size);
          if (memcmp(file1Content, file2Content, file1Size) == 0) {
            result = 1;
          }
          free(file2Content);
          close(fd2);
        }
      }
      free(file1Content);
    }
    close(fd1);
  }
  return result;
}/*
int compareFiles(char* file1, char* file2) {
  int res = 0;
  int fd1 = open(file1, O_RDONLY, 0);
  int fd2 = open(file2, O_RDONLY, 0);
  if (fd1 < 0 || fd2 < 0) {
    _DebugLog("/data/azLog.txt", "Error opening file");
    return 0;
  }

  off_t size1 = getFileSize(fd1);
  off_t size2 = getFileSize(fd2);

  if (size1 != size2) {
    _DebugLog("/data/azLog.txt", "Files have different sizes");
    goto cleanup;
  }

  char* buffer1 = calloc(1, size1 + 1);
  if (buffer1 == NULL) {
    _DebugLog("/data/azLog.txt", "Error allocating buffer1");
    goto cleanup;
  }
  char* buffer2 = calloc(1, size2 + 1);
  if (buffer2 == NULL) {
    _DebugLog("/data/azLog.txt", "Error allocating buffer2");
    free(buffer1);
    goto cleanup;goto cleanup;
  }

  ssize_t read1 = read(fd1, buffer1, size1);
  ssize_t read2 = read(fd2, buffer2, size2);
  if (read1 != read2) {
    _DebugLog("/data/azLog.txt", "Total read bytes do not match");
    goto cleanup;
  }

  if (memcmp(buffer1, buffer2, size1) == 0) {
    res = 1;
  }

cleanup:
  close(fd1);
  close(fd2);
  return res;
}*/

int rmtree(const char* path) {
  DIR* d = opendir(path);
  int r = -1;

  if (d) {
    struct dirent* p;

    r = 0;
    while (!r && (p = readdir(d))) {
      int r2 = -1;
      char* buf;
      size_t len;

      if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, "..")) {
        continue;
      }

      len = strlen(path) + strlen(p->d_name) + 2;
      buf = malloc(len);

      if (buf) {
        struct stat statbuf;

        snprintf(buf, len, "%s/%s", path, p->d_name);
        if (!stat(buf, &statbuf)) {
          if (S_ISDIR(statbuf.st_mode)) {
            r2 = rmtree(buf);
          } else {
            r2 = unlink(buf);
          }
        }
        free(buf);
      }
      r = r2;
    }
    closedir(d);
  }

  if (!r) {
    r = rmdir(path);
  }

  return r;
}

int fgetc_pointer(int fp) {
  char c;
  if (read(fp, &c, 1) == 0) {
    return (-1);
  }
  return (c);
}

void build_iovec(struct iovec** iov, int* iovlen, const char* name, const void* val, size_t len) {
  int i;
  if (*iovlen < 0) {
    return;
  }
  i = *iovlen;
  *iov = realloc(*iov, sizeof * *iov * (i + 2));
  if (*iov == NULL) {
    *iovlen = -1;
    return;
  }
  (*iov)[i].iov_base = strdup(name);
  (*iov)[i].iov_len = strlen(name) + 1;
  ++i;
  (*iov)[i].iov_base = (void*)val;
  if (len == (size_t)-1) {
    if (val != NULL) {
      len = strlen(val) + 1;
    } else {
      len = 0;
    }
  }
  (*iov)[i].iov_len = (int)len;
  *iovlen = ++i;
}

int mount_large_fs(const char* device, const char* mountpoint, const char* fstype, const char* mode, unsigned int flags) {
  struct iovec* iov = NULL;
  int iovlen = 0;
  build_iovec(&iov, &iovlen, "fstype", fstype, -1);
  build_iovec(&iov, &iovlen, "fspath", mountpoint, -1);
  build_iovec(&iov, &iovlen, "from", device, -1);
  build_iovec(&iov, &iovlen, "large", "yes", -1);
  build_iovec(&iov, &iovlen, "timezone", "static", -1);
  build_iovec(&iov, &iovlen, "async", "", -1);
  build_iovec(&iov, &iovlen, "ignoreacl", "", -1);
  if (mode) {
    build_iovec(&iov, &iovlen, "dirmask", mode, -1);
    build_iovec(&iov, &iovlen, "mask", mode, -1);
  }
  return nmount(iov, iovlen, flags);
}
