#include "GameSave.h"

#define FREE_MC_MEM Heap_FreeMemory(sUnk_gp.loadInfoC.dataBuffer, 0x800); \
    Heap_FreeMemory(sUnk_gp.loadInfoB.dataBuffer, 0x2000); \
    Heap_FreeMemory(sUnk_gp.loadInfoA.dataBuffer, 0x2000)

//80072064
void MCSave_WriteMCFileHead(MemCardFileBase* mcFile, SpriteFileHeader* iconData, u8 iconFrameIndex, u32 unused, int saveFileIndex) {
    
    int i, j;
    SpriteFrameHeader* iconFrame;
    Clut4* iconClut;

    mcFile->header.marker = 0x4353;
    mcFile->header.iconFlag = 0x13;
    mcFile->header.blockNumber = 1;

    memset(&mcFile->header.title, 0, 0x40);
    memset(&mcFile->header.padding0, 0, 0x1c);
    if (saveFileIndex < 0) {
        memcpy(&mcFile->header.title, &roData_mcTitle_Sys, 0x20);
    }
    else {
        memcpy(&mcFile->header.title, &roData_mcTitle_Game, 0x20);
        if (saveFileIndex < 10) {
            mcFile->header.title[15] += saveFileIndex;
        }
        else {
            mcFile->header.title[14]++;
            mcFile->header.title[15] += saveFileIndex - 10;
        }
    }

    iconFrame = GET_FRAME_HEADER(iconData, 0);
    iconClut = GET_CLUT4_POINTER(iconData, iconFrame->clutIndex);
    GfxDraw_SetCLUTSemitrans(&iconClut, 16, 1);
    //This is the palette
    for (i = 0; i < 16; i++) {
        mcFile->header.iconClut[i] = iconClut->table[i];
    }

    //This copies the icon bitmaps to the save frames
    for (i = 0; i < 3; i++) {
        iconFrame = GET_FRAME_HEADER(iconData, iconFrameIndex++);
        memcpy(&mcFile->iconBitmaps[i], iconFrame, 0x80);
    }
}

//80072404
void MCSave_InitMCContext() {
    Mcrd_MemCardInit(1);
    sMemCardCtx.unk_10a5a4 = 0xffff;
    sMemCardCtx.unk_10a5a6 = 0;
    sMemCardCtx.unk_10a918 = NULL;
}

//80072438
void MCSave_StartMemCard() {
    Mcrd_MemCardStart();
    sMemCardCtx.mcChannel = 0;
    sMemCardCtx.unk_10a8ec = 0;
    func_8007248C();
}

//8007246C
void MCSave_StopMemCard() {
    Mcrd_MemCardStop();
}

void func_8007248C() {
    int i = 0;
    int j = 0;
    int k = 0;

    sMemCardCtx.unk_10a5a8 = 0;
    sMemCardCtx.unk_10a5ac = 0;
    for (i = 0; i < 15; i++) {
        sMemCardCtx.unk_10a5ad[i] = 0xff;
        sMemCardCtx.unk_10a5bc[i][j] = -1;
        for (j = 0; j < 3; j++) {
            memset(&sMemCardCtx.unk_10a7d8[i].unk_00[j], 0, 3);
        }
    }
}

//80072548
u32 MCSave_CalculateMCDataChecksum(u8* data, u16 len) {
    int i = 0;
    u32 sum = 0;
    u16* pos = (u16*)data;

    for (i = 0; i < len; i += 2) {
        sum += *pos++;
    }

    return sum;
}

//8007259C
void MCSave_UpdateMCDataChecksum(u8* data, u16 len) {
    int i = 0;
    u32 sum = 0;
    u16* pos = (u16*)data;
    u32* setptr = (u32*)data;

    memset(data, 0, 4);
    memset(data + 4, 0xff, 4);
    for (i = 0; i < len; i+=2) {
        sum += *pos++;
    }

    *setptr = sum;
    pos = (u16*)(data + 4);
    *pos++ = (s16)(-1 * ((s32)(sum & 0xffff) + 1));
    *pos = (s16)(-1 * ((s32)(sum >> 16) + 1));
}

//80072624
int MCSave_ChecksumMCData(u8* data, s16 len) {

    int i = 0;
    u16* pos = (u16*)data;
    u32* firstword = (u32*)data;
    u32 sum = 0;

    for (i = 0; i < len; i+=2) {
        sum += *pos++;
    }

    return (sum == *firstword);
}

void func_80076EF8() {
    int i;
    u32 callback_res;

    do {
        if (sDataLoadCtx.unk_ff50c == 0xff) {
            sDataLoadCtx.dataReqOffset = (u32)sGlobalData.unktbl_fabb4[*sDataLoadCtx.dataReqBase];
            sDataLoadCtx.reqStatus = 0;
            if (sDataLoadCtx.dataReqOffset != 0) {
                for (i = 0; i < sDataLoadCtx.dataReqOffset; i++) {
                    *(&sDataLoadCtx.unk_ff50c + i) = sDataLoadCtx.dataReqBase[i];
                }
            }
        }
        callback_res = sGlobalData.sceLoadDelegateTable[sDataLoadCtx.unk_ff50c]();
        if (callback_res & 0x1) {
            sDataLoadCtx.unk_ff50c = 0xff;
            sDataLoadCtx.dataReqBase += sDataLoadCtx.dataReqOffset;
        }

    } while (callback_res < 0x80);
}

void func_80077010(int arg0) {
    //This appears to be the only time sceDataBuffer is written to.
    if (arg0 < 2) sDataLoadCtx.sceDataBuffer = sHeap.fileLoadBuffer;
    sUnk_gp.unk_fea9d = 0;
    sUnk_gp.unk_fea9e = 0;
    sUnk_gp.unk_fea9f = 0;

    sDataLoadCtx.dataReqBase = (roUnk_122ac + arg0) + roUnk_122ac[arg0];
    sDataLoadCtx.unk_ff4fc = 0;
    sDataLoadCtx.unk_ff4fe = 0;
    sDataLoadCtx.dataReqOffset = 0;
    sDataLoadCtx.unk_ff506 = 0;
    sDataLoadCtx.unk_ff50c = 0xff;
    sDataLoadCtx.unk_ff51c = 0;
}

int func_8007708C(int arg0) {
    //Might involve an array of shorts.
    sDataLoadCtx.dataReqOffset += (arg0 << 1) + *(sDataLoadCtx.dataReqBase + sDataLoadCtx.dataReqOffset + (arg0 << 1));
    return 1;
}

int func_800770D4(int arg0, unktype_4 arg1) {
    sDataLoadCtx.unk_ff4b8[sDataLoadCtx.unk_ff4fc++] = arg1;
    sDataLoadCtx.dataReqOffset += *((s16*)(sDataLoadCtx.dataReqBase + sDataLoadCtx.dataReqOffset + (arg0 << 1))) + (arg0 << 1);
    return 1;
}

u8 func_8007713C(u32 arg0) {
    return sGlobalData.unktbl_fabb4[arg0 & 0xff];
}

int func_80077158() {

    u16 temp16;
    u16 temp16_2;

    if (sDataLoadCtx.unk_ff506 == 0) {
        sDataLoadCtx.unk_ff506 = (u16)sDataLoadCtx.unk_ff50d;
        sDataLoadCtx.unk_ff506 |= (u16)sDataLoadCtx.unk_ff50e << 8;
        if (sDataLoadCtx.unk_ff506 == 0) return 0x80;
    }

    temp16 = (u16)sDataLoadCtx.unk_ff50f;
    temp16 |= (u16)sDataLoadCtx.unk_ff510 << 8;
    temp16_2 = *((u16*)(sUnk_gp.unk_fea6c + 0x42));
    if (!(temp16_2 & temp16)) {
        sDataLoadCtx.unk_ff506--;
        if (sDataLoadCtx.unk_ff506 != 0) return 0x80;
        return 1;
    }
    sDataLoadCtx.unk_ff506 = 0;
    return 0x81;
}

int func_800771E4() {
    if (sDataLoadCtx.reqStatus == 0) {
        if (sDataLoadCtx.unk_ff4fc == 0x10) {
            func_80077010(4);
            return 0x80;
        }
        sSaveCtx.unk_ff838 = 0xff;
        //sSaveCtx.unk_ff828 = *((u16*)&sDataLoadCtx.unk_ff50d);
        READU16(&sDataLoadCtx.unk_ff50d, sSaveCtx.unk_ff828);
        CdFiles_requestCdFile(0x10, D_Sce, sSaveCtx.unk_ff828, 0, -1, sDataLoadCtx.sceDataBuffer);
        sSaveCtx.unk_ff830 = 0;
        sSaveCtx.unk_ff82c = 0;
        sDataLoadCtx.reqStatus++;
    }
    else {
        if (sDataLoadCtx.reqStatus != 1) return 0x80;
        if (CdFiles_AllSlotsEmpty()) {
            sDataLoadCtx.dataReqBase = sDataLoadCtx.sceDataBuffer;
            sDataLoadCtx.dataReqOffset = 0;
            return 1;
        }
    }
    return 0x80;
}

int func_800772F4() {
    u32 temp32;
    s16 temp16 = sDataLoadCtx.unk_ff4fc;

    if (sDataLoadCtx.unk_ff4fc == 0x10) {
        func_80077010(2);
    }
    else {
        sDataLoadCtx.unk_ff4fc++;
        if (sDataLoadCtx.unk_ff50d < 0) {
            //Or if highest bit is set...
            sDataLoadCtx.unk_ff4fc = 0;
        }
        else {
            sDataLoadCtx.unk_ff4b8[temp16] = sDataLoadCtx.unk_ff4b8[0] + sDataLoadCtx.dataReqOffset;
        }
        sDataLoadCtx.dataReqOffset = 0;
        temp32 = ((u32)sDataLoadCtx.unk_ff50d & 0x7f);
        sDataLoadCtx.dataReqBase = roUnk_122ac[temp32 + roUnk_122ac[temp32]];
    }
    return 1;
}

int func_800773A0() {
    if (sDataLoadCtx.unk_ff50d < 2) {
        if (*(&sSaveCtx.unk_ff82c + sDataLoadCtx.unk_ff50d) == 0) return 1;
        if (sDataLoadCtx.unk_ff4fc != 0x10) {
            sDataLoadCtx.unk_ff4b8[sDataLoadCtx.unk_ff4fc++] = sDataLoadCtx.dataReqBase + sDataLoadCtx.dataReqOffset;
            sDataLoadCtx.dataReqBase = *(&sSaveCtx.unk_ff82c + sDataLoadCtx.unk_ff50d);
            sDataLoadCtx.dataReqOffset = 0;
            sSaveCtx.unk_ff838 = sDataLoadCtx.unk_ff50d;
            return 1;
        }
    }
    else {
        if (sSaveCtx.unk_ff838 > 1) return 1;
        if (sDataLoadCtx.unk_ff4fc != 0) {
            if ((u32)sDataLoadCtx.unk_ff50d > 0xbf) {
                sSaveCtx.unk_ff838 = sDataLoadCtx.unk_ff50d & 0xf;
            }
            sDataLoadCtx.dataReqOffset = 0;
            sDataLoadCtx.unk_ff4fc--;
            *(&sSaveCtx.unk_ff82c + sSaveCtx.unk_ff838) = sDataLoadCtx.dataReqBase + sDataLoadCtx.unk_ff4b8[sDataLoadCtx.unk_ff4fc];
            sDataLoadCtx.dataReqBase = sDataLoadCtx.unk_ff4b8[sDataLoadCtx.unk_ff4fc];
            return 1;
        }
    }
    func_80077010(2);
    return 1;
}

int func_800774D4() {
    int i;
    s16* tempptr;

    if (sDataLoadCtx.unk_ff50d < 2) {
        tempptr = (s16*)(sDataLoadCtx.dataReqBase + sDataLoadCtx.dataReqOffset);
        sDataLoadCtx.dataReqOffset += 2;
        *(&sSaveCtx.unk_ff82c + sDataLoadCtx.unk_ff50d) = tempptr + *tempptr;
    }
    else {
        tempptr = (s16*)(sDataLoadCtx.dataReqBase + sDataLoadCtx.dataReqOffset);
        for (i = 0; i < 2; i++) {
            *(&sSaveCtx.unk_ff82c + i) = tempptr + *tempptr;
            tempptr++;
        }
        sDataLoadCtx.dataReqOffset += 4;
    }
    return 1;
}

