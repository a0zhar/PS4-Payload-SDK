#include "../include/strings.h"

char* replace_str(char* input_string, char* old_string, char* new_string) {
  char* result_string;
  int i, count = 0;
  size_t new_length = strlen(new_string);
  size_t old_length = strlen(old_string);
  size_t input_length = strlen(input_string);
  for (i = 0; input_string[i] != '\0'; i++) {
    if (strstr(&input_string[i], old_string) == &input_string[i]) {
      count++;
      i += old_length - 1;
    }
  }
  size_t result_length = input_length + count * (new_length - old_length);
  result_string = malloc(result_length + 1); // +1 to allocate space for null terminator
  if (result_string == NULL) {
    return input_string;
  }
  i = 0;
  while (*input_string) {
    if (strstr(input_string, old_string) == input_string) {
      strcpy(&result_string[i], new_string);
      i += new_length;
      input_string += old_length;
    } else {
      result_string[i++] = *input_string++;
    }
  }
  result_string[i] = '\0';
  return result_string;
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
