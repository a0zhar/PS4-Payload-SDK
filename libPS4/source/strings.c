#include "../include/strings.h"

char* replace_str(char* str, char* oldWord, char* newWord) {
  char* resultBuf; // this buffer, will contain the final string (aka with old word replaced)
  int i, count = 0;
  size_t WnewLen = strlen(newWord); // length of new word
  size_t WoldLen = strlen(oldWord); // length of old word
  size_t WfullLen = strlen(str);     // length of input str

  for (i = 0; str[i] != '\0'; i++) {
    if (strstr(&str[i], oldWord) == &str[i]) {
      count++;
      i += WoldLen - 1;
    }
  }
  size_t resLen = WfullLen + count * (WnewLen - WoldLen);
  resultBuf = malloc(resLen + 1); // +1 to allocate space for null terminator
  memset(resultBuf, 0, resLen);
  if (resultBuf == NULL) {
    return str;
  }
  i = 0;
  while (*str) {
    if (strstr(str, oldWord) == str) {
      strcpy(&resultBuf[i], newWord);
      i += WnewLen;
      str += WoldLen;
    } else {
      resultBuf[i++] = *str++;
    }
  }
  resultBuf[i] = '\0';
  return resultBuf;
}

int split_string(char* str, char c, char*** arr) {
  int count = 1;
  int token_len = 1;
  int i = 0;
  char* p;
  char* t;
  p = str;
  while (*p != '\0') {
    if (*p == c) {
      count++;
    }
    p++;
  }
  *arr = (char**)malloc(sizeof(char*) * count);
  if (*arr == NULL) {
    return 0;
  }
  p = str;
  while (*p != '\0') {
    if (*p == c) {
      (*arr)[i] = (char*)malloc(sizeof(char) * token_len);
      if ((*arr)[i] == NULL) {
        return 0;
      }
      token_len = 0;
      i++;
    }
    p++;
    token_len++;
  }
  (*arr)[i] = (char*)malloc(sizeof(char) * token_len);
  if ((*arr)[i] == NULL) {
    return 0;
  }
  i = 0;
  p = str;
  t = ((*arr)[i]);
  while (*p != '\0') {
    if (*p != c && *p != '\0') {
      *t = *p;
      t++;
    } else {
      *t = '\0';
      i++;
      t = ((*arr)[i]);
    }
    p++;
  }
  return count;
}

char* read_string(int fd) {
  char* buf = malloc(65536);
  // if allocation failed!
  if (buf == NULL) {
    return 0;//return
  }

  size_t length = 0;
  for (int c = fgetc_pointer(fd); c != -1;) {
    buf[length++] = c;
  }
  buf[length++] = '\0';

  return realloc(buf, length);
}

int substring(char* haystack, char* needle) {
  int i = 0;
  int d = 0;
  if (strlen(haystack) >= strlen(needle)) {
    for (i = strlen(haystack) - strlen(needle); i >= 0; i--) {
      int found = 1;
      for (d = 0; d < strlen(needle); d++) {
        if (haystack[i + d] != needle[d]) {
          found = 0;
          break;
        }
      }
      if (found == 1) {
        return i;
      }
    }
  }
  return -1;
}
