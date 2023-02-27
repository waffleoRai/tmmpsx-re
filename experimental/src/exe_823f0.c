#include "global.h"

u32 func_800823F0() {
    if (sSceCtx.unk_ff504 == 1) {
        if (!CdFiles_AllSlotsEmpty()) return 0x80;
        func_8005E86C(sUnk_gp.loadDataBuffer);
    }
    else {
        if (sSceCtx.unk_ff504 > 1) {
            if (sSceCtx.unk_ff504 != 2) return 0x80;
            func_80071C5C(sUnk_gp.loadDataBuffer, 0x60000);
            func_80060C98();
            func_80071CF8(0x12);
            func_80071CF8(0x17);
            func_80071E1C(0xf0, 0x10, 0);
            sUnk_feb28.unk_feb28 |= 0x40000000;
            sUnk_ff520.unk_ff716 = 0;
            sUnk_ff520.unk_ff718 = 0;
            return 0x81;
        }
        if (sSceCtx.unk_ff504 != 0) return 0x80;
        func_8005E6D0();
        sUnk_gp.loadDataBuffer = func_800714A4(0x60000);
        CdFiles_requestCdFile(FILE_MODE_FLAG_INDEX_OFFSET | FILE_MODE_FLAG_LZ77, D_Field, sUnk_ff520.unk_ff708, 0, -1, sUnk_gp.loadDataBuffer);
    }
    sSceCtx.unk_ff504++;
    return 0x80;
}

u32 func_80082538() {
    if (sSceCtx.unk_ff504 == 1) {
        if (!CdFiles_AllSlotsEmpty()) return 0x80;
        func_8005E86C(sUnk_gp.loadDataBuffer);
    }
    else {
        if (sSceCtx.unk_ff504 > 1) {
            if (sSceCtx.unk_ff504 != 2) return 0x80;
            func_80071C5C(sUnk_gp.loadDataBuffer, 0x60000);
            func_80061038();
            func_80071CF8(0x12);
            func_80071CF8(0x17);
            func_80071E1C(0xf0, 0x10, 0);
            sUnk_feb28.unk_feb28 |= 0x40000000;
            sUnk_ff520.unk_ff716 = 0;
            sUnk_ff520.unk_ff718 = 0;
            return 0x81;
        }
        if (sSceCtx.unk_ff504 != 0) return 0x80;
        sUnk_gp.loadDataBuffer = func_800714A4(0x60000);
        CdFiles_requestCdFile(FILE_MODE_FLAG_INDEX_OFFSET | FILE_MODE_FLAG_LZ77, D_Field, sUnk_ff520.unk_ff708, 0, -1, sUnk_gp.loadDataBuffer);
        sUnk_feb28.unk_feb28 &= ~0x40000000;
    }
    sSceCtx.unk_ff504++;
    return 0x80;
}

