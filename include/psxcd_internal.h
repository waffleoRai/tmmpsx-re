#ifndef PSXCD_INTERNAL_H
#define PSXCD_INTERNAL_H

#include "psxcd.h"
#include "psx_bios.h"

/*----- Enums -----*/

/*----- Struct Defs -----*/

typedef struct CdDriveDirRecord {
	/*0x00*/ u32 recordIndex;
	/*0x04*/ u32 parentIndex;
	/*0x08*/ s32 sector;
	/*0x0c*/ char name[0x20];
} CdDriveDirRecord; /*0x2c*/

//This isn't a real struct since it's in the data part of the exe. But I'm using this for clustering.
typedef struct CDDriveData {
	/*0x0000*/ void* unk_fd204; //0x800fd204, looks like a callback: CdDrive_Init
	/*0x0004*/ u32 unk_fd208; //0x800fd208: CdDrive_Init
	/*0x0008*/ u8 unk_fd20c[8]; //0x800fd20c
	/*0x0010*/ int32_t cachedDirId; //0x800fd214, in CDDrive_cacheFile checked against arg0
	/*0x0014*/ void* unk_fd218[31]; //0x800fd218: CdDrive_SearchFile. -1, NULL, NULL, then ptrs to the CD I/O registers over and over
	/*0x0090*/ void* ptrCdStat; //0x800fd294
	/*0x0094*/ void* ptrCdIo1; //0x800fd298
	/*0x0098*/ void* ptrCdIo2; //0x800fd29c
	/*0x009c*/ void* ptrCdIo3; //0x800fd2a0
	/*0x00a0*/ void* ptrCdDelay; //0x800fd2a4
	/*0x00a4*/ void* ptrCmnDelay; //0x800fd2a8
	/*0x00a8*/ void* ptrDpcr; //0x800fd2ac
	/*0x00ac*/ void* ptrDicr; //0x800fd2b0
	/*0x00b0*/ void* ptrD1Chcr; //0x800fd2b4
	/*0x00b4*/ void* ptrD1Maddr; //0x800fd2b8
	/*0x00b8*/ void* ptrD2Chcr; //0x800fd2bc
	/*0x00bc*/ void* ptrD2Maddr; //0x800fd2c0
	/*0x00c0*/ void* ptrD3Chcr; //0x800fd2c4
	/*0x00c4*/ void* ptrD3Maddr; //0x800fd2c8
	/*0x00c8*/ void* ptrD5Chcr; //0x800fd2cc
	/*0x00cc*/ void* ptrD5Maddr; //0x800fd2d0
	/*0x00d0*/ void* ptrIStat; //0x800fd2d4
	/*0x00d4*/ void* ptrIMask; //0x800fd2d8
	/*0x00d8*/ s32 unk_fd2dc; //0x800fd2dc
	/*0x00dc*/ u8 unk_fd2e0[12]; //0x800fd2e0
	/*0x00e8*/ void* syncCallback; //0x800fd2ec, looks like a callback.
	/*0x00ec*/ void* readyCallback; //0x800fd2f0, looks like a callback.
	/*0x00f0*/ s32 verbosity; //0x800fd2f4
	/*0x00f4*/ u8 unk_fd2f8; //0x800fd2f8, readCallback?
	/*0x00f5*/ u8 unk_fd2f9[3]; //0x800fd2f9
	/*0x00f8*/ s32 unk_fd2fc; //0x800fd2fc
	/*0x00fc*/ void* unk_fd300; //0x800fd300: CdDrive_SearchFile
	/*0x0100*/ u8 unk_fd304; //0x800fd304
	/*0x0101*/ u8 unk_fd305[3]; //0x800fd305
	/*0x0104*/ u8 unk_fd308; //0x800fd308 // CD Read mode
	/*0x0105*/ CdDriveCommand lastCommand; //0x800fd309
	/*0x0106*/ u8 unk_fd30a[0x6]; //0x800fd30a
	/*0x010c*/ char* commandName[32]; //0x800fd310
	/*0x018c*/ char* intrStatusName[8]; //0x800fd390
	/*0x01ac*/ u8 unk_fd3b0[0x80]; //0x800fd3b0
	/*0x022c*/ u8 unk_fd430[0x80]; //0x800fd430
	/*0x02ac*/ u32 unk_fd4b0[32]; //0x800fd4b0
	/*0x032c*/ u32 unk_fd530[32]; //0x800fd530
	/*0x03ac*/ void* unk_fd5b0; //0x800fd5b0 - Another ptr to CDSTAT
	/*0x03b0*/ void* unk_fd5b4; //0x800fd5b4 - Another ptr to CD3
	/*0x03b4*/ void* unk_fd5b8; //0x800fd5b8 - Another ptr to COMMON_DELAY
	/*0x03b8*/ void* unk_fd5bc; //0x800fd5bc - Another ptr to CD1
	/*0x03bc*/ void* unk_fd5c0; //0x800fd5c0 - Another ptr to CD2
	/*0x03c0*/ void* unk_fd5c4; //0x800fd5c4 - Ptr to SPU_V0_VOL_L
	/*0x03c4*/ CdDriveIntrStatus intrStatusSync; //0x800fd5c8
	/*0x03c5*/ CdDriveIntrStatus intrStatusReady; //0x800fd5c9
	/*0x03c6*/ CdDriveIntrStatus intrStatusEnd; //0x800fd5ca
	/*0x03c7*/ u8 unk_fd5cb; //0x800fd5cb
	/*0x03c8*/ void* unk_fd5cc; //0x800fd5cc - Ptr to 0x800fd5c8
	/*0x03cc*/ void* unk_fd5d0; //0x800fd5d0 - Ptr to 0x8010e900
	/*0x03d0*/ void* unk_fd5d4; //0x800fd5d4 - Ptr to 0x800fd309
	/*0x03d4*/ void* unk_fd5d8; //0x800fd5d8 - Ptr to 0x800fd2f8
	/*0x03d8*/ void* unk_fd5dc; //0x800fd5dc - Ptr to 0x800fd304
	/*0x03dc*/ void* unk_fd5e0; //0x800fd5e0 - Ptr to a string in rodata
	/*0x03e0*/ void* unk_fd5e4; //0x800fd5e4 - Ptr to 0x1f8010b8
	/*0x03e4*/ u8 unk_fd5e8[12]; //0x800fd5e8
	/*0x03f0*/ s32 unk_fd5f4[32]; //0x800fd5f4
	/*0x0470*/ void* unk_fd674; //0x800fd674 - Ptr to 0x1f801800
	/*0x0474*/ void* unk_fd678; //0x800fd678 - Ptr to 0x1f801803
	/*0x0478*/ void* unk_fd67c; //0x800fd67c - Ptr to 0x1f801020
	/*0x047c*/ void* unk_fd680; //0x800fd680 - Ptr to 0x1f801018
	/*0x0480*/ void* unk_fd684; //0x800fd684 - Ptr to 0x1f8010f0
	/*0x0484*/ void* unk_fd688; //0x800fd688 - Ptr to 0x1f8010b8
	/*0x0488*/ void* unk_fd68c; //0x800fd68c - Ptr to 0x1f8010b0
	/*0x048c*/ void* unk_fd690; //0x800fd690 - Ptr to 0x1f8010b4
	/*0x0490*/ void* unk_fd694; //0x800fd694 - Ptr to 0x1f801800
	/*0x0494*/ void* unk_fd698; //0x800fd698 - Ptr to 0x1f801803
	/*0x0498*/ void* unk_fd69c; //0x800fd69c - Ptr to 0x1f801020
	/*0x049c*/ void* unk_fd6a0; //0x800fd6a0 - Ptr to 0x1f801018
	/*0x04a0*/ void* unk_fd6a4; //0x800fd6a4 - Ptr to 0x1f8010f0
	/*0x04a4*/ void* unk_fd6a8; //0x800fd6a8 - Ptr to 0x1f8010b8
	/*0x04a8*/ void* unk_fd6ac; //0x800fd6ac - Ptr to 0x1f8010b0
	/*0x04ac*/ void* unk_fd6b0; //0x800fd6b0 - Ptr to 0x1f8010b4
	/*0x0000*/ u32 unk_fd6b4; //0x800fd6b4
	/*0x0000*/ u32 unk_fd6b8; //0x800fd6b8
	/*0x0000*/ u32 unk_fd6bc; //0x800fd6bc
	/*0x0000*/ u32 unk_fd6c0; //0x800fd6c0
	/*0x0000*/ u32 unk_fd6c4; //0x800fd6c4
	/*0x0000*/ u32 unk_fd6c8; //0x800fd6c8
	/*0x0000*/ u32 unk_fd6cc; //0x800fd6cc
	/*0x0000*/ u32 unk_fd6d0; //0x800fd6d0
	/*0x0000*/ u32 unk_fd6d4; //0x800fd6d4
	/*0x0000*/ u32 unk_fd6d8; //0x800fd6d8
	/*0x0000*/ u32 unk_fd6dc; //0x800fd6dc
	/*0x0000*/ u32 unk_fd6e0; //0x800fd6e0
	/*0x0000*/ u32 unk_fd6e4; //0x800fd6e4
	/*0x0000*/ u32 unk_fd6e8; //0x800fd6e8
	/*0x0000*/ u8 unk_fd6ec[8]; //0x800fd6ec
} CDDriveData;

