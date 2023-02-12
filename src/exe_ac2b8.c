#include "global.h"

void func_800AC2B8(void)

{
    uint uVar1;
    uint uVar2;
    byte bVar3;
    uint uVar4;

    uVar2 = DAT_801012c0 | 0x20;
    if (DAT_800fea50[-1] == 0xff) {
        if ((char)DAT_800ff50d < '\0') {
            DAT_80107392 = 2;
            DAT_80107391 = 0;
            if ((DAT_800ff50d & 0x60) == 0x40) {
                DAT_80107392 = 3;
            }
            DAT_80107393 = ((DAT_800ff50d & 0x60) == 0x20) << 4;
            ram0x0000004c = 0x5000;
            DAT_800ff504 = 6;
            DAT_801012c0 = DAT_801012c0 & 0xffffffdf;
        }
        else {
            if (DAT_800ff50d < 0x40) {
                DAT_800ff504 = 0xc;
                DAT_801012c0 = uVar2;
            }
            else {
                DAT_800ff504 = 0xe;
                DAT_801012c0 = DAT_801012c0 & 0xffffffdf;
            }
        }
    }
    else {
        if (DAT_800fea50[-1] == 0xfc) {
            bVar3 = DAT_800fea50[1];
            if (bVar3 == 0x3e) {
                DAT_80103bd1 = *DAT_800fea50;
                DAT_800ff504 = 9;
                DAT_800fea50 = DAT_800fea50 + 2;
                DAT_801012c0 = DAT_801012c0 & 0xffffffdf;
            }
            else {
                if (bVar3 == 0x3d) {
                    DAT_80103bd1 = 0;
                    if ((DAT_800feb28 >> 0x1c & 1) == 0) {
                        DAT_800fea50 = DAT_800fea50 + 2;
                    }
                    else {
                        uVar1 = 1;
                        uVar4 = (uint)*DAT_800fea50;
                        uVar2 = uVar4 & 0xf;
                        if ((*DAT_800fea50 & 0xf) != 0) {
                            do {
                                uVar2 = uVar2 - 1;
                                uVar1 = (uVar1 & 0x7fff) << 1;
                            } while (uVar2 != 0);
                        }
                        if (uVar4 < 0xc0) {
                            if ((int)(uVar4 << 0x18) < 0) {
                                _DAT_00006a24 = _DAT_00006a24 & ~(ushort)uVar1;
                                DAT_800fea50 = DAT_800fea50 + 2;
                            }
                            else {
                                if ((_DAT_00006a24 & uVar1) == 0) {
                                    DAT_80103bd1 = 2;
                                    DAT_800fea50 = DAT_800fea50 + -1;
                                }
                                else {
                                    DAT_800fea50 = DAT_800fea50 + 2;
                                }
                            }
                        }
                        else {
                            _DAT_00006a24 = (ushort)uVar1 | _DAT_00006a24;
                            DAT_800fea50 = DAT_800fea50 + 2;
                        }
                    }
                    DAT_800ff504 = 9;
                    DAT_801012c0 = DAT_801012c0 & 0xffffffdf;
                }
                else {
                    if (bVar3 == 0x3c) {
                        DAT_801012c0 = DAT_801012c0 & 0xffffffdf;
                        DAT_80103bd1 = 0;
                        bVar3 = *DAT_800fea50 & 7;
                        if ((int)((uint)*DAT_800fea50 << 0x18) < 0) {
                            func_8007416C(bVar3, 0, 0x22, 0xffffffff);
                        }
                        else {
                            func_80074258(bVar3, 0);
                        }
                        if (DAT_800fea50[2] == 0xff) {
                            func_80074258(bVar3, 1);
                        }
                        else {
                            func_8007416C(bVar3, 1, DAT_800fea50[2], 0xffffffff);
                        }
                        uVar2 = (uint)DAT_800fea50[3];
                        if (uVar2 == 0xff) {
                            func_80074258(bVar3, 2);
                        }
                        else {
                            if (uVar2 < 0xf0) {
                                func_8007416C(bVar3, 2, DAT_800fea50[3], 0xffffffff);
                            }
                            else {
                                func_8007416C(bVar3, 2, 1,
                                    (int)(&DAT_8001e76c + (uVar2 & 0xf) * 2) +
                                    (uint) * (ushort*)(&DAT_8001e76c + (uVar2 & 0xf) * 2));
                            }
                        }
                        DAT_800ff504 = 9;
                        DAT_800fea50 = DAT_800fea50 + 4;
                    }
                    else {
                        DAT_801012c0 = uVar2;
                        CdFiles_RequestCdFile('0', 7, (uint)*DAT_800fea50, 0, -1, DAT_800fea54);
                        if ((DAT_800ff50d & 8) == 0) {
                            DAT_801073e9 = *DAT_800fea50;
                            DAT_801073ea = DAT_800fea50[2];
                        }
                        else {
                            DAT_801073ec = *DAT_800fea50;
                            DAT_801073ed = DAT_800fea50[2];
                        }
                        DAT_800ff504 = 4;
                        DAT_800fea50 = DAT_800fea50 + 3;
                    }
                }
            }
        }
        else {
            DAT_800ff504 = 2;
            DAT_801012c0 = uVar2;
        }
    }
    return;
}

