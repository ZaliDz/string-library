#include "s21_string.h"

s21_size_t s21_strlen(const char *str) {
  int counter = 0;
  while (*str != '\0') {
    counter++;
    str++;
  }
  return counter;
}

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  s21_size_t counter = s21_strlen(dest);
  s21_size_t i = 0;
  for (; i < n; ++i) {
    dest[counter + i] = src[i];
  }
  dest[counter + 1 + i] = '\0';
  return dest;
}

char *s21_strchr(const char *str, int n) {
  const char *result = s21_NULL;
  int x = s21_strlen(str) + 1;
  for (int i = 0; i < x; i++) {
    if (str[i] == n) {
      result = (str + i);
      break;
    }
  }
  return (char *)result;
}

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  s21_size_t i = 0, size = 0;
  for (; i < n; ++i) {
    if (str1[i] != str2[i]) {
      size = str1[i] - str2[i];
      break;
    }
  }
  return size;
}

char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  s21_size_t i = 0;
  for (; i < n; i++) {
    if (src[i] != '\0') {
      dest[i] = src[i];
    } else {
      dest[i] = '\0';
    }
  }
  return dest;
}

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  s21_size_t size = 0;
  for (const char *a = str1; *a; a++) {
    s21_size_t flag = 0;
    for (const char *b = str2; *b; b++) {
      if (*a == *b) {
        flag = 1;
        break;
      }
    }
    if (flag) {
      break;
    } else {
      size++;
    }
  }
  return size;
}

void *s21_memchr(const void *str, int c, s21_size_t n) {
  char *p = (char *)str;
  char symbol = c;
  char *pEntry = s21_NULL;
  for (s21_size_t i = 0; i < n; i++) {
    if (p[i] == symbol) {
      pEntry = &p[i];
      break;
    }
  }
  return (void *)pEntry;
}

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  s21_size_t size = 0;
  const char *ptr1 = str1;
  const char *ptr2 = str2;
  for (s21_size_t i = 0; i < n; i++) {
    if (ptr1[i] != ptr2[i]) {
      size = ptr1[i] - ptr2[i];
      break;
    }
  }
  return size;
}

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  char *ptr = dest;
  const char *str = src;
  for (s21_size_t i = 0; i < n; i++) {
    if (str[i] != '\0') {
      ptr[i] = str[i];
    } else {
      ptr[i] = '\0';
    }
  }
  return ptr;
}

void *s21_memset(void *str, int c, s21_size_t n) {
  char *src = str;
  for (s21_size_t i = 0; i < n; i++) {
    src[i] = c;
  }
  return src;
}

char *s21_strpbrk(const char *str1, const char *str2) {
  char *res = s21_NULL;

  for (const char *a = str1; *a; a++) {
    for (const char *b = str2; *b; b++) {
      if (*a == *b) {
        res = (char *)a;
        break;
      }
    }

    if (res) {
      break;
    }
  }
  return res;
}

char *s21_strrchr(const char *str, int n) {
  const char *result = s21_NULL;
  int x = s21_strlen(str);
  for (int i = x; i >= 0; i--) {
    if (str[i] == n) {
      result = (str + i);
      break;
    }
  }
  return (char *)result;
}

char *s21_strstr(const char *haystack, const char *needle) {
  char *res = s21_NULL;
  if (s21_strlen(haystack) >= s21_strlen(needle)) {
    for (s21_size_t i = 0; i <= s21_strlen(haystack) - s21_strlen(needle);
         i++) {
      int found = 1;
      for (s21_size_t j = i, k = 0; needle[k]; k++, j++) {
        if (haystack[j] != needle[k]) {
          found = 0;
          break;
        }
      }
      if (found) {
        res = (char *)haystack + i;
        break;
      }
    }
  }
  return res;
}

char *s21_strtok(char *str, const char *delim) {
  static char *final;
  register int ch;

  if (str == 0) {
    str = final;
  }
  do {
    if ((ch = *str++) == '\0') {
      return 0;
    }
  } while (s21_strchr(delim, ch));
  --str;
  final = str + s21_strcspn(str, delim);
  if (*final != 0) {
    *final++ = 0;
  }
  return str;
}

void *s21_to_upper(const char *str) {
  s21_size_t size = s21_strlen(str);
  char *ptr = malloc((size + 1) * sizeof(char));
  if (ptr != s21_NULL) {
    for (s21_size_t i = 0; i < size; i++) {
      if (str[i] >= 97 && str[i] <= 122) {
        ptr[i] = str[i] - 32;
      } else {
        ptr[i] = str[i];
      }
    }
    ptr[size] = '\0';
  }
  return ptr;
}

