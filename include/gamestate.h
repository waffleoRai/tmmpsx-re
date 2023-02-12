#ifndef SLPM87176_GAMESTATE_H
#define SLPM87176_GAMESTATE_H

#include "global.h"

#define GET_STATEFLAG(index) (sGlobalCtx.applState.stateFlags & (1 << (index & 0x1f)))
#define SET_STATEFLAG(index) (sGlobalCtx.applState.stateFlags |= (1 << (index & 0x1f)))
#define CLEAR_STATEFLAG(index) (sGlobalCtx.applState.stateFlags &= ~(1 << (index & 0x1f)))

//Might be true static data start? Exe would end at ff000, but set data ends at fea48.
//But fields before feb28 appear to be referenced more like .data instead of as structs.
//Plus $gp appears to be ~800fe874?
typedef struct ApplicationState {
	/*0x0000*/ u32 stateFlags; //0x800feb28 bitfield/flags
	/*0x0004*/ u32 unk_feb2c; //0x800feb2c
	/*0x0008*/ u32 unk_feb30; //0x800feb30
	/*0x000c*/ u8 unk_feb34[0x8]; //0x800feb34
	/*0x0014*/ u32 unk_feb3c; //0x800feb3c
	/*0x0018*/ u32 unk_feb40; //0x800feb40
	/*0x001c*/ u32 unk_feb44; //0x800feb44
	/*0x0020*/ u32 unk_feb48; //0x800feb48
	/*0x0024*/ u8 unk_feb4c[0x4]; //0x800feb4c
	/*0x0028*/ u32 unk_feb50; //0x800feb50
	/*0x002c*/ u32 unk_feb54; //0x800feb54
	/*0x0030*/ u32 unk_feb58; //0x800feb58
	/*0x0034*/ u32 unk_feb5c; //0x800feb5c
	/*0x0038*/ s32 unk_feb60; //0x800feb60 @0x38 within struct
	/*0x003c*/ DRAWENV unk_feb64; //0x800feb64
	/*0x0098*/ DRAWENV unk_febc0; //0x800febc0

	/*0x00f4*/ DISPENV unk_fec1c; //0x800fec1c
	/*0x0108*/ DISPENV unk_fec30; //0x800fec30
	/*0x011c*/ u8 unk_fec44[0x70]; //0x800fec44 otag?
	/*0x018c*/ u8 unk_fecb4[0x390]; //0x800fecb4
	/*0x051c*/ u8 unk_ff044[0x400]; //0x800ff044
	/*0x091c*/ u16 unk_ff444; //0x800ff444
	/*0x091e*/ u16 unk_ff446; //0x800ff446
	/*0x0920*/ u8 unk_ff448[6]; //0x800ff448
} ApplicationState;

#endif