unktype_4 func_8008268C() {

    if (*(u16*)(sUnk_gp.unk_fea6c + 0x42) & 0x10) {
        return func_8007708C(2);
    }
    
    if (func_80071EDC(0xf0, 0x10) != 0) {

    }

    ushort uVar1;
    ushort uVar2;
    short sVar3;
    undefined4 uVar4;
    int iVar5;
    uint uVar6;
    int iVar7;
    ushort* puVar8;
    uint uVar9;
    char cVar10;
    char cVar11;
    undefined4 local_50;
    undefined4 local_4c;
    undefined4 local_48;
    undefined4 local_44;
    undefined4 local_40;
    undefined4 local_3c;
    undefined4 local_38;
    undefined4 local_34;
    undefined4 local_30;
    undefined4 local_2c;
    undefined4 local_28;
    undefined4 local_24;

    local_50 = 0xc000cee;
    local_4c = 0x120c12;
    local_48 = 0xf400f412;
    local_44 = 0xeef4ee;
    local_40 = 0x100010e8;
    local_3c = 0x181018;
    local_38 = 0xf000f018;
    local_34 = 0xe8f0e8;
    local_30 = 0xe0380e83;
    local_2c = 0xc11c0770;
    local_28 = 0x8040201;
    local_24 = 0x80402010;
    cVar11 = '\0';


    //@
    iVar5 = FUN_80071edc(0xf0, 0x10);
    if (iVar5 != 0) {
        uVar6 = 0xf0;
        iVar5 = 0;
        do {
            iVar7 = FUN_80071dd0(uVar6 & 0xffff);
            if (iVar7 != 0) {
                FUN_80071d40(uVar6 & 0xffff);
                DAT_800ff714 = (short)iVar5 + 0x120;
                DAT_00001d03 = 0;
                uVar4 = FUN_8007708c(0);
                return uVar4;
            }
            iVar7 = iVar5 + 1;
            uVar6 = iVar5 + 0xf1;
            iVar5 = iVar7;
        } while (iVar7 < 0x10);
    }
    if ((ram0x00000042 & 0x20) == 0) {
    LAB_80082970:
        iVar5 = FUN_80060a1c(0, (int)(short)(ushort)DAT_0000000c, (int)(short)DAT_0000000c._2_2_);
        uVar1 = DAT_800ff718;
        if (-1 < iVar5 << 0x10) {
            iVar5 = iVar5 * 2;
            puVar8 = (ushort*)(DAT_800feaa4 + 0x24 + iVar5);
            uVar2 = *puVar8;
            if (((DAT_800ff716 == uVar2) || (uVar2 == 0)) ||
                ((*(ushort*)(&DAT_00010024 + iVar5 + DAT_800feaa4) & 0xc000) != 0x8000)) {
                uVar6 = (uint)DAT_800ff718;
                if ((uVar6 == 0) || (uVar9 = (uint) * (ushort*)(DAT_800feaa4 + iVar5 + 0x24), uVar6 == uVar9)
                    ) {
                    puVar8 = (ushort*)(DAT_800feaa4 + 0x24 + iVar5);
                    DAT_800ff716 = *puVar8;
                    uVar1 = *puVar8;
                    if ((uVar1 != 0) &&
                        ((*(ushort*)(&DAT_00010024 + iVar5 + DAT_800feaa4) & 0xc000) == 0xc000)) {
                        DAT_800ff718 = uVar1;
                    }
                }
                else {
                    uVar6 = uVar6 & 0xfff;
                    if ((int)(uVar9 << 0x10) < 0) {
                        uVar6 = DAT_800ff718 >> 10 & 0xf;
                        if ((*(byte*)((int)&local_28 + (uRam00000000 >> 0xe & 0xf)) &
                            *(byte*)((int)&local_30 + ((byte)(&DAT_00020024)[uVar6 + DAT_800feaa4] & 7))) != 0)
                        {
                            DAT_800ff71a = (byte)uVar6;
                            DAT_800ff718 = 0;
                            goto LAB_80082b60;
                        }
                    }
                    else {
                        if ((uVar6 - 1 < 0xf) ||
                            ((uVar6 - 0x101 < 0xf && (iVar5 = FUN_80071dd0(uVar6 - 0x20 & 0xffff), iVar5 == 0))))
                        {
                            uVar1 = DAT_800ff718;
                            DAT_800ff718 = 0;
                            DAT_800ff714 = uVar1 & 0xfff;
                            if ((uVar1 & 0xfff) != 0) goto LAB_80082e44;
                        }
                    }
                }
            }
            else {
                uVar1 = *puVar8;
                DAT_800ff716 = uVar2;
                if ((short)*puVar8 < 0) {
                    if ((*(byte*)((int)&local_28 + (uRam00000000 >> 0xe & 0xf)) &
                        *(byte*)((int)&local_30 +
                            ((byte)(&DAT_00020024)[(uVar1 >> 10 & 0xf) + DAT_800feaa4] & 7))) != 0) {
                        DAT_800ff71a = (byte)(*puVar8 >> 10) & 0xf;
                    LAB_80082b60:
                        DAT_00001d03 = 0;
                        DAT_800ff708 = uVar1 & 0x3ff;
                        if ((uVar1 & 0x200) == 0) {
                            uVar4 = FUN_8007708c(1);
                            return uVar4;
                        }
                        uVar4 = FUN_8007708c(4);
                        return uVar4;
                    }
                    DAT_800ff716 = 0;
                }
                else {
                    uVar6 = uVar1 & 0xfff;
                    if ((uVar6 - 1 < 0xf) ||
                        ((uVar6 - 0x101 < 0xf && (iVar5 = FUN_80071dd0(uVar6 - 0x20 & 0xffff), iVar5 == 0)))) {
                        uVar4 = 0;
                        DAT_800ff714 = (ushort)uVar6;
                        goto LAB_80082e50;
                    }
                }
            }
        }
        iVar5 = FUN_80071dd0(0x66);
        if (((iVar5 != 0) || (DAT_800ff71e == 0)) || (DAT_800ff71c <= DAT_800ff71e)) {
            if ((DAT_00000043._1_2_ & 0x40) != 0) {
                cVar11 = -0x80;
            }
            if ((DAT_00000043._1_2_ & 0x4000) == 0) {
                if ((DAT_00000043._1_2_ & 0x1000) == 0) {
                    cVar10 = cVar11 + '\t';
                    if ((DAT_00000043._1_2_ & 0x8000) == 0) {
                        if ((DAT_00000043._1_2_ & 0x2000) == 0) {
                            if (DAT_00001d03 < 0x4b) {
                                if ((DAT_800feb5c & 7) == 0) {
                                    DAT_00001d03 = DAT_00001d03 + 1;
                                }
                                FUN_80061338(0, 1, 1);
                                return 0x80;
                            }
                            if ((DAT_800feb5c & 7) != 0) {
                                return 0x80;
                            }
                            DAT_00001d03 = DAT_00001d03 + 1;
                            if (*(char*)((int)&PTR_caseD_5_800358f4 + DAT_00001d03 + 1) == -1) {
                                DAT_00001d03 = 0;
                            }
                            else {
                                DAT_00000014._0_1_ = *(undefined*)((int)&PTR_caseD_5_800358f4 + DAT_00001d03 + 1);
                                uRam00000000 = uRam00000000 & 0xfffc3fef | 0x4000;
                            }
                            return 0x80;
                        }
                        cVar10 = cVar11 + '\x05';
                    }
                }
                else {
                    cVar10 = cVar11 + '\b';
                    if ((DAT_00000043._1_2_ & 0x8000) == 0) {
                        if ((DAT_00000043._1_2_ & 0x2000) == 0) {
                            cVar10 = cVar11 + '\a';
                        }
                        else {
                            cVar10 = cVar11 + '\x06';
                        }
                    }
                }
            }
            else {
                cVar10 = cVar11 + '\x02';
                if ((DAT_00000043._1_2_ & 0x8000) == 0) {
                    if ((DAT_00000043._1_2_ & 0x2000) == 0) {
                        cVar10 = cVar11 + '\x03';
                    }
                    else {
                        cVar10 = cVar11 + '\x04';
                    }
                }
            }
            FUN_80061338(0, cVar10, 1);
            DAT_00001d03 = 0;
            return 0x80;
        }
        FUN_80060c04();
        FUN_80070004();
        uVar4 = 3;
    }
    else {
        sVar3 = FUN_80066634();
        if ((sVar3 == 0) || (DAT_800ff714 = sVar3 + 0x120, DAT_800ff714 == 0)) {
            iVar5 = 0;
            do {
                iVar7 = (uRam00000000 >> 0xd & 0x1e) + iVar5 * 0x10;
                iVar7 = FUN_80060a1c(0, (int)(((uint)(ushort)DAT_0000000c +
                    (int)*(char*)((int)&local_50 + iVar7)) * 0x10000) >> 0x10,
                    (int)(((uint)DAT_0000000c._2_2_ +
                        (int)*(char*)((int)&local_50 + iVar7 + 1)) * 0x10000) >> 0x10);
                if ((*(ushort*)(&DAT_00010024 + iVar7 * 2 + DAT_800feaa4) & 0xc000) == 0x4000) {
                    puVar8 = (ushort*)(DAT_800feaa4 + 0x24 + iVar7 * 2);
                    uVar1 = *puVar8;
                    uVar6 = uVar1 & 0xfff;
                    if ((short)*puVar8 < 0) {
                        DAT_800ff71a = (byte)(*puVar8 >> 10) & 0xf;
                        goto LAB_80082b60;
                    }
                    if (((uVar6 - 1 < 0xf) ||
                        ((uVar6 - 0x101 < 0xf && (iVar7 = FUN_80071dd0(uVar6 - 0x20 & 0xffff), iVar7 == 0))))
                        && (DAT_800ff714 = (ushort)uVar6, (uVar1 & 0xfff) != 0)) goto LAB_80082e44;
                }
                iVar5 = iVar5 + 1;
            } while (iVar5 < 2);
            goto LAB_80082970;
        }
    LAB_80082e44:
        uVar4 = 0;
    }
LAB_80082e50:
    DAT_00001d03 = 0;
    uVar4 = FUN_8007708c(uVar4);
    return uVar4;
}

