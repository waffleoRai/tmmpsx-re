#include "global.h"

//80051C68
void GfxDraw_loadSprite4Bit(SpriteFileHeader* spriteFile, u8 frameIndex, u8 txSlotGroup, u8 txSlotOff, byte heapBufferSlot) {

    int i;

    RECT rect;
    SpriteFrameHeader* frameHeader;
    u16* clut;
    byte* bufferPtr;
    byte* clutPtr;
    u16 txSlot;

    txSlot = gDrawingCtx->txGroupSlots[txSlotGroup] + (u16)txSlotOff;

    rect.w = gDrawingCtx->spriteTxs[txSlot].width >> 2;
    rect.h = gDrawingCtx->spriteTxs[txSlot].height;
    rect.x = ((u32)gDrawingCtx->spriteTxs[txSlot].u_base >> 2) + ((u32)gDrawingCtx->spriteTxs[txSlot].t_page_xy & 0xf << 6);
    rect.y = ((u32)gDrawingCtx->spriteTxs[txSlot].v_base) + ((u32)gDrawingCtx->spriteTxs[txSlot].t_page_xy & 0x10 << 4);

    frameHeader = GET_FRAME_HEADER(spriteFile, frameIndex);

    //Ghidra's output has it pointing to 4 before sectino pointer (since it doesn't add sizeof header until later). My macro adds the header size, and I put the +1 in too to bring it to first CLUT
    clut = (u16*)(GET_CLUT_SECTION_POINTER(spriteFile)) + 1; 
    GpuSys_LoadImage(&rect, GET_BITMAP_POINTER(spriteFile, frameHeader));
    GfxDraw_SetCLUTSemitrans(clut, 0x10, clut[-2] >> 4);
    clut += 0x10 * frameHeader->clutIndex; //Bring pointer to CLUT for chosen frame

    //Copy the CLUT to certain buffers near the heap
    //These are actually copied by words
    clutPtr = (byte*)clut;
    bufferPtr = &sHeap.unk_1f0000 + (0x20 * heapBufferSlot); //Slot for CLUT entry in this heap buffer?
    for (i = 0; i < 0x20; i++) {
        *bufferPtr++ = *clutPtr++;
    }

    clutPtr = (byte*)clut;
    bufferPtr = &sHeap.unk_1f4000 + (0x20 * heapBufferSlot); //Slot for CLUT entry in this heap buffer?
    for (i = 0; i < 0x20; i++) {
        *bufferPtr++ = *clutPtr++;
    }
}

//80053F18 - Sets all values in a CLUT semitransparent (ie. sets highest bit)
void GfxDraw_SetCLUTSemitrans(u16* clut, int entries, int clutCount) {
    int i, j;

    if (entries == 0) return;
    if (clutCount != 0) {
        if (entries == 1) {
            if (clutCount > 0) {
                for (i = 0; i < clutCount; i++) clut[i] |= 0x8000;
            }
        }
        else {
            for (i = 0; i < clutCount; i++) {
                *clut++ = 0;
                if (entries > 0) {
                    for (j = 0; j < entries; j++) {
                        *clut++ |= 0x8000;
                    }
                }
            }
        }
    }
}

//80056C6C
void GfxDraw_loadSprite8Bit_Y256(SpriteFileHeader* spriteFile, u8 frameIndex, u8 heapBufferSlot) {
    RECT dims;
    u16* clut;
    SpriteFrameHeader* frameHeader;

    u16 clutEntries; 

    u16 slotOffset;
    
    dims.x = (u16)heapBufferSlot * 0x18 + 0x1c0;
    clut = GET_CLUT8_POINTER(spriteFile, 0);

    clutEntries = clut[-2];
    GfxDraw_SetCLUTSemitrans(clut, 0x100, clutEntries >> 8);
    slotOffset = ((u32)heapBufferSlot << 8 + 0x600 & 0xffff) << 1;
    LibC_memcpy(&sHeap.unk_1f0000[slotOffset], clut, clutEntries << 1);
    LibC_memcpy(&sHeap.unk_1f4000[slotOffset], clut, clutEntries << 1);
    frameHeader = GET_FRAME_HEADER(spriteFile, (u32)frameIndex);
    dims.y = 0x100;
    dims.w = frameHeader->width >> 1;
    dims.h = frameHeader->height;
    GpuSys_LoadImage(&dims, GET_BITMAP_POINTER(spriteFile, frameHeader));
}

