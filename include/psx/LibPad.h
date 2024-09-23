#ifndef PSXSDK_LIBPAD_H
#define PSXSDK_LIBPAD_H

#include "psx/PSXTypes.h"

//From pad.h and LIBPAD.H

/* Controller Pad 1 Defines */
#define Pad1Up          (1<<12)
#define Pad1Down        (1<<14)
#define Pad1Left        (1<<15)
#define Pad1Right       (1<<13)
#define Pad1L1          (1<< 2)
#define Pad1L2          (1<< 0)
#define Pad1R1          (1<< 3)
#define Pad1R2          (1<< 1)
#define Pad1tri         (1<< 4)
#define Pad1sqr         (1<< 7)
#define Pad1crc         (1<< 5)
#define Pad1x           (1<< 6)
#define Pad1Start       (1<<11)
#define Pad1Select      (1<<8)

/* Controller Pad 2 Defines */
#define Pad2Up          (1<<28)
#define Pad2Down        (1<<30)
#define Pad2Left        (1<<31)
#define Pad2Right       (1<<29)
#define Pad2L1          (1<<18)
#define Pad2L2          (1<<16)
#define Pad2R1          (1<<19)
#define Pad2R2          (1<<17)
#define Pad2tri         (1<<20)
#define Pad2sqr         (1<<23)
#define Pad2crc         (1<<21)
#define Pad2x           (1<<22)
#define Pad2Start       (1<<27)
#define Pad2Select      (1<<24)

#define PadStateDiscon		0
#define PadStateFindPad		1
#define PadStateFindCTP1	2
#define PadStateFindCTP2	3
#define PadStateReqInfo		4
#define PadStateExecCmd		5
#define PadStateStable		6

#ifdef __cplusplus
extern "C" {
#endif

extern int PadCmd_PadSetActAlign(int, uint8_t*);
extern int PadCmd_PadSetMainMode(int socket, int offs, int lock);
extern  PadEntry_PadGetState(int);
extern int PadEntry_PadInfoMode(int, int, int);
extern void PadCom_PadStartCom(void);
extern void PadCom_PadStopCom(void);
extern void PadDir_PadInitDirect(uint8_t*, uint8_t*);

#ifdef __cplusplus
}
#endif


#endif