int func_800775A4() {
    return 0x80;
}

int func_800775AC() {
    u16 temp;

    temp = (u16)sDataLoadCtx.unk_ff50d;
    temp |= (u16)sDataLoadCtx.unk_ff50e << 8;

    sDataLoadCtx.dataReqOffset = temp + 1;
    return 1;
}

int func_800775DC() {
    u32 temp;

    temp = (u32)sDataLoadCtx.unk_ff50d;
    temp |= (u32)sDataLoadCtx.unk_ff50e << 8;
    temp |= (u32)sDataLoadCtx.unk_ff50f << 16;
    temp |= (u32)sDataLoadCtx.unk_ff510 << 24;

    sDataLoadCtx.dataReqOffset = temp + 1;
    return 1;
}

int func_8007761C() {
    u16 temp;

    if (sDataLoadCtx.unk_ff4fc == 0x10) func_80077010(2);
    else {
        READU16(&sDataLoadCtx.unk_ff50d, temp);
        sDataLoadCtx.unk_ff4b8[sDataLoadCtx.unk_ff4fc++] = sDataLoadCtx.dataReqBase + sDataLoadCtx.dataReqOffset;
        sDataLoadCtx.dataReqOffset = temp + 1;
    }
    return 1;
}

undefined4 func_8007769C(void){
    uint uVar1;

    uVar1 = (uint)DAT_800ff4fc;
    if (uVar1 == 0x10) {
        func_80077010(2);
    }
    else {
        DAT_800ff4fc = DAT_800ff4fc + 1;
        (&DAT_800ff4b8)[uVar1] = DAT_800ff4f8 + DAT_800ff500;
        DAT_800ff500 = ((uint)DAT_800ff510 << 0x18 | (uint)DAT_800ff50f << 0x10 |
            (uint)DAT_800ff50e << 8 | (uint)DAT_800ff50d) + 1;
    }
    return 1;
}

int func_8007772C(){
    return 0x80;
}

undefined4 func_80077734(void)

{
    if (DAT_800ff4fc == 0) {
        func_80077010(2);
    }
    else {
        DAT_800ff500 = 0;
        DAT_800ff4f8 = (&DAT_800ff4b8)[(ushort)(DAT_800ff4fc - 1)];
        DAT_800ff4fc = DAT_800ff4fc - 1;
    }
    return 1;
}

undefined4 func_800778C0(void) {
    undefined4* puVar1;
    undefined* puVar2;
    undefined2* puVar3;
    uint uVar4;
    int iVar5;

    if (DAT_800ff50d == 1) {
        uVar4 = 0x80;
        do {
            Heap_ClearFlag_ff722_71d40(uVar4 & 0xffff);
            uVar4 = uVar4 + 1;
        } while ((int)uVar4 < 0x3ff);
        uVar4 = 0x40;
        do {
            Heap_ClearFlag_ff722_71d40(uVar4 & 0xffff);
            uVar4 = uVar4 + 1;
        } while ((int)uVar4 < 0x57);
        uVar4 = 0x58;
        do {
            Heap_ClearFlag_ff722_71d40(uVar4 & 0xffff);
            uVar4 = uVar4 + 1;
            puVar1 = &DAT_800ff520;
        } while ((int)uVar4 < 0x68);
        iVar5 = 0x7f;
        DAT_800ff840 = 0;
        DAT_800ff83a = 0;
        DAT_800ff844 = DAT_800feb60;
        do {
            *(undefined*)(puVar1 + 0x32) = 0;
            *(undefined*)(puVar1 + 0x52) = 0;
            iVar5 = iVar5 + -1;
            puVar1 = (undefined4*)((int)puVar1 + 1);
        } while (-1 < iVar5);
        iVar5 = 0x1f;
        puVar2 = &DAT_800ff707;
        do {
            *puVar2 = 0;
            iVar5 = iVar5 + -1;
            puVar2 = puVar2 + -1;
        } while (-1 < iVar5);
        puVar1 = &DAT_800ff520;
        iVar5 = 5;
        puVar3 = &DAT_800ff542;
        DAT_800ff708 = 0;
        DAT_800fead1 = 0x30;
        DAT_800ff528 = DAT_800ff528 & 0xfffffff8;
        do {
            *(undefined*)(puVar1 + 5) = 0;
            *(undefined*)((int)puVar1 + 0x15) = 0;
            *(undefined2*)((int)puVar1 + 0x1a) = 0;
            *(undefined*)((int)puVar1 + 0x16) = 0;
            *(undefined*)((int)puVar1 + 0x21) = 0;
            *(undefined2*)((int)puVar1 + 0x26) = 0;
            *(undefined2*)(puVar1 + 10) = 0;
            *(undefined2*)((int)puVar1 + 0x2a) = 0;
            *(undefined2*)(puVar1 + 0xb) = 0;
            puVar3[6] = 0;
            puVar3[7] = 0;
            *puVar3 = 0;
            *(undefined2*)(puVar1 + 9) = 0;
            puVar1 = (undefined4*)((int)puVar1 + 0x1e);
            iVar5 = iVar5 + -1;
            puVar3 = puVar3 + 0xf;
        } while (-1 < iVar5);
        iVar5 = 8;
        puVar2 = (undefined*)((int)&DAT_800ff530 + 3);
        do {
            *puVar2 = 0xff;
            iVar5 = iVar5 + -1;
            puVar2 = puVar2 + -1;
        } while (-1 < iVar5);
    }
    else {
        if (DAT_800ff50d < 2) {
            if (DAT_800ff50d != 0) {
                return 1;
            }
            Heap_ClearAllFlags_ff722_71CD0();
            DAT_800feb28 = DAT_800feb28 & 0xfffffdff;
            return 1;
        }
        uVar4 = 0x80;
        if (DAT_800ff50d != 2) {
            return 1;
        }
        do {
            Heap_ClearFlag_ff722_71d40(uVar4 & 0xffff);
            uVar4 = uVar4 + 1;
        } while ((int)uVar4 < 0x339);
        uVar4 = 0x40;
        do {
            Heap_ClearFlag_ff722_71d40(uVar4 & 0xffff);
            uVar4 = uVar4 + 1;
        } while ((int)uVar4 < 0x57);
        uVar4 = 0x58;
        do {
            Heap_ClearFlag_ff722_71d40(uVar4 & 0xffff);
            uVar4 = uVar4 + 1;
            puVar1 = &DAT_800ff520;
        } while ((int)uVar4 < 0x68);
        iVar5 = 0x7f;
        DAT_800ff840 = 0;
        DAT_800ff83a = 0;
        DAT_800ff844 = DAT_800feb60;
        do {
            *(undefined*)(puVar1 + 0x32) = 0;
            *(undefined*)(puVar1 + 0x52) = 0;
            iVar5 = iVar5 + -1;
            puVar1 = (undefined4*)((int)puVar1 + 1);
        } while (-1 < iVar5);
        iVar5 = 0x1f;
        puVar2 = &DAT_800ff707;
        do {
            *puVar2 = 0;
            iVar5 = iVar5 + -1;
            puVar2 = puVar2 + -1;
        } while (-1 < iVar5);
        puVar1 = &DAT_800ff520;
        iVar5 = 5;
        puVar3 = &DAT_800ff542;
        DAT_800ff708 = 0;
        DAT_800fead1 = 0x30;
        DAT_800ff528 = DAT_800ff528 & 0xfffffff8;
        do {
            *(undefined*)(puVar1 + 5) = 0;
            *(undefined*)((int)puVar1 + 0x15) = 0;
            *(undefined2*)((int)puVar1 + 0x1a) = 0;
            *(undefined*)((int)puVar1 + 0x16) = 0;
            *(undefined*)((int)puVar1 + 0x21) = 0;
            *(undefined2*)((int)puVar1 + 0x26) = 0;
            *(undefined2*)(puVar1 + 10) = 0;
            *(undefined2*)((int)puVar1 + 0x2a) = 0;
            *(undefined2*)(puVar1 + 0xb) = 0;
            puVar3[6] = 0;
            puVar3[7] = 0;
            *puVar3 = 0;
            *(undefined2*)(puVar1 + 9) = 0;
            puVar1 = (undefined4*)((int)puVar1 + 0x1e);
            iVar5 = iVar5 + -1;
            puVar3 = puVar3 + 0xf;
        } while (-1 < iVar5);
        iVar5 = 8;
        puVar2 = (undefined*)((int)&DAT_800ff530 + 3);
        do {
            *puVar2 = 0xff;
            iVar5 = iVar5 + -1;
            puVar2 = puVar2 + -1;
        } while (-1 < iVar5);
    }
    DAT_800feaac = 0xffffffff;
    DAT_800ff6e8 = 1;
    DAT_800ff6e9 = 8;
    DAT_800ff83c = 0;
    return 1;
}

undefined4 func_80077BFC(void) {
    bool bVar1;
    ushort uVar2;
    uint uVar3;
    uint uVar4;

    uVar2 = _DAT_800ff50f;
    uVar4 = (uint)_DAT_800ff512;
    uVar3 = Heap_CheckFlags_ff722_71edc(_DAT_800ff50f & 0x3fff, DAT_800ff511);
    if (uVar2 < 0x4000) {
        if (uVar3 != uVar4) {
            return 1;
        }
    }
    else {
        if ((int)((uint)uVar2 << 0x10) < 0) {
            bVar1 = uVar4 < uVar3;
            if (uVar2 < 0xc000) {
                bVar1 = uVar3 < uVar4;
            }
            if (!bVar1) {
                return 1;
            }
        }
        else {
            if (uVar3 == uVar4) {
                return 1;
            }
        }
    }
    DAT_800ff500 = _DAT_800ff50d + 1;
    return 1;
}

undefined4 func_80077CD0(void) {
    bool bVar1;
    int iVar2;
    uint uVar3;
    undefined4 uVar4;
    undefined auStack24[4];
    int local_14;

    if (DAT_800ff504 != 1) {
        if (1 < DAT_800ff504) {
            if (DAT_800ff504 != 2) {
                return 0x80;
            }
            iVar2 = Mcrd_MemCardSync(1, auStack24, &local_14);
            if (iVar2 == 0) {
                return 0x80;
            }
            if (iVar2 == 1) {
                if (local_14 == 1) {
                    uVar4 = func_8007708C(1);
                    return uVar4;
                }
                if (local_14 == 0) {
                    uVar4 = func_8007708C(0);
                    return uVar4;
                }
                if ((local_14 != 3) &&
                    (uVar3 = DAT_800fea68 + 1, bVar1 = 7 < DAT_800fea68, DAT_800fea68 = uVar3, bVar1)) {
                    if ((local_14 != 2) && (local_14 == 4)) {
                        uVar4 = func_8007708C(2);
                        return uVar4;
                    }
                    uVar4 = func_8007708C(3);
                    return uVar4;
                }
            }
            DAT_800ff504 = DAT_800ff504 - 1;
            return 0x80;
        }
        if (DAT_800ff504 != 0) {
            return 0x80;
        }
        DAT_800fea68 = 0;
        DAT_800ff504 = 1;
    }
    iVar2 = Mcrd_MemCardSync(1, auStack24, &local_14);
    if (iVar2 != 0) {
        Mcrd_MemCardAccept(DAT_8010a5a0);
        DAT_800ff504 = DAT_800ff504 + 1;
    }
    return 0x80;
}