//80056FBC
u8 GfxDraw_renderSpritePolys_56fbc(u8 sprslot, byte arg1, byte txSlotGroup, byte txSlotOff, byte txSpecs, s16 bound_right, s16 bound_down) {

    int i;
    SpriteTxDrawParam* txDrawParam;

    //Look for empty sprite slot
    if (sprslot == 0xff) {
        for (i = 0; i < 0x40; i++) {
            if (!(gSpriteCtx.loadedSprites[i].txSpec.fields.inUse)) break;
        }
        sprslot = (u8)i;
        if (i == 0x40) return 0xff;
    }

    gSpriteCtx.loadedSprites[sprslot].txSpec.value &= ~0x003f;
    gSpriteCtx.loadedSprites[sprslot].txSpec.value |= 0x00c0;
    gSpriteCtx.loadedSprites[sprslot].txSpec.fields.unk_0 = gSpriteCtx.unk_101e04[arg1].unk_00 & 0x7;
    gSpriteCtx.loadedSprites[sprslot].txSpec.fields.unk_3 = arg1 & 0x7;
    gSpriteCtx.loadedSprites[sprslot].txSlotGroup = txSlotGroup;
    gSpriteCtx.loadedSprites[sprslot].txSlotMember = txSlotOff;

    txDrawParam = &gDrawingCtx->spriteTxs[gDrawingCtx->txGroupSlots[txSlotGroup] + txSlotOff];
    //pcVar3 points to gDrawingCtx->unk_000c[gDrawingCtx->unk_280e[arg2] + arg3];
    //iVar2 points to 12 before that.

    if (txSpecs == 0xff) {
        gSpriteCtx.loadedSprites[sprslot].txSpec.value &= ~0xff00U;
        gSpriteCtx.loadedSprites[sprslot].txSpec.fields.clut_loc = txDrawParam->clut & 0x1f;
    }
    else {
        gSpriteCtx.loadedSprites[sprslot].txSpec.value &= ~0xff00U;
        gSpriteCtx.loadedSprites[sprslot].txSpec.fields.clut_loc = txSpecs & 0x1f;
        gSpriteCtx.loadedSprites[sprslot].txSpec.fields.semitrans = (txSpecs >> 7) & 0x1;
        gSpriteCtx.loadedSprites[sprslot].txSpec.fields.tpage_abr = (txSpecs & 0x60) >> 6;
    }

    gSpriteCtx.loadedSprites[sprslot].x_bound = bound_right;
    gSpriteCtx.loadedSprites[sprslot].y_bound = bound_down;
    for (i = 0; i < 2; i++) {
        Gpu_SetSemiTrans(&gSpriteCtx.loadedSprites[sprslot].drawPolys[i], gSpriteCtx.loadedSprites[sprslot].txSpec.fields.semitrans);
        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].r0 = 0x80;
        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].g0 = 0x80;
        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].b0 = 0x80;

        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].u0 = txDrawParam->u_base;
        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].v0 = txDrawParam->v_base;
        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].u1 = txDrawParam->u_base + txDrawParam->width;
        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].v1 = txDrawParam->v_base;
        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].u2 = txDrawParam->u_base;
        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].v2 = txDrawParam->v_base + txDrawParam->height;
        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].u3 = txDrawParam->u_base + txDrawParam->width;
        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].v3 = txDrawParam->v_base + txDrawParam->height;

        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].x0 = bound_right - txDrawParam->x_base;
        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].y0 = bound_down - txDrawParam->y_base;
        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].x1 = (s16)txDrawParam->width + (bound_right - txDrawParam->x_base);
        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].y1 = bound_down - txDrawParam->y_base;
        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].x2 = bound_right - txDrawParam->x_base;
        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].y2 = (s16)txDrawParam->height + (bound_down - txDrawParam->y_base);
        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].x3 = (s16)txDrawParam->width + (bound_right - txDrawParam->x_base);
        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].y3 = (s16)txDrawParam->height + (bound_down - txDrawParam->y_base);

        setClut(&gSpriteCtx.loadedSprites[sprslot].drawPolys[i], gSpriteCtx.loadedSprites[sprslot].txSpec.fields.clut_loc & 0xf, (gSpriteCtx.loadedSprites[sprslot].txSpec.fields.clut_loc >> 4) + 484);

        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].tpage =
            (gSpriteCtx.loadedSprites[sprslot].txSpec.fields.tpage_abr << 6) | (txDrawParam->t_page_xy & 0x10) | (txDrawParam->t_page_xy & 0xf);
    }
    return sprslot;
}

