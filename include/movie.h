#ifndef SLPM87176_MOVIE_H
#define SLPM87176_MOVIE_H

#include "global.h"

typedef struct MovieStreamContext {
	/*0x0000*/ RECT unk_0000;
	/*0x0008*/ u32 unk_0008; //04
	/*0x000c*/ u32 unk_000c; //06
	/*0x0010*/ void* unk_0010; //08
	/*0x0014*/ void* unk_0014; //0a
	/*0x0018*/ u32 unk_0018; //0c
	/*0x001c*/ u32 unk_001c; //0e
	/*0x0020*/ u32 unk_0020; //10
	/*0x0028*/ u32 unk_0028; //14
	/*0x002c*/ u32 unk_002c; //16
	/*0x0030*/ u32 unk_0030; //18
	/*0x0034*/ u32 unk_0034; //1a
	/*0x0038*/ u32 unk_0038; //1c
	/*0x003c*/ CdlLOC unk_003c; //1e
	/*0x0040*/ u32 unk_0040; //20
	/*0x0044*/ RECT unk_0044; //22, 23, 24, 25
	/*0x004c*/ void* unk_004c; //26
	/*0x0060*/ u32 unk_0060; //30
	/*0x0080*/ u32 unk_0080; //40
	/*0x2a4c*/ void* unk_2a4c; //1526
} MovieStreamContext; //Size = 0x544c

#endif