undefined4 func_80077E60(void) {
    bool bVar1;
    int iVar2;
    uint uVar3;
    undefined4 uVar4;
    undefined auStack24[4];
    int local_14;

    if (DAT_800ff504 != 1) {
        if (1 < DAT_800ff504) {
            if (DAT_800ff504 == 2) {
                iVar2 = Mcrd_MemCardSync(1, auStack24, &local_14);
                if (iVar2 == 0) {
                    return 0x80;
                }
                if (local_14 != 1) {
                    if (local_14 == 0) {
                        uVar4 = func_8007708C(3);
                        return uVar4;
                    }
                    if ((local_14 != 3) &&
                        (uVar3 = DAT_800fea68 + 1, bVar1 = 7 < DAT_800fea68, DAT_800fea68 = uVar3, bVar1)) {
                        if ((local_14 != 2) && (local_14 == 4)) {
                            DAT_800fea68 = 0;
                            DAT_800ff504 = DAT_800ff504 + 1;
                            return 0x80;
                        }
                        uVar4 = func_8007708C(4);
                        return uVar4;
                    }
                    DAT_800ff504 = DAT_800ff504 - 1;
                    return 0x80;
                }
            }
            else {
                if (DAT_800ff504 != 3) {
                    return 0x80;
                }
                local_14 = func_800CA6B8(DAT_8010a5a0);
                if (local_14 != 1) {
                    if (local_14 == 0) {
                        func_8007248C();
                        uVar4 = func_8007708C(0);
                        return uVar4;
                    }
                    if (local_14 != -1) {
                        if (7 < DAT_800fea68) {
                            DAT_800fea68 = DAT_800fea68 + 1;
                            uVar4 = func_8007708C(2);
                            return uVar4;
                        }
                        DAT_800fea68 = DAT_800fea68 + 1;
                        return 0x80;
                    }
                    return 0x80;
                }
            }
            uVar4 = func_8007708C(1);
            return uVar4;
        }
        if (DAT_800ff504 != 0) {
            return 0x80;
        }
        DAT_800fea68 = 0;
        DAT_800ff504 = 1;
    }
    iVar2 = Mcrd_MemCardSync(1, auStack24, &local_14);
    if (iVar2 != 0) {
        Mcrd_MemCardAccept(DAT_8010a5a0);
        DAT_800ff504 = DAT_800ff504 + 1;
    }
    return 0x80;
}

int func_8007808C() {

    int i = 0;
    int j = 0;
    int mccmd = 0;
    int mcrst = 0;
    int sysFound = 0;

    struct DIRENTRY* entry = NULL;

    char temp8_0;
    int temp32_0;

    switch (sDataLoadCtx.reqStatus) {
    case 0:
        func_8007248C();
        sUnk_gp.loadInfoA.dataBuffer = Heap_ArenaAlloc(600);
        sUnk_gp.loadInfoC.dataBuffer = NULL;
        sUnk_gp.unk_fea68 = 0;
        sDataLoadCtx.reqStatus++;
        //fallthrough
    case 1:
        mcrst = Mcrd_MemCardGetDirentry(sMemCardCtx.mcChannel, &sMemCardCtx.mcFileName, sUnk_gp.loadInfoA.dataBuffer, &sUnk_gp.loadInfoB.intValue, 0, 15);
        if (mcrst == McErrCardNotExist) {
            Heap_FreeMemory(sUnk_gp.loadInfoA.dataBuffer, 600);
            return func_8007708C(1);
        }
        if (mcrst != McErrNone) {
            if (mcrst == -1) return 0x80;
            if (sUnk_gp.unk_fea68++ > 7) {
                return func_8007708C(2);
            }
            return 0x80;
        }
        sUnk_gp.unk_fea68 = 0;
        sDataLoadCtx.reqStatus++;
        return 0x80;
    case 2:
        sysFound = 0;
        sMemCardCtx.unk_10a8e8 = 15;
        if (sUnk_gp.loadInfoB.intValue != 0) {
            entry = (struct DIRENTRY*)sUnk_gp.loadInfoA.dataBuffer;
            for (i = 0; i < sUnk_gp.loadInfoB.intValue; i++) {
                temp32_0 = entry->size + 0x1fff;
                if(temp32_0 < 0) temp32_0 = entry->size + 0x3ffe;
                sMemCardCtx.unk_10a8e8 -= (temp32_0 >> 13);
                LibC_strcpy(&sMemCardCtx.mcFileName, &entry->name);
                temp8_0 = sMemCardCtx.mcFileName[18];
                sMemCardCtx.mcFileName[18] = 'A';
                temp32_0 = LibC_strcmp(&sMemCardCtx.mcFileName, &roData_mcSaveFileName);
                if ((temp32_0 == 0) && (sMemCardCtx.mcFileName[18] < 15)) {
                    sMemCardCtx.unk_10a5ad[sMemCardCtx.unk_10a5a8] = sMemCardCtx.mcFileName[18];
                    sMemCardCtx.unk_10a5a8++;
                }
                else {
                    if (temp8_0 == 'S') { sysFound++; }
                }
                entry++;
            }
        }
        Heap_FreeMemory(sUnk_gp.loadInfoA.dataBuffer, 600);
        if (sMemCardCtx.unk_10a5a8 == 0) {
            if (sysFound == 0) {
                if (sMemCardCtx.unk_10a8e8-- < 2) {
                    return func_8007708C(5);
                }
            }
            else {
                if (sMemCardCtx.unk_10a8e8 == 0) {
                    return func_8007708C(8);
                }
            }
            return func_8007708C(4);
        }
        if (sMemCardCtx.unk_10a5a8 != 1) {
            for (i = 0; i < (sMemCardCtx.unk_10a5a8 - 1); i++) {
                if (sMemCardCtx.unk_10a5ad[i + 1] < sMemCardCtx.unk_10a5ad[i]) {
                    temp8_0 = sMemCardCtx.unk_10a5ad[i];
                    sMemCardCtx.unk_10a5ad[i] = sMemCardCtx.unk_10a5ad[i + 1];
                    sMemCardCtx.unk_10a5ad[i + 1] = temp8_0;
                    j = i;
                    while (sMemCardCtx.unk_10a5ad[j] < sMemCardCtx.unk_10a5ad[j-1]) {
                        temp8_0 = sMemCardCtx.unk_10a5ad[j];
                        sMemCardCtx.unk_10a5ad[j] = sMemCardCtx.unk_10a5ad[j - 1];
                        sMemCardCtx.unk_10a5ad[j - 1] = temp8_0;
                        j--;
                    }
                }
            }
        }

        if ((sysFound == 0) && (sMemCardCtx.unk_10a8e8-- == 0)) {
            sUnk_gp.loadInfoC.intValue = 9;
        }
        sUnk_gp.loadInfoA.dataBuffer = Heap_ArenaAlloc(0x80);
        sMemCardCtx.unk_10a5ac = 0;
        sDataLoadCtx.reqStatus++;
        break;
    case 3:
        if (Mcrd_MemCardSync(McSyncMode_NoWait, &mccmd, &mcrst) != McSyncStatBusy) {
            memcpy(&sMemCardCtx.mcFileName, "BISLPM-87178MYUMYUA", 20);
            sMemCardCtx.mcFileName[18] += sMemCardCtx.unk_10a8f4;
            Mcrd_MemCardReadFile(sMemCardCtx.mcChannel, &sMemCardCtx.mcFileName, sUnk_gp.loadInfoA.dataBuffer, 0x200, 0x80);
            sDataLoadCtx.reqStatus++;
        }
        break;
    case 4:
        if (Mcrd_MemCardSync(McSyncMode_NoWait, &mccmd, &mcrst) == McSyncStatBusy) return 0x80;
        if (mcrst == McErrCardNotExist) {
            return func_8007708C(1);
        }
        if (mcrst != McErrNone) {
            if (mcrst == McErrNewCard) {
                if (sMemCardCtx.mcChannel == 0) sMemCardCtx.unk_10a5a6 &= 0xfffe;
                else sMemCardCtx.unk_10a5a6 &= 0xfffd;
                return func_8007708C(6);
            }
            if (sUnk_gp.unk_fea68++ < 8) {
                sDataLoadCtx.reqStatus--;
                return 0x80;
            }
            Heap_FreeMemory(sUnk_gp.loadInfoA.dataBuffer, 0x80);
            return func_8007708C(7);
        }
        sUnk_gp.unk_fea68 = 0;
        sDataLoadCtx.reqStatus++;
        return 0x80;
    case 5:
        memcpy(&sMemCardCtx.unk_10a5bc[sMemCardCtx.unk_10a5ac], sUnk_gp.loadInfoA.dataBuffer, 12);
        memcpy(&sMemCardCtx.unk_10a670[sMemCardCtx.unk_10a5ac], sUnk_gp.loadInfoA.dataBuffer + 12, 12);
        memcpy(&sMemCardCtx.unk_10a724[sMemCardCtx.unk_10a5ac], sUnk_gp.loadInfoA.dataBuffer + 24, 12);
        memcpy(&sMemCardCtx.unk_10a7d8[sMemCardCtx.unk_10a5ac], sUnk_gp.loadInfoA.dataBuffer + 36, 18);
        for (i = 0; i < 3; i++) {
            if ((sMemCardCtx.unk_10a5bc[sMemCardCtx.unk_10a5ac][i] | sMemCardCtx.unk_10a5bc[sMemCardCtx.unk_10a670][i]) != ~0) {
                sMemCardCtx.unk_10a5bc[sMemCardCtx.unk_10a5ac][i] = 0xfefefefe;
                sMemCardCtx.unk_10a670[sMemCardCtx.unk_10a5ac][i] = 0x01010101;
            }
        }
        if (++sMemCardCtx.unk_10a5ac == sMemCardCtx.unk_10a5a8) {
            Heap_FreeMemory(sUnk_gp.loadInfoA.dataBuffer, 0x80);
            return func_8007708C(sUnk_gp.loadInfoC.intValue);
        }
        sDataLoadCtx.reqStatus-=2;
        break;
    }
    return 0x80;
}