//800573EC
u8 GfxDraw_renderSpritePolys_573ec(u8 sprslot, unktype_4 arg1, byte txSlotGroup, byte txSlotOff, byte txSpecs, s16 bound_right, s16 bound_down) {

    int i = 0;
    uint8_t u_base_adj;
    SpriteTxDrawParam* txDrawParam;

    if (sprslot == 0xff) {
        for (i = 0; i < 0x40; i++) {
            if (!(gSpriteCtx.loadedSprites[i].txSpec.fields.inUse)) break;
        }
        sprslot = (u8)i;
        if (i == 0x40) return 0xff;
    }

    gSpriteCtx.loadedSprites[sprslot].txSpec.value &= 0x00c0;
    gSpriteCtx.loadedSprites[sprslot].txSpec.fields.unk_0 = gSpriteCtx.unk_101e04[arg1].unk_00 & 0x7;
    gSpriteCtx.loadedSprites[sprslot].txSpec.fields.unk_3 = arg1 & 0x7;
    gSpriteCtx.loadedSprites[sprslot].txSpec.fields.semitrans = (txSpecs >> 7) & 0x1;
    gSpriteCtx.loadedSprites[sprslot].txSpec.fields.tpage_abr = (txSpecs & 0x60) >> 5;
    gSpriteCtx.loadedSprites[sprslot].txSpec.fields.clut_loc = txSpecs & 0x1f;

    gSpriteCtx.loadedSprites[sprslot].txSlotGroup = txSlotGroup;
    gSpriteCtx.loadedSprites[sprslot].txSlotMember = txSlotOff;

    txDrawParam = &gDrawingCtx->spriteTxs[gDrawingCtx->txGroupSlots[txSlotGroup] + txSlotOff];

    gSpriteCtx.loadedSprites[sprslot].x_bound = bound_right;
    gSpriteCtx.loadedSprites[sprslot].y_bound = bound_down;

    for (i = 0; i < 2; i++) {
        Gpu_SetSemiTrans(&gSpriteCtx.loadedSprites[sprslot].drawPolys[i], gSpriteCtx.loadedSprites[sprslot].txSpec.fields.semitrans);
        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].r0 = 0x80;
        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].g0 = 0x80;
        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].b0 = 0x80;

        if (txSlotOff == 0x81) {
            u_base_adj = txDrawParam->u_base >> 1;
        }
        else {
            u_base_adj = txDrawParam->u_base;
        }

        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].u0 = u_base_adj;
        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].v0 = txDrawParam->v_base;
        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].u1 = u_base_adj + txDrawParam->width;
        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].v1 = txDrawParam->v_base;
        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].u2 = u_base_adj;
        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].v2 = txDrawParam->v_base + txDrawParam->height;
        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].u3 = u_base_adj + txDrawParam->width;
        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].v3 = txDrawParam->v_base + txDrawParam->height;

        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].x0 = bound_right - txDrawParam->x_base;
        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].y0 = bound_down - txDrawParam->y_base;
        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].x1 = (s16)txDrawParam->width + (bound_right - txDrawParam->x_base);
        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].y1 = bound_down - txDrawParam->y_base;
        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].x2 = bound_right - txDrawParam->x_base;
        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].y2 = (s16)txDrawParam->height + (bound_down - txDrawParam->y_base);
        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].x3 = (s16)txDrawParam->width + (bound_right - txDrawParam->x_base);
        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].y3 = (s16)txDrawParam->height + (bound_down - txDrawParam->y_base);

        setClut(&gSpriteCtx.loadedSprites[sprslot].drawPolys[i], 0, gSpriteCtx.loadedSprites[sprslot].txSpec.fields.clut_loc | 0x1e0);

        gSpriteCtx.loadedSprites[sprslot].drawPolys[i].tpage =
            (gSpriteCtx.loadedSprites[sprslot].txSpec.fields.tpage_abr << 6) | (txDrawParam->t_page_xy & 0x10) | (txDrawParam->t_page_xy & 0xf) | 0x80;

    }
    return sprslot;
}

