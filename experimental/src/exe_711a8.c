#include "arena.h"

u16 Heap_prng_next10a530_71964();
u16 func_80071FBC(int arg0);

//711A8
u16 Heap_AddAvailableMemory(void* addr, u32 size) {
    //arg0 is a void* - update eventually.

    int i = 0;
    int j = 0;
    byte* newaddr;
    byte* recaddr;

    u32 temp32_0;
    u32 temp32_1;

    if (sHeapCtx.freeBlockCount > 0x3f) return 0xffff;

    //temp32_2 = arg1 + 3U & ~4; //arg1 ceilinged to nearest word.
    newaddr = (byte*)addr;
    size = (size + 3) & ~4; //arg1 ceilinged to nearest word.
    if (sHeapCtx.freeBlockCount != 0) {
        //If there are already records in 10a260...
        for (i = 0; i < sHeapCtx.freeBlockCount; i++) {
            //for each existing record...
            recaddr = sHeapCtx.freeBlocks[i].address;
            if (recaddr == newaddr + size) {
                //If start of this record is end of requested one, merge?
                sHeapCtx.freeBlocks[i].address = newaddr;
                sHeapCtx.freeBlocks[i].len += size;

                if (i >= sHeapCtx.freeBlockCount) return i; //Should not be possible, but hey it's here.
                for (j = i; j < sHeapCtx.freeBlockCount; j++) {
                    //For each record after the one we are looking at...
                    temp32_1 = sHeapCtx.freeBlocks[j].len;
                    if (sHeapCtx.freeBlocks[j].address + temp32_1 == newaddr) {
                        //End of later record is start of new one. Looks like we then merge it into this later one?
                        sHeapCtx.freeBlocks[j].len = temp32_1 + sHeapCtx.freeBlocks[i].len;
                        if (i != --sHeapCtx.freeBlockCount) {
                            sHeapCtx.freeBlocks[i].address = sHeapCtx.freeBlocks[sHeapCtx.freeBlockCount].address;
                            sHeapCtx.freeBlocks[i].len = sHeapCtx.freeBlocks[sHeapCtx.freeBlockCount].len;
                            sHeapCtx.freeBlocks[sHeapCtx.freeBlockCount].address = 0;
                            sHeapCtx.freeBlocks[sHeapCtx.freeBlockCount].len = 0;
                            return j;
                        }
                        sHeapCtx.freeBlocks[i].address = 0;
                        sHeapCtx.freeBlocks[i].len = 0;
                        return j;
                    }
                    if (j > 0xffff) j &= 0xffff;
                }
                return i;
            }

            if (recaddr + sHeapCtx.freeBlocks[i].len == newaddr) {
                //if end of this record is start of requested one.
                sHeapCtx.freeBlocks[i].len += size;
                if (i >= sHeapCtx.freeBlockCount) return i;
                for (j = i; j < sHeapCtx.freeBlockCount; j++) {
                    if (sHeapCtx.freeBlocks[j].address == newaddr + size) {
                        //start of record j is end of new record
                        sHeapCtx.freeBlocks[j].address = sHeapCtx.freeBlocks[i].address;
                        sHeapCtx.freeBlocks[j].len += sHeapCtx.freeBlocks[i].len;

                        if (i == --sHeapCtx.freeBlockCount) {
                            sHeapCtx.freeBlocks[i].address = 0;
                            sHeapCtx.freeBlocks[i].len = 0;
                        }
                        else {
                            sHeapCtx.freeBlocks[i].address = sHeapCtx.freeBlocks[sHeapCtx.freeBlockCount].address;
                            sHeapCtx.freeBlocks[i].len = sHeapCtx.freeBlocks[sHeapCtx.freeBlockCount].len;
                            sHeapCtx.freeBlocks[sHeapCtx.freeBlockCount].address = NULL;
                            sHeapCtx.freeBlocks[sHeapCtx.freeBlockCount].len = 0;
                        }
                        return j;
                    }

                    if (j > 0xffff) j &= 0xffff;
                }
                return i;
            }
            if (i > 0xffff) i &= 0xffff;
        }
    }

    sHeapCtx.freeBlocks[sHeapCtx.freeBlockCount].address = newaddr;
    sHeapCtx.freeBlocks[sHeapCtx.freeBlockCount].len = size;
    return sHeapCtx.freeBlockCount++;
}