int func_800788AC() {
    int i = 0;
    int j = 0;

    int mccmd = 0;
    int mcrst = 0;
    int mcSlot = 1;

    u8* tempptr0 = NULL;

    u8 temp8_0;

    if (sMemCardCtx.mcChannel != 0) { mcSlot = 2; }
    switch (sDataLoadCtx.reqStatus) {
    case 0:
        sUnk_gp.loadInfoA.dataBuffer = Heap_ArenaAlloc(0x2000);
        sUnk_gp.loadInfoB.dataBuffer = Heap_ArenaAlloc(0x2000);
        sUnk_gp.loadInfoC.dataBuffer = Heap_ArenaAlloc(0x800);
        sUnk_gp.unk_fea68 = 0;
        sDataLoadCtx.reqStatus++;
        break;
    case 1:
        sMemCardCtx.unk_10a8f0 = sMemCardCtx.unk_10a5bc[sMemCardCtx.unk_10a5ac >> 3][sMemCardCtx.unk_10a5ac & 0x7];
        sSaveCtx.unk_ff824 = sDataLoadCtx.unk_ff4fc;
        sMemCardCtx.unk_10a8f4 = sMemCardCtx.unk_10a5ad[sMemCardCtx.unk_10a5ac >> 3];
        if (sDataLoadCtx.unk_ff4fc != 0) {
            for (i = 0; i < sSaveCtx.unk_ff824; i++) {
                *(&sSaveCtx.unk_ff7e4 + i) = sDataLoadCtx.unk_ff4b8[i]; //ff7e4 may be an array then?
            }
        }
        if (sMemCardCtx.unk_10a5ad[sMemCardCtx.unk_10a5ac >> 3] == 0xff) {
            sDataLoadCtx.reqStatus = 10;
        }
        else {
            sDataLoadCtx.reqStatus = 20;
        }
        break;
    case 10:
        if (!CdFiles_AllSlotsEmpty()) return 0x80;
        CdFiles_requestCdFile(FILE_MODE_FLAG_INDEX_OFFSET | FILE_MODE_FLAG_LZ77, D_System, Sys_MCIcon_Bell, 0, -1, sUnk_gp.loadInfoC.dataBuffer);
        sDataLoadCtx.reqStatus++;
        //No break
    case 11:
        if (CdFiles_AllSlotsEmpty()) {
            sMemCardCtx.unk_10a8f4 = 0;
            while (i < 15) {
                if (sMemCardCtx.unk_10a5ad[i++] == sMemCardCtx.unk_10a8f4) {
                    sMemCardCtx.unk_10a8f4++;
                    i = 0;
                }
            }
            LibC_memset(sUnk_gp.loadInfoA.dataBuffer, 0, 0x2000);
            MCSave_WriteMCFileHead(sUnk_gp.loadInfoA.dataBuffer, sUnk_gp.loadInfoC.dataBuffer, 0, sMemCardCtx.unk_10a5ac, sMemCardCtx.unk_10a8f4);
            memcpy(&sMemCardCtx.mcFileName, "BISLPM-87178MYUMYUA", 20);
            sMemCardCtx.mcFileName[18] += sMemCardCtx.unk_10a8f4;
            for (i = 0; i < 3; i++) {
                *((u32*)(sUnk_gp.loadInfoA.dataBuffer + 0x200 + (i << 2))) = sMemCardCtx.unk_10a5bc[sMemCardCtx.unk_10a5ac >> 3][i];
                *((u32*)(sUnk_gp.loadInfoA.dataBuffer + 0x20c + (i << 2))) = ~sMemCardCtx.unk_10a5bc[sMemCardCtx.unk_10a5ac >> 3][i];
                *((u32*)(sUnk_gp.loadInfoA.dataBuffer + 0x218 + (i << 2))) = 0;
                for (j = 0; j < 6; j++) {
                    *((u32*)(sUnk_gp.loadInfoA.dataBuffer + 0x224 + (i * 6) + j)) = 0xff;
                }
            }
            sDataLoadCtx.reqStatus++;
            if (Mcrd_MemCardSync(McSyncMode_NoWait, &mccmd, &mcrst) != McSyncStatBusy) {
                Mcrd_MemCardAccept(sMemCardCtx.mcChannel);
                sDataLoadCtx.reqStatus++;
            }
        }
        break;
    case 12:
        if (Mcrd_MemCardSync(McSyncMode_NoWait, &mccmd, &mcrst) != McSyncStatBusy) {
            Mcrd_MemCardAccept(sMemCardCtx.mcChannel);
            sDataLoadCtx.reqStatus++;
        }
        break;
    case 13:
        if (Mcrd_MemCardSync(McSyncMode_NoWait, &mccmd, &mcrst) == McSyncStatBusy) return 0x80;
        if (mcrst == McErrCardNotExist) {
            FREE_MC_MEM;
            return func_8007708C(1);
        }
        if (mcrst == McErrNone) {
            sUnk_gp.unk_fea68 = 0;
            sDataLoadCtx.reqStatus++;
            return 0x80;
        }
        if ((mcrst != McErrNewCard) && (sUnk_gp.unk_fea68++ < 8)) {
            sDataLoadCtx.reqStatus--;
            return 0x80;
        }
        sMemCardCtx.unk_10a5a6 &= ~mcSlot;
        FREE_MC_MEM;
        return func_8007708C(3);
    case 14:
        mcrst = Mcrd_MemCardCreateFile(sMemCardCtx.mcChannel, &sMemCardCtx.mcFileName, 1);
        if (mcrst == McErrCardNotExist) {
            FREE_MC_MEM;
            return func_8007708C(1);
        }
        if (mcrst == McErrNone) {
            sUnk_gp.unk_fea68 = 0;
            sDataLoadCtx.reqStatus += 0x10;
            sMemCardCtx.unk_10a5a8++;
            sMemCardCtx.unk_10a8e8--;
            return 0x80;
        }
        if (mcrst == -1) return 0x80;
        if (sUnk_gp.unk_fea68++ < 8) return 0x80;
        if ((mcrst != 4) && (mcrst < 5 || mcrst > 7)) {
            FREE_MC_MEM;
            return func_8007708C(2);
        }
        sMemCardCtx.unk_10a5a6 &= ~mcSlot;
        FREE_MC_MEM;
        return func_8007708C(3);
    case 20:
        if (Mcrd_MemCardSync(McSyncMode_NoWait, &mccmd, &mcrst) != McSyncStatBusy) {
            memcpy(&sMemCardCtx.mcFileName, "BISLPM-87178MYUMYUA", 20);
            sMemCardCtx.mcFileName[18] += sMemCardCtx.unk_10a8f4;
            Mcrd_MemCardReadFile(sMemCardCtx.mcChannel, &sMemCardCtx.mcFileName, sUnk_gp.loadInfoA.dataBuffer, 0, 0x2080);
            sDataLoadCtx.reqStatus++;
        }
        break;
    case 21:
        if (Mcrd_MemCardSync(McSyncMode_NoWait, &mccmd, &mcrst) == McSyncStatBusy) return 0x80;
        if (mcrst == McErrCardNotExist) {
            FREE_MC_MEM;
            return func_8007708C(1);
        }
        if (mcrst == McErrNone) {
            sUnk_gp.unk_fea68 = 0;
            sDataLoadCtx.reqStatus++;
            return 0x80;
        }
        if (mcrst != McErrNewCard) {
            if (sUnk_gp.unk_fea68++ < 8) {
                sDataLoadCtx.reqStatus--;
                return 0x80;
            }
            if ((mcrst == McErrCardInvalid) || (mcrst != McErrFileNotExist)) {
                FREE_MC_MEM;
                return func_8007708C(2);
            }
        }
        sMemCardCtx.unk_10a5a6 &= ~mcSlot;
        FREE_MC_MEM;
        return func_8007708C(3);
    case 22:
        if (!CdFiles_AllSlotsEmpty()) return 0x80;
        CdFiles_requestCdFile(FILE_MODE_FLAG_INDEX_OFFSET | FILE_MODE_FLAG_LZ77, D_System, Sys_MCIcon_Bell, 0, -1, sUnk_gp.loadInfoC.dataBuffer);
        sDataLoadCtx.reqStatus++;
        //No break
    case 23:
        if (!CdFiles_AllSlotsEmpty()) break;
        sDataLoadCtx.reqStatus = 30;
        //Fallthrough to 30
    case 30:
        sSaveCtx.unk_ff840 += sApplState.unk_feb60 - sSaveCtx.unk_ff844;
        sSaveCtx.unk_ff844 = sApplState.unk_feb60;
        if (sSaveCtx.unk_ff840 > 0x1464b40) {
            sSaveCtx.unk_ff840 = 0x1464b40;
        }
        sSaveCtx.unk_ff826 = 0x102;
        memcpy(sUnk_gp.loadInfoA.dataBuffer + 0x238 + ((sMemCardCtx.unk_10a5ac & 0x7) * 0x328), &sSaveCtx, 0x322);
        MCSave_UpdateMCDataChecksum(sUnk_gp.loadInfoA.dataBuffer + 0x238 + ((sMemCardCtx.unk_10a5ac & 0x7) * 0x328), 0x328);
        sMemCardCtx.unk_10a8f0 = (sSaveCtx.unk_ff708 << 8) | (sSaveCtx.unk_ff83a << 16) | 0xfc | (Heap_CheckFlags_ff722_71edc(0x68, 8) << 24);

        *((u32*)(sUnk_gp.loadInfoA.dataBuffer + 0x200 + ((sMemCardCtx.unk_10a5ac & 0x7) << 2))) = sMemCardCtx.unk_10a8f0;
        *((u32*)(sUnk_gp.loadInfoA.dataBuffer + 0x20c + ((sMemCardCtx.unk_10a5ac & 0x7) << 2))) = ~sMemCardCtx.unk_10a8f0;
        *((u32*)(sUnk_gp.loadInfoA.dataBuffer + 0x218 + ((sMemCardCtx.unk_10a5ac & 0x7) << 2))) = sSaveCtx.unk_ff840;

        j = 0;
        if (sSaveCtx.unk_ff528 & 0x7 != 0) {
            tempptr0 = (u8*)&sSaveCtx.unk_ff528 + 3;
            for (i = 0; i < 9; i++) {
                temp8_0 = *tempptr0++;
                if (temp8_0 != 0xff) {
                    if (i > 2) temp8_0 += 0x10;
                    *(sUnk_gp.loadInfoA.dataBuffer + 0x224 + (j++) + ((sMemCardCtx.unk_10a5ac & 0x7) * 6)) = temp8_0;
                }
                if (j > (sSaveCtx.unk_ff528 & 0x7)) break;
            }
        }

        while (j < 6) {
            *(sUnk_gp.loadInfoA.dataBuffer + 0x224 + (j++) + ((sMemCardCtx.unk_10a5ac & 0x7) * 6)) = 0xff;
        }
        sDataLoadCtx.reqStatus++;
        //fallthrough
    case 31:
        if (Mcrd_MemCardSync(McSyncMode_NoWait, &mccmd, &mcrst) != McSyncStatBusy) {
            LibC_memset(sUnk_gp.loadInfoB.dataBuffer, 0, 0x2000);
            MCSave_WriteMCFileHead(sUnk_gp.loadInfoB.dataBuffer, sUnk_gp.loadInfoC.dataBuffer, 0, sMemCardCtx.unk_10a5ac, sMemCardCtx.unk_10a8f4);
            memcpy(&sMemCardCtx.mcFileName, "BISLPM-87178MYUMYUA", 20);
            sMemCardCtx.mcFileName[18] += sMemCardCtx.unk_10a8f4;
            for (i = 0; i < 3; i++) {
                *((u32*)(sUnk_gp.loadInfoB.dataBuffer + 0x200 + (i << 2))) = 0xff;
                *((u32*)(sUnk_gp.loadInfoB.dataBuffer + 0x20c + (i << 2))) = 0xffffff00;
                *((u32*)(sUnk_gp.loadInfoB.dataBuffer + 0x218 + (i << 2))) = 0;
                for (j = 0; j < 6; j++) {
                    *(sUnk_gp.loadInfoB.dataBuffer + 0x224 + (i * 6) + j) = 0xff;
                }
            }
            Mcrd_MemCardWriteFile(sMemCardCtx.mcChannel, &sMemCardCtx.mcFileName, sUnk_gp.loadInfoB.dataBuffer, 0, 0x2000);
        }
        break;
    case 32:
    case 34:
    case 36:
        //Appears to be identical to case 21
        if (Mcrd_MemCardSync(McSyncMode_NoWait, &mccmd, &mcrst) == McSyncStatBusy) return 0x80;
        if (mcrst == McErrCardNotExist) {
            FREE_MC_MEM;
            return func_8007708C(1);
        }
        if (mcrst == McErrNone) {
            sUnk_gp.unk_fea68 = 0;
            sDataLoadCtx.reqStatus++;
            return 0x80;
        }
        if (mcrst != McErrNewCard) {
            if (sUnk_gp.unk_fea68++ < 8) {
                sDataLoadCtx.reqStatus--;
                return 0x80;
            }
            if ((mcrst == McErrCardInvalid) || (mcrst != McErrFileNotExist)) {
                FREE_MC_MEM;
                return func_8007708C(2);
            }
        }
        sMemCardCtx.unk_10a5a6 &= ~mcSlot;
        FREE_MC_MEM;
        return func_8007708C(3);
    case 33:
        if (Mcrd_MemCardSync(McSyncMode_NoWait, &mccmd, &mcrst) != McSyncStatBusy) {
            MCSave_WriteMCFileHead(sUnk_gp.loadInfoA.dataBuffer, sUnk_gp.loadInfoC.dataBuffer, 0, sMemCardCtx.unk_10a5ac, sMemCardCtx.unk_10a8f4);
            memcpy(&sMemCardCtx.mcFileName, "BISLPM-87178MYUMYUA", 20);
            sMemCardCtx.mcFileName[18] += sMemCardCtx.unk_10a8f4;
            Mcrd_MemCardWriteFile(sMemCardCtx.mcChannel, &sMemCardCtx.mcFileName, sUnk_gp.loadInfoA.dataBuffer, 0, 0x2080);
            sDataLoadCtx.reqStatus++;
        }
        //no break
    case 35:
        if (Mcrd_MemCardSync(McSyncMode_NoWait, &mccmd, &mcrst) != McSyncStatBusy) {
            memcpy(&sMemCardCtx.mcFileName, "BISLPM-87178MYUMYUA", 20);
            sMemCardCtx.mcFileName[18] += sMemCardCtx.unk_10a8f4;
            Mcrd_MemCardReadFile(sMemCardCtx.mcChannel, &sMemCardCtx.mcFileName, sUnk_gp.loadInfoB.dataBuffer, 0, 0x2080);
            sDataLoadCtx.reqStatus++;
        }
        break;
    case 37:
        if (LibC_strncmp2(sUnk_gp.loadInfoA.dataBuffer, sUnk_gp.loadInfoB.dataBuffer, 0x2080) == 0) {
            sMemCardCtx.unk_10a5ad[sMemCardCtx.unk_10a5ac >> 3] = sMemCardCtx.unk_10a8f4;

            sMemCardCtx.unk_10a5bc[sMemCardCtx.unk_10a5ac >> 3][sMemCardCtx.unk_10a5ac & 0x7] = *((u32*)(sUnk_gp.loadInfoB.dataBuffer + 0x200 + ((sMemCardCtx.unk_10a5ac & 0x7) << 2)));
            sMemCardCtx.unk_10a670[sMemCardCtx.unk_10a5ac >> 3][sMemCardCtx.unk_10a5ac & 0x7] = ~*((u32*)(sUnk_gp.loadInfoB.dataBuffer + 0x200 + ((sMemCardCtx.unk_10a5ac & 0x7) << 2)));
            sMemCardCtx.unk_10a724[sMemCardCtx.unk_10a5ac >> 3][sMemCardCtx.unk_10a5ac & 0x7] = *((u32*)(sUnk_gp.loadInfoB.dataBuffer + 0x218 + ((sMemCardCtx.unk_10a5ac & 0x7) << 2)));

            for (i = 0; i < 6; i++) {
                sMemCardCtx.unk_10a7d8[sMemCardCtx.unk_10a5ac >> 3].unk_00[sMemCardCtx.unk_10a5ac & 0x7].unk_00[i] = *(sUnk_gp.loadInfoB.dataBuffer + 0x224 + ((sMemCardCtx.unk_10a5ac & 0x7) * 6));
            }

            FREE_MC_MEM;
            return func_8007708C(0);
        }
        FREE_MC_MEM;
        return func_8007708C(4);
    }
    return 0x80;
}