undefined4 func_800AC720(void)

{
    byte bVar1;
    byte bVar2;
    uint uVar3;
    short sVar4;
    undefined4 uVar5;
    int iVar6;
    char cVar7;
    undefined2 uVar8;
    uint uVar9;
    byte* pbVar10;
    undefined4 uVar11;
    byte* pbVar12;
    int iVar13;

    bVar1 = DAT_800ff50d >> 3;
    bVar2 = DAT_800ff50d >> 4;
    switch (DAT_800ff504) {
    case 0:
        iVar6 = func_80071DD0(0x3f1);
        if (iVar6 == 0) {
            DAT_801012c0 = DAT_801012c0 & 0x7fffff;
        }
        else {
            DAT_801012c0 = DAT_801012c0 & 0x7fffff | 0x78000000;
        }
        if ((bVar1 & 1) == 0) {
            if (DAT_80103bb4 == 0xff) {
                func_8005D7A8();
            }
            func_8005B2B8(DAT_80103bb4);
            DAT_801012c0 = DAT_801012c0 & 0xfffffeff | 2;
            func_8005B9CC(DAT_80103bb4, 0);
            if ((DAT_801012c0 & 0x40) != 0) {
                func_8005B9CC(DAT_80103bb5, 0);
            }
            if (((DAT_801012c0 & 4) != 0) &&
                (func_8005B9CC(DAT_80103bb6, 0x80), (DAT_801012c0 & 0x80) != 0)) {
                func_8005B9CC(DAT_80103bb7, 0x80);
            }
        }
        else {
            if (DAT_80103bb6 == -1) {
                func_8005D8EC();
            }
            func_8005B2B8(DAT_80103bb6);
            DAT_801012c0 = DAT_801012c0 | 0x104;
            func_8005B9CC(DAT_80103bb6, 0);
            if ((DAT_801012c0 & 0x80) != 0) {
                func_8005B9CC(DAT_80103bb7, 0);
            }
            if (((DAT_801012c0 & 2) != 0) &&
                (func_8005B9CC(DAT_80103bb4, 0x80), (DAT_801012c0 & 0x40) != 0)) {
                func_8005B9CC(DAT_80103bb5, 0x80);
            }
        }
        DAT_801012c0 = DAT_801012c0 & 0xffff0fff | 1;
        DAT_800fea4c = func_800714A4(0x400);
        DAT_800fea54 = func_800714A4(0x4000);
        DAT_800fea58 = 0;
        if (((bVar1 & 1) == 0) && ((_DAT_80101e04 & 0x10) != 0)) {
            _DAT_80101e04 = _DAT_80101e04 & 0xffffffef;
            DAT_80101e37 = 0xff;
        }
        if ((DAT_800ff50d & 1) == 0) {
            iVar6 = func_8005D1DC(DAT_800fea4c, DAT_800ff4f8 + DAT_800ff500, 0x26, 3, 0x3ff);
            DAT_800ff500 = DAT_800ff500 + iVar6;
            if ((bVar1 & 1) == 0) {
                DAT_801012c0 = DAT_801012c0 & 0xfffffff7;
            }
            else {
                DAT_801012c0 = DAT_801012c0 & 0xffffffef;
            }
        }
        else {
            if ((bVar1 & 1) == 0) {
                DAT_801012c0 = DAT_801012c0 | 8;
            }
            else {
                DAT_801012c0 = DAT_801012c0 | 0x10;
            }
            if ((DAT_801012c0 & 0x400000) == 0) {
                if (((DAT_801073eb == 0xff) || (DAT_801073ec != *(byte*)(DAT_800ff4f8 + 2))) ||
                    (DAT_801073ed != *(char*)(DAT_800ff4f8 + 3))) goto LAB_800acabc;
                bVar2 = DAT_801073eb;
                if (DAT_801073e8 != 0xff) {
                    func_80057834(DAT_801073e8, 0);
                    bVar2 = DAT_801073eb;
                }
            LAB_800acaf0:
                (&DAT_80103bd8)[(uint)bVar2 * 0x16] = (&DAT_80103bd8)[(uint)bVar2 * 0x16] & 0xfffffff8 | 1;
            }
            else {
                if (((DAT_801073e8 != 0xff) && (DAT_801073e9 == *(byte*)(DAT_800ff4f8 + 2))) &&
                    (DAT_801073ea == *(char*)(DAT_800ff4f8 + 3))) {
                    bVar2 = DAT_801073e8;
                    if (DAT_801073eb != 0xff) {
                        func_80057834(DAT_801073eb, 0);
                        bVar2 = DAT_801073e8;
                    }
                    goto LAB_800acaf0;
                }
            LAB_800acabc:
                DAT_800fea58 = 1;
            }
            DAT_800ff500 = DAT_800ff500 + 2;
            iVar6 = func_8005D1DC(DAT_800fea4c, DAT_800ff4f8 + DAT_800ff500, 0x1e, 3, 0x3ff);
            DAT_800ff500 = DAT_800ff500 + iVar6;
            if (DAT_800fea58 != 0) {
                CdFiles_RequestCdFile('0', 7, (uint) * (byte*)(DAT_800ff4f8 + 2), 0, -1, DAT_800fea54);
            }
        }
        if ((bVar1 & 1) == 0) {
            if (((DAT_800ff50d & 2) == 0) && ((DAT_801012c0 & 0x40) != 0)) {
                func_8005A3B4(1, 0x85, 2);
                func_8005B2B8(DAT_80103bb5);
                func_8005E350(DAT_80103bb5);
                func_8005AE90(DAT_80103bb5);
                DAT_80103bb5 = 0xff;
                DAT_801012c0 = DAT_801012c0 & 0xffffffbf;
            }
            if ((DAT_800ff50d & 4) == 0) {
                if ((_DAT_80101e04 & 0x4000) != 0) {
                    _DAT_80101e04 = _DAT_80101e04 & 0xffffbfff;
                    func_8005A4FC(0, 0, 1);
                }
            }
            else {
                if ((_DAT_80101e04 & 0x4000) == 0) {
                    func_800578DC(0xb2, 0);
                    uVar11 = 0;
                    goto LAB_800acdb8;
                }
            }
        }
        else {
            if (((DAT_800ff50d & 2) == 0) && ((DAT_801012c0 & 0x80) != 0)) {
                func_8005A3B4(3, 0x85, 2);
                func_8005B2B8(DAT_80103bb7);
                func_8005E350(DAT_80103bb7);
                func_8005AE90(DAT_80103bb7);
                DAT_80103bb7 = 0xff;
                DAT_801012c0 = DAT_801012c0 & 0xffffff7f;
            }
            if ((DAT_800ff50d & 4) == 0) {
                if ((_DAT_8010256c & 0x4000) != 0) {
                    _DAT_8010256c = _DAT_8010256c & 0xffffbfff;
                    func_8005A4FC(2, 0, 1);
                }
            }
            else {
                if ((_DAT_8010256c & 0x4000) == 0) {
                    func_800578DC(0xb2, 2);
                    uVar11 = 2;
                LAB_800acdb8:
                    func_8005A4FC(uVar11, 0, 0);
                }
            }
        }
        DAT_800ff504 = DAT_800ff504 + 1;
        break;
    case 1:
        if (((_DAT_80101e04 & 0x1800) != 0) && ((_DAT_80101e04 & 8) != 0)) {
            return 0x80;
        }
        if (((_DAT_8010256c & 0x1800) != 0) && ((_DAT_8010256c & 8) != 0)) {
            return 0x80;
        }
        if ((bVar1 & 1) == 0) {
            if ((DAT_800ff50d & 2) != 0) {
                uVar9 = DAT_801021b8;
                if ((DAT_801012c0 & 0x40) == 0) {
                    sVar4 = func_8005A76C(*(byte*)(DAT_800ff4f8 + 2) | 0x200);
                    func_8005A04C(1, 2, 0, 0x90, sVar4 + 0x10, 0x20);
                    func_8005A4FC(1, 0, 1);
                    func_8005A3B4(1, 2, 3);
                    DAT_801012c0 = DAT_801012c0 | 0x40;
                    DAT_80103bb5 = func_8005A98C(8, 0x98, 3, 0x12, &DAT_8001e208);
                    func_8005E2A4(DAT_80103bb5);
                    uVar9 = DAT_801021b8;
                }
                goto LAB_800ad01c;
            }
        }
        else {
            if ((DAT_800ff50d & 2) != 0) {
                uVar9 = DAT_80102920;
                if ((DAT_801012c0 & 0x80) == 0) {
                    iVar6 = func_8005A76C(*(byte*)(DAT_800ff4f8 + 2) | 0x200);
                    func_8005A04C(3, 2, (0x130 - iVar6) * 0x10000 >> 0x10, 0x40, iVar6 + 0x10U & 0xffff, 0x20);
                    func_8005A4FC(3, 0, 1);
                    func_8005A3B4(3, 2, 3);
                    DAT_801012c0 = DAT_801012c0 | 0x80;
                    DAT_80103bb7 = func_8005A98C(0xc0, 0x48, 3, 0x12, &DAT_8001e208);
                    func_8005E2A4(DAT_80103bb7);
                    uVar9 = DAT_80102920;
                }
            LAB_800ad01c:
                if ((uVar9 & 0x1800) != 0) {
                    return 0x80;
                }
            }
        }
        if (((DAT_800ff50d & 1) != 0) && (DAT_800fea58 != 0)) {
            iVar6 = CdFiles_AllSlotsEmpty();
            if (iVar6 == 0) {
                return 0x80;
            }
            if ((DAT_801012c0 & 0x400000) == 0) {
                bVar2 = DAT_801073eb;
                if (DAT_801073e8 != 0xff) {
                    func_80057834(DAT_801073e8, 0);
                    bVar2 = DAT_801073eb;
                }
            }
            else {
                bVar2 = DAT_801073e8;
                if (DAT_801073eb != 0xff) {
                    func_80057834(DAT_801073eb, 0);
                    bVar2 = DAT_801073e8;
                }
            }
            (&DAT_80103bd8)[(uint)bVar2 * 0x16] = (&DAT_80103bd8)[(uint)bVar2 * 0x16] & 0xfffffff8 | 1;
        }
        if ((DAT_800ff50d & 2) != 0) {
            if ((bVar1 & 1) == 0) {
                sVar4 = func_8005A76C(*(byte*)(DAT_800ff4f8 + 2) | 0x200);
                func_8005A480(1, 2, sVar4 + 0x10, 0x20, 0);
                func_8005B2B8(DAT_80103bb5);
                func_8005E0D4(1, *(byte*)(DAT_800ff4f8 + 2) | 0x200);
            }
            else {
                iVar6 = func_8005A76C(*(byte*)(DAT_800ff4f8 + 2) | 0x200);
                func_8005A480(3, 0x22, iVar6 + 0x10U & 0xffff, 0x20, 0);
                func_8005B2B8(DAT_80103bb7);
                func_8005BAD4(DAT_80103bb7, (0x138 - iVar6) * 0x10000 >> 0x10, 0x48);
                func_8005E0D4(3, *(byte*)(DAT_800ff4f8 + 2) | 0x200);
            }
        }
        if ((DAT_800ff50d & 8) == 0) {
            uVar11 = 0x3c;
            if ((DAT_800ff50d & 1) == 0) {
                uVar11 = 8;
            }
            uVar9 = DAT_801012c0 >> 0xc & 0xf | 0x80;
            uVar5 = DAT_800fea4c;
            goto LAB_800add88;
        }
        DAT_800fea50 = func_8005BC00(DAT_80103bb6, DAT_801012c0 >> 0xc & 0xf | 0x80, 8, 8, DAT_800fea4c);
        goto LAB_800add34;
    case 2:
        if (((ram0x00000042 & 100) == 0) && ((DAT_00000043._1_2_ & 8) == 0)) {
            return 0x80;
        }
        DAT_801012c0 = DAT_801012c0 & 0xffffffdf;
        if ((DAT_800ff50d & 8) != 0) {
            func_8005B2B8(DAT_80103bb6);
            DAT_800fea50 = func_8005BC00(DAT_80103bb6, DAT_801012c0 >> 0xc & 0xf | 0x80, 8, 8, DAT_800fea50);
            goto LAB_800add34;
        }
        func_8005B2B8(DAT_80103bb4);
        uVar11 = 0x3c;
        if ((DAT_800ff50d & 1) == 0) {
            uVar11 = 8;
        }
        uVar9 = DAT_801012c0 >> 0xc & 0xf | 0x80;
        uVar5 = DAT_800fea50;
        goto LAB_800add88;
    case 3:
        if (((ram0x00000042 & 100) == 0) && ((DAT_00000043._1_2_ & 8) == 0)) {
            return 0x80;
        }
        DAT_800ff504 = DAT_800ff504 + 1;
    case 4:
        iVar6 = CdFiles_AllSlotsEmpty();
        if (iVar6 == 0) {
            return 0x80;
        }
        if ((bVar1 & 1) == 0) {
            if ((DAT_800ff50d & 2) != 0) {
                sVar4 = func_8005A76C(DAT_801073e9 | 0x200);
                func_8005A480(1, 2, sVar4 + 0x10, 0x18, 0);
                func_8005B2B8(DAT_80103bb5);
                func_8005E0D4(1, DAT_801073e9 | 0x200);
            }
            uVar11 = 0x3c;
            if ((DAT_800ff50d & 1) == 0) {
                uVar11 = 8;
            }
            uVar9 = DAT_801012c0 >> 0xc & 0xf | 0xa0;
            uVar5 = DAT_800fea50;
        LAB_800add88:
            DAT_800fea50 = func_8005BC00(DAT_80103bb4, uVar9, uVar11, 8, uVar5);
        LAB_800add9c:
            func_8005B88C(DAT_80103bb4);
        }
        else {
            uVar9 = DAT_801012c0;
            uVar3 = DAT_801012c0;
            if ((DAT_800ff50d & 2) != 0) {
                sVar4 = func_8005A76C(DAT_801073ec | 0x200);
                func_8005A480(3, 2, sVar4 + 0x10, 0x18, 0);
                func_8005B2B8(DAT_80103bb7);
                func_8005E0D4(3, DAT_801073ec | 0x200);
                uVar9 = DAT_801012c0;
                uVar3 = DAT_801012c0;
            }
        LAB_800add20:
            DAT_801012c0 = uVar3;
            DAT_800fea50 = func_8005BC00(DAT_80103bb6, uVar9 >> 0xc & 0xf | 0xa0, 8, 8, DAT_800fea50);
        LAB_800add34:
            func_8005B88C(DAT_80103bb6);
        }
        func_800AC2B8();
        return 0x80;
    case 5:
        if (((ram0x00000042 & 100) != 0) || ((DAT_00000043._1_2_ & 8) != 0)) {
        LAB_800ad644:
            if ((bVar2 & 1) != 0) {
                func_80041F74(0xe000000);
                func_80041F74(0x10000000);
            }
            func_80071C5C(DAT_800fea54, 0x4000);
            func_80071C5C(DAT_800fea4c, 0x400);
            DAT_801012c0 = DAT_801012c0 & 0xffffffdf;
            return 1;
        }
        if ((DAT_8010001c == '\0') &&
            ((iVar6 = CdFiles_AllSlotsEmpty(), iVar6 != 0 && (iVar6 = func_80071DD0(0x3f4), iVar6 != 0)))
            ) {
            if ((DAT_801012c0 & 0xff800000) == 0) {
                if (DAT_800ff50d != 0) goto LAB_800ad644;
                goto LAB_800adf20;
            }
        }
        else {
        LAB_800adf20:
            if ((DAT_801012c0 & 0xff800000) == 0) {
                return 0x80;
            }
        }
    LAB_800adf2c:
        DAT_801012c0 = DAT_801012c0 & 0x7fffff | ((DAT_801012c0 >> 0x17) - 1) * 0x800000;
        break;
    case 6:
        if ((ram0x00000042 & 0x20) == 0) {
            if ((ram0x00000042 & 0x1000) != 0) {
                DAT_80100074 = 1;
                bVar1 = DAT_80107392;
                if (DAT_80107391 != 0) {
                    bVar1 = DAT_80107391;
                }
                DAT_80107391 = bVar1 - 1;
            }
            if ((ram0x00000042 & 0x4000) != 0) {
                DAT_80100074 = 1;
                uVar9 = (uint)DAT_80107391;
                DAT_80107391 = DAT_80107391 + 1;
                if (uVar9 == DAT_80107392 - 1) {
                    DAT_80107391 = 0;
                }
            }
            iVar6 = (DAT_800feb28 >> 1 & 1) * 0x28;
            uVar9 = _DAT_0000280e + 8 & 0xffff;
            if ((DAT_800ff50d & 1) == 0) {
                iVar13 = uVar9 * 8;
                *(short*)(&DAT_801072f0 + iVar6) = (char)(&DAT_800ed584)[DAT_80107390] + 8;
                *(ushort*)(&DAT_801072f2 + iVar6) = DAT_80107393 + 0xb0 + (DAT_80107391 & 3) * 0x10;
                pbVar12 = (byte*)(iVar13 + 0xe);
                *(ushort*)(&DAT_801072f8 + iVar6) =
                    (short)(char)(&DAT_800ed584)[DAT_80107390] + *pbVar12 + 8;
                *(ushort*)(&DAT_801072fa + iVar6) = DAT_80107393 + 0xb0 + (DAT_80107391 & 3) * 0x10;
                pbVar10 = (byte*)(iVar13 + 0xf);
                *(short*)(&DAT_80107300 + iVar6) = (char)(&DAT_800ed584)[DAT_80107390] + 8;
                *(ushort*)(&DAT_80107302 + iVar6) =
                    (ushort)*pbVar10 + (ushort)DAT_80107393 + (DAT_80107391 & 3) * 0x10 + 0xb0;
                cVar7 = (&DAT_800ed584)[DAT_80107390];
                sVar4 = *pbVar12 + 8;
            }
            else {
                iVar13 = uVar9 * 8;
                *(short*)(&DAT_801072f0 + iVar6) = (char)(&DAT_800ed584)[DAT_80107390] + 0x3c;
                *(ushort*)(&DAT_801072f2 + iVar6) = DAT_80107393 + 0xb0 + (DAT_80107391 & 3) * 0x10;
                pbVar12 = (byte*)(iVar13 + 0xe);
                *(ushort*)(&DAT_801072f8 + iVar6) =
                    (short)(char)(&DAT_800ed584)[DAT_80107390] + *pbVar12 + 0x3c;
                *(ushort*)(&DAT_801072fa + iVar6) = DAT_80107393 + 0xb0 + (DAT_80107391 & 3) * 0x10;
                pbVar10 = (byte*)(iVar13 + 0xf);
                *(short*)(&DAT_80107300 + iVar6) = (char)(&DAT_800ed584)[DAT_80107390] + 0x3c;
                *(ushort*)(&DAT_80107302 + iVar6) =
                    (ushort)*pbVar10 + (ushort)DAT_80107393 + (DAT_80107391 & 3) * 0x10 + 0xb0;
                cVar7 = (&DAT_800ed584)[DAT_80107390];
                sVar4 = *pbVar12 + 0x3c;
            }
            *(short*)(&DAT_80107308 + iVar6) = cVar7 + sVar4;
            *(ushort*)(&DAT_8010730a + iVar6) =
                (ushort)*pbVar10 + (ushort)DAT_80107393 + (DAT_80107391 & 3) * 0x10 + 0xb0;
            Gpu_AddPrim(&DAT_800fec44 +
                (DAT_800feb28 & 2) * 0x200 +
                (uint)(byte)(&DAT_800fe8bc)
                [*(ushort*)(&DAT_801012f8 + (uint)DAT_80103bb4 * 0xf) & 7] * 4,
                &DAT_801072e8 + iVar6);
            uVar9 = DAT_800feb28 >> 1 & 1;
            iVar6 = uVar9 * 0x24;
            uVar8 = 8;
            if ((DAT_800ff50d & 1) != 0) {
                uVar8 = 0x3c;
            }
            *(undefined2*)(&DAT_80107340 + iVar6) = uVar8;
            bVar2 = DAT_80107393;
            bVar1 = DAT_80107391;
            *(undefined2*)(&DAT_80107348 + iVar6) = 0x138;
            *(ushort*)(&DAT_80107342 + iVar6) = bVar2 + 0xb0 + (bVar1 & 3) * 0x10;
            bVar2 = DAT_80107393;
            bVar1 = DAT_80107391;
            *(undefined2*)(&DAT_80107350 + iVar6) = uVar8;
            *(ushort*)(&DAT_8010734a + iVar6) = bVar2 + 0xb0 + (bVar1 & 3) * 0x10;
            bVar2 = DAT_80107393;
            bVar1 = DAT_80107391;
            *(undefined2*)(&DAT_80107358 + iVar6) = 0x138;
            *(ushort*)(&DAT_80107352 + iVar6) = (ushort)bVar2 + (bVar1 & 3) * 0x10 + 0xc0;
            *(ushort*)(&DAT_8010735a + iVar6) = (ushort)DAT_80107393 + (DAT_80107391 & 3) * 0x10 + 0xc0;
            Gpu_AddPrim(&UNK_800fec48 +
                (uint)(byte)(&DAT_800fe8bc)
                [*(ushort*)(&DAT_801012f8 + (uint)DAT_80103bb4 * 0xf) & 7] * 4 +
                uVar9 * 0x400, &DAT_80107338 + iVar6);
            uVar9 = DAT_800feb28 >> 1 & 1;
            Gpu_AddPrim(&UNK_800fec48 +
                (uint)(byte)(&DAT_800fe8bc)
                [*(ushort*)(&DAT_801012f8 + (uint)DAT_80103bb4 * 0xf) & 7] * 4 +
                uVar9 * 0x400, (void*)(uVar9 * 8 + -0x7fef8c80));
            return 0x80;
        }
        DAT_80100074 = 2;
        func_80071C5C(DAT_800fea54, 0x4000);
        func_80071C5C(DAT_800fea4c, 0x400);
        return 1;
    case 7:
        if (((ram0x00000042 & 100) == 0) && ((DAT_00000043._1_2_ & 8) == 0)) {
            return 0x80;
        }
        DAT_800ff504 = DAT_800ff504 + 1;
    case 8:
        iVar6 = CdFiles_AllSlotsEmpty();
        if (iVar6 == 0) {
            return 0x80;
        }
        if ((DAT_800ff50d & 8) != 0) {
            uVar9 = DAT_801012c0 >> 0xc;
            DAT_801012c0 = DAT_801012c0 & 0xffffffdf;
            DAT_800fea50 = func_8005BC00(DAT_80103bb6, uVar9 & 0xf | 0xa0, 8, 8, DAT_800fea50);
            goto LAB_800add34;
        }
        uVar11 = 0x3c;
        if ((DAT_800ff50d & 1) == 0) {
            uVar11 = 8;
        }
        uVar9 = DAT_801012c0 >> 0xc;
        DAT_801012c0 = DAT_801012c0 & 0xffffffdf;
        DAT_800fea50 = func_8005BC00(DAT_80103bb4, uVar9 & 0xf | 0xa0, uVar11, 8, DAT_800fea50);
        goto LAB_800add9c;
    case 9:
        if (DAT_80103bd1 != '\0') {
            DAT_80103bd1 = DAT_80103bd1 + -1;
            return 0x80;
        }
        uVar9 = DAT_801012c0;
        uVar3 = DAT_801012c0 & 0xffffffdf;
        if ((DAT_800ff50d & 8) == 0) {
            if ((DAT_800ff50d & 1) == 0) {
                uVar11 = 8;
            }
            else {
                uVar11 = 0x3c;
            }
            uVar9 = DAT_801012c0 >> 0xc & 0xf | 0xa0;
            uVar5 = DAT_800fea50;
            DAT_801012c0 = DAT_801012c0 & 0xffffffdf;
            goto LAB_800add88;
        }
        goto LAB_800add20;
    case 10:
        func_80071C5C(DAT_800fea54, 0x4000);
        func_80071C5C(DAT_800fea4c, 0x400);
        DAT_801012c0 = DAT_801012c0 & 0xffffffdf;
        return 1;
    case 0xb:
        if (((ram0x00000042 & 100) != 0) || ((DAT_00000043._1_2_ & 8) != 0)) {
        LAB_800adec0:
            if ((bVar2 & 1) != 0) {
                func_80041F74(0xe000000);
                func_80041F74(0x10000000);
            }
            func_80071C5C(DAT_800fea54, 0x4000);
            func_80071C5C(DAT_800fea4c, 0x400);
            DAT_801012c0 = DAT_801012c0 & 0xffffffdf;
            return 1;
        }
        if (DAT_8010001c == '\0') {
            iVar6 = CdFiles_AllSlotsEmpty();
            if ((iVar6 != 0) && ((DAT_801012c0 & 0xff800000) == 0)) goto LAB_800adec0;
            if ((DAT_8010001c == '\0') &&
                ((iVar6 = CdFiles_AllSlotsEmpty(), iVar6 != 0 && (iVar6 = func_80071DD0(0x3f4), iVar6 != 0)
                    ))) {
                if ((DAT_801012c0 & 0xff800000) != 0) goto LAB_800adf2c;
                if (DAT_800ff50d != 0) goto LAB_800adec0;
            }
        }
        goto LAB_800adf20;
    case 0xc:
        DAT_800ff504 = DAT_800ff504 + 1;
        break;
    case 0xd:
        DAT_800ff504 = 5;
        break;
    case 0xe:
        DAT_800ff504 = DAT_800ff504 + 1;
        break;
    case 0xf:
        DAT_800ff504 = 0xb;
    }
    return 0x80;
}

