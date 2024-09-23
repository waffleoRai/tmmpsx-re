#ifndef PSXSDK_TYPES_H
#define PSXSDK_TYPES_H

typedef	unsigned char uint8_t;
typedef	unsigned short uint16_t;
typedef	unsigned int uint32_t;

typedef	uint8_t u8;
typedef	uint16_t u16;
typedef	uint32_t u32;

typedef	char int8_t;
typedef	short int16_t;
typedef	int int32_t;

typedef	int8_t s8;
typedef	int16_t s16;
typedef	int32_t s32;

typedef	unsigned char uchar;
typedef	unsigned short ushort;
typedef	unsigned int uint;

typedef	unsigned int size_t;

typedef int BOOL;

typedef void (*CallbackFunction)(void);

#define NULL ((void*)0)
#define EMPTY_PTR ((void*)~0)

#define TRUE 1
#define FALSE 0

#endif