//8005B054
void func_8005B054(u16 baseSlot, u16* arg1, u16 len) {

    u16 slot = baseSlot;

    int i = 0;
    int j = 0;
    int k = 0;
    int pos = 1; //iVar9
    u16 temp16_0 = 0; //uVar2
    
    for (i = 0; i < len; i++) {
        arg1[i] = 0xffff;
    }
    if (!(gSpriteCtx.unk_1012c4[slot].unk_034 & 0x100)) {
        return;
    }

    i = 0;
    arg1[0] = slot;
    while (i++ != pos) {
        if (gSpriteCtx.unk_1012c4[slot].unk_034 & 0x100) {
            for (j = 0; j < 4; j++) {

                if (gSpriteCtx.unk_1012c4[slot].unk_00[j].unk_004.value != 0xffff) {
                    k = 0;
                    temp16_0 = gSpriteCtx.unk_1012c4[slot].unk_00[j].unk_004.bytes.hi;
                    if ((pos > 0) && (arg1[0] != temp16_0)) {
                        for (k = 1; k < pos; k++) {
                            if (arg1[k] == temp16_0) break;
                        }
                    }
                    if (k == pos) {
                        arg1[pos] = temp16_0;
                        if (++pos >= len) return;
                    }
                }

                temp16_0 = gSpriteCtx.unk_1012c4[slot].unk_00[j].unk_006 >> 8;
                if (gSpriteCtx.unk_1012c4[slot].unk_00[j].unk_006 != 0xffff) {
                    k = 0;
                    if ((pos > 0) && (arg1[0] != temp16_0)) {
                        for (k = 1; k < pos; k++) {
                            if (arg1[k] == temp16_0) break;
                        }
                    }
                    if (k == pos) {
                        arg1[pos] = temp16_0;
                        if (++pos >= len) return;
                    }
                }
            }
        }
        slot = arg1[i];
    }
}

//8005B88C
void func_8005B88C(char arg0) {

    int i = 0;
    RECT local_88;
    u16 local_80[0x30];
    u16 temp16_0;

    if (arg0 != -1) {
        func_8005B054(arg0, local_80, 0x30);
        for (i = 0; i < 0x30; i++) {
            if (local_80[i] == 0xffff) break;
            temp16_0 = (local_80[i] & 0xf0) >> 3;
            gSpriteCtx.unk_1012c4[local_80[i]].unk_034 |= 0x3000;
            local_88.x = gSpriteCtx.unk_103bc4[temp16_0] + ((local_80[i] & 0xc) * 4);
            local_88.y = gSpriteCtx.unk_103bc4[temp16_0] + ((local_80[i] & 0x3) * 0x40);
            local_88.w = 0x10;
            local_88.h = 0x40;
            GpuSys_LoadImage(&local_88, gSpriteCtx.unk_1012c4[local_80[i]].unk_038);
        }
    }
}

