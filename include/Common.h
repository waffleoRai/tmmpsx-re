#ifndef MYUC_MAINCMN_H
#define MYUC_MAINCMN_H

#include "psx/LibGpu.h"

#ifdef __cplusplus
extern "C" {
#endif

	/*
	D_main_bss_feb2c
		D_main_bss_feb5c
		D_main_bss_feb60
		D_main_bss_feb64
		D_main_bss_fec1c
		D_main_bss_fec40
		D_main_bss_fec44
		D_main_bss_fec48
		D_main_bss_fec64
		D_main_bss_fec80
		D_main_bss_fec84
		D_main_bss_fecb4
		D_main_bss_fecbc
		D_main_bss_fecc4
		D_main_bss_fecc8
		D_main_bss_fee40
		D_main_bss_fee44*/

	typedef struct {
		uint16_t x;
		uint16_t y;
		uint16_t w;
		uint16_t h;
	} URECT;

	typedef struct {
		/*0x00*/ u8 unk0[0x20];
		/*0x20*/ u8 unk20;
		/*0x21*/ u8 unk21;
		/*0x22*/ u16 unk22;
	} UnkStruct_fea6c;

	typedef struct {
		/*0x0000*/ u8 unk_0000[0x8]; //fec44
		/*0x0008*/ u8 unk_0008[0x68]; //fec4c
		/*0x0070*/ u8 unk_0070[0x16c]; //feceb
		/*0x01dc*/ u8 unk_01dc[0x20]; //fee20
		/*0x01fc*/ u8 unk_01fc[0x200]; //fee40
		/*0x03fc*/ u8 unk_03fc[0x4]; //ff040
	} UnkStruct_fec44; //Size = 0x400

    typedef struct ApplState {
		/*0x0000*/ u32 stateFlags[3]; //0x800feb28
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
		/*0x003c*/ DRAWENV drawEnv[2]; //0x800feb64, 0x800febc0
		/*0x00f4*/ DISPENV dispEnv[2]; //0x800fec1c, 0x800fec30
		/*0x011c*/ UnkStruct_fec44 unk_fec44[2]; //0x800fec44, ff044 otag
		/*0x091c*/ u16 unk_ff444; //0x800ff444
		/*0x091e*/ u16 unk_ff446; //0x800ff446
    } ApplState;

    extern ApplState gApplState;


#ifdef __cplusplus
}
#endif

#endif
