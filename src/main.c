#include "global.h"

void Main_DummyFunc() {
    return;
}

void main(void) {

    RECT rect;

    Main_DummyFunc();
    EtcVid_SetVideoMode(MODE_NTSC);
    EtcIntr_ResetCallback();
    PsApi_ExitCriticalSection();
    EtcVid_VSync(VSYNC_MODE_BLOCK);
    GpuSys_SetDispMask(FALSE);  //Sets whether display mask is displayed
    GpuSys_ResetGraph(0); //Complete reset

    rect.x = 0; rect.y = 0;
    rect.w = 640; rect.h = 511;
    GpuSys_ClearImage(&rect, 0, 0, 0);
    GpuSys_DrawSync(0);

    sApplState.stateFlags &= ~0xe0 | 0x20;
    Heap_ResetArena();

    func_8003BBB4();
    MCSave_InitMCContext();

    uint uVar1;
    uint uVar2;
    int iVar3;
    uint* ot;
    undefined2 uStack112;
    undefined2 uStack110;
    undefined2 uStack108;
    undefined2 uStack106;
    undefined4 uStack104;
    undefined* puStack100;
    undefined4* puStack96;
    undefined4* puStack92;
    undefined4* puStack88;
    undefined4 uStack84;
    undefined4 uStack80;
    undefined4 uStack76;
    code* pcStack72;
    undefined2 local_30;
    undefined2 local_2e;
    undefined2 local_2c;
    undefined2 local_2a;

    /* Copy storage registers and $ra to stack */
    _entry();
    EtcVid_SetVideoMode(0);
    EtcIntr_ResetCallback();
    PsApi_ExitCriticalSection();
    EtcVid_VSync(0);
    GpuSys_SetDispMask(0);
    GpuSys_ResetGraph(0);
    local_30 = 0;
    local_2e = 0;
    local_2c = 0x280;
    local_2a = 0x1ff;
    GpuSys_ClearImage(&local_30, '\0', '\0', '\0');
    GpuSys_DrawSync(0);
    DAT_800feb28 = DAT_800feb28 & 0xffffff1f | 0x20;
    Heap_ResetArena();
    func_8003BBB4();
    func_80072404();
    //@
    PadMain_padStartCom();
    CdFiles_ClearLoadRequests();
    func_800405A4();
    func_80039D48();
    GpuSys_DrawSync(0);
    EtcVid_VSync(0);
    PsApi_Counter_StartRCnt(0);
    func_8003ADB8();
    func_80077010(0);
    Heap_ClearAllFlags_ff722_71CD0();
    DAT_800ff444 = 0;
    DAT_800ff446 = 0;
    DAT_800feb28 = DAT_800feb28 & 0x20e695e3 | 5;
    DAT_800feb2c = DAT_800feb2c & 0xf9c40000 | 0x8000000;
    DAT_800feb30 = DAT_800feb30 & 0xfffffffe;
    Heap_prng_seed_71874();
    DAT_800feb54 = 0;
    DAT_800feb44 = 0;
    DAT_800feb28 = DAT_800feb28 & 0xfffffeff;
    do {
        func_80039F80();
        func_8003AF4C();
        func_80076EF8();
        func_80072ABC();
        func_80048B6C();
        if ((DAT_801012c0 & 1) != 0) {
            func_8005DA30();
        }
        func_8003C2BC();
        func_8005F298();
        func_80076DCC();
        if ((DAT_800feb2c >> 0xf & 1) != 0) {
            func_8006D348();
        }
        func_80069B98();
        func_8005B3E4();
        func_80052100();
        func_80040994();
        DAT_800fea9d = func_80054994();
        if (DAT_80100074 != 0xffff) {
            func_80041AF8(DAT_80100074 >> 8, DAT_80100074 & 0xff, 0x7c, 0x7c);
            DAT_80100074 = 0xffff;
        }
        if ((DAT_800feb2c >> 0x14 & 1) != 0) {
            EtcVid_VSync(0);
            GpuSys_SetDispMask(0);
            func_80076DA0();
            func_8003BDFC(0, 0);
            func_800407CC(0);
            func_8003A230();
            func_8003FE5C();
            CdDrive_CdReadyCallback((void*)0x0);
            CdDrive_CdControlB('\t', (void*)0x0, (void*)0x0);
            EtcVid_VSync(3);
            Heap_ResetArena();
            func_8003FDF8();
            func_800405A4();
            func_80039D48();
            func_8003ADB8();
            DAT_800feb28 = DAT_800feb28 & 0xe7ffffff;
            func_8004893C();
            DAT_800ff444 = 0;
            DAT_800ff446 = 0;
            DAT_80107391 = 0;
            DAT_801077bc = 0;
            DAT_801077bd = 0;
            DAT_801077be = 0;
            DAT_801077c0 = 0;
            DAT_80107a58 = 0;
            DAT_80107a59 = 0;
            DAT_800feb28 = DAT_800feb28 & 0x3f6607e2 | 4;
            DAT_800feb2c = DAT_800feb2c & 0xffd43fe1;
            DAT_801012c0 = DAT_801012c0 & 0x3fffdf;
            func_80077010(1);
            DAT_800feb28 = DAT_800feb28 & 0xffffff1f | 0x20;
            DAT_800feb2c = DAT_800feb2c & 0xffefffff;
            func_80041F74(0x7000000);
            Heap_prng_seed_71874();
        }
        DAT_800feb5c = DAT_800feb5c + 1;
        uVar1 = DAT_800feb28 & 0xfffffffd | (DAT_800feb5c & 1) << 1;
        uVar2 = DAT_800feb28 >> 8;
        DAT_800feb28 = uVar1;
    } while ((uVar2 & 1) == 0);
    GpuSys_DrawSyncCallback((void*)0x0);
    EtcVid_VSync(0);
    GpuSys_DrawSync(0);
    EtcVid_VSync(0);
    GpuSys_SetDispMask(0);
    GpuSys_ResetGraph(1);
    func_800407CC(1);
    func_8003FE5C();
    PsApi_Counter_StopRCnt(0xf2000000);
    PsApi_Counter_StopRCnt(0xf2000001);
    PsApi_Counter_StopRCnt(0xf2000002);
    iVar3 = -0xdfffffd;
    PsApi_Counter_StopRCnt();
    LibCard_unk_800C8BB8();
    PadMain_padStopCom();
    EtcIntr_StopCallback();
    pcStack72 = func_80039D48;
    LibC_exit(iVar3);
}