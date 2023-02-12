#include "global.h"

unktype_4 func_800B74B8() {

    const u8 local_50[] = { 0x00, 0x08, 0x3c };
    const u8 local_48[] = { 0x08, 0x38, 0x08 };

    byte* dataReq = sDataLoadCtx.dataReqBase + sDataLoadCtx.dataReqOffset;
    int temp32_0;
    u32 temp32_1;
    int i = 0;
    int j = 0;
    POLY_FT4* poly;
    POLY_G4* poly_g;
    SpriteTxDrawParam* txDrawParam;

    //temp32_0 = (int)sUnk_1012c0.unk_107afe * 30;
    switch (sDataLoadCtx.reqStatus) {
    case 0:
        func_8003ADB8();
        sUnk_gp.unk_feb20 = 0;
        if (dataReq[1] & 0x80) {
            sUnk_gp.unk_feb20 = 0;
            sDataLoadCtx.reqStatus = 4;
            return DATA_LOAD_FLAG_UNK_80;
        }
        sUnk_gp.loadDataBufferB = Heap_ArenaAlloc(0x10000);
        if (sUnk_ff520.unk_ff534[gSpriteCtx.unk_107afe].unk_01 == 7) {
            CdFiles_requestCdFile(FILE_MODE_FLAG_INDEX_OFFSET | FILE_MODE_FLAG_LZ77, D_Face, 1, 0, -1, sUnk_gp.loadDataBufferB);
        }
        else {
            CdFiles_requestCdFile(FILE_MODE_FLAG_INDEX_OFFSET | FILE_MODE_FLAG_LZ77, D_Face, (u32)sUnk_ff520.unk_ff534[gSpriteCtx.unk_107afe].unk_01, 0, -1, sUnk_gp.loadDataBufferB);
            //ff534 might be array of structs of size 30?
        }
        break;
    case 1:
        if (!CdFiles_AllSlotsEmpty()) return DATA_LOAD_FLAG_UNK_80;
        GfxDraw_loadSprite8Bit_Y256(sUnk_gp.loadDataBufferB, 0, gSpriteCtx.unk_1012c0 >> 0x16 & 0x1);
        break;
    case 2:
        Heap_ReserveFreeMemory(sUnk_gp.loadDataBufferB, 0x10000);
        sUnk_gp.loadDataBufferB = Heap_ArenaAlloc(0x18000);
        CdFiles_requestCdFile(FILE_MODE_FLAG_INDEX_OFFSET | FILE_MODE_FLAG_LZ77, D_System, Sys_UI_Battlers, 0, -1, sUnk_gp.loadDataBufferB);
        break;
    case 3:
        if (!CdFiles_AllSlotsEmpty()) return DATA_LOAD_FLAG_UNK_80;
        if (sUnk_ff520.unk_ff534[gSpriteCtx.unk_107afe].unk_01 == 7) {
            GfxDraw_loadSprite4Bit(sUnk_gp.loadDataBufferB, 1, 0, 0xa1, 0x45);
        }
        else {
            GfxDraw_loadSprite4Bit(sUnk_gp.loadDataBufferB, (u32)sUnk_ff520.unk_ff534[gSpriteCtx.unk_107afe].unk_00 << 1 + 1, 0, 0xa1, 0x45);
        }
        sUnk_gp.unk_feb20 = 1;
        Heap_ReserveFreeMemory(sUnk_gp.loadDataBufferB, 0x18000);
        if (dataReq[1] & 0x80) {
            temp32_1 = gSpriteCtx.unk_1012c0 >> 0x16 & 0x1;
            GfxDraw_renderSpritePolys_5df98(0x30, dataReq[0], temp32_1 + 0x80 & 0xff, temp32_1 | 0x6, 8, 0x32);
            GfxDraw_renderSpritePolys_5df54(0x31, dataReq[0], 0xa1, 5, 0xc, 0x22);
            sDataLoadCtx.dataReqOffset += 2;
            gSpriteCtx.unk_1012c0 = gSpriteCtx.unk_1012c0 & ~0x400000 | (gSpriteCtx.unk_1012c0 >> 0x16 & 1 ^ 1) << 0x16;
            return DATA_LOAD_FLAG_UNK_80 | DATA_LOAD_FLAG_UNK_01;
        }
        sDataLoadCtx.reqStatus++;
        //No break?
    case 4:
        func_8003A788(sUnk_ff520.unk_ff534[gSpriteCtx.unk_107afe].unk_06, 3);
        func_8003A788(sUnk_ff520.unk_ff534[gSpriteCtx.unk_107afe].unk_04, 3);
        for (i = 0; i < 2; i++) {
            for (j = 0; j < 3; j++) {
                poly = &(gSpriteCtx.unk_107b00[dataReq[0]].unk_004[i][j]);
                txDrawParam = &gDrawingCtx->spriteTxs[gDrawingCtx->txGroupSlots[0] + 0x37 + j];

                Gpu_SetPolyFT4(poly);
                Gpu_SetSemiTrans(poly, 0);

                poly->r0 = 0x80; poly->g0 = 0x80; poly->b0 = 0x80;
                poly->u0 = txDrawParam->u_base;
                poly->v0 = txDrawParam->v_base;
                poly->u1 = txDrawParam->u_base + txDrawParam->width;
                poly->v1 = txDrawParam->v_base;
                poly->u2 = txDrawParam->u_base;
                poly->v2 = txDrawParam->v_base + txDrawParam->height;
                poly->u3 = txDrawParam->u_base + txDrawParam->width;
                poly->v3 = txDrawParam->v_base + txDrawParam->height;

                poly->x0 = local_50[j] + 0x40;
                poly->y0 = 0x43;
                poly->x1 = local_50[j] + local_48[j] + 0x40;
                poly->y1 = 0x43;
                poly->x2 = local_50[j] + 0x40;
                poly->y2 = txDrawParam->height + 0x43;
                poly->x2 = local_50[j] + local_48[j]  + 0x40;
                poly->y2 = txDrawParam->height + 0x43;

                poly->clut = (txDrawParam->clut & 0x3f) | 0x7900;
                poly->tpage = txDrawParam->t_page_xy & 0x1f;
            }
            poly_g = &(gSpriteCtx.unk_107b00[dataReq[0]].unk_0f4[i]);
            Gpu_SetPolyG4(poly_g);
            Gpu_SetSemiTrans(poly_g, 0);

            poly_g->r0 = 0xff; poly_g->g0 = 0x00; poly_g->b0 = 0x00;
            poly_g->r1 = 0xff; poly_g->g1 = 0xbf; poly_g->b1 = 0xef;
            poly_g->r2 = 0xff; poly_g->g2 = 0x00; poly_g->b2 = 0x00;
            poly_g->r3 = 0xff; poly_g->g3 = 0xbf; poly_g->b3 = 0xef;
            poly_g->x0 = 0x42; poly_g->y0 = 0x46;

            if (sUnk_ff520.unk_ff534[gSpriteCtx.unk_107afe].unk_04 == 0) {}; //trap(7)
            poly_g->x1 = ((sUnk_ff520.unk_ff534[gSpriteCtx.unk_107afe].unk_06 * 0x3f) / sUnk_ff520.unk_ff534[gSpriteCtx.unk_107afe].unk_04) + 0x42;
            poly_g->y1 = 0x46;
            poly_g->x2 = 0x42; poly_g->y2 = 0x49;

            if (sUnk_ff520.unk_ff534[gSpriteCtx.unk_107afe].unk_04 == 0) {}; //trap(7)
            poly_g->x2 = ((sUnk_ff520.unk_ff534[gSpriteCtx.unk_107afe].unk_06 * 0x3f) / sUnk_ff520.unk_ff534[gSpriteCtx.unk_107afe].unk_04) + 0x42;
            poly_g->y3 = 0x49;
        }
        gSpriteCtx.unk_107b00[dataReq[0]].unk_000 |= 0x3;
        GfxDraw_renderSpritePolys_5df54(0x28, dataReq[0], 0x40, 7, 0x55, 0x52);
        func_8003A788(sUnk_ff520.unk_ff534[gSpriteCtx.unk_107afe].unk_0e, 3);
        func_8005E0D4(3, 1);

        //TODO (a lot)
        break;
    default: return DATA_LOAD_FLAG_UNK_80;
    }

        do {
            uVar20 = 0;
            puVar18 = local_40 + uVar19 * 0x78;
            do {
                
            } while (uVar20 < 3);

        } while (uVar19 < 2);
        (&DAT_80107b00)[uVar21 * 0xf + local_30 * 0x10] =
            (&DAT_80107b00)[uVar21 * 0xf + local_30 * 0x10] | 3;
        func_8005DF54(0x28, uVar21, 0x40, 7, 0x55, 0x52);
        func_8003A788((&DAT_800ff542)[uVar13 * 0xf], 3);
        func_8005E0D4(3, 1);
        iVar10 = (int)*(char*)((int)&DAT_800ff534 + iVar12 + 2);
        if (iVar10 < 2) {
            func_8005DF54(0x29, uVar21, 0xc6, 8, 0x44, 0x60);
        }
        else {
            iVar14 = 10;
            if (iVar10 < 10) {
                iVar14 = iVar10;
            }
            func_8005DF54(0x29, uVar21, (&DAT_800fe92f)[(byte)(&DAT_80011c88)[iVar14]], 8, 0x44, 0x60);
        }
        iVar10 = (int)*(char*)((int)&DAT_800ff534 + iVar12 + 2);
        if (iVar10 < 0xc) {
            func_8005DF54(0x2a, uVar21, 0xc6, 8, 0x4c, 0x60);
        }
        else {
            iVar14 = 10;
            if (iVar10 < 0x14) {
                iVar14 = iVar10 + -10;
            }
            func_8005DF54(0x2a, uVar21, (&DAT_800fe92f)[(byte)(&DAT_80011c88)[iVar14]], 8, 0x4c, 0x60);
        }
        iVar10 = (int)*(char*)((int)&DAT_800ff534 + iVar12 + 2);
        if (iVar10 < 0x16) {
            func_8005DF54(0x2b, uVar21, 0xc6, 8, 0x54, 0x60);
        }
        else {
            iVar14 = 10;
            if (iVar10 < 0x1e) {
                iVar14 = iVar10 + -0x14;
            }
            func_8005DF54(0x2b, uVar21, (&DAT_800fe92f)[(byte)(&DAT_80011c88)[iVar14]], 8, 0x54, 0x60);
        }
        iVar10 = (int)*(char*)((int)&DAT_800ff534 + iVar12 + 2);
        if (iVar10 < 0x20) {
            func_8005DF54(0x2c, uVar21, 0xc6, 8, 0x5c, 0x60);
        }
        else {
            iVar14 = 10;
            if (iVar10 < 0x28) {
                iVar14 = iVar10 + -0x1e;
            }
            func_8005DF54(0x2c, uVar21, (&DAT_800fe92f)[(byte)(&DAT_80011c88)[iVar14]], 8, 0x5c, 0x60);
        }
        func_8005B2B8(DAT_80103bb8);
        uVar13 = 0;
        if ((local_2c & 0xff) == 0) {
            iVar10 = 0;
            do {
                psVar17 = (short*)((int)&DAT_800ff54c + iVar10 + iVar12);
                if (*psVar17 == 0) {
                    func_8003AE24(0x300);
                    func_80057834(uVar13 + 0x2d & 0xff, uVar21);
                }
                else {
                    func_8005DF54(uVar13 + 0x2d & 0xff, uVar21, 0x7b, 7, 0x18, uVar13 * 0x10 + 0x70);
                    func_8003AE24(*psVar17 + 0x300);
                }
                uVar13 = uVar13 + 1 & 0xff;
                iVar10 = uVar13 << 1;
            } while (uVar13 < 3);
        }
        else {
            iVar10 = 0;
            do {
                puVar16 = (ushort*)((int)&DAT_800ff546 + iVar10 + iVar12);
                uVar6 = *puVar16;
                if (uVar6 == 0) {
                    func_8003AE24(0x800);
                    func_80057834(uVar13 + 0x2d & 0xff, uVar21);
                }
                else {
                    func_8003AE24(uVar6 + 0x800);
                    if ((&DAT_800101a2)[(uint)*puVar16 * 4] == '\0') {
                        uVar15 = 0xa7;
                    }
                    else {
                        uVar15 = 0xb5;
                    }
                    func_8005DF54(uVar13 + 0x2d & 0xff, uVar21, uVar15, 7, 0x18, uVar13 * 0x10 + 0x70);
                }
                uVar13 = uVar13 + 1 & 0xff;
                iVar10 = uVar13 << 1;
            } while (uVar13 < 3);
        }
        func_8005E0D4(4, 2);
        if (DAT_800feb20 != '\0') {
            uVar13 = DAT_801012c0 >> 0x16 & 1;
            func_8005DF98(0x30, uVar21, uVar13 + 0x80 & 0xff, uVar13 | 6, 8, 0x32);
            func_8005DF54(0x31, uVar21, 0xa1, 5, 0xc, 0x22);
            DAT_801012c0 = DAT_801012c0 & 0xffbfffff | (DAT_801012c0 >> 0x16 & 1 ^ 1) << 0x16;
        }
        DAT_800ff500 = DAT_800ff500 + 2;
        return 0x81;
    default:
        goto switchD_800b7588_caseD_5;
    }
    DAT_800ff504 = DAT_800ff504 + 1;
switchD_800b7588_caseD_5:
    return 0x80;
}