//714A4
void* Heap_ArenaAlloc(u32 size) {

    int i;
    //u32 temp32_0;
    u32 minsize = ~0;
    u32 recordIndex = 0;
    byte* allocAddr;

    size = (size + 3) & ~0x4; //Snap to word.
    if (sHeapCtx.freeBlockCount != 0) {
        for (i = 0; i < sHeapCtx.freeBlockCount; i++) {
            if ((sHeapCtx.freeBlocks[i].len >= size) && (sHeapCtx.freeBlocks[i].len < minsize)) {
                minsize = sHeapCtx.freeBlocks[i].len;
                recordIndex = i;
            }
        }
    }

    if (minsize != ~0) {
        allocAddr = sHeapCtx.freeBlocks[recordIndex].address;
        sHeapCtx.freeBlocks[recordIndex].address += size;
        sHeapCtx.freeBlocks[recordIndex].len -= size;
        if (sHeapCtx.freeBlocks[recordIndex].len == 0) {
            if (recordIndex == --sHeapCtx.freeBlockCount) {
                sHeapCtx.freeBlocks[recordIndex].address = NULL;
            }
            else {
                sHeapCtx.freeBlocks[i].address = sHeapCtx.freeBlocks[sHeapCtx.freeBlockCount].address;
                sHeapCtx.freeBlocks[i].len = sHeapCtx.freeBlocks[sHeapCtx.freeBlockCount].len;
                sHeapCtx.freeBlocks[sHeapCtx.freeBlockCount].address = NULL;
                sHeapCtx.freeBlocks[sHeapCtx.freeBlockCount].len = 0;
            }
        }
        return allocAddr;
    }
    return UNKPTR;
}

//Treat count bits as number and increment it?
//715D4
void Heap_IncrAsNumber_ff722_715d4(u16 baseFlagId, u16 count) {

    int i;
    u16 temp16_0; //uvar5
    u16 temp16_1; //uvar4
    u16 temp16_2; //uvar1
    u32 temp32_0; //uvar6
    
    temp16_0 = 0;
    temp16_1 = 1;
    temp16_2 = baseFlagId;
    if (((baseFlagId + count) < 0x600) && (count < 0x11)) {
        //Looks like this one copies the requested flags into a temp
        for (i = 0; i < count; i++) {
            if (sUnk_ff520.unk_ff722[baseFlagId >> 3] & sUnk_gp.byteBitMasks[baseFlagId & 0x7]) {
                temp16_0 |= temp16_1;
            }
            temp16_1 <<= 1;
            temp16_2++;
        }
    }

    temp32_0 = (1 << (count & 0x1f)) - 1;
    if (temp16_0 < temp32_0) {
        temp16_0++; //Why?
        temp16_1 = 1;
        //Then this loop re-sets the flags based on a modification of what it fetched before?
        if (((baseFlagId + count) < 0x600) && (count < 0x11)) {
            if (temp32_0 < temp16_0) {
                //Can't figure out why it would do this? temp32_0 should be a mask with all requested bits on.
                //But why use it to replace other?
                temp16_0 = (u16)temp32_0;
            }
            for (i = 0; i < count; i++) {
                if (!(temp16_0 & temp16_1)) {
                    sUnk_ff520.unk_ff722[baseFlagId >> 3] &= ~sUnk_gp.byteBitMasks[baseFlagId & 0x7];
                }
                else {
                    sUnk_ff520.unk_ff722[baseFlagId >> 3] |= sUnk_gp.byteBitMasks[baseFlagId & 0x7];
                }
                baseFlagId++;
                temp16_1 <<= 1;
            }
        }
    }
}

