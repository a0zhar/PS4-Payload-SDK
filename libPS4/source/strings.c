#include "../include/file.h"
#include "../include/libc.h"
#include "../include/strings.h"

// Helper function to count occurrences of `word` in `str`
size_t countInstances(const char *str, const char *word) {
  size_t count = 0, word_len = strlen(word);
  const char *p = str;

  while (*p) {
    if (strstr(p, word) == p) {
      count++;
      p += word_len;
    } else {
      p++;
    }
  }
  return count;
}

char *replaceString(const char *_fullstr, const char *_search, const char *_replace) {
  int len1 = strlen(_fullstr);
  int len2 = strlen(_search);
  int len3 = strlen(_replace);
  char *pos, *tmp, *str = (char *)_fullstr;

  if (len2 == 0) return "";


  if ((tmp = malloc(len1)) != NULL) {
    while ((pos = strstr(str, _search)) != NULL) {
      strcpy(tmp, str);
      tmp[pos - str] = '\0';
      strcat(tmp, _replace);
      strcat(tmp, pos + len2);
      strcpy(str, tmp);
      pos = strstr(str, _search);
    }
    return tmp;
  }
  return "";
}


char **splitString(const char *str, const char *delimiter, int *count) {
  // allocate memory for the array of strings
  char **tokens = malloc(sizeof(char *));
  // Error: Failed to allocate memory for tokens array.
  if (tokens == NULL) {return NULL; }

  // initialize the count of tokens to 0
  *count = 0;

  // split the string into tokens using strtok()
  char *token = strtok(str, delimiter);
  while (token != NULL) {
    // add the token to the array of tokens
    tokens[*count] = token;
    (*count)++;

    // resize the array of tokens
    tokens = realloc(tokens, sizeof(char *) * (*count + 1));
    if (tokens == NULL) return NULL;
    // get the next token
    token = strtok(NULL, delimiter);
  }

  // add a null terminator to the end of the array
  tokens[*count] = NULL;

  return tokens;
}

char *read_string(int f) {
  char *str = malloc(65536); // allocate buffer of 65536 bytes
  // Check if the allocation was successful.
  if (str == NULL)
    return NULL;
  int c = 0, length = 0;

  // Read the file character by character until the end of the file is reached.
  for (;;) {
    if ((c = fgetc_pointer(f)) == -1)
      break;
    str[length++] = c; // Store each character in the string.
  }
  str[length++] = '\0';// Add a null terminator to the end of the string.

  // Reallocate the memory for the string to its actual size, freeing up any unused memory.
  return realloc(str, length);
}
/*
The code checks if a "needle" string exists as a substring within a "haystack" string.
It starts by checking if the length of the haystack is greater than or equal to the length of the needle.
If so, it enters a for loop that iterates backwards from the end of the haystack string minus the length
of the needle to the beginning of the haystack string. For each iteration, it sets a variable found to 1.

Then, it enters another for loop that iterates through each character of the needle string and compares
it with the corresponding character in the haystack string. If a mismatch is found, it sets found to 0
and breaks out of the inner for loop. If the inner for loop completes successfully, it means all characters
in the needle string match the corresponding characters in the haystack string, and the function returns the
index of the first character of the needle in the haystack. If the outer for loop finishes without finding a
match, the function returns -1, indicating that the needle is not a substring of the haystack.
*/
// Checks if the string(needle) exists as a substring within string(haystack)
int substring(char *haystack, char *needle) {
  // if length of haystack is more or equal to the length of needle
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