undefined4 func_800ADFB0(void)

{
    uint uVar1;

    if (DAT_800ff504 == 0) {
        if ((DAT_800ff50d & 0x80) == 0) {
            if ((DAT_800ff50d & 8) == 0) {
                func_8005A3B4(0, 0x85, 8);
                func_8005B2B8(DAT_80103bb4);
                if ((DAT_801012c0 & 0x40) != 0) {
                    func_8005A3B4(1, 0x85, 8);
                    func_8005B2B8(DAT_80103bb5);
                }
                _DAT_80101e04 = _DAT_80101e04 & 0xffffffef;
                func_80057834(0xff, 0);
                func_8005E028(0xff, 0);
                _DAT_80101e04 = _DAT_80101e04 & 0xffffbfff;
            }
            else {
                func_8005A3B4(2, 0x85, 8);
                func_8005B2B8(DAT_80103bb6);
                if ((DAT_801012c0 & 0x80) != 0) {
                    func_8005A3B4(3, 0x85, 8);
                    func_8005B2B8(DAT_80103bb7);
                }
                _DAT_8010256c = _DAT_8010256c & 0xffffffef;
                func_80057834(0xff, 2);
                func_8005E028(0xff, 2);
                _DAT_8010256c = _DAT_8010256c & 0xffffbfff;
            }
        }
        else {
            if ((_DAT_80101e04 & 8) != 0) {
                func_8005A3B4(0, 0x85, 8);
                func_8005B2B8(DAT_80103bb4);
                if ((DAT_801012c0 & 0x40) != 0) {
                    func_8005A3B4(1, 0x85, 8);
                    func_8005B2B8(DAT_80103bb5);
                }
                _DAT_80101e04 = _DAT_80101e04 & 0xffffffef;
                func_80057834(0xff, 0);
                func_8005E028(0xff, 0);
                _DAT_80101e04 = _DAT_80101e04 & 0xffffbfff;
            }
            if ((_DAT_8010256c & 8) != 0) {
                func_8005A3B4(2, 0x85, 8);
                func_8005B2B8(DAT_80103bb6);
                if ((DAT_801012c0 & 0x80) != 0) {
                    func_8005A3B4(3, 0x85, 8);
                    func_8005B2B8(DAT_80103bb7);
                }
                _DAT_8010256c = _DAT_8010256c & 0xffffffef;
                func_80057834(0xff, 2);
                func_8005E028(0xff, 2);
                _DAT_8010256c = _DAT_8010256c & 0xffffbfff;
            }
        }
    }
    else {
        if (DAT_800ff504 == 7) {
            if ((DAT_800ff50d & 0x80) != 0) {
                if ((DAT_801012c0 & 0x40) != 0) {
                    func_8005E350(DAT_80103bb5);
                    func_8005AE90(DAT_80103bb5);
                    DAT_80103bb5 = 0xff;
                }
                if ((DAT_801012c0 & 0x80) != 0) {
                    func_8005E350(DAT_80103bb7);
                    func_8005AE90(DAT_80103bb7);
                    DAT_80103bb7 = 0xff;
                }
                DAT_801073e8 = 0xff;
                DAT_801012c0 = DAT_801012c0 & 0xffffff38;
                func_8005E400();
                DAT_801073eb = 0xff;
                DAT_801012c0 = DAT_801012c0 & 0xffbfffff;
                func_8005E4DC();
                return 0x81;
            }
            if ((DAT_800ff50d & 8) != 0) {
                if ((DAT_801012c0 & 0x80) != 0) {
                    func_8005E350(DAT_80103bb7);
                    func_8005AE90(DAT_80103bb7);
                    DAT_80103bb7 = 0xff;
                }
                uVar1 = DAT_801012c0 & 0xfffffffb;
                if (((DAT_801012c0 & 0xfffffffb) >> 1 & 1) == 0) {
                    uVar1 = DAT_801012c0 & 0xfffffffa;
                }
                DAT_801012c0 = uVar1;
                DAT_801073eb = 0xff;
                DAT_801012c0 = DAT_801012c0 & 0xffbfff7f;
                func_8005E4DC();
                return 0x81;
            }
            if ((DAT_801012c0 & 0x40) != 0) {
                func_8005E350(DAT_80103bb5);
                func_8005AE90(DAT_80103bb5);
                DAT_80103bb5 = 0xff;
            }
            uVar1 = DAT_801012c0 & 0xfffffffb;
            if (((DAT_801012c0 & 0xfffffffb) >> 1 & 1) == 0) {
                uVar1 = DAT_801012c0 & 0xfffffffa;
            }
            DAT_801012c0 = uVar1;
            DAT_801073e8 = 0xff;
            DAT_801012c0 = DAT_801012c0 & 0xffbfffbf;
            func_8005E400();
            return 0x81;
        }
    }
    DAT_800ff504 = DAT_800ff504 + 1;
    return 0x80;
}

