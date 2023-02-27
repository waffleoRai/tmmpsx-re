#ifndef SLPM87176_GAMESAVE_H
#define SLPM87176_GAMESAVE_H

#include "global.h"

typedef struct MemCardDataReq { //Dummy struct?
	/*0x00*/ u8 unk_00;
	/*0x01*/ u8 unk_01;
	/*0x02*/ u8 unk_02;
	/*0x03*/ u8 unk_03;
	/*0x04*/ u8 unk_04;
	/*0x05*/ u8 unk_05;
	/*0x06*/ u8 unk_06;
	/*0x07*/ u8 unk_07;
} MemCardDataReq;

typedef struct MemCardStruct_10a91c {
	/*0x00*/ u32 unk_00;
	/*0x04*/ u32 unk_04;
	/*0x08*/ u32 unk_08;
	/*0x0c*/ u32 unk_0c;
	/*0x10*/ u8 unk_10[0x50];
} MemCardStruct_10a91c; //Size = 0x60

typedef struct MemCardStruct_10a7d8 {
	/*0x00*/ MemCardStruct_10a7d8_b unk_00[3];
} MemCardStruct_10a7d8; //Size = 0x12

typedef struct MemCardStruct_10a7d8_b {
	/*0x00*/ u8 unk_00[6];
} MemCardStruct_10a7d8_b; //Size = 0x6

typedef struct GameMCFile_struct_238 {
	/*0x000*/ u8 unk_000[0x328];
} GameMCFile_struct_238; //Size = 0x328

typedef struct MemCardFileHeader {
	/*0x00*/ u16 marker; //Should be 0x4353 "SC"
	/*0x02*/ u8 iconFlag;
	/*0x03*/ u8 blockNumber;
	/*0x04*/ u16 title[0x20]; //Shift-JIS encoded string
	/*0x44*/ u8 padding0[0x0c];
	/*0x50*/ u8 padding1[0x10];
	/*0x60*/ u16 iconClut[0x20];
} MemCardFileHeader; //Size = 0x80

typedef struct MemCardFileBase {
	/*0x0000*/ MemCardFileHeader header;
	/*0x0080*/ u8 iconBitmaps[3][0x80];
} MemCardFileBase; //Size = 0x200. Includes header and icon.

typedef struct GameMCFile {
	/*0x0000*/ MemCardFileBase header;
	/*0x0200*/ u32 unk_200;
	/*0x0204*/ u8 unk_204[8];
	/*0x020c*/ u32 unk_20c;
	/*0x0218*/ u32 unk_218;
	/*0x021c*/ u8 unk_21c[0x1c];
	/*0x0238*/ GameMCFile_struct_238 unk_238[8];
} GameMCFile;//Size = 

typedef struct SystemMCFile {
	/*0x0000*/ MemCardFileBase header;
	/*0x0200*/ u32 checksum; //Sum of all shorts for next 0xd0 bytes
	/*0x0204*/ u8 unk_204[4];
	/*0x0208*/ u32 unk_208;
	/*0x020c*/ u16 unk_20c;
	/*0x020e*/ u16 unk_20e;
	/*0x0210*/ u8 unk_210[1]; //Unk len. Appears to be the flags loaded into ff722?
} SystemMCFile;//Size = 0x2000

typedef struct MemCardContext {
	/*0x0000*/ u32 mcChannel; //0x8010a5a0
	/*0x0000*/ u16 unk_10a5a4; //0x8010a5a4
	/*0x0000*/ u16 unk_10a5a6; //0x8010a5a6
	/*0x0000*/ u32 unk_10a5a8; //0x8010a5a8
	/*0x0000*/ u8 unk_10a5ac; //0x8010a5ac
	/*0x0000*/ u8 unk_10a5ad[15]; //0x8010a5ad
	/*0x0000*/ s32 unk_10a5bc[15][3]; //0x8010a5bc
	/*0x0000*/ s32 unk_10a670[15][3]; //0x8010a670
	/*0x0000*/ s32 unk_10a724[15][3]; //0x8010a724
	/*0x0000*/ MemCardStruct_10a7d8 unk_10a7d8[1]; //0x8010a7d8

	/*0x0000*/ u32 unk_10a8e8; //0x8010a8e8
	/*0x0000*/ u32 unk_10a8ec; //0x8010a8ec
	/*0x0000*/ u32 unk_10a8f0; //0x8010a8f0
	/*0x0000*/ u8 unk_10a8f4; //0x8010a8f4
	/*0x0000*/ char mcFileName[20]; //0x8010a8f5
	/*0x0000*/ u8 unk_10a909[0xf]; //0x8010a909
	/*0x0000*/ void* unk_10a918; //0x8010a918
	/*0x0000*/ MemCardStruct_10a91c unk_10a91c[2]; //0x8010a91c

} MemCardContext;

void MCSave_InitMCContext();

void MCSave_StartMemCard();
void MCSave_StopMemCard();

int MCSave_ChecksumMCData(u8* data, s16 len);
void MCSave_WriteMCFileHead(MemCardFileBase* mcFile, SpriteFileHeader* iconData, u8 iconFrameIndex, u32 unused, int saveFileIndex);

#endif