//8005BC00
u8* func_8005BC00(u8 arg0, u8 arg1, u16 arg2, u16 arg3, u8* arg4) {

    int repeat = TRUE;

    //Draw data
    u16 local_40;
    u8 local_3e;
    u8 local_3d;
    u8 local_3c;
    u8 local_3b;
    u8 local_3a;
    u8 local_39;

    RECT local_38;

    //This is almost certainly a struct of some kind
    UnkStruct_gfx5c3ec local_68;

    u8* ptrStack[8];

    u32 index0 = arg0;
    u32 index1 = 0;

    u16 temp16_0 = arg2;
    u16 temp16_1 = arg3;
    u16 temp16_2 = 0; //uVar5

    u32 temp32_0 = 0; //uVar1
    u32 temp32_1 = 0; //uVar4

    u8* ptr0 = arg4;
    u8* ptr1 = NULL; //pbVar3

    if (!(arg1 & 0x20)) {
        if (gSpriteCtx.unk_1012c4[index0].unk_00[0].unk_00a <= temp16_0) {
            do {
                temp16_0 -= gSpriteCtx.unk_1012c4[index0].unk_00[index1].unk_00a;
                if (gSpriteCtx.unk_1012c4[index0].unk_00[index1].unk_004.value == 0xffff) {
                    return arg4;
                }
                index0 = (u32)gSpriteCtx.unk_1012c4[index0].unk_00[index1].unk_004.bytes.hi;
                index1 = (u32)gSpriteCtx.unk_1012c4[index0].unk_00[index1].unk_004.bytes.lo;
            } while (gSpriteCtx.unk_1012c4[index0].unk_00[index1].unk_00a <= temp16_0);
        }
        if (gSpriteCtx.unk_1012c4[index0].unk_00[index1].unk_00b <= temp16_1) {
            do {
                temp16_1 -= gSpriteCtx.unk_1012c4[index0].unk_00[index1].unk_00b;
                if (gSpriteCtx.unk_1012c4[index0].unk_00[index1].unk_006 == 0xffff) {
                    return arg4;
                }
                index0 = (u32)(gSpriteCtx.unk_1012c4[index0].unk_00[index1].unk_006) >> 8;
                index1 = (u32)(gSpriteCtx.unk_1012c4[index0].unk_00[index1].unk_006) & 0xff;
            } while (gSpriteCtx.unk_1012c4[index0].unk_00[index1].unk_00b <= temp16_1);
        }
        local_68.unk_05 = (u8)index0;
        local_68.unk_06 = (u8)index1;
        local_68.unk_00 = (s16)temp16_0;
        local_68.unk_02 = (s16)temp16_1;
    }
    else {
        temp16_0 = (u32)gSpriteCtx.unk_1073f2;
        temp16_1 = (u32)gSpriteCtx.unk_1073f4;
        index0 = (u32)gSpriteCtx.unk_1073f8;
        index1 = (u32)gSpriteCtx.unk_1073f9;
        local_68.unk_00 = (s16)gSpriteCtx.unk_1073ee;
        local_68.unk_02 = (s16)gSpriteCtx.unk_1073f0;
        local_68.unk_05 = (u8)gSpriteCtx.unk_1073f6;
        local_68.unk_06 = (u8)gSpriteCtx.unk_1073f7;
    }

    local_68.unk_07 = 0;
    local_68.unk_04 = arg1;
    
    //switchD_8005be6c_caseD_5:
    while (repeat) {
        while (temp16_2 <= 0xef) {
            ptr1 = ptr0;
            temp16_2 = *ptr0++;
            while (((temp16_2 + 0x10) & 0xff) <= 10) {
                func_8005C3EC(&local_68, (temp16_2 << 8) + ((u16)*ptr0++ + 0x1100));
                ptr1 = ptr0;
                temp16_2 = *ptr0++;
            }
            if (temp16_2 > 0xef) break;
            func_8005C3EC(&local_68, temp16_2);
        }

        switch (temp16_2) {
        case 0xfb:
            local_68.unk_04 ^= 0x40;
            continue; //goto switchD_8005be6c_caseD_5

        case 0xfc:
            temp32_0 = (u32)*ptr0;
            temp32_1 = (u32)ptr1[2];
            ptr0 = ptr1 + 3;
            if (temp32_0 == 0xff) {
                temp32_0 = func_8003AE90();
            }
            if (temp32_1 == 0xff) {
                temp32_1 = func_8003AE90();
            }
            if (temp32_1 < 12) {
                if (local_68.unk_07 < 8) {
                    ptrStack[local_68.unk_07] = ptr0;
                    ptr1 = (u8*)roUnk_1e774 + (temp32_1 << 2) + (temp32_0 << 2) + (int)roUnk_1e774[temp32_1];
                    ptr0 = (u8*)roUnk_1e774 + (temp32_1 << 2) + (temp32_0 << 2) + (int)roUnk_1e774[temp32_1] + *((u32*)ptr1);
                    local_68.unk_07++;
                }
                continue; //goto switchD_8005be6c_caseD_5
            }
            if (temp32_1 < 18) {
                if (local_68.unk_07 < 8) {
                    ptrStack[local_68.unk_07] = ptr0;
                    ptr0 = &sGlobalCtx.heap.unk_111600[sGlobalCtx.heap.unk_111400[(temp32_1 & 1 << 8 | temp32_0) << 1]];
                    local_68.unk_07++;
                }
                continue;
            }
            if (temp32_1 < 32) {
                if (!(temp32_0 & 0x1)) {
                    local_68.unk_04 = (local_68.unk_04 & 0xf0) | (u8)(temp32_0 & 0xe);
                    gSpriteCtx.unk_1012c0 &= 0xffff0fff;
                    gSpriteCtx.unk_1012c0 |= (temp32_0 & 0xe) << 12;
                }
                else {
                    local_40 = 0;
                    local_3e = ptr0[0];
                    local_3d = ptr1[4];
                    local_3c = ptr1[5];
                    local_3b = ptr1[6];
                    local_3a = ptr0[0];
                    local_39 = ptr1[4];
                    local_38.x = (u16)temp32_0 & 0xfe | 0x100;
                    local_38.y = 0x1e0;
                    local_38.w = 4;
                    local_38.h = 1;
                    GpuSys_LoadImage(&local_38, &local_40);
                    GpuSys_DrawSync(0);
                    ptr0 = ptr1 + 7;
                }
                continue;
            }
            if ((s32)(temp32_1 << 24) > -1) {
                gSpriteCtx.unk_103bd2 = local_68.unk_00 + gSpriteCtx.unk_1012c4[local_68.unk_05].unk_030 + gSpriteCtx.unk_1012c4[local_68.unk_05].unk_00[local_68.unk_06].unk_000;
                gSpriteCtx.unk_103bd4 = local_68.unk_02 + gSpriteCtx.unk_1012c4[local_68.unk_05].unk_032 + gSpriteCtx.unk_1012c4[local_68.unk_05].unk_00[local_68.unk_06].unk_000;

                gSpriteCtx.unk_1073ee = local_68.unk_00;
                gSpriteCtx.unk_1073f0 = local_68.unk_02;
                gSpriteCtx.unk_1073f2 = temp16_0;
                gSpriteCtx.unk_1073f4 = temp16_1;
                gSpriteCtx.unk_1073f6 = local_68.unk_05;
                gSpriteCtx.unk_1073f7 = local_68.unk_06;
                gSpriteCtx.unk_1073f8 = (u8)index0;
                gSpriteCtx.unk_1073f9 = (u8)index1;
                return ++ptr1;
            }
            index1 = 0;
            temp16_0 = (temp32_0 & 0x3f) << 3;
            temp16_1 = (temp32_1 & 0x3f) << 3;
            index0 = arg0;
            while (gSpriteCtx.unk_1012c4[index0].unk_00[index1].unk_00a <= temp16_0) {
                temp16_2 = gSpriteCtx.unk_1012c4[index0].unk_00[index1].unk_004.value;
                temp16_0 -= gSpriteCtx.unk_1012c4[index0].unk_00[index1].unk_00a;
                if (temp16_2 == 0xffff) return ptr0;
                index0 = (temp16_2 >> 8);
                index1 = (temp16_2 & 0xff);
            }

            if (gSpriteCtx.unk_1012c4[index0].unk_00[index1].unk_00b > temp16_1) {
                local_68.unk_05 = (u8)index0;
            }
            else {
                while (gSpriteCtx.unk_1012c4[index0].unk_00[index1].unk_00b <= temp16_1) {
                    temp16_2 = gSpriteCtx.unk_1012c4[index0].unk_00[index1].unk_006;
                    temp16_1 -= gSpriteCtx.unk_1012c4[index0].unk_00[index1].unk_00b;
                    if (temp16_2 == 0xffff) return ptr0;
                    index0 = (temp16_2 >> 8);
                    index1 = (temp16_2 & 0xff);
                }
                local_68.unk_05 = (u8)index0;
            }
            break;

        case 0xfd:
            temp32_0 = temp16_1 + 8;
            if(!(local_68.unk_04 & 0x40)) temp32_0 = temp16_1 + 0x10;
            temp16_1 = (u16)temp32_0;
            if (gSpriteCtx.unk_1012c4[index0].unk_00[index1].unk_00b <= temp16_1) {
                temp16_2 = gSpriteCtx.unk_1012c4[index0].unk_00[index1].unk_006;
                temp16_1 -= gSpriteCtx.unk_1012c4[index0].unk_00[index1].unk_00b;
                if (temp16_2 == 0xffff) return ptr0;
                index1 = (temp16_2 & 0xff);
            }
            local_68.unk_05 = (u8)index0;
            break;
        case 0xfe:
            repeat = FALSE;
            break;
        case 0xff:
            if (local_68.unk_07 == 0) { repeat = FALSE; break; }
            ptr0 = ptrStack[--local_68.unk_07];
            continue;
        default:
            continue;
        }

        if (repeat) {
            local_68.unk_06 = (u8)index1;
            local_68.unk_00 = temp16_0;
            local_68.unk_02 = temp16_1;
        }
    } //goto switchD_8005be6c_caseD_5:

    gSpriteCtx.unk_103bd2 = local_68.unk_00 + gSpriteCtx.unk_1012c4[local_68.unk_05].unk_030 + gSpriteCtx.unk_1012c4[local_68.unk_05].unk_00[local_68.unk_06].unk_000;
    gSpriteCtx.unk_103bd4 = local_68.unk_02 + gSpriteCtx.unk_1012c4[local_68.unk_05].unk_032 + gSpriteCtx.unk_1012c4[local_68.unk_05].unk_00[local_68.unk_06].unk_000;

    return ptr0;
}

