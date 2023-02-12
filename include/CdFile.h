#ifndef SLPM87176_CDFILE_H
#define SLPM87176_CDFILE_H

#include "psx_bios.h"
#include "psxetc.h"
#include "psxgpu.h"
#include "psxcd_internal.h" //Might need internal, we'll see.
#include "pcdebug.h"
#include "psxpress.h"
#include "psxsnd.h"

#include "exemisc.h"

//These are guesses.
#define FILE_MODE_FLAG_XASTREAM 0x80
#define FILE_MODE_FLAG_AUDIO 0x40 //??? Used only for BGM and SE
#define FILE_MODE_FLAG_LZ77 0x20
#define FILE_MODE_FLAG_INDEX_OFFSET 0x10

#define FILE_MEDIUM_PC 0
#define FILE_MEDIUM_PSXCD 2

#define PREFIX_FILENAME(dst_ptr, src_ptr, filename) src_ptr = CDFILE_PATH_PRE; \
		STRCPY_INCR(dst_ptr, src_ptr); \
		src_ptr = filename; \
		STRCPY_INCR(dst_ptr, src_ptr); \
	    *dst_ptr++ = '\0';

#define SUFFIX_FILENAME(dst_ptr, src_ptr) STRCPY_INCR(dst_ptr, src_ptr); \
		src_ptr = CDFILE_PATH_SUF; \
		STRCPY_INCR(dst_ptr, src_ptr); \
		*dst_ptr++ = '\0';

/*----- Constants -----*/

const char* CDFILE_PATH_PRE = "i:\\cd";
const char* CDFILE_PATH_SUF = ";10";

extern u16* globalptr; //PLACEHOLDER

typedef enum GameFileID {
	NoFile, // 0
	D_Unit, // 1
	D_Screen, // 2
	D_System, // 3
	D_Sce, // 4
	D_Anime, // 5
	D_Effect, // 6
	D_Face, // 7
	D_Field, // 8
	Movie, // 9
	D_Bgm, // 10
	D_Se, // 11
	VoiceStr // 12
} GameFileID;

typedef enum ReqStatus {
	ReqStatus_Idle, //0
	unk01, //1,
	ReqStatus_Error, //2
	ReqStatus_PendingData = 6, //6
	ReqStatus_PendingData_PC, //7
	unk08, //8
	unk09, //9
	unk10, //10
	unk11, //11
	unk12, //12
	unk13, //13 Audio, PC, 1
	unk14, //14
	unk15, //15 Audio, CD, 1
	unk16, //16 Audio, CD, 2
	unk17 //17
} ReqStatus;

typedef enum FileLoaderState {
	LoaderState_unk00, //0
} FileLoaderState;

typedef struct CdFileLoadRequest {
	/*0x00*/ u8 mode; //0x800ffde0 (E)
	/*0x01*/ ReqStatus status; //0x800ffde1 (F)
	/*0x02*/ s16 fileId; //0x800ffde2 (G)
	/*0x04*/ u32 fileOffset; //0x800ffde4 (H), looks like offset from start of indiv file. Or maybe the file index within arc?
	/*0x08*/ s32 unk_08; //0x800ffde8
	/*0x0c*/ s32 len; //0x800ffdec //bytes remaining?
	/*0x10*/ byte* dst; //0x800ffdf0
} CdFileLoadRequest; /*0x14*/

typedef struct CdFileContext {
	/*Actual struct probably starts here*/
	/*0x0000*/ s16 fileMedium; //0x800ffd08 "J"
	/*0x0000*/ u16 unk_ffd0a; //0x800ffd0a
	/*0x0000*/ u16 unk_ffd0c; //0x800ffd0c
	/*0x0000*/ u16 slotsUsed; //0x800ffd0e "A" Maybe a counter for how many slots in use?
	/*0x0000*/ u16 nextSlot; //0x800ffd10 "B"
	/*0x0000*/ u16 slotIndex; //0x800ffd12 "C"
	/*0x0000*/ u8 unk_ffd14; //0x800ffd14 "D"
	/*0x0000*/ u8 unk_ffd15; //0x800ffd15 Some kind of overall status enum?
	/*0x0000*/ u8 unk_ffd16; //0x800ffd16
	/*0x0000*/ u8 unk_ffd17; //0x800ffd17
	/*0x0000*/ u8 driveResponse[8]; //0x800ffd18 Looks like it stores responses to commands for the drive
	/*0x0000*/ u32 unk_ffd20; //0x800ffd20
	/*0x0000*/ u8 unk_ffd24[4]; //0x800ffd24
	/*0x0000*/ u32 loadOffset; //0x800ffd28 - USually has request.fileOffset (or file index) written to it?
	/*0x0000*/ u32 loadRemaining; //0x800ffd2c - Has request.unk_0c written to it
	/*0x0000*/ byte* loadDest; //0x800ffd30 Has request.unk_10 written to it
	/*0x0000*/ byte* comprDataBuffer; //0x800ffd34
	/*0x0000*/ byte* comprFileBuffer; //0x800ffd38
	/*0x0000*/ byte* decompDataEnd; //0x800ffd3c - Maybe end of buffer, actually?
	/*0x0000*/ CdlLOC unk_ffd40; //0x800ffd40 - Often used as control params.
	/*0x0000*/ CdlLOC unk_ffd44; //0x800ffd44
	/*0x0000*/ s32 reqSector; //0x800ffd48
	/*0x0000*/ u32 unk_ffd4c; //0x800ffd4c
	/*0x0000*/ u8 unk_ffd50[4]; //0x800ffd50
	/*0x0000*/ u32 reqSectorOffset; //0x800ffd54
	/*0x0000*/ CdlLOC fileLocs[13]; //0x800ffd58. Looks like it holds file positions (in BCD time)
	/*0x0000*/ u32 fileSizes[13]; //0x800ffd8c, "L". Looks like it holds file sizes. Ends at ffdc0
	/*0x0000*/ CdlATV unk_ffdc0; //0x800ffdc0 used in audio stuff?
	/*0x0000*/ u8 unk_ffdc4; //0x800ffdc4 used for CdControl params during streaming VOICE
	/*0x0000*/ u8 unk_ffdc5; //0x800ffdc5
	/*0x0000*/ u8 unk_ffdc6[2]; //0x800ffdc6
	/*0x0000*/ CdlLOC unk_ffdc8; //0x800ffdc8 used for CdControl params during streaming VOICE
	/*0x0000*/ CdlLOC unk_ffdcc; //0x800ffdcc
	/*0x0000*/ u32 unk_ffdd0; //0x800ffdd0
	/*0x0000*/ u32 unk_ffdd4; //0x800ffdd4
	/*0x0000*/ u32 unk_ffdd8; //0x800ffdd8
	/*0x0000*/ u16 unk_ffddc; //0x800ffddc
	/*0x0000*/ u8 unk_ffdde[2];
	/*0x0000*/ CdFileLoadRequest requests[11]; //0x800ffde0 - ffebc

} CdFileContext;

extern char* sGamefileNameTable[];
extern MovieStreamContext* sMovieCtx; //Ptr @ gp+0x208
extern DECDCTTAB* sDctTbl; //Ptr @ gp+0x214

extern u8 sCdReadBuffer[]; //0x801f8000

int CdFiles_requestCdFile(u8 mode, s16 fileId, u32 offset, s32 arg3, s32 arg4, void* dest);
int CdFiles_AllSlotsEmpty(); //3fec4

int func_8003FEA0(unktype_2 arg0); //3fea0


#endif //SLPM87176_CDFILE_H
