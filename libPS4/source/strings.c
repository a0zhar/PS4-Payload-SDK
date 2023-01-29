#include "file.h"
#include "libc.h"

#include "strings.h"
size_t count_occurrences(char* str, char* word) {
  size_t count = 0;
  char* p = (char*)str;
  while (*p != '\0') {
    if (strstr(p, word) == p) {
      count++;
      p += strlen(word);
    } else {
      p++;
    }
  }
  return count;
}

char* replace_str(char* str, char* word, char* replacement) {
    // Allocate a buffer to store the modified string
  size_t str_len = strlen(str);
  size_t word_len = strlen(word);
  size_t replacement_len = strlen(replacement);
  size_t buffer_len = str_len + (replacement_len - word_len) * count_occurrences(str, word) + 1;
  char* buffer = calloc(buffer_len, sizeof(char));
  if (buffer == NULL) {
    return (char*)str;
  }

  // Iterate through the string and replace occurrences of the word with the replacement
  char* p = (char*)str;
  char* q = buffer;
  while (*p != '\0') {
    if (strstr(p, word) == p) {
        // Found an occurrence of the word, copy the replacement
      strcpy(q, replacement);
      p += word_len;
      q += replacement_len;
    } else {
        // Not an occurrence of the word, copy the character
      *q++ = *p++;
    }
  }
  *q = '\0';

  return buffer;
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

char* read_string(int f) {
  char* string = malloc(sizeof(char) * 65536);
  int c;
  int length = 0;
  if (!string) {
    return string;
  }
  while ((c = fgetc_pointer(f)) != -1) {
    string[length++] = c;
  }
  string[length++] = '\0';

  return realloc(string, sizeof(char) * length);
}

int substring(char* haystack, char* needle) {
  if (strlen(haystack) >= strlen(needle)) {
    for (int i = strlen(haystack) - strlen(needle); i >= 0; i--) {
      int found = 1;
      for (size_t d = 0; d < strlen(needle); d++) {
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