//This might be the same as above, but a decrement?
//71728
void Heap_DecrAsNumber_ff722_71728(u16 baseFlagId, u16 count) {

    u16 temp16_1;
    u16 temp16_2;
    u16 temp16_3;
    u16 temp16_4;
    u16 temp16_5;
    u16 temp16_6;

    s32 temp32_1;
    s32 temp32_2;

    int i;

    temp16_5 = 1;
    temp16_6 = 0;
    if (((baseFlagId + count) < 0x600) && (count < 0x11)) {
        temp16_3 = baseFlagId;
        for (i = 0; i < count; i++) {
            if (sUnk_ff520.unk_ff722[temp16_3 >> 3] & sUnk_gp.byteBitMasks[temp16_3 & 0x7]) {
                temp16_6 |= temp16_5;
            }
            temp16_5 <<= 1;
            temp16_3++;
        }
    }

    if (temp16_6 != 0) {
        temp32_2 = (s32)temp16_6 - 1;
        temp16_6 = 1;
        if (((baseFlagId + count) < 0x600) && (count < 0x11)) {
            temp32_1 = (1 << (count & 0x1f)) - 1;
            if (temp32_1 < temp32_2) {
                temp16_5 = (u16)temp32_1;
            }
            temp16_3 = baseFlagId;
            for (i = 0; i < count; i++) {
                if (!(temp16_5 & temp16_6)) {
                    sUnk_ff520.unk_ff722[temp16_3 >> 3] &= ~sUnk_gp.byteBitMasks[temp16_3 & 0x7];
                }
                else {
                    sUnk_ff520.unk_ff722[temp16_3 >> 3] |= sUnk_gp.byteBitMasks[temp16_3 & 0x7];
                }
                temp16_3++;
                temp16_6 <<= 1;
            }
        }
    }
}

//71874
void Heap_prng_seed_71874() {

    int i;
    int random_num;
    int temp32_0;
    int temp32_1;

    //Seems to be seeding 10a530 with random values???
    random_num = LibC_rand() & 0xfff;
    sHeapCtx.unk_10a530[0x36] = (u16)random_num;
    temp32_1 = 1;
    for (i = 0; i < 0x36; i++) {
        temp32_0 = i * 0x15;
        sHeapCtx.unk_10a530[temp32_0 % 0x37] = (u16)temp32_1;
        temp32_1 = random_num - temp32_1;
        if (temp32_1 < 0) temp32_1 += 0x1000;
        random_num = (int)sHeapCtx.unk_10a530[temp32_0 % 0x37];
    }
    sUnk_gp.unk_feadc = 0x36; //Sets feadc. Index into 10a530?

    //Seeds 10a468 using values from 10a530
    for (i = 0; i < 0x61; i++) {
        sHeapCtx.unk_10a468[i] = Heap_prng_next10a530_71964();
    }

    sUnk_gp.unk_fead4 = 0x60;
    for (i = 0; i < 0x61; i++) {
        func_80071FBC(0x1000);
    }
}

//Seems to retrieve a value from 10a530, incrementing index each time.
//When reaches end of array, it sort of scrambles all the values in the array.
//Might be a PRNG, but no idea why this would be necessary?
//71964
u16 Heap_prng_next10a530_71964() {

    int i, j;
    int temp32_0;

    if (++sUnk_gp.unk_feadc > 0x36) {
        for (i = 0; i < 24; i++) {
            temp32_0 = (int)sHeapCtx.unk_10a530[i] - (int)sHeapCtx.unk_10a530[i + 31]; //Or a56e[i]
            if (temp32_0 < 0) temp32_0 += 0x1000;
            sHeapCtx.unk_10a530[i] = (u16)temp32_0;
        }
        for (i = 0; i < 31; i++) {
            j = i + 24;
            temp32_0 = sHeapCtx.unk_10a530[j] - sHeapCtx.unk_10a530[i];
            if (temp32_0 < 0) temp32_0 += 0x1000;
            sHeapCtx.unk_10a530[j] = (u16)temp32_0;
        }
        //Does it reset feadc? Seems like it would segfault if not?
    }
    return sHeapCtx.unk_10a530[sUnk_gp.unk_feadc];
}