int func_800797F0() {

    int mcSlot = 1;
    int mccmd = 0;
    int mcrst = 0;

    int temp32_0;
    int temp32_1;

    if (sMemCardCtx.mcChannel != 0) { mcSlot = 2; }
    if (sDataLoadCtx.reqStatus == 1) {
        if (Mcrd_MemCardSync(McSyncMode_NoWait, &mccmd, &mcrst) == McSyncStatBusy) return 0x80;
        memcpy(&sMemCardCtx.mcFileName, "BISLPM-87178MYUMYUA", 20);
        sMemCardCtx.mcFileName[18] += sMemCardCtx.unk_10a5ad[sMemCardCtx.unk_10a5ac >> 3];
        Mcrd_MemCardReadFile(sMemCardCtx.mcChannel, &sMemCardCtx.mcFileName, sUnk_gp.loadInfoA.dataBuffer, 0, 0x2000);
    }
    else {
        if (sDataLoadCtx.reqStatus > 1) {
            if (sDataLoadCtx.reqStatus != 2) {
                if (sDataLoadCtx.reqStatus != 3) return 0x80;
                temp32_0 = MCSave_ChecksumMCData((sUnk_gp.loadInfoA.dataBuffer + (sMemCardCtx.unk_10a5ac & 7) * 0x328) + 0x238, 0x328);
                temp32_1 = (((u32)sMemCardCtx.unk_10a5ac >> 3) * 3) + (sMemCardCtx.unk_10a5ac & 7);
                if (!temp32_0 || (sMemCardCtx.unk_10a5bc[sMemCardCtx.unk_10a5ac >> 3][sMemCardCtx.unk_10a5ac & 7]) != ~sMemCardCtx.unk_10a670[sMemCardCtx.unk_10a5ac >> 3][sMemCardCtx.unk_10a5ac & 7]) {
                    sMemCardCtx.unk_10a5bc[sMemCardCtx.unk_10a5ac >> 3][sMemCardCtx.unk_10a5ac & 7] = 0xfefefefe;
                    sMemCardCtx.unk_10a670[sMemCardCtx.unk_10a5ac >> 3][sMemCardCtx.unk_10a5ac & 7] = 0x01010101;
                }
                else {
                    memcpy(&sSaveCtx, sUnk_gp.loadInfoA.dataBuffer + ((sMemCardCtx.unk_10a5ac & 7) * 0x328) + 0x238, 0x302);
                    if (sSaveCtx.unk_ff826 == 0x102) {
                        Heap_FreeMemory(sUnk_gp.loadInfoA.dataBuffer, 0x2000);
                        return func_8007708C(0);
                    }
                }
                Heap_FreeMemory(sUnk_gp.loadInfoA.dataBuffer, 0x2000);
                return func_8007708C(4);
            }
            if (Mcrd_MemCardSync(McSyncMode_NoWait, &mccmd, &mcrst) == McSyncStatBusy) return 0x80;
            if (mcrst != 1) {
                if (mcrst == 0) {
                    sUnk_gp.unk_fea68 = 0;
                    sDataLoadCtx.reqStatus++;
                    return 0x80;
                }
                if (mcrst != 3) {
                    if (sUnk_gp.unk_fea68++ < 8) {
                        sDataLoadCtx.reqStatus--;
                        return 0x80;
                    }
                    if ((mcrst == 2) || (mcrst != 5)) {
                        Heap_FreeMemory(sUnk_gp.loadInfoA.dataBuffer, 0x2000);
                        return func_8007708C(2);
                    }
                }
                sMemCardCtx.unk_10a5a6 &= ~mcSlot;
                Heap_FreeMemory(sUnk_gp.loadInfoA.dataBuffer, 0x2000);
                return func_8007708C(3);
            }
            Heap_FreeMemory(sUnk_gp.loadInfoA.dataBuffer, 0x2000);
            return func_8007708C(1);
        }
        if (sDataLoadCtx.reqStatus != 0) return 0x80;
        sUnk_gp.loadInfoA.dataBuffer = Heap_ArenaAlloc(0x2000);
        sUnk_gp.unk_fea68 = 0;
    }
    sDataLoadCtx.reqStatus++;
    return 0x80;
}

int func_80079C78() {

    int i = 0;
    int mccmd = 0;
    int mcrst = 0;
    char fileId;
    int sysFilesFound;
    struct DIRENTRY* direntry;

    int temp32_0;

    switch (sDataLoadCtx.reqStatus) {
    case 0:
        sUnk_gp.loadInfoA.dataBuffer = Heap_ArenaAlloc(600);
        sUnk_gp.loadInfoC.intValue = 2; //Don't know why sometimes these are integer numbers
        sUnk_gp.unk_fea68 = 0;
        sMemCardCtx.mcChannel = 0;
        sDataLoadCtx.reqStatus++;
        return 0x80;
    case 1:
        if (Mcrd_MemCardSync(McSyncMode_NoWait, &mccmd, &mcrst) == McSyncStatBusy) return 0x80;
        Mcrd_MemCardAccept(sMemCardCtx.mcChannel);
        sDataLoadCtx.reqStatus++;
        return 0x80;
    case 2:
        if (Mcrd_MemCardSync(McSyncMode_NoWait, &mccmd, &mcrst) == McSyncStatBusy) return 0x80;
        if (mcrst == McErrCardNotExist) {
            sUnk_gp.unk_fea68 = 0;
            sDataLoadCtx.reqStatus += 4;
            return 0x80;
        }
        if (mcrst == McErrNone) {
            sUnk_gp.unk_fea68 = 0;
            sDataLoadCtx.reqStatus++;
            return 0x80;
        }
        if ((mcrst == McErrNewCard) || (sUnk_gp.unk_fea68 ++ < 8)) {
            sDataLoadCtx.reqStatus--;
            return 0x80;
        }
        if ((mcrst == McErrCardInvalid) || (mcrst != McErrNotFormat)) {
            sUnk_gp.unk_fea68 = 0;
            sDataLoadCtx.reqStatus += 4;
            return 0x80;
        }
        break;
    case 3:
        func_8007248C();
        sDataLoadCtx.reqStatus++;
        //No break
    case 4:
        mcrst = Mcrd_MemCardGetDirentry(sMemCardCtx.mcChannel, &sUnk_gp.unk_fe954, sUnk_gp.loadInfoA.dataBuffer, &sUnk_gp.loadInfoB.intValue, 0, 15);
        if (mcrst != McErrCardNotExist) {
            if (mcrst != McErrNone) {
                sUnk_gp.unk_fea68 = 0;
                sDataLoadCtx.reqStatus++;
                return 0x80;
            }
            if (mcrst != -1) return 0x80;
            if (sUnk_gp.unk_fea68++ < 8) {
                return 0x80;
            }
        }
        sUnk_gp.unk_fea68 = 0;
        sDataLoadCtx.reqStatus += 2;
        return 0x80;
    case 5:
        sMemCardCtx.unk_10a8e8 = 15;
        sysFilesFound = 0;
        if (sUnk_gp.loadInfoB.intValue != 0) {
            direntry = (struct DIRENTRY*)sUnk_gp.loadInfoA.dataBuffer;
            for (i = 0; i < sUnk_gp.loadInfoB.intValue; i++) {
                temp32_0 = direntry->size + 0x1fff;
                if (temp32_0 < 0) {
                    temp32_0 = direntry->size + 0x3ffe;
                }
                sMemCardCtx.unk_10a8e8 -= (temp32_0 >> 13);
                LibC_strcpy(&sMemCardCtx.mcFileName, &direntry->name);
                fileId = sMemCardCtx.mcFileName[18];
                sMemCardCtx.mcFileName[18] = 'A';
                temp32_0 = LibC_strcmp(&sMemCardCtx.mcFileName, &roData_mcSaveFileName);
                if ((temp32_0 == 0) && (fileId - 'A' < 15)) {
                    sMemCardCtx.unk_10a5ad[sMemCardCtx.unk_10a5a8] = fileId - 'A';
                    sMemCardCtx.unk_10a5a8++;
                }
                else if((temp32_0 == 0) && (fileId == 'S')) {
                    sysFilesFound++;
                }
            }
        }
        if (sMemCardCtx.unk_10a5a8 == 0) {
            if (sysFilesFound != 0) {
                if (sMemCardCtx.unk_10a8e8 != 0) break;
            }
            if (sMemCardCtx.unk_10a8e8 < 2) {
                sUnk_gp.loadInfoC.intValue = 1;
                if (sysFilesFound == 0) {
                    sDataLoadCtx.reqStatus++;
                    return 0x80;
                }
                sUnk_gp.loadInfoC.intValue = 3;
                sDataLoadCtx.reqStatus++;
                return 0x80;
            }
        }
        else {
            if (sysFilesFound == 0) {
                if (sMemCardCtx.unk_10a8e8 == 0) {
                    sUnk_gp.loadInfoC.intValue = 1;
                    if (sysFilesFound != 0) {
                        sDataLoadCtx.reqStatus++;
                        return 0x80;
                    }
                    sUnk_gp.loadInfoC.intValue = 3;
                    sDataLoadCtx.reqStatus++;
                    return 0x80;
                }
            }
        }
    case 6:
        if (sMemCardCtx.mcChannel == 0) {
            sUnk_gp.unk_fea58 = sUnk_gp.loadInfoC.intValue;
            sMemCardCtx.mcChannel = 0x10;
            sDataLoadCtx.reqStatus++;
            return 0x80;
        }
        Heap_FreeMemory(sUnk_gp.loadInfoA.dataBuffer, 600);
        if(sUnk_gp.unk_fea58 == 1){ sMemCardCtx.mcChannel = 0; }
        temp32_0 = sUnk_gp.loadInfoC.intValue;
        if ((sUnk_gp.unk_fea58 == 3) && (sUnk_gp.loadInfoC.intValue == 1)) {
            temp32_0 = 3;
        }
        return func_8007708C(temp32_0);
    default:
        return 0x80;
    }
    Heap_FreeMemory(sUnk_gp.loadInfoA.dataBuffer, 600);
    return func_8007708C(0);
}

