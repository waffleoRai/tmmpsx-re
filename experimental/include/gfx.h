#ifndef SLPM87176_GFX_H
#define SLPM87176_GFX_H

#include "global.h"

#define GET_FRAME_HEADER(fileHeader, index) (SpriteFrameHeader*)((u32)fileHeader + sizeof(SpriteFileHeader) + (index * sizeof(SpriteFrameHeader)))

#define GET_BITMAP_POINTER(fileHeader, frameHeader) (uint32_t*)((u32)fileHeader + frameHeader->bitmapOffset)

#define GET_CLUT_SECTION_POINTER(spriteFile) (uint16_t*)((u32)spriteFile + sizeof(SpriteFileHeader) + (spriteFile->frameCount << 4))
#define GET_CLUT_ENTRY_COUNT(spriteFile) *(GET_CLUT_SECTION_POINTER(spriteFile))
#define GET_CLUT4_POINTER(spriteFile, clutIndex) (uint16_t*)((byte*)(GET_CLUT_SECTION_POINTER(spriteFile)) + 4 + (clutIndex << 4))
#define GET_CLUT8_POINTER(spriteFile, clutIndex) (uint16_t*)((byte*)(GET_CLUT_SECTION_POINTER(spriteFile)) + 4 + (clutIndex << 8))

typedef struct Clut4 {
	/*0x00*/ uint16_t table[16];
} Clut4;

typedef struct Clut8 {
	/*0x00*/ uint16_t table[256];
} Clut8;

typedef struct SpriteFrameHeader {
	/*0x00*/ u32 bitmapOffset;
	/*0x04*/ u8 clutIndex;
	/*0x05*/ u8 unk_05[3];
	/*0x08*/ u16 width;
	/*0x0a*/ u16 height;
	/*0x0c*/ u16 scaleX;
	/*0x0e*/ u16 scaleY;
}SpriteFrameHeader; //Size = 0x10

typedef struct SpriteFileHeader {
	/*0x00*/ u16 flags; //4 bit if 0x71, 8 bit if 0x51?
	/*0x02*/ u16 frameCount;
	//Then a SpriteFrameHeader for each frame - though this is variable length.
	//Then the total palette entry count, then palettes, then bitmaps
}SpriteFileHeader;

typedef struct FaceSpriteFile {
	/*0x0000*/ SpriteFileHeader fileHeader;
	/*0x0004*/ SpriteFrameHeader frameHeader;
	/*0x0014*/ uint16_t clutTableSize; //Always 0x100
	/*0x0016*/ uint8_t pad_16[2];
	/*0x0018*/ Clut8 clut;
	/*0x0218*/ uint32_t bitmap[0x240];
} FaceSpriteFile;

typedef struct SpriteTxDrawParam {
	/*0x00*/ u8 u_base;
	/*0x01*/ u8 v_base;
	/*0x02*/ u8 width;
	/*0x03*/ u8 height;
	/*0x04*/ u8 x_base;
	/*0x05*/ u8 y_base;
	/*0x06*/ u8 clut;
	/*0x07*/ u8 t_page_xy;
} SpriteTxDrawParam; //Size = 0x8

typedef struct SpriteDrawSlot {
	/*0x00*/ union {
		struct { //Assuming highest to lowest
			u16 clut_loc : 5;
			u16 tpage_abr : 2;
			u16 semitrans : 1;
			u16 unk_7 : 1;
			u16 inUse : 1;
			u16 unk_3 : 3;
			u16 unk_0 : 3;
		} fields;
		u16 value;
	} txSpec;
	/*0x02*/ uint8_t txSlotGroup;
	/*0x03*/ uint8_t txSlotMember;
	/*0x04*/ s16 x_bound;
	/*0x06*/ s16 y_bound;
	/*0x08*/ POLY_FT4 drawPolys[2]; //Primitive type is a guess based on size.
} SpriteDrawSlot; //size = 0x58

typedef struct UnkStruct_101e04 {
	/*0x00*/ u16 unk_00;
} UnkStruct_101e04; //size = 0x3b4

typedef struct UnkStruct_107b04 {
	/*0x000*/ u32 unk_000;
	/*0x000*/ POLY_FT4 unk_004[2][3];
	/*0x0f0*/ POLY_G4 unk_0f4[2];
} UnkStruct_107b04; //size = 0x13c

typedef union union_1012c8 {
	u16 value;
	struct {
		u8 lo;
		u8 hi;
	} bytes;
} union_1012c8; //Size = 0x2

typedef struct UnkStruct_1012c8 {
	/*0x00*/ u16 unk_000; //12c4
	/*0x02*/ u16 unk_002; //12c6
	/*0x04*/ union_1012c8 unk_004; //12c8
	/*0x06*/ u16 unk_006; //12ca
	/*0x08*/ u8 unk_008; //12cc
	/*0x09*/ u8 unk_009; //12cd
	/*0x0a*/ u8 unk_00a; //12ce
	/*0x0b*/ u8 unk_00b; //12cf
} UnkStruct_1012c8; //size = 0xc

typedef struct UnkStruct_1012f8 {
	/*0x000*/ UnkStruct_1012c8 unk_00[4]; //12c4
	/*0x030*/ u16 unk_030; //12f4
	/*0x032*/ u16 unk_032; //12f6
	/*0x034*/ u32 unk_034; //12f8, flags of some kind?
	/*0x038*/ u8* unk_038; //12fc, pointer to data to send to GPU with loadImage
} UnkStruct_1012f8; //size = 0x3c

