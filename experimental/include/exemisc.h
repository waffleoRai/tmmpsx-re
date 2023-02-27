#ifndef EXEMISC_H
#define EXEMISC_H

#include "CdFile.h"

typedef u32 (*SceLoadDelegate)();

typedef union UnionUnkType4 {

	struct {
		byte b0;
		byte b1;
		byte b2;
		byte b3;
	} asBytes;

	struct {
		u16 hw0;
		u16 hw2;
	} asShorts;

	u32 asInt;

} UnionUnkType4;

typedef struct UnkStruct_ed5b4 {
	u8 unk_ed5b4[0x800]; //0x800ed5b4
	u8 unk_eddb4[1][16][2]; //0x800eddb4
} UnkStruct_ed5b4;


typedef struct UnkStruct_ff868 {
	void* unk_; //0x800ff868
} UnkStruct_ff868;

typedef struct SpriteLoadRequest {
	/*0x00*/ u8 unk_00;
	/*0x01*/ u8 unk_01;
	/*0x02*/ u8 unk_02;
} SpriteLoadRequest; //Size 0x3 (if that's possible?)

typedef struct UnkStruct_e20e4 {
	/*0x00*/ u8 unk_00;
	/*0x00*/ u8 unk_01;
} UnkStruct_e20e4;

typedef struct UnkStruct_e215c {
	/*0x00*/ u8 unk_00;
	/*0x00*/ u8 unk_01;
	/*0x00*/ u8 unk_02;
	/*0x00*/ u8 unk_03;
	/*0x00*/ u8 unk_04;
	/*0x00*/ u8 unk_05;
} UnkStruct_e215c;


typedef struct UnkStruct_ffb80_inner {
	/*0x0000*/ u16 unk_00[16];
	/*0x0020*/ u8 unk_20[4];
	/*0x0024*/ u8 unk_24[0x1e];
	/*0x0042*/ u16 unk_42; //21
	/*0x0042*/ u16 unk_44; //22
	/*0x0042*/ u16 unk_46; //23
	/*0x0042*/ u16 unk_48; //24
	/*0x0042*/ u16 unk_4a; //25
	/*0x0042*/ u16 unk_4c; //26
	/*0x0042*/ u16 unk_4e; //27
	/*0x0042*/ u16 unk_50; //28
	/*0x0052*/ u16 unk_52; //29
	/*0x0054*/ u8 unk_54;
	/*0x0055*/ u8 unk_55;
	/*0x0056*/ u8 unk_56;
	/*0x0057*/ u8 unk_57;
} UnkStruct_ffb80_inner; //Size 0x58

typedef struct UnkStruct_ffc30_inner {
	/*0x0000*/ u16 unk_00;
	/*0x0002*/ u16 unk_02;
	/*0x0004*/ u16 unk_04;
	/*0x0006*/ u16 unk_06;
	/*0x0008*/ u16 unk_08;
	/*0x000a*/ u16 unk_0a;
	/*0x000c*/ u16 unk_0c;
	/*0x000e*/ u8 unk_0e[6];
	/*0x0014*/ POLY_FT4 unk_14;
	/*0x003c*/ POLY_FT4 unk_3c;
	/*0x0064*/ u16 unk_64;
	/*0x0066*/ u16 unk_66;
	/*0x0068*/ u32 unk_68;
} UnkStruct_ffc30_inner; //Size = 0x6c

//Looks like something heapy?
typedef struct UnkStruct_ffb80 {
	void* unk_ffb70; //0x800ffb70 (No idea if these go here, but they do now.)
	void* unk_ffb74; //0x800ffb74
	u16 unk_ffb78; //0x800ffb78
	u16 unk_ffb7a; //0x800ffb7a
	u16 unk_ffb7c; //0x800ffb7c
	u8 unk_ffb7e[2]; //0x800ffb7e

	UnkStruct_ffb80_inner unk_ffb80; //0x800ffb80
	UnkStruct_ffb80_inner unk_ffbd8; //0x800ffbd8
	UnkStruct_ffc30_inner unk_ffc30; //0x800ffc30
	UnkStruct_ffc30_inner unk_ffc9c; //0x800ffc9c
} UnkStruct_ffb80;