int func_8007A154() {

    int i = 0;
    int mcSlot = 1;
    int mccmd = 0;
    int mcrst = 0;
    SystemMCFile* sysSave;

    u32 temp32_0 = 0;

    if (sMemCardCtx.mcChannel != 0) mcSlot = 2;
    if (sDataLoadCtx.reqStatus < 13) {
        switch (sDataLoadCtx.reqStatus) {
        case 0:
            sUnk_gp.loadInfoA.dataBuffer = Heap_ArenaAlloc(0x2000);
            sUnk_gp.loadInfoB.dataBuffer = Heap_ArenaAlloc(0x2000);
            sUnk_gp.loadInfoC.dataBuffer = Heap_ArenaAlloc(0x800);
            memcpy(&sUnk_gp.unk_fea58, sDataLoadCtx.dataReqBase + sDataLoadCtx.dataReqOffset, 16);
            CdFiles_requestCdFile(FILE_MODE_FLAG_INDEX_OFFSET | FILE_MODE_FLAG_LZ77, D_System, Sys_MCIcon_Masha, 0, -1, sUnk_gp.loadInfoC.dataBuffer);
            sDataLoadCtx.dataReqOffset += 8;
            sDataLoadCtx.reqStatus++;
            break;
        case 1:
            if (CdFiles_AllSlotsEmpty()) {
                memcpy(&sMemCardCtx.mcFileName, "BISLPM-87178MYUMYUS", 20);
                sDataLoadCtx.reqStatus++;
            }
            break;
        case 2:
            mcrst = Mcrd_MemCardOpen(sMemCardCtx.mcChannel, &sMemCardCtx.mcFileName, 1);
            if (mcrst != McErrCardNotExist) {
                if (mcrst != McErrNone) {
                    if (mcrst == -1) return 0x80;
                    if (sUnk_gp.unk_fea68++ < 8) return 0x80;
                    if (mcrst != McErrNotFormat) {
                        FREE_MC_MEM;
                        return func_8007708C(4);
                    }
                    if ((mcrst < 5) || (mcrst != McErrFileNotExist)) {
                        FREE_MC_MEM;
                        return func_8007708C(2);
                    }
                }
                Mcrd_MemCardClose();
                sUnk_gp.unk_fea60 = 0;
                sDataLoadCtx.reqStatus++;
                return 0x80;
            }
            FREE_MC_MEM;
            return func_8007708C(1);
        case 3:
            if (Mcrd_MemCardSync(McSyncMode_NoWait, &mccmd, &mcrst) != McSyncStatBusy) {
                memcpy(&sMemCardCtx.mcFileName, "BISLPM-87178MYUMYUS", 20);
                LibC_memset(sUnk_gp.loadInfoA.dataBuffer, 0, 0x2000);
                Mcrd_MemCardReadFile(sMemCardCtx.mcChannel, &sMemCardCtx.mcFileName, sUnk_gp.loadInfoA.dataBuffer + 0x200, 0x200, 0x100);
                sDataLoadCtx.reqStatus++;
            }
            break;
        default:
            if (Mcrd_MemCardSync(McSyncMode_NoWait, &mccmd, &mcrst) != McSyncStatBusy) {
                if (mcrst == McErrCardNotExist) {
                    FREE_MC_MEM;
                    return func_8007708C(1);
                }
                if (mcrst == McErrNone) {
                    sUnk_gp.unk_fea60 = 0;
                    sDataLoadCtx.reqStatus++;
                    return 0x80;
                }
                if (mcrst == McErrNewCard) {
                    sMemCardCtx.unk_10a5a6 &= ~mcSlot;
                    FREE_MC_MEM;
                    return func_8007708C(6);
                }
                if (sUnk_gp.unk_fea68++ > 7) {
                    if ((mcrst == McErrCardInvalid) || (mcrst != McErrFileNotExist)) {
                        FREE_MC_MEM;
                        return func_8007708C(8);
                    }
                }
                sDataLoadCtx.reqStatus--;
            }
            break;
        case 5:
            sysSave = sUnk_gp.loadInfoA.dataBuffer;
            if (sMemCardCtx.unk_10a5a6 & mcSlot) {
                if ((LibC_strncmp2(&sysSave->checksum, &sMemCardCtx.unk_10a91c[mcSlot-1], 0x10) != 0) || (LibC_strncmp2(&sysSave->unk_210, &sMemCardCtx.unk_10a91c[mcSlot - 1], 0x50) != 0)) {
                    sMemCardCtx.unk_10a5a6 &= ~mcSlot;
                    FREE_MC_MEM;
                    return func_8007708C(7);
                }
            }
            LibC_memset(&sysSave->checksum, 0, 8);
            sysSave->unk_20c = 0x102;
            sysSave->unk_208 = sSaveCtx.unk_ff840;
            sysSave->unk_20e = Heap_prng_71fbc(0xffff);
            if (sMemCardCtx.unk_10a5a6 & (mcSlot << 2)) {
                LibC_memset(&sysSave->unk_210, 0, 0xc0);
            }

            temp32_0 = sUnk_gp.unk_fea58;
            if (temp32_0 < (sUnk_gp.unk_fea58 + sUnk_gp.unk_fea5c)) {
                sysSave->unk_210[(temp32_0 >> 3)] |= sSaveCtx.unk_ff722[(temp32_0 >> 3)] & (1 << (temp32_0 & 0x7));
                temp32_0++;
            }

            temp32_0 = sUnk_gp.unk_fea60;
            if (temp32_0 < (sUnk_gp.unk_fea60 + sUnk_gp.unk_fea64)) {
                sysSave->unk_210[(temp32_0 >> 3)] &= ~(1 << (temp32_0 & 0x7));
                sysSave->unk_210[(temp32_0 >> 3)] |= sSaveCtx.unk_ff722[(temp32_0 >> 3)] & (1 << (temp32_0 & 0x7));
                temp32_0++;
            }
            MCSave_UpdateMCDataChecksum(&sysSave->checksum, 0xd0);
            sDataLoadCtx.reqStatus++;
            break;
        case 6:
            if (Mcrd_MemCardSync(McSyncMode_NoWait, &mccmd, &mcrst) != McSyncStatBusy) {
                LibC_memset(sUnk_gp.loadInfoB.dataBuffer, 0, 0x2000);
                MCSave_WriteMCFileHead(sUnk_gp.loadInfoB.dataBuffer, sUnk_gp.loadInfoC.dataBuffer, 0, 0, -1);
                memcpy(&sMemCardCtx.mcFileName, "BISLPM-87178MYUMYUS", 20);
                Mcrd_MemCardWriteFile(sMemCardCtx.mcChannel, &sMemCardCtx.mcFileName, sUnk_gp.loadInfoB.dataBuffer, 0, 0x2000);
                sDataLoadCtx.reqStatus++;
            }
            break;
        case 8:
            if (Mcrd_MemCardSync(McSyncMode_NoWait, &mccmd, &mcrst) != McSyncStatBusy) {
                LibC_memset(sUnk_gp.loadInfoB.dataBuffer, 0, 0x2000);
                MCSave_WriteMCFileHead(sUnk_gp.loadInfoA.dataBuffer, sUnk_gp.loadInfoC.dataBuffer, 0, 0, -1);
                memcpy(&sMemCardCtx.mcFileName, "BISLPM-87178MYUMYUS", 20);
                Mcrd_MemCardWriteFile(sMemCardCtx.mcChannel, &sMemCardCtx.mcFileName, sUnk_gp.loadInfoA.dataBuffer, 0, 0x300);
                sDataLoadCtx.reqStatus++;
            }
            break;
        case 10:
            if (Mcrd_MemCardSync(McSyncMode_NoWait, &mccmd, &mcrst) != McSyncStatBusy) {
                memcpy(&sMemCardCtx.mcFileName, "BISLPM-87178MYUMYUS", 20);
                Mcrd_MemCardReadFile(sMemCardCtx.mcChannel, &sMemCardCtx.mcFileName, sUnk_gp.loadInfoB.dataBuffer, 0, 0x300);
                sDataLoadCtx.reqStatus++;
            }
            break;
        case 12:
            sysSave = sUnk_gp.loadInfoA.dataBuffer;
            if (LibC_strncmp2(sUnk_gp.loadInfoA.dataBuffer, sUnk_gp.loadInfoB.dataBuffer, 0x300) != 0) {
                FREE_MC_MEM;
                return func_8007708C(3);
            }
            memcpy(&sMemCardCtx.unk_10a91c[mcSlot - 1], &sysSave->checksum, 0x60);
            sMemCardCtx.unk_10a5a6 |= mcSlot;
            sMemCardCtx.unk_10a5a6 &= ~(mcSlot << 4);
            FREE_MC_MEM;
            return func_8007708C(0);
        }
    }
    return 0x80;
}

int func_8007AD64() {
    
    int i = 0;
    int mcSlot = 1;
    int mccmd = 0;
    int mcrst = 0;
    SystemMCFile* sysSave;

    int imax = 0;

    if (sMemCardCtx.mcChannel != 0) mcSlot = 2;
    if (sDataLoadCtx.reqStatus == 1) {
        if (Mcrd_MemCardSync(McSyncMode_NoWait, &mccmd, &mcrst) != McSyncStatBusy) {
            memcpy(&sMemCardCtx.mcFileName, "BISLPM-87178MYUMYUS", 20);
            Mcrd_MemCardReadFile(sMemCardCtx.mcChannel, &sMemCardCtx.mcFileName, sUnk_gp.loadInfoA.dataBuffer, 0, 0x2000);
            sDataLoadCtx.reqStatus++;
        }
    }
    else {
        if (sDataLoadCtx.reqStatus < 2) {
            if (sDataLoadCtx.reqStatus != 0) return 0x80;
            sUnk_gp.loadInfoA.dataBuffer = Heap_ArenaAlloc(0x2000);
            memcpy(sDataLoadCtx.dataReqBase + sDataLoadCtx.dataReqOffset, &sUnk_gp.unk_fea58, 16);
            sUnk_gp.unk_fea68 = 0;
            sDataLoadCtx.dataReqOffset++;
            sDataLoadCtx.reqStatus++;
            return 0x80;
        }
        if (sDataLoadCtx.reqStatus != 2) {
            if (sDataLoadCtx.reqStatus != 3) return 0x80;
            sysSave = (SystemMCFile*)sUnk_gp.loadInfoA.dataBuffer;
            if (sysSave->unk_20c == 0x102) {
                if (MCSave_ChecksumMCData(&sysSave->checksum, 0xd0)) {
                    imax = (int)(sUnk_gp.unk_fea58 + sUnk_gp.unk_fea5c);
                    for (i = sUnk_gp.unk_fea58; i < imax; i++) {
                        sSaveCtx.unk_ff722[i >> 3] |= (sysSave->unk_210[i >> 3] & (1 << i & 0x7));
                    }

                    imax = (int)(sUnk_gp.unk_fea60 + sUnk_gp.unk_fea64);
                    for (i = sUnk_gp.unk_fea60; i < imax; i++) {
                        sSaveCtx.unk_ff722[i >> 3] &= ~(1 << i & 0x7);
                        sSaveCtx.unk_ff722[i >> 3] |= (sysSave->unk_210[i >> 3] & (1 << i & 0x7));
                    }
                    sMemCardCtx.unk_10a5a6 &= 0xfffc;
                    sMemCardCtx.unk_10a5a6 |= mcSlot;
                    memcpy(&sMemCardCtx.unk_10a91c[mcSlot-1], &sysSave->checksum, 0x10);
                    memcpy(&sMemCardCtx.unk_10a91c[mcSlot - 1].unk_10, &sysSave->unk_210, 0x10);
                    Heap_FreeMemory(sUnk_gp.loadInfoA.dataBuffer, 0x2000);
                    return func_8007708C(0);
                }
            }
            sMemCardCtx.unk_10a5a6 &= ~mcSlot;
            Heap_FreeMemory(sUnk_gp.loadInfoA.dataBuffer, 0x2000);
            return func_8007708C(4);
        }

        if (Mcrd_MemCardSync(McSyncMode_NoWait, &mccmd, &mcrst) != McSyncStatBusy) {
            if (mcrst == McErrCardNotExist) {
                Heap_FreeMemory(sUnk_gp.loadInfoA.dataBuffer, 0x2000);
                return func_8007708C(1);
            }
            if (mcrst == McErrNone) {
                sUnk_gp.unk_fea68 = 0;
                sDataLoadCtx.reqStatus++;
                return 0x80;
            }
            if (mcrst == McErrNewCard) {
                sMemCardCtx.unk_10a5a6 &= ~mcSlot;
                Heap_FreeMemory(sUnk_gp.loadInfoA.dataBuffer, 0x2000);
                return func_8007708C(6);
            }

            if (sUnk_gp.unk_fea68++ > 7) {
                if ((mcrst != McErrCardInvalid) && (mcrst == McErrFileNotExist)) {
                    Heap_FreeMemory(sUnk_gp.loadInfoA.dataBuffer, 0x2000);
                    return func_8007708C(3);
                }
                Heap_FreeMemory(sUnk_gp.loadInfoA.dataBuffer, 0x2000);
                return func_8007708C(5);
            }
            sDataLoadCtx.reqStatus--;
        }
    }

    return 0x80;
}