//71A14
int Heap_setfead8_feae0_71A14(u32 arg0, u16 arg1, u32 arg2) {
    //Returns pseudo bool
    //Do next
    u16 temp16_0;
    u16 temp16_1;
    u16* temp_ptr;
    int ret;

    arg0 &= 0xff;
    arg1 &= 0xff;
    arg2 &= 0xff;

    sUnk_gp.unk_fead4 = ((int)sHeapCtx.unk_10a468[sUnk_gp.unk_fead4] * 0x61) >> 12;
    temp16_0 = sHeapCtx.unk_10a468[sUnk_gp.unk_fead4];
    sHeapCtx.unk_10a468[sUnk_gp.unk_fead4] = Heap_prng_next10a530_71964();
    ret = (arg0 <= (((u32)temp16_0 * 100) >> 12));
    if (ret) temp_ptr = &sUnk_gp.unk_feae0;
    else temp_ptr = &sUnk_gp.unk_fead8;
    temp_ptr[arg2] += arg0;
    temp16_1 = sUnk_gp.unk_fead8[arg2] - sUnk_gp.unk_feae0[arg2];
    if (sUnk_gp.unk_fead8[arg2] <= sUnk_gp.unk_feae0[arg2]) {
        temp16_1 = sUnk_gp.unk_feae0[arg2] - sUnk_gp.unk_fead8[arg2];
    }

    if (arg1 <= temp16_1) {
        if (sUnk_gp.unk_feae0[arg2] < sUnk_gp.unk_fead8[arg2]) {
            ret = TRUE;
            sUnk_gp.unk_fead8[arg2] -= arg0;
            sUnk_gp.unk_feae0[arg2] += 100 - arg0;
        }
        else {
            ret = FALSE;
            sUnk_gp.unk_feae0[arg2] -= arg0;
            sUnk_gp.unk_fead8[arg2] += 100 - arg0;
        }
    }

    if (sUnk_gp.unk_fead8[arg2] <= sUnk_gp.unk_feae0[arg2]) {
        temp16_1 = sUnk_gp.unk_fead8[arg2];
    }

    if (((s32)temp16_1 << 16) < 0) {
        sUnk_gp.unk_fead8[arg2] += 0x8000;
        sUnk_gp.unk_feae0[arg2] += 0x8000;
    }
    return ret;
}

//71bd0
void Heap_ResetArena() {
    int i;

    sHeapCtx.freeBlockCount = 0;
    for (i = 0; i < 64; i++) {
        sHeapCtx.freeBlocks[i].address = 0;
        sHeapCtx.freeBlocks[i].len = 0;
    }
    memset(&sHeap.unk_111000, 0, 0xa7000);
    Heap_AddAvailableMemory(&sHeap.arena, 0xd6200);
    for (i = 0; i < 2; i++) {
        sUnk_gp.unk_fead8[i] = 0;
        sUnk_gp.unk_feae0[i] = 0;
    }
}

//71C5C
void Heap_FreeMemory(void* addr, uint32_t size) {
    //void* arg0
    if (addr != UNKPTR) {
        Heap_AddAvailableMemory(addr, size);
    }
}

//Find index of 10a260 record with smallest word1 (of used)
//71C84
u16 Heap_SmallestFreeBlockIndex() {
    int i;
    u32 ret = 0;

    for (i = 0; i < sHeapCtx.freeBlockCount; i++) {
        if (ret < sHeapCtx.freeBlocks[i].len) {
            ret = sHeapCtx.freeBlocks[i].len;
        }
    }
    return ret;
}