typedef struct CDDriveContext {
	/*0x00*/ u32 unk_10c490;
	/*0x00*/ u32 unk_10c494;
	/*0x00*/ u8 unk_10c498[4];
	/*0x00*/ u32 unk_10c49c;
	/*0x00*/ u8 unk_10c4a0[12];
	/*0x00*/ u32 unk_10c4ac;
	/*0x00*/ u8 unk_10c4b0[4];
	/*0x00*/ u32 unk_10c4b4;
	/*0x00*/ u32 unk_10c4b8;
	/*0x00*/ u32 unk_10c4bc;
	/*0x00*/ u8 unk_10c4c0[16];
	/*0x00*/ u32 unk_10c4d0;
	/*0x00*/ u32 unk_10c4d4;
	/*0x00*/ CdlFILE fileCache[64]; //0x8010c4d8 - if 64, would be 8010cad8? end. mm maybe not?
	/*0x00*/ CdDriveDirRecord pathTable[128]; //0x8010cad8 size 0x1600 Directories?
	/*0x00*/ u8 secdataBuffer[0x800]; //0x8010e0d8 - This looks like a buffer for sector data: CdDrive_newMedia
	/*0x00*/ u8 unk_10e8d8[0x28]; //0x8010e8d8
	/*0x00*/ u8 responseQueueSync[8]; //0x8010e900
	/*0x00*/ u8 responseQueueReady[8]; //0x8010e908
	/*0x00*/ u8 responseQueueEnd[8]; //0x8010e910
	/*0x00*/ u32 unk_10e918; //0x8010e918
	/*0x00*/ u32 timeoutCounter; //0x8010e91c
	/*0x00*/ char* activeSubName; //0x8010e920
} CDDriveContext;

/*----- Data -----*/

/*----- Stream Lib -----*/

/*----- CD Lib -----*/

int CdDrive_CdBiosInit();
int CdDrive_CdBiosInitVol();


#endif //PSXCD_INTERNAL_H