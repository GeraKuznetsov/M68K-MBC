#ifndef STRING_H_
#define STRING_H_

#include <stddef.h>
#include <stdint.h>

int memcmp(const void* s1, const void* s2, size_t n);

void* memset(void* dest, int c, size_t n);

void* memcpy(void* __restrict dest, const void* __restrict src, size_t n);

void* memmove(void* dest, const void* src, size_t n);

void* memchr(const void* s, int c, size_t n);

void* memmem(const void* l, size_t l_len, const void* s, size_t s_len);

size_t strlen(const char* str);

size_t strnlen(const char* str, size_t maxlen);

char* strcpy(char* __restrict dst, const char* __restrict src);

char* strncpy(char* __restrict dst, const char* __restrict src, size_t maxlen);

char* strstr(const char* string, const char* substring);

char* strnstr(const char* s, const char* find, size_t slen);

int strcmp(const char* s1, const char* s2);

int strncmp(const char* s1, const char* s2, size_t n);

char* strdup(const char* str);

char* strndup(const char* str, size_t n);

char* strchr(const char* s, int c);

char* strrchr(const char* s, int c);

char* strcat(char* __restrict dst, const char* __restrict src);

char* strncat(char* __restrict dst, const char* __restrict src, size_t maxlen);

char* strtok(char* s, const char* delim);
char* strtok_r(char* s, const char* delim, char** last);

// TODO: documentation
size_t strxfrm(char* __restrict, const char* __restrict, size_t);
size_t strcspn(const char*, const char*);
size_t strspn(const char*, const char*);
char* strpbrk(const char*, const char*);
int strcoll(const char*, const char*);
char* strerror(int);
int strerror_r(int, char*, size_t);

#endif /* STRING_H_ */