undefined4 func_800AE480(void)

{
    byte bVar1;
    char cVar2;
    int iVar3;
    uint uVar4;

    cVar2 = DAT_800ff50e;
    bVar1 = DAT_800ff50d;
    uVar4 = (uint)DAT_800ff50d;
    if (DAT_800ff504 == 1) {
        iVar3 = CdFiles_AllSlotsEmpty();
        if (iVar3 != 0) {
            if ((DAT_801012c0 & 0x400000) == 0) {
                if (DAT_801073e8 != -1) {
                    func_80057834(DAT_801073e8, 0);
                }
                func_80056C6C(DAT_800fea4c, cVar2, 0);
                DAT_801073e8 = func_80056D88(0);
                DAT_801073e9 = bVar1;
                DAT_801073ea = cVar2;
            }
            else {
                if (DAT_801073eb != -1) {
                    func_80057834(DAT_801073eb, 0);
                }
                func_80056C6C(DAT_800fea4c, cVar2, 1);
                DAT_801073eb = func_80056D88(1);
                DAT_801073ec = bVar1;
                DAT_801073ed = cVar2;
            }
            DAT_801012c0 = DAT_801012c0 & 0xffbfffff | (DAT_801012c0 >> 0x16 & 1 ^ 1) << 0x16;
            DAT_800ff504 = DAT_800ff504 + 1;
        }
    }
    else {
        if (1 < DAT_800ff504) {
            if (DAT_800ff504 == 2) {
                func_80071C5C(DAT_800fea4c, 0x4000);
                return 0x81;
            }
            if (DAT_800ff504 == 3) {
                if ((DAT_801012c0 & 0x400000) == 0) {
                    if (DAT_801073e8 != -1) {
                        func_80057834(DAT_801073e8, 0);
                    }
                    DAT_801073e8 = func_80056D88(0);
                }
                else {
                    if (DAT_801073eb != -1) {
                        func_80057834(DAT_801073eb, 0);
                    }
                    DAT_801073eb = func_80056D88(1);
                }
                DAT_801012c0 = DAT_801012c0 & 0xffbfffff | (DAT_801012c0 >> 0x16 & 1 ^ 1) << 0x16;
                return 1;
            }
            return 0x80;
        }
        if (DAT_800ff504 != 0) {
            return 0x80;
        }
        if ((DAT_801012c0 & 0x400000) == 0) {
            if (((DAT_801073e8 != -1) && (DAT_801073e9 == *(byte*)(DAT_800ff4f8 + 2))) &&
                (DAT_801073ea == *(char*)(DAT_800ff4f8 + 3))) {
                DAT_800ff504 = 3;
                return 0x80;
            }
        }
        else {
            if (((DAT_801073eb != -1) && (DAT_801073ec == *(byte*)(DAT_800ff4f8 + 2))) &&
                (DAT_801073ed == *(char*)(DAT_800ff4f8 + 3))) {
                DAT_800ff504 = 3;
                return 0x80;
            }
        }
        DAT_800fea4c = func_800714A4(0x4000);
        CdFiles_RequestCdFile('0', 7, uVar4, 0, -1, DAT_800fea4c);
        DAT_800ff504 = DAT_800ff504 + 1;
    }
    return 0x80;
}