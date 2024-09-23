#ifndef MYUPSX_GBUFF_H
#define MYUPSX_GBUFF_H

#include "psx/PSXTypes.h"

#define GHEAP_SIZE 0xd6200

#ifdef __cplusplus
extern "C" {
#endif

/*----- .bss 0x80111400*/	
extern uint8_t gHeap[];

/*----- .bss 0x801e8000*/
extern uint8_t gFileLoadBuffer[];


/*----- .bss 0x801f8000*/
extern uint8_t gCdLoadBuffer[];

#ifdef __cplusplus
}
#endif

#endif