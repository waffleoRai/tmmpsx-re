#include "global.h"

void func_8004893C() {

    int i, j;
    RECT rect;

    sUnk_gp.unk_fea98 = Heap_ArenaAlloc(0x6af4);
    sUnk_gp.unk_fea98->unk_280c = 0;
    for (i = 0; i < 0x20; i++) {
        sUnk_gp.unk_fea98->unk_280e[i] = 0;
        sUnk_gp.unk_fea98->unk_284e[i] = 0;
    }
    for (i = 0; i < 2; i++) {
        Gpu_SetPolyF4(&sUnk_gp.unk_fea98->unk_292c[i]);
        sUnk_gp.unk_fea98->unk_295c[i].unk_03 = 1;
        sUnk_gp.unk_fea98->unk_295c[i].unk_04 = 0xe1000200;
    }
    for (i = 0; i < 0x98; i++) {
        sUnk_gp.unk_fea98->unk_296c[i].unk_00 &= ~0x21;
        sUnk_gp.unk_fea98->unk_296c[i].unk_18 = ~0;
        for (j = 0; j < 2; j++) {
            Gpu_SetPolyFT4(&sUnk_gp.unk_fea98->unk_3a0c[i]);
            sUnk_gp.unk_fea98->unk_3a0c[i].r0 = 0x80;
            sUnk_gp.unk_fea98->unk_3a0c[i].g0 = 0x80;
            sUnk_gp.unk_fea98->unk_3a0c[i].b0 = 0x80;
            Gpu_SetPolyFT4(&sUnk_gp.unk_fea98->unk_51cc[i]);
            sUnk_gp.unk_fea98->unk_51cc[i].r0 = 0x80;
            sUnk_gp.unk_fea98->unk_51cc[i].g0 = 0x80;
            sUnk_gp.unk_fea98->unk_51cc[i].b0 = 0x80;
        }
    }

    sUnk_gp.unk_fea98->unk_6a24 = 0;
    sUnk_gp.unk_fea98->unk_28ae = 0;
    sUnk_gp.unk_fea98->unk_28b0 = 0;
    sUnk_gp.unk_fea98->unk_6aae = 0;
    sUnk_gp.unk_fea98->unk_6ab0 = 0;
    sUnk_100588.unk_100626 = 0;
    sUnk_100588.unk_100628 = 0;
    sUnk_100588.unk_10062e = 0x140;
    sUnk_100588.unk_100630 = 0xf0;
    sUnk_100588.unk_10062a = 0;
    sUnk_100588.unk_10062c = 0;
    
    rect.x = 0;
    rect.y = 0;
    rect.w = 0x140;
    rect.h = 0xf0;
    GpuSys_SetDrawArea(&sUnk_gp.unk_fea98->unk_6acc, &rect);
    GpuSys_SetDrawArea(&sUnk_gp.unk_fea98->unk_6ab4, &rect);

    rect.x = 0;
    rect.y = 0xf0;
    rect.w = 0x140;
    rect.h = 0xf0;
    GpuSys_SetDrawArea(&sUnk_gp.unk_fea98->unk_6ad8, &rect);
    GpuSys_SetDrawArea(&sUnk_gp.unk_fea98->unk_6ac0, &rect);

    sApplState.stateFlags &= ~0x20000000;
    sApplState.stateFlags |=  0x10000000;
}