#pragma once

#include <stdio.h>

typedef __SIZE_TYPE__   size_t;

typedef __UINT8_TYPE__  uint8_t;
typedef __UINT16_TYPE__ uint16_t;
typedef __UINT32_TYPE__ uint32_t;
typedef __UINT64_TYPE__ uint64_t;

typedef __INT8_TYPE__   int8_t;
typedef __INT16_TYPE__  int16_t;
typedef __INT32_TYPE__  int32_t;
typedef __INT64_TYPE__  int64_t;

void *malloc(size_t size);
void *realloc(void *ptr, size_t n);
void free(void *ptr);

size_t strlen(const char *str);
int strcmp(const char *s1, const char *s2);

void exit(int code);
void abort();

typedef uint8_t         byte_t;
typedef uint32_t        ucs4_t;

typedef struct {byte_t d; size_t i, p;} capframe_t;
typedef struct {capframe_t* frm; size_t len, cap; } cap_t;
typedef struct {size_t pi, si, cli, cpi, pri; } callframe_t;

enum {
    ENOMATCH = 0,
    EWRONGCHR = 1,
};