int func_8007B2E8() {
    int i = 0;

    int mcSlot = 1;
    int mccmd = 0;
    int mcrst = 0;

    SystemMCFile* sysSave;

    if (sMemCardCtx.mcChannel != 0) mcSlot = 2;
    if (sDataLoadCtx.reqStatus > 13) return 0x80;
    switch (sDataLoadCtx.reqStatus) {
    case 0:
        sUnk_gp.loadInfoA.dataBuffer = Heap_ArenaAlloc(0x2000);
        sUnk_gp.loadInfoC.dataBuffer = Heap_ArenaAlloc(0x2000);
        sUnk_gp.loadInfoB.dataBuffer = Heap_ArenaAlloc(0x2000);
        sUnk_gp.unk_fea68 = 0;
        Mcrd_MemCardClose();
        CdFiles_requestCdFile(FILE_MODE_FLAG_INDEX_OFFSET | FILE_MODE_FLAG_LZ77, D_System, Sys_MCIcon_Masha, 0, -1, sUnk_gp.loadInfoC.dataBuffer);
        sDataLoadCtx.reqStatus++;
        break;
    case 1:
        if (CdFiles_AllSlotsEmpty()) sDataLoadCtx.reqStatus++;
        break;
    case 2:
        if (Mcrd_MemCardSync(McSyncMode_NoWait, &mccmd, &mcrst) != McSyncStatBusy) {
            Mcrd_MemCardAccept(sMemCardCtx.mcChannel);
            sDataLoadCtx.reqStatus++;
        }
        //No break
    case 3:
        if (Mcrd_MemCardSync(McSyncMode_NoWait, &mccmd, &mcrst) != McSyncStatBusy) {
            if (mcrst == McErrCardNotExist) {
                FREE_MC_MEM;
                return func_8007708C(1);
            }
            if (mcrst == McErrNone) {
                sUnk_gp.unk_fea68 = 0;
                sDataLoadCtx.reqStatus++;
                return 0x80;
            }
            if (mcrst == McErrNewCard) {
                FREE_MC_MEM;
                return func_8007708C(8);
            }
            if (sUnk_gp.unk_fea68++ > 7) {
                if ((mcrst == McErrCardInvalid) || (mcrst != McErrNotFormat)) {
                    sUnk_gp.unk_fea68 = 0;
                    sDataLoadCtx.reqStatus += 2;
                    return 0x80;
                }
                FREE_MC_MEM;
                return func_8007708C(4);
            }
            sDataLoadCtx.reqStatus--;
        }
        break;
    case 4:
        memcpy(&sMemCardCtx.mcFileName, "BISLPM-87178MYUMYUS", 20);
        sDataLoadCtx.reqStatus++;
        //No break
    case 5:
        mcrst = Mcrd_MemCardOpen(sMemCardCtx.mcChannel, &sMemCardCtx.mcFileName, McIoRead);
        if (mcrst == McErrCardNotExist) {
            FREE_MC_MEM;
            return func_8007708C(1);
        }
        else if (mcrst == McErrNone) {
            Mcrd_MemCardClose();
            FREE_MC_MEM;
            return func_8007708C(6);
        }
        else if (mcrst == McErrFileNotExist) {
            sUnk_gp.unk_fea68 = 0;
            sDataLoadCtx.reqStatus++;
            return 0x80;
        }
        else if (mcrst == -1) {
            return 0x80;
        }

        if (sUnk_gp.unk_fea68++ < 8) return 0x80;
        if ((mcrst == McErrCardInvalid) || (mcrst != McErrNotFormat)) {
            FREE_MC_MEM;
            return func_8007708C(2);
        }
        FREE_MC_MEM;
        return func_8007708C(4);
    case 6:
        LibC_memset(sUnk_gp.loadInfoA.dataBuffer, 0, 0x2000);
        MCSave_WriteMCFileHead(sUnk_gp.loadInfoA.dataBuffer, sUnk_gp.loadInfoC.dataBuffer, 0, 0, -1);
        sDataLoadCtx.reqStatus++;
        //No break
    case 7:
        mcrst = Mcrd_MemCardCreateFile(sMemCardCtx.mcChannel, &sMemCardCtx.mcFileName, 1);
        if (mcrst == McErrCardNotExist) {
            FREE_MC_MEM;
            return func_8007708C(1);
        }
        else if (mcrst == McErrNone) {
            sUnk_gp.unk_fea68 = 0;
            sDataLoadCtx.reqStatus++;
            return 0x80;
        }
        else if (mcrst == -1) {
            return 0x80;
        }

        if (sUnk_gp.unk_fea68++ < 8) return 0x80;
        if (mcrst != McErrNotFormat) {
            if (mcrst < 5) {
                FREE_MC_MEM;
                return func_8007708C(9);
            }
            if (mcrst == McErrAlreadyExist) {
                FREE_MC_MEM;
                return func_8007708C(6);
            }
            else if (mcrst != McErrBlockFull) {
                FREE_MC_MEM;
                return func_8007708C(9);
            }
            FREE_MC_MEM;
            return func_8007708C(8);
        }
        FREE_MC_MEM;
        return func_8007708C(4);
    case 8:
        sysSave = (SystemMCFile*)sUnk_gp.loadInfoA.dataBuffer;
        LibC_memset(sUnk_gp.loadInfoA.dataBuffer + 0x200, 0, 0x2000); //With a 3 frame icon, 0x200 would be the start of the actual file data
        sysSave->unk_20c = 0x102;
        sysSave->unk_208 = sSaveCtx.unk_ff840;
        sysSave->unk_20e = Heap_prng_71fbc(0xffff);
        MCSave_UpdateMCDataChecksum(sysSave->checksum, 0xd0);
        sDataLoadCtx.reqStatus++;
        //No break
    case 9:
        if (Mcrd_MemCardSync(McSyncMode_NoWait, &mccmd, &mcrst) != McSyncStatBusy) {
            LibC_memset(sUnk_gp.loadInfoB.dataBuffer, 0, 0x2000);
            MCSave_WriteMCFileHead(sUnk_gp.loadInfoA.dataBuffer, sUnk_gp.loadInfoC.dataBuffer, 0, 0, -1);
            memcpy(&sMemCardCtx.mcFileName, "BISLPM-87178MYUMYUS", 20);
            Mcrd_MemCardWriteFile(sMemCardCtx.mcChannel, &sMemCardCtx.mcFileName, sUnk_gp.loadInfoA.dataBuffer, 0, 0x2000);
            sDataLoadCtx.reqStatus++;
        }
        break;
    default:
        if (Mcrd_MemCardSync(McSyncMode_NoWait, &mccmd, &mcrst) == McSyncStatBusy) return 0x80;
        if (mcrst == McErrCardNotExist) {
            FREE_MC_MEM;
            return func_8007708C(1);
        }
        else if (mcrst == McErrNone) {
            sUnk_gp.unk_fea68 = 0;
            sDataLoadCtx.reqStatus++;
            return 0x80;
        }
        else if (mcrst == McErrNewCard) {
            sMemCardCtx.unk_10a5a6 &= ~((u16)mcSlot);
            FREE_MC_MEM;
            return func_8007708C(6);
        }

        if (sUnk_gp.unk_fea68++ > 7) {
            if ((mcrst != McErrCardInvalid) || (mcrst == McErrFileNotExist)) {
                FREE_MC_MEM;
                return func_8007708C(5);
            }
            FREE_MC_MEM;
            return func_8007708C(2);
        }
        sDataLoadCtx.reqStatus--;
        break;
    case 11:
        if (Mcrd_MemCardSync(McSyncMode_NoWait, &mccmd, &mcrst) != McSyncStatBusy) {
            memcpy(&sMemCardCtx.mcFileName, "BISLPM-87178MYUMYUS", 20);
            Mcrd_MemCardReadFile(sMemCardCtx.mcChannel, &sMemCardCtx.mcFileName, sUnk_gp.loadInfoB.dataBuffer, 0, 0x2000);
            sDataLoadCtx.reqStatus++;
        }
        break;
    case 13:
        if (LibC_strncmp2(sUnk_gp.loadInfoA.dataBuffer, sUnk_gp.loadInfoB.dataBuffer, 0x2000) == 0) {
            sMemCardCtx.unk_10a5a6 &= 0xfffc | ((u16)mcSlot);
            //It copies the first four words as words, then goes to a loop.
            //But honestly, since I'm not trying to match I'll just make it copy the whole 0x60
            memcpy(&sMemCardCtx.unk_10a91c[mcSlot - 1], sUnk_gp.loadInfoA.dataBuffer + 0x200, 0x60);
            FREE_MC_MEM;
            return func_8007708C(0);
        }
        FREE_MC_MEM;
        return func_8007708C(3);
    }

    return 0x80;
}