undefined4 FUN_80082e98(void)

{
    short sVar1;
    short sVar2;
    int iVar3;
    int iVar4;
    short* psVar5;
    int iVar6;
    int iVar7;
    int iVar8;
    int iVar9;

    psVar5 = (short*)(DAT_800ff4f8 + DAT_800ff500);
    iVar9 = (int)*psVar5;
    iVar8 = (int)psVar5[1];
    sVar1 = psVar5[2];
    if (iVar9 < 0) {
        iVar9 = 0;
    }
    else {
        if ((int)((uint) * (ushort*)(&DAT_0002113c + DAT_800feaa4) * 0x10) <= iVar9 + 0x140) {
            iVar9 = (int)(((uint) * (ushort*)(&DAT_0002113c + DAT_800feaa4) * 0x10 + -0x140) * 0x10000) >>
                0x10;
        }
    }
    if (iVar8 < 0) {
        iVar8 = 0;
    }
    else {
        if ((int)((uint) * (ushort*)(&DAT_0002113e + DAT_800feaa4) * 0x10) <= iVar8 + 0xf0) {
            iVar8 = (int)(((uint) * (ushort*)(&DAT_0002113e + DAT_800feaa4) * 0x10 + -0xf0) * 0x10000) >>
                0x10;
        }
    }
    if (((uRam00000000 & 3) == 3) && (iVar3 = FUN_80071dd0(0x12), iVar3 == 1)) {
        iVar3 = ((uint)(ushort)DAT_0000000c - iVar9) * 0x10000;
        iVar7 = iVar3 >> 0x10;
        iVar4 = ((uint)DAT_0000000c._2_2_ - iVar8) * 0x10000;
        iVar6 = iVar4 >> 0x10;
        if (iVar6 < 0x60) {
            if (iVar8 < 1) {
                iVar8 = 0;
            }
            else {
                iVar8 = (iVar6 + iVar8 + -0x60) * 0x10000 >> 0x10;
            }
        }
        if (0xb0 < iVar6) {
            sVar2 = (short)((uint) * (ushort*)(&DAT_0002113e + DAT_800feaa4) * 0x10) + -0xf0;
            if (iVar8 + 0xf0 < (int)((uint) * (ushort*)(&DAT_0002113e + DAT_800feaa4) * 0x10)) {
                sVar2 = (short)((uint)iVar4 >> 0x10) + (short)iVar8 + -0xb0;
            }
            iVar8 = (int)sVar2;
        }
        if (iVar7 < 0x60) {
            if (iVar9 < 1) {
                iVar9 = 0;
            }
            else {
                iVar9 = (iVar7 + iVar9 + -0x60) * 0x10000 >> 0x10;
            }
        }
        if (0xe0 < iVar7) {
            sVar2 = (short)((uint) * (ushort*)(&DAT_0002113c + DAT_800feaa4) * 0x10) + -0x140;
            if (iVar9 + 0x140 < (int)((uint) * (ushort*)(&DAT_0002113c + DAT_800feaa4) * 0x10)) {
                sVar2 = (short)((uint)iVar3 >> 0x10) + (short)iVar9 + -0xe0;
            }
            iVar9 = (int)sVar2;
        }
    }
    iVar3 = DAT_800feaa4;
    if (sVar1 == 0) {
        *(short*)(DAT_800feaa4 + 0x10) = (short)iVar9;
        *(short*)(iVar3 + 0x12) = (short)iVar8;
    }
    else {
        *(short*)(DAT_800feaa4 + 0x18) = (short)iVar9;
        *(short*)(iVar3 + 0x1a) = (short)iVar8;
        *(short*)(iVar3 + 0x14) = sVar1;
    }
    DAT_800ff500 = DAT_800ff500 + 6;
    return 0x81;
}