void *s21_to_lower(const char *str) {
  s21_size_t size = s21_strlen(str);
  char *ptr = malloc((size + 1) * sizeof(char));
  if (ptr != s21_NULL) {
    for (s21_size_t i = 0; i < size; i++) {
      if (str[i] >= 65 && str[i] <= 90) {
        ptr[i] = str[i] + 32;
      } else {
        ptr[i] = str[i];
      }
    }
    ptr[size] = '\0';
  }
  return ptr;
}

void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  char *res = s21_NULL;

  if (src != s21_NULL && str != s21_NULL) {
    s21_size_t src_len = s21_strlen(src);
    s21_size_t str_len = s21_strlen(str);
    s21_size_t new_len = src_len + str_len;

    res = calloc(new_len + 1, sizeof(char));

    if (res) {
      for (s21_size_t i = 0, j = 0, m = 0; i < new_len; i++) {
        if (i < start_index || !str[m])
          res[i] = src[j++];
        else
          res[i] = str[m++];
      }
      res[new_len] = '\0';
    }
  } else if (str == s21_NULL && src != s21_NULL) {
    res = calloc(s21_strlen(src) + 1, sizeof(char));
    s21_strcpy(res, src);
  }
  return res;
}

void *s21_trim(const char *src, const char *trim_chars) {
  char *res = s21_NULL;
  if (src && trim_chars) {
    char *start = (char *)src;
    char *end = (char *)src + s21_strlen(src);
    for (; *start && s21_strchr(trim_chars, *start); start++)
      ;
    for (; end != start && s21_strchr(trim_chars, *(end - 1)); end--)
      ;
    res = (char *)malloc((end - start + 1) * sizeof(char));
    if (res) {
      s21_strncpy(res, start, end - start);
      *(res + (end - start)) = '\0';
    }
  }
  return res;
}

char *s21_strcat(char *dest, const char *src) {
  int dest_len = s21_strlen(dest);
  int i = 0;

  for (; src[i]; i++) {
    dest[dest_len + i] = src[i];
  }

  dest[dest_len + i] = '\0';

  return dest;
}

char *s21_strcpy(char *dest, const char *src) {
  for (int i = 0; src[i]; i++) {
    dest[i] = src[i];
  }

  dest[s21_strlen(src)] = '\0';

  return dest;
}

s21_size_t s21_strspn(const char *str1, const char *str2) {
  s21_size_t counter = 0;

  for (const char *a = str1; *a; a++) {
    unsigned short was = 0;
    for (const char *b = str2; *b; b++) {
      if (*a == *b) {
        counter++;
        was = 1;
        break;
      }
    }
    if (!was) break;
  }
  return counter;
}

unsigned long long int s21_strntollu(const char *string, char **endptr,
                                     int basis, int n_byte) {
  unsigned long long res = 0;
  short sign = 1;
  if (endptr) *endptr = s21_NULL;
  char dict[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  if (*string == '-') {
    sign = -1;
    string++;
  }
  if (basis == 16 &&
      (!s21_strncmp(string, "0x", 2) || !s21_strncmp(string, "0X", 2)))
    string += 2;
  long long val;
  short exit = 0;
  while (*string && n_byte && !exit) {
    char *tmp2;
    char current_sim =
        (*string >= 'a' && *string <= 'z') ? *string - 'a' + 'A' : *string;
    tmp2 = s21_strchr(dict, (int)current_sim);
    if (!tmp2) {
      exit = 1;
    } else {
      val = (tmp2 - dict) / sizeof(char);

      if (val >= basis) {
        exit = 1;
      } else {
        res = res * basis + val;
        string++;
        n_byte--;
      }
    }
    if (exit) *endptr = (char *)string;
  }
  return res * sign;
}

int s21_atoi(const char *str) {
  int res = 0;
  int sign = 1;
  int overflow = 0;

  while (*str == ' ') str++;

  if (*str == '-') {
    str++;
    sign = -1;
  }

  if (*str == '+') {
    str++;
  }

  while (*str && *str >= '0' && *str <= '9') {
    res = res * 10 + (*str - '0');
    if (res < 0) {
      overflow = 1;
      break;
    }
    str++;
  }
  if (overflow)
    res = sign > 0 ? INT32_MAX : INT32_MIN;
  else
    res *= sign;

  return res;
}