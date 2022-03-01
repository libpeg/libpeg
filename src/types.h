#pragma once

#include <stdint.h>
#include <stdio.h>
#include <limits.h>

void *malloc(size_t size);
void *realloc(void *ptr, size_t n);
void free(void *ptr);

size_t strlen(const char *str);
int strcmp(const char *s1, const char *s2);

void exit(int code);
void abort();

typedef uint8_t         byte_t;
typedef uint32_t        ucs4_t;
typedef uint8_t         err_t;

typedef struct {uint32_t op, p1, p2;} insn_t;
typedef struct {size_t pi, si, cli, cpi, pri, cut; } callframe_t;
typedef struct {byte_t open; size_t pi, si;} capframe_t;
typedef struct {capframe_t* frms; size_t flen, fcap;} cap_t;

enum {
    EOK = 0,
    ENOMATCH = 1,
    EWRONGCHR = 2,
    ENOTHINGTOCMT = 3,
    EEOT = 4,
    EUNPAIRPREC = 5,
    ECUT = 6,
};