undefined4 FUN_80083124(void)

{
    short sVar1;
    short sVar2;
    int iVar3;
    int iVar4;
    int iVar5;
    byte* pbVar6;
    int iVar7;
    int iVar8;
    int iVar9;

    pbVar6 = (byte*)(DAT_800ff4f8 + DAT_800ff500);
    iVar9 = (int)(((int)*(short*)(pbVar6 + 1) + (uint) * (ushort*)((uint)*pbVar6 * 0x3c + 0xc)) *
        0x10000) >> 0x10;
    iVar8 = (int)(((int)*(short*)(pbVar6 + 3) + (uint) * (ushort*)((uint)*pbVar6 * 0x3c + 0xe)) *
        0x10000) >> 0x10;
    sVar1 = *(short*)(pbVar6 + 5);
    if (iVar9 < 0) {
        iVar9 = 0;
    }
    else {
        if ((int)((uint) * (ushort*)(&DAT_0002113c + DAT_800feaa4) * 0x10) <= iVar9 + 0x140) {
            iVar9 = (int)(((uint) * (ushort*)(&DAT_0002113c + DAT_800feaa4) * 0x10 + -0x140) * 0x10000) >>
                0x10;
        }
    }
    if (iVar8 < 0) {
        iVar8 = 0;
    }
    else {
        if ((int)((uint) * (ushort*)(&DAT_0002113e + DAT_800feaa4) * 0x10) <= iVar8 + 0xf0) {
            iVar8 = (int)(((uint) * (ushort*)(&DAT_0002113e + DAT_800feaa4) * 0x10 + -0xf0) * 0x10000) >>
                0x10;
        }
    }
    if (((uRam00000000 & 3) == 3) && (iVar3 = FUN_80071dd0(0x12), iVar3 == 1)) {
        iVar3 = ((uint)(ushort)DAT_0000000c - iVar9) * 0x10000;
        iVar7 = iVar3 >> 0x10;
        iVar4 = ((uint)DAT_0000000c._2_2_ - iVar8) * 0x10000;
        iVar5 = iVar4 >> 0x10;
        if (iVar5 < 0x60) {
            if (iVar8 < 1) {
                iVar8 = 0;
            }
            else {
                iVar8 = (iVar5 + iVar8 + -0x60) * 0x10000 >> 0x10;
            }
        }
        if (0xb0 < iVar5) {
            sVar2 = (short)((uint) * (ushort*)(&DAT_0002113e + DAT_800feaa4) * 0x10) + -0xf0;
            if (iVar8 + 0xf0 < (int)((uint) * (ushort*)(&DAT_0002113e + DAT_800feaa4) * 0x10)) {
                sVar2 = (short)((uint)iVar4 >> 0x10) + (short)iVar8 + -0xb0;
            }
            iVar8 = (int)sVar2;
        }
        if (iVar7 < 0x60) {
            if (iVar9 < 1) {
                iVar9 = 0;
            }
            else {
                iVar9 = (iVar7 + iVar9 + -0x60) * 0x10000 >> 0x10;
            }
        }
        if (0xe0 < iVar7) {
            sVar2 = (short)((uint) * (ushort*)(&DAT_0002113c + DAT_800feaa4) * 0x10) + -0x140;
            if (iVar9 + 0x140 < (int)((uint) * (ushort*)(&DAT_0002113c + DAT_800feaa4) * 0x10)) {
                sVar2 = (short)((uint)iVar3 >> 0x10) + (short)iVar9 + -0xe0;
            }
            iVar9 = (int)sVar2;
        }
    }
    iVar3 = DAT_800feaa4;
    if (sVar1 == 0) {
        *(short*)(DAT_800feaa4 + 0x10) = (short)iVar9;
        *(short*)(iVar3 + 0x12) = (short)iVar8;
    }
    else {
        *(short*)(DAT_800feaa4 + 0x18) = (short)iVar9;
        *(short*)(iVar3 + 0x1a) = (short)iVar8;
        *(short*)(iVar3 + 0x14) = sVar1;
    }
    DAT_800ff500 = DAT_800ff500 + 7;
    return 0x81;
}