//Clear all (ff722) flags
//71CD0
void Heap_ClearAllFlags_ff722_71CD0() {
    int i;
    byte* ptr;

    //This clears everything in ff722. Update to make this clearer...
    ptr = &sUnk_ff520.unk_ff7e1;
    for (i = 0; i < 0xc0; i++) {
        *ptr-- = 0;
    }
}

//set flag (ff722)
//71cf8
void Heap_SetFlag_ff722_71cf8(u16 flagId) {
    if (flagId < 0x600) {
        sUnk_ff520.unk_ff722[flagId >> 3] |= sUnk_gp.byteBitMasks[flagId & 0x7];
    }
}

//clear flag (ff722)
//71d40
void Heap_ClearFlag_ff722_71d40(u16 flagId) {
    if (flagId < 0x600) {
        sUnk_ff520.unk_ff722[flagId >> 3] &= ~sUnk_gp.byteBitMasks[flagId & 0x7];
    }
}

//xor flag (ff722)
//71d88
void Heap_XorFlag_ff722_71d88(u16 flagId) {
    if (flagId < 0x600) {
        sUnk_ff520.unk_ff722[flagId >> 3] ^= sUnk_gp.byteBitMasks[flagId & 0x7];
    }
}

//is flag (ff722) set?
//71dd0
int Heap_CheckFlag_ff722_71dd0(u16 flagId) {
    if (flagId >= 0x600) return FALSE;
    if (!(sUnk_ff520.unk_ff722[flagId >> 3] & sUnk_gp.byteBitMasks[flagId & 0x7])) return FALSE;
    return TRUE;
}

//Turn on or off (count) flags specified by mask
//71e1c
void Heap_SetFlags_ff722_71e1c(u16 baseFlagId, u16 count, u16 mask) {
    int i;
    u16 temp16_0;
    u32 temp32_0;

    temp16_0 = 1;
    if ((baseFlagId + count < 0x600) && (count < 17)) {
        temp32_0 = (1 << (count & 0x1f)) - 1;
        if (temp32_0 < mask) mask = (u16)temp32_0;
        for (i = 0; i < count; i++) {
            if (!(mask & temp16_0)) {
                sUnk_ff520.unk_ff722[baseFlagId >> 3] &= ~sUnk_gp.byteBitMasks[baseFlagId & 0x7];
            }
            else {
                sUnk_ff520.unk_ff722[baseFlagId >> 3] |= sUnk_gp.byteBitMasks[baseFlagId & 0x7];
            }
            baseFlagId++;
            temp16_0 << 1;
        }
    }
}

//Get (count) flags from base
//71edc
u16 Heap_CheckFlags_ff722_71edc(u16 baseFlagId, u16 count) {
    int i;
    u16 mask = 0x1;
    u16 flags = 0;

    if ((baseFlagId + count < 0x600) && (count < 0x11)) {
        for (i = 0; i < count; i++) {
            if (sUnk_ff520.unk_ff722[baseFlagId >> 3] & sUnk_gp.byteBitMasks[baseFlagId & 0x7]) {
                flags |= mask;
            }
            baseFlagId++;
            mask <<= 1;
        }
    }
    return flags;
}

//71f64
void Heap_prng_71f64() {
    int32_t temp;

    temp = PsApi_Counter_GetRCnt(0);
    temp = (temp & 0x7f) | 0x80;
    while (--temp != -1) {
        func_80071FBC(0x1000);
    }
}

//71fbc
u16 Heap_prng_71fbc(int arg0) {
    u16 ret;

    if (arg0 < 2) return 0;
    ret = (u16)((int)sHeapCtx.unk_10a468[sUnk_gp.unk_fead4] * 0x61) >> 12;
    sUnk_gp.unk_fead4 = ret;
    ret = (u16)((int)sHeapCtx.unk_10a468[sUnk_gp.unk_fead4] * arg0) >> 12;
    sHeapCtx.unk_10a468[sUnk_gp.unk_fead4] = Heap_prng_next10a530_71964();
    return ret;
}
