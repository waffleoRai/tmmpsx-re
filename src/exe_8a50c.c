//FACE1

#include "global.h"

//8008A50C
u32 UIGfx_LoadFaceToVmem() {

    int i = 0;
    RECT dims;
    FaceSpriteFile* sprite;
    byte* loadReq;

    u32 drawslot;

    u32 temp32_0; //uvar11

    loadReq = sDataLoadCtx.dataReqBase + sDataLoadCtx.dataReqOffset;
    temp32_0 = (u32)(loadReq[1]);

    if (temp32_0 == 0xff) {
        temp32_0 = (u32)Heap_CheckFlags_ff722_71edc(0x38, 8) & 0xff;
    }
    else {
        if (temp32_0 & 0x80) {
            temp32_0 = (u32)sUnk_108ac0.unk_108ac4[temp32_0 & 0x7f];
        }
    }

    if (sDataLoadCtx.reqStatus == 0) {
        sUnk_gp.loadDataBufferA = Heap_ArenaAlloc(0x10000);
        CdFiles_requestCdFile(FILE_MODE_FLAG_INDEX_OFFSET | FILE_MODE_FLAG_LZ77, D_Face, temp32_0 + (u32)(loadReq[2]), 0, -1, sUnk_gp.loadDataBufferA);
        sDataLoadCtx.reqStatus++;
    }
    else {
        if (sDataLoadCtx.reqStatus != 1) return DATA_LOAD_FLAG_UNK_80;
        if (CdFiles_AllSlotsEmpty()) {
            sprite = (FaceSpriteFile*)sUnk_gp.loadDataBufferA;

            GfxDraw_SetCLUTSemitrans(sprite->clut.table, 0x100, 1);
            temp32_0 = (u32)(loadReq[0]) & 0xf;
            //First checks if src ptr & 3 == 0, but does same thing either way.
            for (i = 0; i < 0x200; i++) {
                sHeap.unk_1f2000[temp32_0][i] = sUnk_gp.loadDataBufferA[0x18 + i];
            }

            for (i = 0; i < 0x200; i++) {
                sHeap.unk_1f6000[temp32_0][i] = sUnk_gp.loadDataBufferA[0x18 + i];
            }
            
            drawslot = (u32)sUnk_gp.drawingContext->txGroupSlots[2] + (u32)loadReq[0]; //loadReq[0] is slot idx within FACE group? And the unk_280e[2] is the slot idx of the base for FACE slots?

            dims.x = (s16)(sUnk_gp.drawingContext->spriteTxs[drawslot].u_base >> 1) + (s16)(sUnk_gp.drawingContext->spriteTxs[drawslot].t_page_xy & 0xf) << 6;
            dims.y = (s16)sUnk_gp.drawingContext->spriteTxs[drawslot].v_base + (s16)(sUnk_gp.drawingContext->spriteTxs[drawslot].t_page_xy & 0x10) << 4;
            dims.w = sprite->frameHeader.width >> 1;
            dims.h = sprite->frameHeader.height;
            GpuSys_LoadImage(&dims, (uint32_t*)((u32)sprite + sprite->frameHeader.bitmapOffset));
            GpuSys_DrawSync(0);
            Heap_ReserveFreeMemory(sUnk_gp.loadDataBufferA, 0x10000);
            sDataLoadCtx.dataReqOffset += 3;
            return DATA_LOAD_FLAG_UNK_01;
        }
    }
    return DATA_LOAD_FLAG_UNK_80;
}