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

int S_ISDIR(mode_t m) { return ((m & 0170000) == 0040000); }
int S_ISCHR(mode_t m) { return ((m & 0170000) == 0020000); }
int S_ISBLK(mode_t m) { return ((m & 0170000) == 0060000); }
int S_ISREG(mode_t m) { return ((m & 0170000) == 0100000); }
int S_ISFIFO(mode_t m) { return ((m & 0170000) == 0010000); }
int S_ISLNK(mode_t m) { return ((m & 0170000) == 0120000); }
int S_ISSOCK(mode_t m) { return ((m & 0170000) == 0140000); }
int S_ISWHT(mode_t m) { return ((m & 0170000) == 0160000); }


off_t getFileSize(const char* path) {
  int fd = open(path, O_RDONLY | O_CLOEXEC, 0777);
  if (fd == -1) return -1;

  struct stat st;
  if (fstat(fd, &st) == -1) {
    close(fd);
    return -1;
  }
  close(fd);
  return st.st_size;
}


int fileExists(const char* fname) {
  struct stat buffer;
  return (stat(fname, &buffer) == 0);
}

int directoryExists(const char* dname) {
  struct stat buffer;
  if (stat(dname, &buffer) == 0) {
    return S_ISDIR(buffer.st_mode);
  }
  return 0;
}

int symlinkExsist(const char* fname) {
  struct stat statbuf;
  if (lstat(fname, &statbuf) < 0)
    return -1;

  return (S_ISLNK(statbuf.st_mode) == 1);
}

void touchFile(const char* destfile) {
  int fd = open(destfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
  if (fd != -1) {
    close(fd);
  }
}

void copyFile(char* sourcefile, char* destfile) {
  // Open source file in read-only mode
  int src = open(sourcefile, O_RDONLY, 0777);
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
}

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


int getSandboxDirectory(char* destination, int* length) {
  return syscall(602, 0, destination, length);
}