#ifndef PSXSDK_LIBGPU_H
#define PSXSDK_LIBGPU_H

#include "psx/PSXTypes.h"
#include "psx/LibGpuMacro.h"

#ifdef __cplusplus
extern "C" {
#endif

//From LIBGPU.H

typedef struct {
	/*0x00*/ short x, y;
	/*0x04*/ short w, h;
} RECT; //Size: 0x8

typedef struct {
	/*0x00*/ uint32_t	tag;
	/*0x04*/ uint32_t	code[15];
} DR_ENV; //Size: 0x40

typedef struct {
	/*0x00*/ uint32_t	tag;
	/*0x04*/ uint32_t	code[2];
} DR_AREA; //Size: 0x0c

typedef struct {
	/*0x00*/ uint32_t	tag;
	/*0x04*/ uint32_t	code[2];
} DR_OFFSET; //Size: 0x0c

typedef struct {
	/*0x00*/ RECT	disp;
	/*0x08*/ RECT	screen;
	/*0x10*/ uint8_t	isinter;
	/*0x11*/ uint8_t	isrgb24;
	/*0x12*/ uint8_t	pad0, pad1;
} DISPENV; //Size: 0x14

typedef struct {
	/*0x000*/ RECT		clip;
	/*0x008*/ short		ofs[2];
	/*0x00c*/ RECT		tw;
	/*0x014*/ uint16_t	tpage;
	/*0x016*/ uint8_t	dtd;
	/*0x017*/ uint8_t	dfe;
	/*0x018*/ uint8_t	isbg;
	/*0x019*/ uint8_t	r0, g0, b0;
	/*0x01c*/ DR_ENV	dr_env;
} DRAWENV; //Size: 0x5c

typedef struct {
	/*0x000*/ uint32_t	tag;
	/*0x004*/ uint8_t	r0, g0, b0, code;
	/*0x008*/ short		x0, y0;
	/*0x00c*/ short		x1,	y1;
	/*0x010*/ short		x2,	y2;
	/*0x014*/ short		x3,	y3;
} POLY_F4; //Size: 0x18

typedef struct {
	/*0x000*/ uint32_t	tag;
	/*0x004*/ uint8_t	r0, g0, b0, code;
	/*0x008*/ short		x0, y0;
	/*0x00c*/ uint8_t	u0, v0;	
	/*0x00e*/ uint16_t	clut;
	/*0x010*/ short		x1,	y1;
	/*0x014*/ uint8_t	u1, v1;	
	/*0x016*/ uint16_t	tpage;
	/*0x018*/ short		x2,	y2;
	/*0x01c*/ uint8_t	u2, v2;	
	/*0x01e*/ uint16_t	pad1;
	/*0x020*/ short		x3,	y3;
	/*0x024*/ uint8_t	u3, v3;	
	/*0x026*/ uint16_t	pad2;
} POLY_FT4;	//Size: 0x28

typedef struct {
	/*0x000*/ uint32_t	tag;
	/*0x004*/ uint8_t	r0, g0, b0, code;
	/*0x008*/ short		x0, y0;
	/*0x00c*/ uint8_t	r1, g1, b1, pad1;
	/*0x010*/ short		x1,	y1;
	/*0x014*/ uint8_t	r2, g2, b2, pad2;
	/*0x018*/ short		x2,	y2;
} POLY_G3; //Size: 0x1c

typedef struct {
	/*0x000*/ uint32_t	tag;
	/*0x004*/ uint8_t	r0, g0, b0, code;
	/*0x008*/ short		x0, y0;
	/*0x00c*/ uint8_t	r1, g1, b1, pad1;
	/*0x010*/ short		x1,	y1;
	/*0x014*/ uint8_t	r2, g2, b2, pad2;
	/*0x018*/ short		x2,	y2;
	/*0x01c*/ uint8_t	r3, g3, b3, pad3;
	/*0x020*/ short		x3,	y3;
} POLY_G4; //Size: 0x24

typedef struct {
	/*0x00*/ uint32_t	tag;
	/*0x04*/ uint8_t	r0, g0, b0, code;
	/*0x08*/ short		x0, y0;
	/*0x0c*/ uint8_t	u0, v0;	
	/*0x0e*/ uint16_t	clut;
	/*0x10*/ short		w,	h;
} SPRT;	//Size: 0x14

typedef struct {
	/*0x00*/ uint32_t	tag;
	/*0x04*/ uint8_t	r0, g0, b0, code;
	/*0x08*/ short		x0, y0;
	/*0x0c*/ short		w,	h;
} TILE; //Size: 0x10

extern DISPENV* GpuSys_GetDispEnv(DISPENV* env);
extern DISPENV* GpuSys_PutDispEnv(DISPENV* env);
extern DISPENV* Gpu_SetDefDispEnv(DISPENV* env, int x, int y, int w, int h);
extern DRAWENV* GpuSys_GetDrawEnv(DRAWENV* env);
extern DRAWENV* GpuSys_PutDrawEnv(DRAWENV* env);
extern DRAWENV* Gpu_SetDefDrawEnv(DRAWENV* env, int x, int y, int w, int h);
extern int GpuSys_ClearImage(RECT* rect, uint8_t r, uint8_t g, uint8_t b);
extern int GpuSys_ClearImage2(RECT* rect, uint8_t r, uint8_t g, uint8_t b);
extern int GpuSys_DrawSync(int mode);
extern int GpuSys_GetGraphDebug(void) ;
extern int GpuSys_LoadImage(RECT* rect, uint32_t* p);
extern int GpuSys_MoveImage(RECT* rect, int x, int y);
extern int GpuSys_ResetGraph(int mode);
extern int GpuSys_SetGraphDebug(int level);
extern int GpuSys_StoreImage(RECT* rect, uint32_t* p);
extern uint32_t* GpuSys_ClearOTag(uint32_t* ot, int n);
extern uint32_t* GpuSys_ClearOTagR(uint32_t* ot, int n);
extern uint32_t GpuSys_DrawSyncCallback(CallbackFunction func);
extern uint16_t Gpu_GetTPage(int tp, int abr, int x, int y);
extern void Gpu_AddPrim(void* ot, void* p);
extern void GpuSys_DrawOTag(uint32_t* p);
extern void GpuSys_DrawOTagEnv(uint32_t* p, DRAWENV* env);
extern void GpuSys_DrawPrim(void* p);
extern void GpuSys_SetDispMask(int mask);
extern void GpuSys_SetDrawArea(DR_AREA* p, RECT* r);
extern void GpuSys_SetDrawEnv(DR_ENV* dr_env, DRAWENV* env);
extern void GpuSys_SetDrawOffset(DR_OFFSET* p, uint16_t* ofs);
extern void Gpu_SetPolyF4(POLY_F4* p);
extern void Gpu_SetPolyFT4(POLY_FT4* p);
extern void Gpu_SetPolyG3(POLY_G3* p);
extern void Gpu_SetPolyG4(POLY_G4* p);
extern void Gpu_SetSemiTrans(void* p, int abe);
extern void Gpu_SetSprt(SPRT* p);
extern void Gpu_SetTile(TILE* p);
extern int GpuSys_GetODE(void);
extern int GpuSys_LoadImage2(RECT* rect, uint32_t* p);
extern int GpuSys_StoreImage2(RECT* rect, uint32_t* p);
extern int GpuSys_MoveImage2(RECT* rect, int x, int y);
extern int GpuSys_DrawOTag2(uint32_t* p);

#ifdef __cplusplus
}
#endif

#endif