typedef struct AudioContext {
	/*0x0000*/ void* unk_0fff10; //0x800fff10
	/*0x0004*/ void* unk_0fff14; //0x800fff14
	/*0x0008*/ u32 unk_0fff18; //0x800fff18
	/*0x000c*/ u8 unk_0fff1c[0xe4]; //0x800fff1c
	/*0x00f0*/ u8 unk_100000[0x10]; //0x80100000
	/*0x0100*/ u32 unk_100010; //0x80100010
	/*0x0104*/ u32 unk_100014; //0x80100014
	/*0x0108*/ UnionUnkType4 unk_100018; //0x80100018 (u16?) Probably part of a struct based on how it is accessed...
	/*0x010c*/ u8 unk_10001c; //0x8010001c
	/*0x010d*/ u8 unk_10001d; //0x8010001d
	/*0x010e*/ u16 unk_10001e; //0x8010001e
	/*0x0110*/ u32 unk_100020[4]; //0x80100020
	/*0x0120*/ u32 unk_100030[4]; //0x80100030
	/*0x0130*/ byte* unk_100040; //0x80100040 
	/*0x0134*/ u32 loadFileOffset; //0x80100044 Used when loading BGM and SE files
	/*0x0138*/ u32 loadFileSize; //0x80100048 Used when loading BGM and SE files
	/*0x013c*/ u32 unk_10004c; //0x8010004c
	/*0x014a*/ s16 seqIds[6]; //0x80100050. May or may not be array?
	/*0x014c*/ s16 vabIds[4]; //0x8010005c VAB IDs?
	/*0x0156*/ u16 unk_100064; //0x80100064
	/*0x0156*/ u16 vabLoadSlot; //0x80100066 (@0x156)
	/*0x0158*/ s8 seqFileIndex; //0x80100068
	/*0x0159*/ u8 unk_100069; //0x80100069
	/*0x015a*/ u8 unk_10006a; //0x8010006a
	/*0x015b*/ u8 unk_10006b; //0x8010006b
	/*0x015c*/ u8 unk_10006c; //0x8010006c
	/*0x015d*/ s8 unk_10006d; //0x8010006d
	/*0x015e*/ u16 unk_10006e; //0x8010006e
	/*0x0160*/ SndVolume2 seqVolume; //0x80100070 
	/*0x0164*/ s16 unk_100074; //0x80100074
	/*0x0166*/ u8 unk_100076; //0x80100076
	/*0x0167*/ u8 unk_100077; //0x80100077
	/*0x0168*/ s16 unk_100078[4]; //0x80100078
	/*0x0170*/ u16 unk_100080; //0x80100080
	/*0x0172*/ s16 unk_100082[4]; //0x80100082
	/*0x017a*/ u8 unk_10008a; //0x8010008a
	/*0x017b*/ u8 unk_10008b; //0x8010008b
	/*0x017c*/ u32 unk_10008c; //0x8010008c VOICE.STR file offset
	/*0x0180*/ u32 unk_100090; //0x80100090
	/*0x0184*/ u8 unk_100094; //0x80100094
	/*0x0185*/ u8 unk_100095; //0x80100095
	/*0x0186*/ u8 unk_100096[2]; //0x80100096
} AudioContext;

typedef struct UnkStruct_100588 {
	/*0x009e*/ u16 unk_100626;
	/*0x00a0*/ u16 unk_100628;
	/*0x00a2*/ u16 unk_10062a;
	/*0x00a4*/ u16 unk_10062c;
	/*0x00a6*/ u16 unk_10062e;
	/*0x00a8*/ u16 unk_100630;
} UnkStruct_100588;

void func_8003A788(s16, int);
void func_8003ADB8();
u8 func_8003AE90();
void func_8003BBB4();
UnkStruct_ffb80_inner* func_8003BC44(UnkStruct_ffb80_inner* arg0);
UnkStruct_ffc30_inner* func_8003BD18(UnkStruct_ffc30_inner* arg0);
void func_8003BDFC(int arg0, u32 arg1);
void func_8003BE70(int arg0, u16 arg1, u16 arg2, u16 arg3, u16 arg4);

void func_8005DF98(unktype_4, unktype_4, unktype_4, unktype_4, unktype_4, unktype_4);
void func_8005E6D0();
void func_8005E86C(void* arg0);
void func_80060C98(); 
void func_80061038();

void func_80077010(int arg0);
int func_8007708C(int arg0);

void func_800C6228(void*, void*); //Looks like it's in LibPad


#endif