//8005C3EC
void func_8005C3EC(UnkStruct_gfx5c3ec* arg0, u16 arg1) {

    int i = 0;
    int j = 0;

    u32 index0 = (u32)arg0->unk_05;
    u32 index1 = (u32)arg0->unk_06;
    u32 index2 = 0;
    u32 index3 = 0;

    UnkStruct_1012c8* strptr = &gSpriteCtx.unk_1012c4[index0].unk_00[index1];

    u8* pos_a = NULL;
    u8* pos_b = NULL;
    u8* ptr_a = gSpriteCtx.unk_1012c4[index0].unk_038 + ((arg0->unk_00 + strptr->unk_008) >> 1) + ((arg0->unk_02 + strptr->unk_009) << 5);
    u8* ptr_b = ptr_a;

    u8* base_a = ptr_a; //local_34
    u8* base_b = ptr_a; //local_30

    u8 addend = ((arg0->unk_04 & 0xe) << 4) | (arg0->unk_04 & 0xe);
    u16 sz = strptr->unk_00a - arg0->unk_00;
    u16 cap = sz >> 1;

    u8 idxn_a = 0;
    u8 idxn_b = 0;
    u8 wval[6];

    s32 ctr = gSpriteCtx.unk_1012c4[index0].unk_00[index1].unk_00b - arg0->unk_02;

    if (strptr->unk_004.value != 0xffff) {
        strptr = &gSpriteCtx.unk_1012c4[strptr->unk_004.bytes.hi].unk_00[strptr->unk_004.bytes.lo];
        ptr_b = gSpriteCtx.unk_1012c4[strptr->unk_004.bytes.hi].unk_038 + (strptr->unk_008 >> 1) + ((arg0->unk_02 + strptr->unk_009) << 5);
        strptr = &gSpriteCtx.unk_1012c4[index0].unk_00[index1];
    }

    if (strptr->unk_006 != 0xffff) {
        strptr = &gSpriteCtx.unk_1012c4[strptr->unk_004.bytes.hi].unk_00[strptr->unk_008];
        base_a = gSpriteCtx.unk_1012c4[strptr->unk_004.bytes.hi].unk_038 + ((arg0->unk_00 + strptr->unk_008) >> 1) + (strptr->unk_009 << 5);
        if (strptr->unk_004.value != 0xffff) {
            index0 = strptr->unk_004.bytes.hi;
            strptr = &gSpriteCtx.unk_1012c4[strptr->unk_004.bytes.hi].unk_00[strptr->unk_004.bytes.lo];
            base_b = gSpriteCtx.unk_1012c4[strptr->unk_004.bytes.hi].unk_038 + (strptr->unk_008 >> 1) + (strptr->unk_009 << 5);
        }
    }

    index0 = 0;
    index1 = 0;
    index2 = 0;
    if (!(arg0->unk_04 & 0x40)) {
        if (arg1 > 0x1ff) {
            for (i = 0; i < 16; i++) {
                pos_a = ptr_a;
                pos_b = ptr_b;

                idxn_a = sUnk_ed5b4.unk_eddb4[arg1 - 0x200][i][0];
                idxn_b = sUnk_ed5b4.unk_eddb4[arg1 - 0x200][i][1];

                wval[0] = sGlobalData.unktbl_fa754[2][index0] + sGlobalData.unktbl_fa754[0][idxn_a >> 4] + addend;
                wval[1] = sGlobalData.unktbl_fa754[3][index0] + sGlobalData.unktbl_fa754[1][idxn_a >> 4] + addend;
                wval[2] = sGlobalData.unktbl_fa754[4][index0] + sGlobalData.unktbl_fa754[2][index1] + sGlobalData.unktbl_fa754[0][idxn_a & 0xf] + addend;
                wval[3] = sGlobalData.unktbl_fa754[3][index1] + sGlobalData.unktbl_fa754[1][idxn_a & 0xf] + addend;
                wval[4] = sGlobalData.unktbl_fa754[4][index1] + sGlobalData.unktbl_fa754[2][index2] + sGlobalData.unktbl_fa754[0][idxn_b >> 4] + addend;
                wval[5] = sGlobalData.unktbl_fa754[3][index2] + sGlobalData.unktbl_fa754[1][idxn_b >> 4] + addend;

                for (j = 0; j < 6; j++) {
                    if (j < cap) { *pos_a++ = wval[j]; }
                    else { *pos_b++ = wval[j]; }
                }

                if (--ctr != 0) {
                    ptr_a += 0x20;
                    ptr_b += 0x20;
                }
                else {
                    ptr_a = base_a;
                    ptr_b = base_b;
                }

                if (arg0->unk_04 & 0x80) {
                    index0 = idxn_a >> 4;
                    index1 = idxn_a & 0xf;
                    index2 = idxn_b >> 4;
                }
            }
        }
        else { //arg1 < 0x1ff
            for (i = 0; i < 16; i++) {
                pos_a = ptr_a;
                pos_b = ptr_b;

                idxn_a = sUnk_ed5b4.unk_ed5b4[(arg1 << 4) + i];

                wval[0] = sGlobalData.unktbl_fa754[2][index0] + sGlobalData.unktbl_fa754[0][idxn_a >> 4] + addend;
                wval[1] = sGlobalData.unktbl_fa754[3][index0] + sGlobalData.unktbl_fa754[1][idxn_a >> 4] + addend;
                wval[2] = sGlobalData.unktbl_fa754[4][index0] + sGlobalData.unktbl_fa754[2][index1] + sGlobalData.unktbl_fa754[0][idxn_a & 0xf] + addend;
                wval[3] = sGlobalData.unktbl_fa754[3][index1] + sGlobalData.unktbl_fa754[1][idxn_a & 0xf] + addend;

                for (j = 0; j < 4; j++) {
                    if (j < cap) { *pos_a++ = wval[j]; }
                    else { *pos_b++ = wval[j]; }
                }

                if (--ctr != 0) {
                    ptr_a += 0x20;
                    ptr_b += 0x20;
                }
                else {
                    ptr_a = base_a;
                    ptr_b = base_b;
                }

                if (arg0->unk_04 & 0x80) {
                    index0 = idxn_a >> 4;
                    index1 = idxn_a & 0xf;
                }
            }
        }
    }
    else { //0x40 flag is set
        for (i = 0; i < 8; i++) {
            pos_a = ptr_a;
            pos_b = ptr_b;

            idxn_a = sUnk_ed5b4.unk_ed5b4[(((arg1 - 0x20) & 0x7f) << 3) + i];

            wval[0] = sGlobalData.unktbl_fa754[2][index0] + sGlobalData.unktbl_fa754[0][idxn_a >> 4] + addend;
            wval[1] = sGlobalData.unktbl_fa754[3][index0] + sGlobalData.unktbl_fa754[1][idxn_a >> 4] + addend;
            wval[2] = sGlobalData.unktbl_fa754[4][index0] + sGlobalData.unktbl_fa754[2][index1] + sGlobalData.unktbl_fa754[0][idxn_a & 0xf] + addend;
            wval[3] = sGlobalData.unktbl_fa754[3][index1] + sGlobalData.unktbl_fa754[1][idxn_a & 0xf] + addend;

            for (j = 0; j < 4; j++) {
                if (j < cap) { *pos_a++ = wval[j]; }
                else { *pos_b++ = wval[j]; }
            }

            if (--ctr != 0) {
                ptr_a += 0x20;
                ptr_b += 0x20;
            }
            else {
                ptr_a = base_a;
                ptr_b = base_b;
            }

            if (arg0->unk_04 & 0x80) {
                index0 = idxn_a >> 4;
                index1 = idxn_a & 0xf;
            }
        }
    }

    arg0->unk_00 += 8;
    index0 = (u32)arg0->unk_05;
    index1 = (u32)arg0->unk_06;
    if (gSpriteCtx.unk_1012c4[index0].unk_00[index1].unk_00a <= arg0->unk_00) {
        arg0->unk_00 -= gSpriteCtx.unk_1012c4[index0].unk_00[index1].unk_00a;
        if (gSpriteCtx.unk_1012c4[index0].unk_00[index1].unk_004.value != 0xffff) {
            arg0->unk_05 = gSpriteCtx.unk_1012c4[index0].unk_00[index1].unk_004.bytes.hi;
            arg0->unk_06 = gSpriteCtx.unk_1012c4[index0].unk_00[index1].unk_004.bytes.lo;
        }
    }
}

