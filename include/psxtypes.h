#ifndef PSXTYPES_H
#define PSXTYPES_H

#include <stdint.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

#define UNKPTR (void*)~0

#define INT_FROM_POINTER(ptr) *((int32_t*)(ptr))
#define UINT_FROM_POINTER(ptr) *((uint32_t*)(ptr))
#define USHORT_FROM_POINTER(ptr) *((uint16_t*)(ptr))

#define STRCPY_INCR(dst, src) while(*src != '\0'){*dst++ = *src++;}

#define READU16(ptr, dst) dst = (u16)*((u8*)ptr); dst |= (u16)*((u8*)ptr + 1) << 8
#define READU24(ptr, dst) dst = (u32)*((u8*)ptr); dst |= (u32)*((u8*)ptr + 1) << 8; dst |= (u32)*((u8*)ptr + 2) << 16
#define READU32(ptr, dst) dst = (u32)*((u8*)ptr); dst |= (u32)*((u8*)ptr + 1) << 8; dst |= (u32)*((u8*)ptr + 2) << 16; dst |= (u32)*((u8*)ptr + 3) << 24

typedef int8_t s8;
typedef uint8_t u8;
typedef int16_t s16;
typedef uint16_t u16;
typedef int32_t s32;
typedef uint32_t u32;

typedef uint8_t byte;

typedef uint8_t unktype_1;
typedef uint16_t unktype_2;
typedef uint32_t unktype_4;

#endif //PSXTYPES_H