typedef struct SpriteManager {
	/*0x0000*/ u32 unk_1012c0; //0x801012c0
	/*0x0004*/ UnkStruct_1012f8 unk_1012c4[1]; //0x801012c4

	/*0x0b44*/ UnkStruct_101e04 unk_101e04[1]; //0x80101e04

	/*0x28f4*/ u16 unk_103bb4[0x10]; //0x80103bb4
	/*0x2904*/ u16 unk_103bc4[0x6]; //0x80103bc4
	/*0x2910*/ u8 unk_103bd0; //0x80103bd0
	/*0x2911*/ u8 unk_103bd1; //0x80103bd1
	/*0x2912*/ u16 unk_103bd2; //0x80103bd2
	/*0x2914*/ u16 unk_103bd4; //0x80103bd4
	/*0x2916*/ u16 unk_103bd6; //0x80103bd6
	/*0x2918*/ SpriteDrawSlot loadedSprites[0x40]; //0x80103bd8 - 51d8

	/*0x612e*/ u16 unk_1073ee; //0x801073ee
	/*0x6130*/ u16 unk_1073f0; //0x801073f0
	/*0x6132*/ u16 unk_1073f2; //0x801073f2
	/*0x6134*/ u16 unk_1073f4; //0x801073f4
	/*0x6136*/ u8 unk_1073f6; //0x801073f6
	/*0x6137*/ u8 unk_1073f7; //0x801073f7
	/*0x6138*/ u8 unk_1073f8; //0x801073f8
	/*0x6139*/ u8 unk_1073f9; //0x801073f9

	/*0x683e*/ u8 unk_107afe; //0x80107afe
	/*0x683f*/ u8 unk_107aff; //0x80107aff
	/*0x6840*/ UnkStruct_107b04 unk_107b00[1]; //0x80107b00, unk size
} SpriteManager;

typedef struct UnkStruct_gfx5c3ec {
	/*0x00*/ u16 unk_00;
	/*0x02*/ u16 unk_02;
	/*0x04*/ u8 unk_04;
	/*0x05*/ u8 unk_05;
	/*0x06*/ u8 unk_06;
	/*0x07*/ u8 unk_07;
} UnkStruct_gfx5c3ec; //Size = 0x8

typedef struct UnkStruct_gfx295c {
	/*0x00*/ u8 unk_00[3];
	/*0x03*/ u8 unk_03;
	/*0x04*/ u32 unk_04;
} UnkStruct_gfx295c; //Size = 0x8

typedef struct UnkStruct_gfx296c {
	/*0x00*/ u32 unk_00;
	/*0x04*/ u8 unk_04[0x14];
	/*0x18*/ u32 unk_18;
} UnkStruct_gfx296c; //Size = 0x1c

//Pointed to by 0x800fea98?
typedef struct GraphicsContext {
	/*0x0000*/ u32 unk_0000;
	/*0x0004*/
	/*0x000c*/ SpriteTxDrawParam spriteTxs[1]; //Number unknown. Frequently used in determining where to draw a sprite.

	/*0x280c*/ u16 unk_280c;
	/*0x280e*/ u16 txGroupSlots[0x20]; //Used in part to index unk_000c?
	/*0x284e*/ u16 unk_284e[0x20]; //284e - 288e
	/*0x288e*/
	/*0x28ae*/ u16 unk_28ae;
	/*0x28b0*/ u16 unk_28b0;
	/*0x292c*/ POLY_F4 unk_292c[2]; //292c - 295c
	/*0x295c*/ UnkStruct_gfx295c unk_295c[2];
	/*0x296c*/ UnkStruct_gfx296c unk_296c[0x98];
	/*0x3a0c*/ POLY_FT4 unk_3a0c[0x98];
	/*0x51cc*/ POLY_FT4 unk_51cc[0x98];
	/*0x698c*/
	/*0x6a24*/ u16 unk_6a24;
	/*0x6aae*/ u16 unk_6aae;
	/*0x6ab0*/ u16 unk_6ab0;
	/*0x6ab4*/ DR_AREA unk_6ab4;
	/*0x6ac0*/ DR_AREA unk_6ac0;
	/*0x6acc*/ DR_AREA unk_6acc;
	/*0x6ad8*/ DR_AREA unk_6ad8;
} GraphicsContext; //Size = 0x6af4

void GfxDraw_loadSprite4Bit(SpriteFileHeader* spriteFile, u8 frameIndex, u8 arg2, u8 arg3, byte heapBufferSlot);
void GfxDraw_SetCLUTSemitrans(u16* clut, int entries, int clutCount);
void GfxDraw_loadSprite8Bit_Y256(SpriteFileHeader* spriteFile, u8 frameIndex, u8 heapBufferSlot);

void GfxDraw_renderSpritePolys_5df54(u8 sprslot, byte arg1, byte txSlotOff, byte txSpecs, s16 bound_right, s16 bound_down);
void GfxDraw_renderSpritePolys_5df98(u8 sprslot, byte arg1, byte txSlotOff, byte txSpecs, s16 bound_right, s16 bound_down);
void func_8005E0D4(u32 arg0, u16 arg1);

#endif