//8005DF54
void GfxDraw_renderSpritePolys_5df54(u8 sprslot, byte arg1, byte txSlotOff, byte txSpecs, s16 bound_right, s16 bound_down) {
    GfxDraw_renderSpritePolys_56fbc(sprslot, arg1, 0, txSlotOff, txSpecs, bound_right, bound_down);
}

//8005DF98
void GfxDraw_renderSpritePolys_5df98(u8 sprslot, byte arg1, byte txSlotOff, byte txSpecs, s16 bound_right, s16 bound_down) {
    GfxDraw_renderSpritePolys_573ec(sprslot, arg1, 0, txSlotOff, txSpecs, bound_right, bound_down);
}

//8005E0D4
void func_8005E0D4(u32 arg0, u16 arg1) {

    int index0 = (int)arg0 & 0xf;
    u32 temp32_0 = ((u32)arg1 >> 8) & 0x7;
    u32 temp32_2 = (u32)arg1 & 0xff;
    u32* tempptr_0 = (u32*)((u8*)&roUnk_1e774[temp32_0] + roUnk_1e774[temp32_0]); //ie. Value in table points to how far below the entry data of interest are.
    u32* tempptr_2 = &tempptr_0[temp32_2]; //This in turn is another table.


    func_8005BC00(gSpriteCtx.unk_103bb4[index0], 0x80, 0, 0, (byte*)(&tempptr_0[*tempptr_2 + temp32_2]));
    func_8005B88C(gSpriteCtx.unk_103bb4[index0]);
}