undefined4 FUN_800833e0(void)

{
    byte bVar1;
    undefined4 uVar2;
    byte* pbVar3;
    int iVar4;
    short sVar5;
    ushort uVar6;
    uint uVar7;
    uint uVar8;
    undefined4 local_20;
    undefined local_1c;
    undefined local_1b;
    undefined local_1a;

    local_20 = 0x2010400;
    local_1c = 3;
    local_1b = 7;
    local_1a = 8;
    switch (DAT_800ff504) {
    case 0:
        iVar4 = FUN_80071dd0(0x5e);
        if (iVar4 == 0) {
            DAT_801077bd = 0;
        }
        uVar6 = 0;
        DAT_800feb2c = DAT_800feb2c | 2;
        register0x0000000c = ram0x0000004c | 0x5000;
        pbVar3 = (byte*)(DAT_800ff500 + DAT_800ff4f8);
        DAT_801077be = (ushort)*pbVar3 + (ushort)pbVar3[1] * 0x100;
        DAT_801077bc = 0;
        DAT_801077c0 = (ushort)pbVar3[2] + (ushort)pbVar3[3] * 0x100;
        do {
            iVar4 = FUN_80071dd0(uVar6 + 0x40);
            if (iVar4 != 0) {
                if (DAT_800feb28 < 0) {
                    uVar8 = (uint)DAT_801077bc;
                    DAT_801077bc = DAT_801077bc + 1;
                    (&DAT_801077c2)[uVar8] = (ushort) * (byte*)((int)&local_20 + uVar8);
                }
                else {
                    uVar8 = (uint)DAT_801077bc;
                    DAT_801077bc = DAT_801077bc + 1;
                    (&DAT_801077c2)[uVar8] = uVar6;
                }
            }
            uVar6 = uVar6 + 1 & 0xff;
        } while (uVar6 < 10);
        uVar8 = 0;
        do {
            iVar4 = uVar8 * 400 + -0x7fef8c04;
            bVar1 = 0;
            if (DAT_801077bc != 0) {
                do {
                    bVar1 = bVar1 + 1;
                    *(undefined*)(iVar4 + 4) = 0x40;
                    *(undefined*)(iVar4 + 5) = 0x40;
                    *(undefined*)(iVar4 + 6) = 0x40;
                    iVar4 = iVar4 + 0x28;
                } while (bVar1 < DAT_801077bc);
            }
            uVar7 = uVar8 + 1 & 0xff;
            iVar4 = uVar8 * 400 + (uint)DAT_801077bd * 0x28 + -0x7fef8c04;
            *(undefined*)(iVar4 + 4) = 0x80;
            *(undefined*)(iVar4 + 5) = 0x80;
            *(undefined*)(iVar4 + 6) = 0x80;
            uVar8 = uVar7;
        } while (uVar7 < 2);
        if (DAT_800feb28 < 0) {
            FUN_80071d40(0x48);
        }
        DAT_800ff504 = DAT_800ff504 + 1;
        break;
    case 1:
        FUN_8003adb8();
        if (DAT_800feb28 < 0) {
            FUN_8005e0d4(0, (&DAT_801077c2)[DAT_801077bd] + 0x139);
        }
        else {
            FUN_8005e0d4(0, (&DAT_801077c2)[DAT_801077bd] + 0x130);
        }
        DAT_800ff504 = DAT_800ff504 + 1;
        break;
    case 2:
        if ((ram0x00000042 & 0x1000) == 0) {
            if ((ram0x00000042 & 0x4000) == 0) {
                if ((ram0x00000042 & 0x20) != 0) {
                    iVar4 = FUN_80071dd0(0x82);
                    if ((iVar4 == 0) || (DAT_801077bd != 2)) {
                        DAT_80100074 = 2;
                        DAT_800feb2c = DAT_800feb2c & 0xfffffffd;
                        if (DAT_800feb28 < 0) {
                            uVar8 = 0;
                            do {
                                FUN_80071d40(uVar8 + 0x40);
                                uVar8 = uVar8 + 1 & 0xff;
                            } while (uVar8 < 10);
                        }
                        else {
                            iVar4 = FUN_80071dd0(0x48);
                            uVar8 = 0;
                            if (iVar4 == 0) {
                                do {
                                    FUN_80071d40(uVar8 + 0x40);
                                    uVar8 = uVar8 + 1 & 0xff;
                                } while (uVar8 < 9);
                            }
                            else {
                                do {
                                    FUN_80071d40(uVar8 + 0x40);
                                    uVar8 = uVar8 + 1 & 0xff;
                                } while (uVar8 < 9);
                                FUN_80071cf8(0x48);
                            }
                        }
                        DAT_800ff500 = DAT_800ff500 + 4;
                        uVar2 = FUN_8007708c(DAT_801077bd);
                        return uVar2;
                    }
                    DAT_80100074 = 4;
                }
                if ((ram0x00000042 & 0x40) != 0) {
                    if (-1 < DAT_800feb28) {
                        DAT_80100074 = 3;
                        DAT_800feb2c = DAT_800feb2c & 0xfffffffd;
                        iVar4 = FUN_80071dd0(0x48);
                        uVar8 = 0;
                        if (iVar4 == 0) {
                            do {
                                FUN_80071d40(uVar8 + 0x40);
                                uVar8 = uVar8 + 1 & 0xff;
                            } while (uVar8 < 9);
                        }
                        else {
                            do {
                                FUN_80071d40(uVar8 + 0x40);
                                uVar8 = uVar8 + 1 & 0xff;
                            } while (uVar8 < 9);
                            FUN_80071cf8(0x48);
                        }
                        DAT_800ff500 = DAT_800ff500 + 4;
                        uVar2 = FUN_8007708c(6);
                        return uVar2;
                    }
                    DAT_80100074 = 4;
                }
            }
            else {
                uVar8 = (uint)DAT_801077bd;
                DAT_801077bd = DAT_801077bd + 1;
                if ((int)(DAT_801077bc - 1) <= (int)uVar8) {
                    DAT_801077bd = 0;
                }
                DAT_80100074 = 1;
                uVar8 = 0;
                do {
                    iVar4 = uVar8 * 400 + -0x7fef8c04;
                    bVar1 = 0;
                    if (DAT_801077bc != 0) {
                        do {
                            bVar1 = bVar1 + 1;
                            *(undefined*)(iVar4 + 4) = 0x40;
                            *(undefined*)(iVar4 + 5) = 0x40;
                            *(undefined*)(iVar4 + 6) = 0x40;
                            iVar4 = iVar4 + 0x28;
                        } while (bVar1 < DAT_801077bc);
                    }
                    iVar4 = uVar8 * 400 + (uint)DAT_801077bd * 0x28 + -0x7fef8c04;
                    uVar8 = uVar8 + 1 & 0xff;
                    *(undefined*)(iVar4 + 4) = 0x80;
                    *(undefined*)(iVar4 + 5) = 0x80;
                    *(undefined*)(iVar4 + 6) = 0x80;
                } while (uVar8 < 2);
                DAT_800ff504 = DAT_800ff504 + 1;
            }
        }
        else {
            if (DAT_801077bd == 0) {
                DAT_801077bd = DAT_801077bc;
            }
            DAT_801077bd = DAT_801077bd - 1;
            DAT_80100074 = 1;
            uVar8 = 0;
            do {
                iVar4 = uVar8 * 400 + -0x7fef8c04;
                bVar1 = 0;
                if (DAT_801077bc != 0) {
                    do {
                        bVar1 = bVar1 + 1;
                        *(undefined*)(iVar4 + 4) = 0x40;
                        *(undefined*)(iVar4 + 5) = 0x40;
                        *(undefined*)(iVar4 + 6) = 0x40;
                        iVar4 = iVar4 + 0x28;
                    } while (bVar1 < DAT_801077bc);
                }
                iVar4 = uVar8 * 400 + (uint)DAT_801077bd * 0x28 + -0x7fef8c04;
                uVar8 = uVar8 + 1 & 0xff;
                *(undefined*)(iVar4 + 4) = 0x80;
                *(undefined*)(iVar4 + 5) = 0x80;
                *(undefined*)(iVar4 + 6) = 0x80;
            } while (uVar8 < 2);
            DAT_800ff504 = DAT_800ff504 + 1;
        }
        break;
    case 3:
        FUN_8005b2b8(DAT_80103bb4);
        FUN_8003adb8();
        if (DAT_800feb28 < 0) {
            uVar6 = (&DAT_801077c2)[DAT_801077bd];
            sVar5 = uVar6 + 0x139;
            if (uVar6 < 6) goto LAB_80083b58;
            if (uVar6 == 7) {
                FUN_8005e0d4(0, 0x13e);
            }
            else {
                FUN_8005e0d4(0, 0x13f);
            }
        }
        else {
            sVar5 = (&DAT_801077c2)[DAT_801077bd] + 0x130;
        LAB_80083b58:
            FUN_8005e0d4(0, sVar5);
        }
        DAT_800ff504 = DAT_800ff504 + 1;
        break;
    case 4:
    case 5:
    case 6:
        DAT_800ff504 = 2;
    }
    return 0x80;
}

