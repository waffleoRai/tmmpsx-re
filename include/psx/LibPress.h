#ifndef PSXSDK_LIBPRESS_H
#define PSXSDK_LIBPRESS_H

#include "psx/PSXTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef	uint16_t DECDCTTAB[34816];

typedef struct {
	uint8_t	iq_y[64];	/* IQ (Y): zig-zag order */
	uint8_t	iq_c[64];	/* IQ (Cb,Cr): zig-zag order */
	int16_t	dct[64];	/* IDCT coef (reserved) */
} DECDCTENV;

extern void Press_DecDCTReset(int mode);
extern DECDCTENV* Press_DecDCTGetEnv(DECDCTENV* env);
extern DECDCTENV* Press_DecDCTPutEnv(DECDCTENV* env);
extern void Press_DecDCTin(uint32_t* buf, int mode);
extern void Press_DecDCTout(uint32_t* buf, int size);
extern int Press_DecDCTinSync(int mode) ;
extern int Press_DecDCToutSync(int mode) ;
extern int Press_DecDCTinCallback(void (*func)());
extern int Press_DecDCToutCallback(void (*func)());
extern int Press_VLC_DecDCTvlc2(uint32_t* bs, uint32_t* buf, DECDCTTAB table);
extern int Press_VLC_DecDCTvlcSize2(int size);
extern void Press_Build_DecDCTvlcBuild(uint16_t* table);

#ifdef __cplusplus
}
#endif

#endif