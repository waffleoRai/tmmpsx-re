#ifndef PSXSDK_LIBC_H
#define PSXSDK_LIBC_H

#include "psx/PSXTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void LibC_exit(int32_t err);
extern void LibC_puts(char* s);
extern int32_t LibC_setjmp(void* p);
extern char* LibC_strcat(char* dest, char* src);
extern int32_t LibC_strcmp(char* s1, char* s2);
extern int32_t LibC_strncmp(char* s1, char* s2, int32_t n);
extern char* LibC_strcpy(char* dest, char* src);
extern char* LibC_strncpy(char* dest, char* src, int32_t n);
extern void LibC_bzero(uint8_t* p, int32_t n);
extern void* LibC_memcpy(uint8_t* dest, uint8_t* src, int32_t n);
extern void* LibC_memset(uint8_t* s, uint8_t c, int32_t n);
extern int32_t LibC_rand(void);
extern int32_t LibC_printf(char* fmt);
extern int32_t LibC2_strncmp(char* s1, char* s2, int32_t n);

#ifdef __cplusplus
}
#endif

#endif