undefined4 FUN_80083bcc(void)

{
    undefined4 uVar1;
    uint uVar2;

    if (DAT_800ff714 < 0x10) {
        uVar1 = FUN_8007708c(DAT_800ff714);
    }
    else {
        if (((ushort)(DAT_800ff714 - 0x101) < 0xf) || ((ushort)(DAT_800ff714 - 0x120) < 0x10)) {
            uVar2 = (uint)DAT_800ff4fc;
            DAT_800ff4fc = DAT_800ff4fc + 1;
            (&DAT_800ff4b8)[uVar2] = DAT_800ff4f8 + DAT_800ff500 + 0x20;
            DAT_800ff500 = 0;
            DAT_800ff4f8 = DAT_800ff508 +
                (uint) * (ushort*)((uint)(byte)DAT_800ff714 * 2 + DAT_800ff508) +
                (uint)(byte)DAT_800ff714 * 2;
        }
        else {
            DAT_800ff500 = DAT_800ff500 + 0x20;
        }
        uVar1 = 1;
    }
    return uVar1;
}

undefined4 FUN_80083cb8(void)

{
    int iVar1;
    undefined4* puVar2;

    if (DAT_800ff504 == 0) {
        DAT_800ff838 = 0xff;
        DAT_800ff73e = 0;
        DAT_800ff73f = 0;
        DAT_800ff740 = 0;
        DAT_800ff741 = 0;
        DAT_800ff828 = DAT_800ff708;
        CdFiles_requestCdFile('\x10', 4, (uint)DAT_800ff708, 0, -1, DAT_800ff508);
        iVar1 = 1;
        puVar2 = &DAT_800ff830;
        do {
            *puVar2 = 0;
            iVar1 = iVar1 + -1;
            puVar2 = puVar2 + -1;
        } while (-1 < iVar1);
        DAT_800ff504 = DAT_800ff504 + 1;
    }
    else {
        if (DAT_800ff504 != 1) {
            return 0x80;
        }
        iVar1 = FUN_8003fec4();
        if (iVar1 != 0) {
            return 0x81;
        }
    }
    return 0x80;
}