int func_8007BCE4() {
    u16 mcSlot = 1;
    int retval = 0;

    int mccmd = 0;
    int mcrst = 0;

    if (sDataLoadCtx.reqStatus > 8) return 0x80;
    if (sMemCardCtx.mcChannel != 0) mcSlot = 2;
    switch (sDataLoadCtx.reqStatus) {
    case 0:
        sUnk_gp.unk_fea68 = 0;
        sDataLoadCtx.reqStatus++;
        //No break
    case 1:
        retval = 0x80;
        if (Mcrd_MemCardSync(McSyncMode_NoWait, &mccmd, &mcrst) != McSyncStatBusy) {
            Mcrd_MemCardAccept(sMemCardCtx.mcChannel);
            sDataLoadCtx.reqStatus++;
        }
        break;
    case 2:
        if (Mcrd_MemCardSync(McSyncMode_NoWait, &mccmd, &mcrst) == McSyncStatBusy) { retval = 0x80; break; }
        if (mcrst == McErrCardNotExist) {
            retval = func_8007708C(1);
            break;
        }
        if (mcrst == McErrNone) {
            retval = 0x80;
            sUnk_gp.unk_fea68 = 0;
            sDataLoadCtx.reqStatus++;
            break;
        }
        if (mcrst == McErrNewCard) {
            sMemCardCtx.unk_10a5a6 &= ~mcSlot;
            return func_8007708C(7);
        }

        if (sUnk_gp.unk_fea68++ < 8) {
            sDataLoadCtx.reqStatus--;
            retval = 0x80;
        }
        if ((mcrst == McErrCardInvalid) || (mcrst != McErrNotFormat)) {
            retval = func_8007708C(2);
            break;
        }
        retval = func_8007708C(3);
        break;
    case 3:
        memcpy(&sMemCardCtx.mcFileName, "BISLPM-87178MYUMYUS", 20);
        sDataLoadCtx.reqStatus++;
        //No break
    case 4:
        mcrst = Mcrd_MemCardOpen(sMemCardCtx.mcChannel, &sMemCardCtx.mcFileName, McIoRead);
        if (mcrst == McErrCardNotExist) {
            retval = func_8007708C(1);
        }
        else if(mcrst == McErrNone){
            Mcrd_MemCardClose();
            retval = 0x80;
            sUnk_gp.unk_fea68 = 0;
            sDataLoadCtx.reqStatus++;
        }
        else if (mcrst == -1) {
            retval = 0x80;
        }
        else {
            if (mcrst == McErrFileNotExist) {
                sUnk_gp.unk_fea68 += 3;
            }
            retval = 0x80;
            if (sUnk_gp.unk_fea68++ > 7) {
                if (mcrst == McErrNotFormat) {
                    retval = func_8007708C(3);
                }
                else {
                    if ((mcrst < 5) || (mcrst != 5)) {
                        retval = func_8007708C(2);
                    }
                    else {
                        sMemCardCtx.unk_10a5a6 &= (~mcSlot | (mcSlot << 4));
                        retval = func_8007708C(4);
                    }
                }
            }
        }
        break;

    case 5:
        sUnk_gp.loadInfoA.dataBuffer = Heap_ArenaAlloc(0x2000);
        sDataLoadCtx.reqStatus++;
        //No break
    case 6:
        if (Mcrd_MemCardSync(McSyncMode_NoWait, &mccmd, &mcrst) != McSyncStatBusy) {
            memcpy(&sMemCardCtx.mcFileName, "BISLPM-87178MYUMYUS", 20);
            Mcrd_MemCardReadFile(sMemCardCtx.mcChannel, &sMemCardCtx.mcFileName, sUnk_gp.loadInfoA.dataBuffer, 0x200, 0x100);
            sDataLoadCtx.reqStatus++;
            retval = 0x80;
        }
        retval = 0x80;
        break;
    case 7:
        if (Mcrd_MemCardSync(McSyncMode_NoWait, &mccmd, &mcrst) != McSyncStatBusy) {
            if (mcrst == McErrCardNotExist) {
                Heap_FreeMemory(sUnk_gp.loadInfoA.dataBuffer, 0x2000);
                retval = func_8007708C(1);
                break;
            }
            else if (mcrst == McErrNone) {
                retval = 0x80;
                sUnk_gp.unk_fea68 = 0;
                sDataLoadCtx.reqStatus++;
                break;
            }
            else if (mcrst == McErrNewCard) {
                sMemCardCtx.unk_10a5a6 &= ~mcSlot;
                Heap_FreeMemory(sUnk_gp.loadInfoA.dataBuffer, 0x2000);
                return func_8007708C(7);
            }
            else {
                if (sUnk_gp.unk_fea68++ > 7) {
                    if ((mcrst == McErrCardInvalid) || (mcrst != McErrFileNotExist)) {
                        Heap_FreeMemory(sUnk_gp.loadInfoA.dataBuffer, 0x2000);
                        retval = func_8007708C(2);
                        break;
                    }
                    Heap_FreeMemory(sUnk_gp.loadInfoA.dataBuffer, 0x2000);
                    retval = func_8007708C(3);
                }
                sDataLoadCtx.reqStatus--;
                break;
            }
        }
        else {
            retval = 0x80;
            break;
        }
    case 8:
        if ((*((u16*)(sUnk_gp.loadInfoA.dataBuffer + 12)) != 0x102) || (func_80072624(sUnk_gp.loadInfoA.dataBuffer, 0xd0) == 0)) {
            sMemCardCtx.unk_10a5a6 &= (~mcSlot | (mcSlot << 2));
            Heap_FreeMemory(sUnk_gp.loadInfoA.dataBuffer, 0x2000);
            return func_8007708C(6);
        }
        sMemCardCtx.unk_10a5a6 &= (~(mcSlot << 2) | ~(mcSlot << 4));
        if (sMemCardCtx.unk_10a5a6 & mcSlot) {
            if ((LibC_strncmp2(sUnk_gp.loadInfoA.dataBuffer, &sMemCardCtx.unk_10a91c[mcSlot - 1], 0x10) != 0) ||
                (LibC_strncmp2(sUnk_gp.loadInfoA.dataBuffer + 0x10, &sMemCardCtx.unk_10a91c[mcSlot - 1].unk_10, 0x50) != 0)) {
                sMemCardCtx.unk_10a5a6 &= ~mcSlot;
                Heap_FreeMemory(sUnk_gp.loadInfoA.dataBuffer, 0x2000);
                return func_8007708C(7);
            }
        }
        Heap_FreeMemory(sUnk_gp.loadInfoA.dataBuffer, 0x2000);
        retval = func_8007708C(0);
    }
    return retval;
}

int func_8007C2EC() {
    int b_cond = FALSE;
    MemCardDataReq* req = sDataLoadCtx.dataReqBase + sDataLoadCtx.dataReqOffset++;

    if (req->unk_00 == 1) {
        b_cond = (sMemCardCtx.mcChannel != sMemCardCtx.unk_10a8ec);
        sMemCardCtx.mcChannel = sMemCardCtx.unk_10a8ec;
    }
    else if(req->unk_00 == 2){
        sMemCardCtx.unk_10a5a4 = sMemCardCtx.mcChannel;
        return 1;
    }
    else if (req->unk_00 == 0) {
        b_cond = (sMemCardCtx.mcChannel != sMemCardCtx.unk_10a5a4);
        sMemCardCtx.unk_10a8ec = sMemCardCtx.mcChannel;
        sMemCardCtx.mcChannel = sMemCardCtx.unk_10a5a4;
        if ((sMemCardCtx.unk_10a5a4 != 0) && (sMemCardCtx.unk_10a5a4 != 0x10)) {
            return func_8007708C(2);
        }
    }

    if(!b_cond) return func_8007708C(1);
    return func_8007708C(0);
}

int func_8007C3F8() {
    int i = 0;
    MemCardDataReq* req = sDataLoadCtx.dataReqBase + sDataLoadCtx.dataReqOffset++;

    u16 temp16_0 = 0;
    u16 temp16_1 = 0;
    u16 temp16_2 = 0;

    switch (req->unk_00) {
    case 1:
        if (sMemCardCtx.unk_10a918 == NULL) return 1;
        memcpy(&sSaveCtx.unk_ff520, sMemCardCtx.unk_10a918, 0x322);
        Heap_FreeMemory(sMemCardCtx.unk_10a918, 0x328);
        sMemCardCtx.unk_10a918 = NULL;
        break;
    case 2:
        if (sMemCardCtx.unk_10a918 != NULL) {
            temp16_0 = req->unk_01;
            temp16_1 = req->unk_03 + req->unk_01;
            temp16_2 = req->unk_01 >> 3;
            while (temp16_2 < 0xc0) {
                sSaveCtx.unk_ff722[temp16_2] |= *((u8*)sMemCardCtx.unk_10a918 + temp16_2 + 0x202) & (1 << (temp16_0 & 0x7));
                if (++temp16_0 >= temp16_1) break;
                temp16_2 = temp16_0 >> 3;
            }

            temp16_0 = req->unk_05;
            temp16_1 = req->unk_07 + req->unk_05;
            temp16_2 = req->unk_05 >> 3;
            while (temp16_2 < 0xc0) {
                sSaveCtx.unk_ff722[temp16_2] &= ~(1 << (temp16_0 & 0x7));
                sSaveCtx.unk_ff722[temp16_2] |= *((u8*)sMemCardCtx.unk_10a918 + temp16_2 + 0x202) & (1 << (temp16_0 & 0x7));
                if (++temp16_0 >= temp16_1) break;
                temp16_2 = temp16_0 >> 3;
            }
        }
        sDataLoadCtx.dataReqOffset += 8;
        break;
    case 3:
        if (sMemCardCtx.unk_10a918 == NULL) return 1;
        Heap_FreeMemory(sMemCardCtx.unk_10a918, 0x328);
        sMemCardCtx.unk_10a918 = NULL;
        break;
    case 4:
        if (sMemCardCtx.unk_10a918 != NULL) return 1;
        //no break?
    case 0:
        if (sMemCardCtx.unk_10a918 == NULL) {
            sMemCardCtx.unk_10a918 = Heap_ArenaAlloc(0x328);
        }
        memcpy(sMemCardCtx.unk_10a918, &sSaveCtx.unk_ff520, 0x322);
        break;
    case 5:
        if (sMemCardCtx.unk_10a918 != NULL) {
            temp16_0 = req->unk_01;
            temp16_1 = req->unk_03 + req->unk_01;
            temp16_2 = req->unk_01 >> 3;
            while (temp16_2 < 0xc0) {
                *((u8*)sMemCardCtx.unk_10a918 + temp16_2 + 0x202) &= ~(1 << (temp16_0 & 0x7));
                *((u8*)sMemCardCtx.unk_10a918 + temp16_2 + 0x202) |= (sSaveCtx.unk_ff722[temp16_2] & (1 << (temp16_0 & 0x7)));
                if (++temp16_0 >= temp16_1) break;
                temp16_2 = temp16_0 >> 3;
            }
        }
        sDataLoadCtx.dataReqOffset += 4;
        break;
    }
    return 1;
}

u32 func_8007C868() {

    uint32_t mcCommand; //local_10
    uint32_t mcResult; //local_c

    switch (sDataLoadCtx.reqStatus) {
    case 0:
        sMemCardCtx.mcChannel = 0;
        sUnk_gp.loadInfoA.dataBuffer = NULL;
        sUnk_gp.loadInfoB.dataBuffer = 0;
        sUnk_gp.unk_fea68 = 0;
        sDataLoadCtx.reqStatus++;
        break;
    case 1:
    case 11:
        if (Mcrd_MemCardSync(McSyncMode_NoWait, &mcCommand, &mcResult) != McSyncStatBusy) {
            Mcrd_MemCardAccept(sMemCardCtx.mcChannel);
            sDataLoadCtx.reqStatus++;
        }
        //No break?
    case 2:
    case 12:
        if (Mcrd_MemCardSync(McSyncMode_NoWait, &mcCommand, &mcResult) != McSyncStatBusy) {
            if (mcResult == McErrCardNotExist) {
                sUnk_gp.loadInfoB.dataBuffer = mcResult;
            }
            else {
                if (mcResult == McErrNone) {
                    sUnk_gp.unk_fea68 = 0;
                    sDataLoadCtx.reqStatus++;
                    return 0x80;
                }
                if ((mcResult == McErrNewCard) || (++sUnk_gp.unk_fea68 < 8)) {
                    sDataLoadCtx.reqStatus--;
                    return 0x80;
                }
            }
            sUnk_gp.unk_fea68 = 0;
            sDataLoadCtx.reqStatus += 8;
            return 0x80;
        }
        break;
    case 3:
    case 13:
        if (Mcrd_MemCardSync(McSyncMode_NoWait, &mcCommand, &mcResult) != McSyncStatBusy) {
            memcpy(&sMemCardCtx.mcFileName, "BISLPM-87178MYUMYUS", 20); //System file name
            sDataLoadCtx.reqStatus++;
        }
        //No break?
    case 4:
    case 14:
        mcResult = Mcrd_MemCardOpen(sMemCardCtx.mcChannel, &sMemCardCtx.mcFileName, McIoRead);
        if (mcResult != McErrCardNotExist) {
            if (mcResult == McErrNone) {
                Mcrd_MemCardClose();
                return func_8007708C(0);
            }
            if (mcResult == -1) return 0x80;
            if (mcResult == McErrFileNotExist) {
                sUnk_gp.unk_fea68 = 8;
            }

            mcResult = sUnk_gp.loadInfoB.dataBuffer;
            if (sUnk_gp.unk_fea68 < 8) {
                sUnk_gp.unk_fea68++;
                return 0x80;
            }
        }
        sUnk_gp.loadInfoB.dataBuffer = mcResult;
        sUnk_gp.unk_fea68 = 0;
        sDataLoadCtx.reqStatus += 6;
        return 0x80;
    case 10:
        sUnk_gp.loadInfoA.dataBuffer = sUnk_gp.loadInfoB.dataBuffer; //Does fea50 contain a pointer? But it also stores Mc command results?
        sMemCardCtx.mcChannel = 0x10;
        sUnk_gp.loadInfoB.dataBuffer = NULL;
        sDataLoadCtx.reqStatus++;
        break;
    case 20:
        sMemCardCtx.unk_10a5a4 = 0x20;
        sMemCardCtx.mcChannel = 0x00;
        if ((sUnk_gp.loadInfoA.dataBuffer != NULL) && (sUnk_gp.loadInfoB.dataBuffer != NULL)) {
            return func_8007708C(2);
        }
        return func_8007708C(1);
    }
    return 0x80;
}


//Move or remove this eventually?
// Looks like another strncmp
//800E1288
int LibC_strncmp2(u8* arg0, u8* arg1, int arg2) {
    int i = 0;
    for (i = 0; i < arg2; i++) {
        if (*arg0 != *arg1) return *arg0 - *arg1;
        arg0++; arg1++;
    }
    return 0;
}