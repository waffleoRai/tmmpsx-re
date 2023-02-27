#ifndef SLPM87176_ARENA_H
#define SLPM87176_ARENA_H

#include "global.h"

typedef struct MemoryBlock {
	byte* address;
	u32 len;
} MemoryBlock;

typedef struct HeapContext {
	/*0x0000*/ MemoryBlock freeBlocks[64]; //0x8010a260
	/*0x0200*/ u16 freeBlockCount; //0x8010a460 index to above array
	/*0x0202*/ u16 unk_10a462[6]; //0x8010a462
	/*0x0208*/ u16 unk_10a468[0x61]; //0x8010a468
	/*0x02ca*/ u16 unk_10a52a[6]; //0x8010a52a

	/*0x02d0*/ u16 unk_10a530[0x37]; //0x8010a530 a56e is usually accessed with an offset of 0x3e, but I'm putting it here for now.
	//Must go to at least 10a59e - 24 halfwords from a56e
} HeapContext;

typedef struct HeapData {
	//Main heap mem 80111e00 - 801e8000?
	//Zeroed out at apparent init: 80111000 - 801b8000

	/*0x0000*/ byte unk_1101ec[0xe14]; //0x801101ec
	/*0x0000*/ byte unk_111000[0x300]; //0x80111000
	/*0x0000*/ byte unk_111400[0x200]; //0x80111400
	/*0x0000*/ byte unk_111600[0x900]; //0x80111600
	/*0x0000*/ byte arena[0xd6200]; //0x80111e00
	/*0x0000*/ byte fileLoadBuffer[0x8000]; //0x801e8000, used as a load buffer
	/*0x0000*/ byte unk_1f0000[0x2000]; //0x801f0000; Can contain CLUTs. 4-bit before 0x600, 8-bit after
	/*0x0000*/ byte unk_1f2000[16][512]; //0x801f2000; Also appears to hold CLUTs.
	/*0x0000*/ byte unk_1f4000[0x2000]; //0x801f4000; Can contain CLUTs. 4-bit before 0x600, 8-bit after
	/*0x0000*/ byte unk_1f6000[16][512]; //0x801f6000; Also appears to hold CLUTs.
	/*0x0000*/ byte cdReadBuffer[0x800]; //0x801f8000, CD load buffer
} HeapData;

//Arena
u16 Heap_AddAvailableMemory(void* addr, u32 size);
void* Heap_ArenaAlloc(u32 size);
void Heap_ResetArena();
void Heap_FreeMemory(void* addr, uint32_t size);
u16 Heap_SmallestFreeBlockIndex();

//RNG(?)
void Heap_prng_seed_71874();
u16 Heap_prng_next10a530_71964();
int Heap_setfead8_feae0_71A14(u32 arg0, u16 arg1, u32 arg2);
void Heap_prng_71f64();
u16 Heap_prng_71fbc(int arg0);

//ff722 flags
void Heap_IncrAsNumber_ff722_715d4(u16 baseFlagId, u16 count);
void Heap_DecrAsNumber_ff722_71728(u16 baseFlagId, u16 count);
void Heap_ClearAllFlags_ff722_71CD0();
void Heap_SetFlag_ff722_71cf8(u16 flagId);
void Heap_ClearFlag_ff722_71d40(u16 flagId);
void Heap_XorFlag_ff722_71d88(u16 flagId);
int Heap_CheckFlag_ff722_71dd0(u16 flagId);
void Heap_SetFlags_ff722_71e1c(u16 baseFlagId, u16 count, u16 mask);
u16 Heap_CheckFlags_ff722_71edc(u16 baseFlagId, u16 count);


#endif
