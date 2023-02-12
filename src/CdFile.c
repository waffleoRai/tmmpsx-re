#include "CdFile.h"

#define READ_LE(ptr) (ptr)[3] << 24 | (ptr)[2] << 16 | (ptr)[1] << 8 | (ptr)[0]
#define CLEAR_CDLOC(loc) loc.minute = 0; loc.second = 0; loc.sector = 0; loc.track = 0
#define CURRENT_REQUEST sCdFileCtx.requests[sCdFileCtx.slotIndex]
#define CURRENT_SLOT_FILE_ID sCdFileCtx.requests[sCdFileCtx.slotIndex].fileId

#define GP_208 (globalptr + 0x208)

void CdFiles_MdecOutCallback();
void* CdFiles_DecompressLZ77(void* dst, void* src);

int func_80040348();
void func_80040430(MovieStreamContext* movieCtx);
int func_800404B0(char* filename);

int CdFiles_PCLoadFileData(char* filename, int len, u32 offset, void* dst);

//CdFiles_clearLoadRequests
//func_8003C1CC
int CdFiles_clearLoadCache() {

	int i;

	CdDrive_CdInit();
	CdDrive_CdSetDebug(0);

	sCdFileCtx.fileMedium = 3;
	sCdFileCtx.unk_ffd14 = 0;
	sCdFileCtx.unk_ffd0c = 0;
	sCdFileCtx.driveResponse[0] = 0;
	sCdFileCtx.unk_ffd16 = 0;

	for (i = 0; i < 12; i++) {
		CLEAR_CDLOC(sCdFileCtx.fileLocs[i]);
		sCdFileCtx.fileSizes[i] = 0;
	}

	sCdFileCtx.slotsUsed = 0;
	sCdFileCtx.nextSlot = 0;
	sCdFileCtx.slotIndex = 0;
	sCdFileCtx.unk_ffddc = 0;

	for (i = 0; i < 11; i++) {
		sCdFileCtx.requests[i].mode = 0;
		sCdFileCtx.requests[i].status = ReqStatus_Idle;
		sCdFileCtx.requests[i].fileId = -1;
		sCdFileCtx.requests[i].fileOffset = 0;
		sCdFileCtx.requests[i].unk_08 = 0;
		sCdFileCtx.requests[i].len = 0;
		sCdFileCtx.requests[i].dst = UNKPTR;
	}

	sCdFileCtx.unk_ffd0a = 0;
	sCdFileCtx.unk_ffd15 = 0;
	CdDrive_CdReadyCallback(func_8003C8E4);
	sApplState.stateFlags |= 0x400;

	return TRUE;
}

unktype_4 func_8003C2BC() {
	/*This one is also pretty big?
	Ghidra splits it up into case statements, so it's ahrd to tell exactly where the func ends.
	But I think the case blocks are part of this function?*/
}

//CdFiles_requestCdFile
//func_8003C544
int CdFiles_requestCdFile(u8 mode, s16 fileId, u32 offset, s32 arg3, s32 size, void* dest) {

	int slot;

	if (sCdFileCtx.slotsUsed > 11) return FALSE;
	slot = sCdFileCtx.nextSlot;
	sCdFileCtx.requests[slot].mode = mode;
	sCdFileCtx.requests[slot].status = ReqStatus_Idle;
	sCdFileCtx.requests[slot].fileId = fileId;
	sCdFileCtx.requests[slot].fileOffset = offset;
	sCdFileCtx.requests[slot].unk_08 = arg3;
	sCdFileCtx.requests[slot].len = size;
	sCdFileCtx.requests[slot].dst = dest;

	if (slot < 11) sCdFileCtx.nextSlot++;
	else sCdFileCtx.nextSlot = 0;
	sCdFileCtx.slotsUsed++;
	sCdFileCtx.unk_ffddc++;

	return TRUE;
}

int func_8003C664() {
	int i;
	int c = 0;

	if (sCdFileCtx.slotsUsed != 0) {
		if (((sCdFileCtx.fileMedium != 0) && (CURRENT_REQUEST.mode == 0x80)) && (CURRENT_REQUEST.status == ReqStatus_Idle)) {
			CdDrive_CdControl(CdCommand_Mute, NULL, NULL);
			CdDrive_CdControl(CdCommand_Pause, NULL, NULL);
		}
		if (sCdFileCtx.slotIndex == sCdFileCtx.nextSlot) {
			for (i = 0; i < sCdFileCtx.slotIndex; i++) {
				if (i == 12) i = 0;
				if ((sCdFileCtx.requests[i].mode & 0xc0) == 0x80) {
					sCdFileCtx.requests[i].status = 1;
					c++;
				}
			}
		}
		sCdFileCtx.unk_ffdd0 = 0;
	}
	return c;
}

int func_8003C790() {
	if (sCdFileCtx.slotsUsed == 0) return 0;
	if ((CURRENT_REQUEST.mode & 0xc0) == 0x80) return 0;

	if ((sCdFileCtx.fileMedium != 0) && ((CURRENT_REQUEST.status - 9) < 2)) {
		SoundSs_SsSetSerialVol(0, 0, 0);
		Spu_SpuSetCommonCDMix(0);
		CdDrive_CdControl(CdCommand_Mute, NULL, NULL);
		CdDrive_CdControl(CdCommand_Pause, NULL, NULL);
		sCdFileCtx.unk_ffd20 = sCdFileCtx.unk_ffd16;
		CdDrive_CdControlB(CdCommand_Setmode, &sCdFileCtx.unk_ffd20, &sCdFileCtx.driveResponse);
	}

	if (sCdFileCtx.slotsUsed != 0) {
		CURRENT_REQUEST.status = ReqStatus_Idle;
		if (sCdFileCtx.slotIndex < 11) sCdFileCtx.slotIndex++;
		else sCdFileCtx.slotIndex = 0;
		sCdFileCtx.slotsUsed--;
	}
	return 1;
}

inline int func_8003C8E4_case3(byte* buffer) {
	//This definitely looks like it's finding a subfile from an ARC offset table...
	int tempi;
	u32 tempui;
	u32 tempui2;
	u32 tempui3;

	CdDrive_CdGetSector(buffer, 3);
	CdDrive_CdGetSector(sCdReadBuffer, 0x200);

	tempui = (CURRENT_REQUEST.fileOffset << 2) & 0x7ff;
	tempui2 = READ_LE(sCdReadBuffer + tempui);
	tempui3 = READ_LE(sCdReadBuffer + tempui + 4);
	if (tempui3 != 0) {
		sCdFileCtx.comprDataBuffer = -1;
		sCdFileCtx.comprFileBuffer = -1;
		sCdFileCtx.loadDest = CURRENT_REQUEST.dst;
		sCdFileCtx.loadOffset = CURRENT_REQUEST.fileOffset << 2;
		sCdFileCtx.loadRemaining = tempui3;

		tempi = CdDrive_CdPosToInt(&sCdFileCtx.fileLocs[CURRENT_SLOT_FILE_ID]);
		sCdFileCtx.reqSector = ((tempi * 0x800) + sCdFileCtx.loadOffset) >> 11;
		sCdFileCtx.reqSectorOffset = tempi & 0x7ff;
		CdDrive_CdIntToPos(sCdFileCtx.reqSector, &sCdFileCtx.unk_ffd40);
		sCdFileCtx.unk_ffd15 = 4;
		CdDrive_CdControl(CdCommand_Setloc, &sCdFileCtx.unk_ffd40, &sCdFileCtx.driveResponse);
		CdDrive_CdControlF(CdCommand_ReadN, NULL);
		return TRUE;
	}
	return FALSE;
}

inline int func_8003C8E4_case4(byte* buffer) {
	int relpos;
	u32 sec_off;
	u32 tempui;

	CdDrive_CdGetSector(buffer, 3);
	relpos = CdDrive_CdPosToInt(buffer);
	if (sCdFileCtx.reqSector++ == relpos) {
		sec_off = 0x800 - sCdFileCtx.reqSectorOffset;
		if (sCdFileCtx.loadRemaining < sec_off) {
			sec_off = sCdFileCtx.loadRemaining;
		}

		if (!(CURRENT_REQUEST.mode & FILE_MODE_FLAG_LZ77)) {
			CdDrive_CdGetSector(sCdReadBuffer, sCdFileCtx.reqSectorOffset >> 2);
			CdDrive_CdGetSector2(sCdFileCtx.loadDest, 0x200);
			sCdFileCtx.loadDest += sec_off;
		}
		else {
			if (sCdFileCtx.comprDataBuffer == -1) {
				CdDrive_CdGetSector(sCdReadBuffer, sCdFileCtx.reqSectorOffset >> 2);
				tempui = UINT_FROM_POINTER(sCdReadBuffer + sCdFileCtx.reqSectorOffset);
				sCdFileCtx.comprFileBuffer = (int)sCdFileCtx.loadDest + (((tempui >> 3) + tempui) - sCdFileCtx.loadRemaining) + 7 & ~0x3;
				sCdFileCtx.comprDataBuffer = sCdFileCtx.comprFileBuffer + 4;
			}
			else {
				CdDrive_CdGetSector(sCdReadBuffer, sCdFileCtx.reqSectorOffset >> 2);
			}
			CdDrive_CdGetSecto2r(sCdFileCtx.comprFileBuffer, 0x200);
			sCdFileCtx.comprFileBuffer += sec_off;
		}
		sCdFileCtx.loadRemaining -= sec_off;
		if (sCdFileCtx.loadRemaining != 0) sCdFileCtx.unk_ffd15 = 6;
		else sCdFileCtx.unk_ffd15 = 0;
		return TRUE;
	}
	return FALSE;
}

inline int func_8003C8E4_case5() {

}

inline int func_8003C8E4_case6() {

}

inline int func_8003C8E4_case7() {

}

inline int func_8003C8E4_case8() {

}

inline int func_8003C8E4_case9() {

}

inline int func_8003C8E4_case10() {

}

void func_8003C8E4(uint8_t arg0, uint8_t* arg1) {
	//Default ready callback
	/*This one is also chunked up into a ton of case blocks.
	Originally blocked into 3c790, but there's an obvious jrra, so not sure why.*/


}

inline int func_8003D208_nextslot() {
	//This can be used as label 8003d6dc
	if (sCdFileCtx.slotsUsed == 0) return TRUE;
	CURRENT_REQUEST.status = 0;
	if (sCdFileCtx.slotIndex < 11) sCdFileCtx.slotIndex++; //LAB 8003f99c (inside brackets)
	else sCdFileCtx.slotIndex = ReqStatus_Idle;
	return FALSE;
}

inline void func_8003D208_L8003dae8() {
	sCdFileCtx.comprDataBuffer = -1;
	sCdFileCtx.loadOffset = CURRENT_REQUEST.fileOffset;
	sCdFileCtx.loadDest = CURRENT_REQUEST.dst;
	sCdFileCtx.loadRemaining = CURRENT_REQUEST.len;
	CURRENT_REQUEST.status = 7;
}

inline int func_8003D208_A(CdlFILE* fileInfo, char* stringBuffer) {
	char* strpos;
	char* strbuff_pos;
	CdlFILE* cdfile;

	//Copy name to local buffer, then add some stuff for the CD file system.
	strpos = sGamefileNameTable[CURRENT_SLOT_FILE_ID];
	strbuff_pos = stringBuffer;
	STRCPY_INCR(strbuff_pos, strpos);

	strpos = CDFILE_PATH_SUF;
	STRCPY_INCR(strbuff_pos, strpos);
	*strbuff_pos++ = '\0';

	cdfile = CdDrive_SearchFile(fileInfo, stringBuffer);
	if (cdfile < 1) {
		//??? This seems like a weird compare to use for a pointer.
		// Especially given most will have highest bit set...

		//Maybe it's supposed to mean if it's null?
		CURRENT_REQUEST.status = ReqStatus_Error;
		return TRUE;
	}
	else {
		sCdFileCtx.fileLocs[CURRENT_SLOT_FILE_ID] = fileInfo->pos;
		sCdFileCtx.fileSizes[CURRENT_SLOT_FILE_ID] = fileInfo->size;
	}
	return FALSE;
}

inline int func_8003D208_case0_mode40(CdlFILE* fileInfo, char* stringBuffer) {

	char* strpos;
	char* strbuff_pos;

	int cdpos;
	int loadres;

	if (sCdFileCtx.fileMedium > 1) {
		if (sCdFileCtx.fileSizes[CURRENT_SLOT_FILE_ID] == 0) {
			//File Info hasn't been retrieved yet.
			if (func_8003D208_A(&fileInfo, stringBuffer)) return TRUE;
		}

		sCdFileCtx.comprDataBuffer = -1;
		sCdFileCtx.loadRemaining = CURRENT_REQUEST.len;
		sCdFileCtx.loadOffset = CURRENT_REQUEST.fileOffset;

		//Calculate position of requested data on CD...
		cdpos = CdDrive_CdPosToInt(&sCdFileCtx.fileLocs[CURRENT_SLOT_FILE_ID]);
		cdpos = (cdpos << 11) + CURRENT_REQUEST.fileOffset; //sll 11 is * 0x800
		sCdFileCtx.reqSector = cdpos >> 11; // sra 11 is /= 0x800
		sCdFileCtx.reqSectorOffset = cdpos & 0x7ff;
		CdDrive_CdIntToPos(sCdFileCtx.reqSector, &sCdFileCtx.unk_ffd40);
		CdDrive_CdIntToPos(sCdFileCtx.reqSector, &sCdFileCtx.unk_ffd44);


		sCdFileCtx.unk_ffdd8 = sApplState.unk_feb60;
		sCdFileCtx.unk_ffd15 = 7;
		CURRENT_REQUEST.status = 15;
		CdDrive_CdControl(CdCommand_Setloc, &sCdFileCtx.unk_ffd40, &sCdFileCtx.driveResponse);
		CdDrive_CdControl(CdCommand_ReadN, NULL, NULL);
		return TRUE;
	}

	strbuff_pos = stringBuffer;
	PREFIX_FILENAME(strbuff_pos, strpos, sGamefileNameTable[CURRENT_SLOT_FILE_ID]);

	sCdFileCtx.comprDataBuffer = -1;
	sCdFileCtx.loadOffset = CURRENT_REQUEST.fileOffset;
	sCdFileCtx.loadRemaining = CURRENT_REQUEST.len;
	loadres = CdFiles_PCLoadFileData(stringBuffer, 8, sCdFileCtx.loadOffset, sCdReadBuffer);
	if (loadres != 0) {
		//LABEL 8003deec
		CURRENT_REQUEST.status = ReqStatus_Error;
		return TRUE;
	}

	//NOTE: Treating unk_100040 and unk_1f8004 like u8* when writing this.
	sAudioCtx.unk_100040 -= UINT_FROM_POINTER(sCdReadBuffer + 4) + 4;
	sAudioCtx.unk_100020[sAudioCtx.vabLoadSlot] = sAudioCtx.unk_100040;
	sAudioCtx.unk_100030[sAudioCtx.vabLoadSlot] = UINT_FROM_POINTER(sCdReadBuffer + 4) + 4;
	loadres = CdFiles_PCLoadFileData(stringBuffer, UINT_FROM_POINTER(sCdReadBuffer + 4) + 4, sCdFileCtx.loadOffset, sAudioCtx.unk_100040);
	if (loadres != 0) {
		//Technically, goto 8003deec, but same thing.
		CURRENT_REQUEST.status = ReqStatus_Error;
		return TRUE;
	}

	if ((sAudioCtx.vabLoadSlot == 1) && (sAudioCtx.seqFileIndex >= 0)) {
		sAudioCtx.vabIds[1] = -1;
		CURRENT_REQUEST.status = 0;
		if (sCdFileCtx.slotsUsed == 0) return TRUE;

		if (sCdFileCtx.slotIndex < 11)sCdFileCtx.slotIndex++;
		else sCdFileCtx.slotIndex = 0;
		return FALSE; //?
	}

	//Allocates a VAB header
	sAudioCtx.vabIds[sAudioCtx.vabLoadSlot] = SoundVs_SsVabOpenHead(sAudioCtx.unk_100040 + *(sAudioCtx.unk_100040), -1);
	if (sAudioCtx.vabIds[sAudioCtx.vabLoadSlot] != -1) {
		sCdFileCtx.loadRemaining -= sAudioCtx.unk_100040[2] + 8;
		CURRENT_REQUEST.status = 13;
		sCdFileCtx.loadOffset += sAudioCtx.unk_100040[2] + 8;
		return TRUE;
	}
	return func_8003D208_nextslot();
}

inline int func_8003D208_case0_modedefo(CdlFILE* fileInfo, char* stringBuffer) {

	s32 temps32;
	u32 tempu32;
	u32 tempu32_2;
	u32 tempu32_3;
	u8* tempbptr;

	char* strpos;
	char* strbuff_pos;
	int relsec;

	if (sCdFileCtx.fileMedium > 1) {
		if (sCdFileCtx.fileSizes[CURRENT_SLOT_FILE_ID] == 0) {
			//Get file info/location if not already loaded
			if (func_8003D208_A(&fileInfo, stringBuffer)) return TRUE;
		}

		if (!(CURRENT_REQUEST.mode & FILE_MODE_FLAG_INDEX_OFFSET)) {
			if (CURRENT_REQUEST.len == 0) {
				CURRENT_REQUEST.len = sCdFileCtx.fileSizes[CURRENT_SLOT_FILE_ID] - CURRENT_REQUEST.fileOffset;
			}
			sCdFileCtx.loadOffset = CURRENT_REQUEST.fileOffset;
			sCdFileCtx.loadRemaining = CURRENT_REQUEST.len;
			sCdFileCtx.loadDest = CURRENT_REQUEST.dst;
			sCdFileCtx.comprDataBuffer = UNKPTR;
			sCdFileCtx.comprFileBuffer = UNKPTR;
		}
		else {
			if (sGlobalData.fileposTable[CURRENT_SLOT_FILE_ID] == UNKPTR) {
				relsec = CdDrive_CdPosToInt(&sCdFileCtx.fileLocs[CURRENT_SLOT_FILE_ID]);
				CdDrive_CdIntToPos(relsec + (CURRENT_REQUEST.fileOffset >> 9), &sCdFileCtx.unk_ffd40);
				sCdFileCtx.unk_ffdd8 = sApplState.unk_feb60;
				sCdFileCtx.unk_ffd15 = 3;
				sCdFileCtx.reqSectorOffset = 0;
				CURRENT_REQUEST.status = ReqStatus_PendingData;
				CdDrive_CdControl(CdCommand_Setloc, &sCdFileCtx.unk_ffd40, &sCdFileCtx.driveResponse);
				CdDrive_CdControl(CdCommand_ReadN, NULL, NULL);
				return TRUE;
			}
			
			tempbptr = sGlobalData.fileposTable[CURRENT_SLOT_FILE_ID] + CURRENT_REQUEST.fileOffset;
			tempu32 = UINT_FROM_POINTER(tempbptr);
			sCdFileCtx.loadOffset = CURRENT_REQUEST.fileOffset;
			sCdFileCtx.loadDest = CURRENT_REQUEST.dst;
			sCdFileCtx.comprDataBuffer = -1;
			sCdFileCtx.comprFileBuffer = -1;
			sCdFileCtx.loadRemaining = UINT_FROM_POINTER(tempbptr + 4) - (tempu32 - 4);
			if (CURRENT_REQUEST.mode & FILE_MODE_FLAG_LZ77) {
				if (sGlobalData.fileMemsizeTable[CURRENT_SLOT_FILE_ID] != -1) {
					tempbptr = sGlobalData.fileMemsizeTable[CURRENT_SLOT_FILE_ID] + CURRENT_REQUEST.fileOffset;
					tempu32 = UINT_FROM_POINTER(tempbptr);
					sCdFileCtx.comprFileBuffer = sCdFileCtx.loadDest + (((((tempu32 >> 3) + tempu32) - sCdFileCtx.loadRemaining) + 7) & ~0x3U);
					sCdFileCtx.comprDataBuffer = sCdFileCtx.comprFileBuffer + 4;
				}
			}
			if (sCdFileCtx.loadRemaining == 0) {
				return func_8003D208_nextslot();
			}
		}

		relsec = CdDrive_CdPosToInt(&sCdFileCtx.fileLocs[CURRENT_SLOT_FILE_ID]);
		sCdFileCtx.reqSector = (sCdFileCtx.loadOffset + relsec << 11) >> 11;
		sCdFileCtx.reqSectorOffset = (sCdFileCtx.loadOffset + relsec << 11) & 0x7ff;
		CdDrive_CdIntToPos(sCdFileCtx.reqSector, &sCdFileCtx.unk_ffd40);
		sCdFileCtx.unk_ffdd8 = sApplState.unk_feb60;
		sCdFileCtx.unk_ffd15 = 0;
		if (sCdFileCtx.reqSectorOffset != 0) sCdFileCtx.unk_ffd15 = 4;
		CURRENT_REQUEST.status = ReqStatus_PendingData;
		CdDrive_CdControl(CdCommand_Setloc, &sCdFileCtx.unk_ffd40, &sCdFileCtx.driveResponse);
		CdDrive_CdControl(CdCommand_ReadN, NULL, NULL);
		return TRUE;
	}

	strpos = CDFILE_PATH_PRE;
	strbuff_pos = stringBuffer;
	STRCPY_INCR(strbuff_pos, strpos);

	strpos = sGamefileNameTable[CURRENT_SLOT_FILE_ID];
	STRCPY_INCR(strbuff_pos, strpos);

	if (!(CURRENT_REQUEST.mode & 0x10)) {
		if (CURRENT_REQUEST.len != 0) {
			func_8003D208_L8003dae8();
			return TRUE;
		}
		sCdFileCtx.loadRemaining = func_800404B0(stringBuffer);
		if (sCdFileCtx.loadRemaining != -1) {
			func_8003D208_L8003dae8();
			return TRUE;
		}
		if (sCdFileCtx.slotsUsed == 0) return TRUE;
		CURRENT_REQUEST.status = ReqStatus_Idle;
	}
	else {
		if (CdFiles_PCLoadFileData(stringBuffer, (CURRENT_REQUEST.fileOffset + 2) * 4, 0, sCdReadBuffer) != 0) {
			CURRENT_REQUEST.status = ReqStatus_Error;
			return TRUE;
		}
		tempbptr = sCdReadBuffer;
		tempbptr += CURRENT_REQUEST.fileOffset * 4;
		tempu32_2 = UINT_FROM_POINTER(tempbptr);
		tempu32 = (tempbptr[7] << 24 | tempbptr[6] << 16 | tempbptr[5] << 8 | *(sCdReadBuffer + 4 + CURRENT_REQUEST.fileOffset)) - (tempu32_2 - 4);
		if (tempu32 != 0) {
			sCdFileCtx.comprDataBuffer = -1;
			sCdFileCtx.loadDest = CURRENT_REQUEST.dst;
			sCdFileCtx.loadOffset = tempu32 + tempu32_2;
			sCdFileCtx.loadRemaining = tempu32;
			CURRENT_REQUEST.status = ReqStatus_PendingData_PC;
			return TRUE;
		}
		if (sCdFileCtx.slotsUsed == 0) return TRUE;
		CURRENT_REQUEST.status = ReqStatus_Idle;
	}

	if (sCdFileCtx.slotIndex < 11) sCdFileCtx.slotIndex++;
	else sCdFileCtx.slotIndex = 0;
	return FALSE;
}

inline int func_8003D208_case0_mode80(CdlFILE* fileInfo, char* stringBuffer) {
	
	char* strpos;
	char* strbuff_pos;
	
	char* cptr1;
	char* cptr2;
	int i;
	CdlFILE* cdfile;

	int idx_min;
	int idx_max;
	
	u8 tempu8;
	u16 tempu16;
	u32 tempu32;
	s32 temps32;
	u32 tempu32_2;
	u32 tempu32_3;
	u8* tempbptr;

	if ((sCdFileCtx.fileMedium != FILE_MEDIUM_PC) && (sCdFileCtx.fileSizes[VoiceStr] == 0)) {

		strpos = "\\VOICE.STR";
		strbuff_pos = stringBuffer;
		SUFFIX_FILENAME(strbuff_pos, strpos);

		cdfile = CdDrive_SearchFile(&fileInfo, stringBuffer);
		if (cdfile < 1) {
			//The weird pointer compare again.
			CURRENT_REQUEST.status = ReqStatus_Error;
			return TRUE;
		}
		else {
			sCdFileCtx.fileLocs[VoiceStr] = fileInfo->pos;
			sCdFileCtx.fileSizes[VoiceStr] = fileInfo->size;
		}
	}

	sAudioCtx.unk_10008c = CURRENT_REQUEST.fileOffset;
	sAudioCtx.unk_100090 = CURRENT_REQUEST.unk_08;
	//tempu32 = sAudioCtx.unk_10008c << 1;
	if ((sAudioCtx.unk_10008c < 0x3a) & ((sAudioCtx.unk_10008c << 1) != 0)) {
		idx_min = sGlobalData.unktbl_e20e4[sAudioCtx.unk_10008c].unk_00 + (sGlobalData.unktbl_e20e4[sAudioCtx.unk_10008c].unk_01 << 8);
		idx_max = sGlobalData.unktbl_e20e4[sAudioCtx.unk_10008c + 1].unk_00 + (sGlobalData.unktbl_e20e4[sAudioCtx.unk_10008c + 1].unk_01 << 8);

		if (idx_max <= idx_min) {
			return func_8003D208_nextslot();
		}

		//tempbptr = &sUnk_e1f68.unk_e2161 + tempu32_2;
		i = idx_min;
		do {
			if (sAudioCtx.unk_100090 == sGlobalData.unktbl_e215c[i].unk_00) {
				sCdFileCtx.unk_ffdc4 = 1;
				sCdFileCtx.unk_ffdc5 = sGlobalData.unktbl_e215c[i].unk_01;
				sAudioCtx.unk_100095 = 0x78;
				sAudioCtx.unk_100094 = sCdFileCtx.unk_ffdc5;
				temps32 = CdDrive_CdPosToInt(&sCdFileCtx.fileLocs[VoiceStr]);
				temps32 += sGlobalData.unktbl_e215c[i].unk_02 + (sGlobalData.unktbl_e215c[i].unk_03 << 8) + ((sGlobalData.unktbl_e215c[i].unk_04 & 0x3) << 16);
				CdDrive_CdIntToPos(temps32, &sCdFileCtx.unk_ffdc8);
				CdDrive_CdIntToPos(temps32 + (((sGlobalData.unktbl_e215c[i].unk_04 >> 2) + sGlobalData.unktbl_e215c[i].unk_05 * 64) * 16), &sCdFileCtx.unk_ffdcc);
				sCdFileCtx.unk_ffdd0 = (((sGlobalData.unktbl_e215c[i].unk_04 >> 2) + (sGlobalData.unktbl_e215c[i].unk_05 * 64) * 32) / 5) + 6;

				if (sCdFileCtx.fileMedium == FILE_MEDIUM_PC) {
					sCdFileCtx.unk_ffdd4 = sApplState.unk_feb60;
					CURRENT_REQUEST.status = 8;
				}
				else {
					SoundSs_SsSetSerialAttr(0, 0, 1);
					SoundSs_SsSetSerialAttr(0, 1, 0);
					SoundSs_SsSetSerialVol(0, sAudioCtx.unk_100095, sAudioCtx.unk_100095);
					CdDrive_CdControl(CdCommand_Demute, NULL, NULL);
					CdDrive_CdControl(CdCommand_Setfilter, &sCdFileCtx.unk_ffdc4, &sCdFileCtx.driveResponse);
					CdDrive_CdControl(CdCommand_Setloc, &sCdFileCtx.unk_ffdc8, &sCdFileCtx.driveResponse);
					CdDrive_CdControl(CdCommand_SeekL, NULL, &sCdFileCtx.driveResponse);
					CURRENT_REQUEST.status = 9;
				}
				break;
			}
		} while (++i < idx_max);
	}
	if (i < idx_max) return TRUE;
	return func_8003D208_nextslot();
}

inline int func_8003D208_case0_modec0(CdlFILE* fileInfo, char* stringBuffer) {
	u8 tempu8;
	u16 tempu16;
	u32 tempu32;
	s32 temps32;

	if (sCdFileCtx.fileMedium != 0) {
		tempu32 = (u32)sGlobalData.unktbl_e1eac[CURRENT_SLOT_FILE_ID].asBytes.unk_03;
		sMovieCtx->unk_0034 = tempu32;
		tempu8 = sGlobalData.unktbl_e1f60[tempu32];
		sMovieCtx->unk_0018 = (u32)tempu8;
		sMovieCtx->unk_0000.x = sGlobalData.unktbl_e1f20[tempu32];

		if (tempu8 != 0) {
			sMovieCtx->unk_0000.x = (sMovieCtx->unk_0000.x * 3) >> 1;
		}
		temps32 = sMovieCtx->unk_0034;
		sMovieCtx->unk_0000.y = sGlobalData.unktbl_e1f20[(temps32 << 1) + 1];

		if (sMovieCtx->unk_0018 == 0) {
			tempu16 = sGlobalData.unktbl_e1f20[temps32 + 1];
		}
		else {
			tempu16 = (sGlobalData.unktbl_e1f20[temps32 + 1] * 3) >> 1;
		}

		sMovieCtx->unk_0000.w = tempu16 & 0xffff;
		tempu16 = sGlobalData.unktbl_e1f20[(sMovieCtx->unk_0034 << 3) | 6U];
		sMovieCtx->unk_0000.w |= tempu16 << 16;
		if (sMovieCtx->unk_0018 == 0) {
			sMovieCtx->unk_0044.w = 0x18;
		}
		else sMovieCtx->unk_0044.w = 0x10;

		sMovieCtx->unk_0044.h = tempu16;
		sMovieCtx->unk_0020 = 0;
		sMovieCtx->unk_0040 = ((sGlobalData.unktbl_e1eac[CURRENT_SLOT_FILE_ID + 1].asInt & 0xffffff) - (sGlobalData.unktbl_e1eac[CURRENT_SLOT_FILE_ID].asInt & 0xffffff)) / 10 - 4;
		temps32 = CdDrive_CdPosToInt(&sCdFileCtx.fileLocs[Movie]); //ffd7c
		CdDrive_CdIntToPos(temps32 + (sGlobalData.unktbl_e1eac[CURRENT_SLOT_FILE_ID].asInt & 0xffffff), &sMovieCtx->unk_003c);

		sMovieCtx->unk_0010 = sMovieCtx->unk_004c;
		sMovieCtx->unk_0008 = sMovieCtx->unk_0000.w;
		sMovieCtx->unk_0014 = sMovieCtx->unk_2a4c;
		sMovieCtx->unk_000c = 0x0023000 + sMovieCtx->unk_0000.w;

		Spu_SpuSetCommonCDMix(1);
		Spu_SpuSetCommonCDVolume(0x7000, 0x7000);
		CdDrive_CdControl(CdCommand_Demute, NULL, NULL);
		CURRENT_REQUEST.status = 11;
		sApplState.stateFlags |= 0x40000;
		return TRUE;
	}
	return func_8003D208_nextslot();
}

inline int func_8003D208_case0(CdlFILE* fileInfo, char* stringBuffer) {

	u8 modehi;

	modehi = CURRENT_REQUEST.mode & 0xC0;
	if (modehi == 0x40) {
		return func_8003D208_case0_mode40(fileInfo, stringBuffer);
	}
	if (modehi < 0x41) {
		if (modehi == 0) return TRUE;
		return func_8003D208_case0_modedefo(fileInfo, stringBuffer);
	}
	else {
		if (modehi == 0x80) {
			return func_8003D208_case0_mode80(fileInfo, stringBuffer);
		}
		if (modehi != 0xc0) return TRUE;
		return func_8003D208_case0_modec0(fileInfo, stringBuffer);
	}
}

inline int func_8003D208_case6() {

	if (sCdFileCtx.unk_ffd15 != 0) {
		if (sCdFileCtx.unk_ffd15 == 1) {
			CURRENT_REQUEST.status = ReqStatus_Error;
			CdDrive_CdControl(CdCommand_Pause, NULL, NULL);
			sCdFileCtx.unk_ffd15 = 0;
			return;
		}
		if (sApplState.unk_feb60 <= (sCdFileCtx.unk_ffdd8 + 300)) return TRUE;
		CdDrive_CdControl(CdCommand_Pause, NULL, NULL);
		sCdFileCtx.unk_ffd15 = 0;
		//grab mode address and goto LAB_8003f398;
		//What this does is sets the status (tempptr[1]) to 2 and returns.
		CURRENT_REQUEST.status = ReqStatus_Error;
		return TRUE;
	}

	CdDrive_CdDataSync(0);
	CdDrive_CdControl(CdCommand_Pause, NULL, NULL);

	if (!(CURRENT_REQUEST.mode & FILE_MODE_FLAG_LZ77)) {
		sCdFileCtx.decompDataEnd = sCdFileCtx.loadDest;
	}
	else {
		sCdFileCtx.decompDataEnd = CdFiles_DecompressLZ77(sCdFileCtx.loadDest, sCdFileCtx.comprDataBuffer);
	}

//LAB_8003e698:
	CURRENT_REQUEST.status = ReqStatus_Idle;
	if (sCdFileCtx.slotsUsed == 0) return TRUE;
	if (sCdFileCtx.slotIndex < 11) {
		//goto LAB_8003f99c (see func_8003D208_nextslot, just increments slotIndex)
		sCdFileCtx.slotIndex++; 
	}
	sCdFileCtx.slotIndex = 0;
	return FALSE;
}

inline int func_8003D208_case7(char* stringBuffer) {
	int i, res;
	char* strpos;
	char* strBuff_pos;

	u32 decsize;
	u32 cpysize;

	strBuff_pos = stringBuffer;
	PREFIX_FILENAME(strBuff_pos, strpos, sGamefileNameTable[CURRENT_SLOT_FILE_ID]);

	cpysize = 0x5000; //These fields might actually be unions? Or Ghidra is just wrong about them being ptrs? 
	if (sCdFileCtx.loadRemaining < 0x5000) {
		cpysize = sCdFileCtx.loadRemaining;
	}

	res = CdFiles_PCLoadFileData(stringBuffer, cpysize, sCdFileCtx.loadOffset, sCdReadBuffer);
	if (res != 0) {
		CURRENT_REQUEST.status = ReqStatus_Error;
		return TRUE;
	}

	if (!(CURRENT_REQUEST.mode & FILE_MODE_FLAG_LZ77)) {
		memcpy(sCdFileCtx.loadDest, sCdReadBuffer, cpysize);
		sCdFileCtx.loadOffset += cpysize;
		sCdFileCtx.loadDest += cpysize;
		sCdFileCtx.loadRemaining -= cpysize;
		if (sCdFileCtx.loadRemaining != 0) return TRUE;
		sCdFileCtx.decompDataEnd = sCdFileCtx.loadDest;

		CURRENT_REQUEST.status = ReqStatus_Idle;
		if (sCdFileCtx.slotsUsed == 0) return TRUE;
		if (sCdFileCtx.slotIndex < 11) sCdFileCtx.slotIndex++;
		else sCdFileCtx.slotIndex = 0;
	}
	else {
		if (sCdFileCtx.comprDataBuffer == UNKPTR) {
			decsize = UINT_FROM_POINTER(sCdReadBuffer);
			sCdFileCtx.comprFileBuffer = sCdFileCtx.loadDest + ((decsize >> 3) + 4 + decsize) - sCdFileCtx.loadRemaining;
			sCdFileCtx.comprDataBuffer = sCdFileCtx.comprFileBuffer + 4;
		}
		memcpy(sCdFileCtx.comprFileBuffer, sCdReadBuffer, cpysize);
		sCdFileCtx.loadOffset += cpysize;
		sCdFileCtx.comprFileBuffer += cpysize;
		sCdFileCtx.loadRemaining -= cpysize;
		if (sCdFileCtx.loadRemaining != 0) return TRUE;
		sCdFileCtx.decompDataEnd = CdFiles_DecompressLZ77(sCdFileCtx.loadDest, sCdFileCtx.comprDataBuffer);

		CURRENT_REQUEST.status = ReqStatus_Idle;
		if (sCdFileCtx.slotsUsed == 0) return TRUE;
		//LAB_8003df64 is inside the first if
		if (sCdFileCtx.slotIndex < 11) sCdFileCtx.slotIndex++;
		else sCdFileCtx.slotIndex = 0;
		return TRUE; //?
	}
	return FALSE;
}

inline int func_8003D208_case8() {
	if ((sApplState.unk_feb60 - sCdFileCtx.unk_ffdd4) <= sCdFileCtx.unk_ffdd0) return TRUE;
	return func_8003D208_nextslot();
}

inline int func_8003D208_case9() {
	CdDrive_CdControl(CdCommand_Getstat, NULL, &sCdFileCtx.driveResponse);
	if (sCdFileCtx.driveResponse[0] & CdlStatSeek) return TRUE;
	CdDrive_CdRead2(200);
	sCdFileCtx.unk_ffdd4 = sApplState.unk_feb60;
	CURRENT_REQUEST.status = 10;
	return FALSE;
}

inline int func_8003D208_case10() {
	int pos1, pos2;

	if ((sApplState.unk_feb60 - sCdFileCtx.unk_ffdd4) <= sCdFileCtx.unk_ffdd0) return TRUE;
	CdDrive_CdControl(CdCommand_GetlocL, NULL, &sCdFileCtx.driveResponse);
	pos1 = CdDrive_CdPosToInt(&sCdFileCtx.driveResponse);
	pos2 = CdDrive_CdPosToInt(&sCdFileCtx.unk_ffdcc);
	if ((pos1 < pos2) && (sCdFileCtx.unk_ffdd0 != 0)) return TRUE;
	SoundSs_SsSetSerialVol(0, 0, 0);
	Spu_SpuSetCommonCDVolume(0, 0);
	CdDrive_CdControl(CdCommand_Mute, NULL, NULL);
	CdDrive_CdControl(CdCommand_Pause, NULL, NULL);
	sCdFileCtx.unk_ffd20 = sCdFileCtx.unk_ffd16;
	CdDrive_CdControlB(CdCommand_Setmode, &sCdFileCtx.unk_ffd20, &sCdFileCtx.driveResponse);
	return func_8003D208_nextslot();
}

inline int func_8003D208_case11() {
	u16 temp16;

	UINT_FROM_POINTER(GP_208 + 0x12) = 0;
	UINT_FROM_POINTER(GP_208 + 0x14) = 0;
	UINT_FROM_POINTER(GP_208 + 0x0e) = 0;

	sUnk_feb28.unk_feb64.g0 = 0;
	sUnk_feb28.unk_febc0.isbg = 0;
	sUnk_feb28.unk_fec2d = *(u8*)(GP_208 + 0x0c);
	sUnk_feb28.unk_fec41 = *(u8*)(GP_208 + 0x0c);

	GP_208[0x22] = *GP_208;
	temp16 = GP_208[1];
	if (INT_FROM_POINTER(GP_208 + 0x16) != 0) {
		temp16 += 0xf0;
	}
	GP_208[0x23] = temp16;

	INT_FROM_POINTER(GP_208 + 0x1c) = ((int)GP_208[0x24] * (int)GP_208[0x25]) / 2;
	UINT_FROM_POINTER(GP_208 + 0x16) = sApplState.stateFlags >> 1 & 1 ^ 1;

	CURRENT_REQUEST.status = 12;
	return TRUE;
}

inline int func_8003D208_case12() {

	int res = 0;
	u8 buffer[8];

	if ((sApplState.stateFlags >> 17) & 1) return 1;
	sApplState.stateFlags |= 0x10000;

	Press_DecDCTReset(0);
	Press_DecDCToutCallback(CdFiles_MdecOutCallback);
	CdDrive_StSetRing((void*)(GP_208 + 8), 0x20);
	CdDrive_StClearRing();
	CdDrive_StSetStream(UINT_FROM_POINTER(GP_208 + 0x0C), 1, -1, 0, 0);

	do {
		do {
			res = CdDrive_CdControl(CdCommand_Setloc, (u8*)(GP_208 + 0x1e), &sCdFileCtx.driveResponse);
		} while (res == 0);
		res = CdDrive_CdRead2(0x1e0);
	} while (res == 0);

	PressBuild_DecDCTvlcBuild((void*)(GP_208 + 0xc));
	while (func_80040348() == -1) {
		do {
			do {
				res = CdDrive_CdControl(CdCommand_Setloc, (u8*)(GP_208 + 0x1e), &sCdFileCtx.driveResponse);
			} while (res == 0);
			res = CdDrive_CdRead2(0x1e0);
		} while (res == 0);
	}

	UINT_FROM_POINTER(GP_208 + 0x18) = 0;

	do {
		Press_DecDCTin(UINT_FROM_POINTER(GP_208 + INT_FROM_POINTER(GP_208 + 0x12) * 2 + 4), INT_FROM_POINTER(GP_208 + 0xc) + 2);
		Press_DecDCTout(UINT_FROM_POINTER(GP_208 + INT_FROM_POINTER(GP_208 + 0x14) * 2 + 8), INT_FROM_POINTER(GP_208 + 0x1c));
		while (func_80040348() == -1) {
			res = CdDrive_StGetBackloc(buffer);
			if ((INT_FROM_POINTER(GP_208 + 0x20) < res) || (res < 1)) {
				do {
					do {
						res = CdDrive_CdControl(CdCommand_Setloc, (u8*)(GP_208 + 0x1e), &sCdFileCtx.driveResponse);
					} while (res == 0);
					res = CdDrive_CdRead2(0x1e0);
				} while (res == 0);
			}
		}
		func_80040430(GP_208);
		EtcVid_VSync(0);
		GpuSys_PutDispEnv((void*)((u32)&sUnk_feb28.unk_fec1c + UINT_FROM_POINTER(GP_208 + 0x16) ^ 1 * 0x14));
		GpuSys_SetDispMask(1);
	} while ((INT_FROM_POINTER(GP_208 + 0x18) == 0 ) && 
		(((sCdFileCtx.unk_fea6c[0x20] != '\0') || 
			(((sCdFileCtx.unk_fea6c[0x20] & 0x70 != 0x40) && (sCdFileCtx.unk_fea6c[0x20] & 0x70 != 0x70)) || 
				(sCdFileCtx.unk_fea6c[0x20] & 0x4008 == 0x4008)))));

	Press_DecDCToutCallback(NULL);
	CdDrive_CdDataCallback(NULL);
	CdDrive_CdReadyCallback(func_8003C8E4);
	Spu_SpuSetCommonCDVolume(0,0);
	CdDrive_CdControl(CdCommand_Mute, NULL, NULL);
	CdDrive_CdControl(CdCommand_Pause, NULL, NULL);
	EtcVid_VSync(4);
	sApplState.stateFlags = (sApplState.stateFlags & ~0x02) | (((UINT_FROM_POINTER(GP_208 + 0x16) ^ 1) & 1) << 1);
	GpuSys_ClearOTag(&sUnk_feb28.unk_fec44, 0x100);
	GpuSys_ClearOTag(&sUnk_feb28.unk_ff044, 0x100);
	Heap_FreeMemory(&GP_208[6], 0x11000);
	Heap_FreeMemory(&GP_208[4], 0x10000);
	Heap_FreeMemory(&GP_208[2], 0x46000);
	Heap_FreeMemory(&GP_208[0], 0x0544c);
	sApplState.stateFlags |= 0x20000;
	return func_8003D208_nextslot();
}

inline int func_8003D208_case13(char* stringBuffer) {

	char* cptr1;
	char* cptr2;
	int i;
	u32 temp32;

	cptr1 = stringBuffer;
	cptr2 = "i:\\cd";
	for (i = 0; i < 5; i++) *cptr1++ = *cptr2++;
	cptr2 = sGamefileNameTable[CURRENT_SLOT_FILE_ID];
	while(*cptr2 != '\0') *cptr1++ = *cptr2++; //No zero at end?

	temp32 = 0x5000;
	if (sCdFileCtx.loadRemaining < 0x5000) {
		temp32 = sCdFileCtx.loadRemaining;
	}
	if (CdFiles_PCLoadFileData(stringBuffer, temp32, sCdFileCtx.loadOffset, sCdReadBuffer) != 0) {
		CURRENT_REQUEST.status = ReqStatus_Error;
		return TRUE;
	}

	if (SoundVs_SsVabTransBodyPartly(sCdReadBuffer, temp32, sAudioCtx.vabIds[sAudioCtx.vabLoadSlot]) == -1) {
		SoundVs_SsVabClose(sAudioCtx.vabIds[sAudioCtx.vabLoadSlot]);
		sAudioCtx.vabIds[sAudioCtx.vabLoadSlot] = -1;
		if (sCdFileCtx.slotsUsed == 0) return TRUE;
		CURRENT_REQUEST.status = ReqStatus_Idle;
		if (sCdFileCtx.slotIndex < 11) sCdFileCtx.slotIndex++;
		else sCdFileCtx.slotIndex = 0;
	}
	else {
		sCdFileCtx.loadOffset += temp32;
		sCdFileCtx.loadRemaining -= temp32;
		if (sCdFileCtx.loadRemaining != 0) {
			CURRENT_REQUEST.status = 14;
			return TRUE;
		}
		SoundVs_SsVabTransCompleted(1);
		CURRENT_REQUEST.status = ReqStatus_Idle;
		if (sCdFileCtx.slotsUsed == 0) return TRUE;
		if (sCdFileCtx.slotIndex < 11) sCdFileCtx.slotIndex++;
		else sCdFileCtx.slotIndex = 0;
	}
	return FALSE;
}

inline int func_8003D208_case14(char* stringBuffer) {
	if (!SoundVs_SsVabTransCompleted(0)) return TRUE;
	return func_8003D208_case13(stringBuffer);
}

inline int func_8003D208_case15() {

	int32_t temp32;

	if (sCdFileCtx.unk_ffd15 != 0) {
		if (sCdFileCtx.unk_ffd15 != 1) {
			if (sApplState.unk_feb60 <= sCdFileCtx.unk_ffdd8 + 300) return TRUE;
			CdDrive_CdControl(CdCommand_Pause, NULL, NULL);
			sCdFileCtx.unk_ffd15 = 0;
			CURRENT_REQUEST.status = ReqStatus_Error;
			sAudioCtx.unk_100040 = sAudioCtx.unk_100020[sAudioCtx.vabLoadSlot] + sAudioCtx.unk_100030[sAudioCtx.vabLoadSlot];
			return TRUE;
		}
		CURRENT_REQUEST.status = ReqStatus_Error;
		sAudioCtx.unk_100040 = sAudioCtx.unk_100020[sAudioCtx.vabLoadSlot] + sAudioCtx.unk_100030[sAudioCtx.vabLoadSlot];
		CdDrive_CdControl(CdCommand_Pause, NULL, NULL);
		sCdFileCtx.unk_ffd15 = 0;
		return TRUE;
	}
	CdDrive_CdDataSync(0);
	CdDrive_CdControl(CdCommand_Pause, NULL, NULL);

	if ((sAudioCtx.vabLoadSlot != 1) || (sAudioCtx.seqFileIndex < 0)) {
		sAudioCtx.vabIds[sAudioCtx.vabLoadSlot] = SoundVs_SsVabOpenHead(sAudioCtx.unk_100040 + *sAudioCtx.unk_100040, -1);
		if (sAudioCtx.vabIds[sAudioCtx.vabLoadSlot] != -1) {
			CURRENT_REQUEST.status = 16;
			temp32 = CdDrive_CdPosToInt(&sCdFileCtx.unk_ffd44);
			CdDrive_CdIntToPos(temp32 + (sAudioCtx.unk_100040[2] + 0x807U >> 0x11), &sCdFileCtx.unk_ffd44);
			sCdFileCtx.reqSector = CdDrive_CdPosToInt(&sCdFileCtx.unk_ffd44);
			sCdFileCtx.loadDest = sCdReadBuffer;
			sCdFileCtx.unk_ffd15 = 9;
			sCdFileCtx.unk_ffd4c = 5;
			sCdFileCtx.unk_ffdd8 = sApplState.unk_feb60;
			CdDrive_CdControl(CdCommand_Setloc, &sCdFileCtx.unk_ffd44, &sCdFileCtx.driveResponse);
			CdDrive_CdControl(CdCommand_ReadN, NULL, NULL);
			return TRUE;
		}
		sAudioCtx.unk_100040 = sAudioCtx.unk_100020[sAudioCtx.vabLoadSlot] + sAudioCtx.unk_100030[sAudioCtx.vabLoadSlot];
		CURRENT_REQUEST.status = ReqStatus_Error;
		return TRUE;
	}
	sAudioCtx.vabIds[1] = -1;

	CURRENT_REQUEST.status = ReqStatus_Idle;
	if (sCdFileCtx.slotsUsed == 0) return TRUE;
	if (sCdFileCtx.slotIndex < 11) sCdFileCtx.slotIndex++;
	else sCdFileCtx.slotIndex = 0;
	return FALSE;
}

inline int func_8003D208_case16() {
	
	int res;

	if (sCdFileCtx.unk_ffd15 != 1) {
		if (sCdFileCtx.unk_ffd15 < 2){
			if (sCdFileCtx.unk_ffd15 == 0) {
				CdDrive_CdDataSync(0);
				res = SoundVs_SsVabTransBodyPartly(sCdReadBuffer, 0x2800, sAudioCtx.vabIds[sAudioCtx.vabLoadSlot]);
				if (res != sAudioCtx.vabIds[sAudioCtx.vabLoadSlot]) {
					if (res == -1) {
						CdDrive_CdControlB(CdCommand_Pause, NULL, NULL);
						sAudioCtx.unk_100040 = sAudioCtx.unk_100020[sAudioCtx.vabLoadSlot] + sAudioCtx.unk_100030[sAudioCtx.vabLoadSlot];
						SoundVs_SsVabClose(sAudioCtx.vabIds[sAudioCtx.vabLoadSlot]);
						sCdFileCtx.unk_ffd15 = 0;
						sAudioCtx.vabIds[sAudioCtx.vabLoadSlot] = (s16)res;
						CURRENT_REQUEST.status = ReqStatus_Error;
						return TRUE;
					}
					SoundVs_SsVabTransCompleted(1);
					CURRENT_REQUEST.status = 17;
					if (sCdFileCtx.unk_ffd15 != 11) return TRUE;
					CdDrive_CdControl(CdCommand_Pause, NULL, NULL);
					CdDrive_CdIntToPos(sCdFileCtx.reqSector, &sCdFileCtx.unk_ffd40);
					sCdFileCtx.loadDest = sCdReadBuffer;
					sCdFileCtx.unk_ffd15 = 9;
					sCdFileCtx.unk_ffd4c = 5;
					sCdFileCtx.unk_ffdd8 = sApplState.unk_feb60;
					CdDrive_CdControl(CdCommand_Setloc, &sCdFileCtx.unk_ffd40, &sCdFileCtx.driveResponse);
					CdDrive_CdControl(CdCommand_ReadN, NULL, NULL);
					return TRUE;
				}
				SoundVs_SsVabTransCompleted(1);
				CdDrive_CdControl(CdCommand_Pause, NULL, NULL);
				return func_8003D208_nextslot();
			}
		}
		else {
			if ((sCdFileCtx.unk_ffd15 < 12) && (sCdFileCtx.unk_ffd15 > 9)) {
				CdDrive_CdDataSync(0);
				res = SoundVs_SsVabTransBodyPartly(sCdReadBuffer, 0x2800, sAudioCtx.vabIds[sAudioCtx.vabLoadSlot]);
				if (res != sAudioCtx.vabIds[sAudioCtx.vabLoadSlot]) {
					if (res == -1) {
						CdDrive_CdControlB(CdCommand_Pause, NULL, NULL);
						sAudioCtx.unk_100040 = sAudioCtx.unk_100020[sAudioCtx.vabLoadSlot] + sAudioCtx.unk_100030[sAudioCtx.vabLoadSlot];
						SoundVs_SsVabClose(sAudioCtx.vabIds[sAudioCtx.vabLoadSlot]);
						sCdFileCtx.unk_ffd15 = 0;
						sAudioCtx.vabIds[sAudioCtx.vabLoadSlot] = (s16)res;
						CURRENT_REQUEST.status = ReqStatus_Error;
						return TRUE;
					}
					SoundVs_SsVabTransCompleted(1);
					CURRENT_REQUEST.status = 17;
					if (sCdFileCtx.unk_ffd15 != 11) return TRUE;
					CdDrive_CdControl(CdCommand_Pause, NULL, NULL);
					CdDrive_CdIntToPos(sCdFileCtx.reqSector, &sCdFileCtx.unk_ffd40);
					sCdFileCtx.loadDest = sCdReadBuffer;
					sCdFileCtx.unk_ffd15 = 9;
					sCdFileCtx.unk_ffd4c = 5;
					sCdFileCtx.unk_ffdd8 = sApplState.unk_feb60;
					CdDrive_CdControl(CdCommand_Setloc, &sCdFileCtx.unk_ffd40, &sCdFileCtx.driveResponse);
					CdDrive_CdControl(CdCommand_ReadN, NULL, NULL);
					return TRUE;
				}
				SoundVs_SsVabTransCompleted(1);
				CdDrive_CdControl(CdCommand_Pause, NULL, NULL);
				return func_8003D208_nextslot();
			}
		}

		if (sApplState.unk_feb60 <= (sCdFileCtx.unk_ffdd8 + 300)) return TRUE;
		CdDrive_CdControl(CdCommand_Pause, NULL, NULL);
		sAudioCtx.unk_100040 = sAudioCtx.unk_100020[sAudioCtx.vabLoadSlot] + sAudioCtx.unk_100030[sAudioCtx.vabLoadSlot];
		SoundVs_SsVabClose(sAudioCtx.vabIds[sAudioCtx.vabLoadSlot]);
		sCdFileCtx.unk_ffd15 = 0;
		sAudioCtx.vabIds[sAudioCtx.vabLoadSlot] = -1;
		CURRENT_REQUEST.status = ReqStatus_Error;
		return TRUE;
	}
	SoundVs_SsVabTransCompleted(1);
	sAudioCtx.unk_100040 = sAudioCtx.unk_100020[sAudioCtx.vabLoadSlot] + sAudioCtx.unk_100030[sAudioCtx.vabLoadSlot];
	SoundVs_SsVabClose(sAudioCtx.vabIds[sAudioCtx.vabLoadSlot]);
	sAudioCtx.vabIds[sAudioCtx.vabLoadSlot] = -1;
	CURRENT_REQUEST.status = ReqStatus_Error;
	CdDrive_CdControl(CdCommand_Pause, NULL, NULL);
	sCdFileCtx.unk_ffd15 = 0;
	return TRUE;
}

inline int func_8003D208_case17() {

	int res;

	if (sCdFileCtx.unk_ffd15 != 1) {
		if (sCdFileCtx.unk_ffd15 < 2) {
			if (sCdFileCtx.unk_ffd15 == 0) {
				CdDrive_CdDataSync(0);
				res = SoundVs_SsVabTransBodyPartly(sCdReadBuffer, 0x2800, sAudioCtx.vabIds[sAudioCtx.vabLoadSlot]);
				if (res != sAudioCtx.vabIds[sAudioCtx.vabLoadSlot]) {
					SoundVs_SsVabTransCompleted(1);
					CdDrive_CdControlB(CdCommand_Pause, NULL, NULL);
					return func_8003D208_nextslot();
				}
				if (res == -1) {
					CdDrive_CdControlB(CdCommand_Pause, NULL, NULL);
					sAudioCtx.unk_100040 = sAudioCtx.unk_100020[sAudioCtx.vabLoadSlot] + sAudioCtx.unk_100030[sAudioCtx.vabLoadSlot];
					SoundVs_SsVabClose(sAudioCtx.vabIds[sAudioCtx.vabLoadSlot]);
					sCdFileCtx.unk_ffd15 = 0;
					sAudioCtx.vabIds[sAudioCtx.vabLoadSlot] = (short)res;
					CURRENT_REQUEST.status = ReqStatus_Error;
					return TRUE;
				}

				SoundVs_SsVabTransCompleted(1);
				CURRENT_REQUEST.status = 16;
				if (sCdFileCtx.unk_ffd15 != 12) return TRUE;
				CdDrive_CdControlB(CdCommand_Pause, NULL, NULL);
				CdDrive_CdIntToPos(sCdFileCtx.reqSector, &sCdFileCtx.unk_ffd40);
				sCdFileCtx.loadDest = sCdReadBuffer + 0x2800; //801fa800
				sCdFileCtx.unk_ffd15 = 10;
				sCdFileCtx.unk_ffd4c = 5;
				sCdFileCtx.unk_ffdd8 = sApplState.unk_feb60;
				CdDrive_CdControl(CdCommand_Setloc, &sCdFileCtx.unk_ffd40, &sCdFileCtx.driveResponse);
				CdDrive_CdControl(CdCommand_ReadN, NULL, NULL);
				return TRUE;
			}
		}
		else {
			if ((sCdFileCtx.unk_ffd15 == 9) && (sCdFileCtx.unk_ffd15 == 12)) {
				CdDrive_CdDataSync(0);
				res = SoundVs_SsVabTransBodyPartly(sCdReadBuffer, 0x2800, sAudioCtx.vabIds[sAudioCtx.vabLoadSlot]);
				if (res != sAudioCtx.vabIds[sAudioCtx.vabLoadSlot]) {
					SoundVs_SsVabTransCompleted(1);
					CdDrive_CdControlB(CdCommand_Pause, NULL, NULL);
					return func_8003D208_nextslot();
				}
				if (res == -1) {
					CdDrive_CdControlB(CdCommand_Pause, NULL, NULL);
					sAudioCtx.unk_100040 = sAudioCtx.unk_100020[sAudioCtx.vabLoadSlot] + sAudioCtx.unk_100030[sAudioCtx.vabLoadSlot];
					SoundVs_SsVabClose(sAudioCtx.vabIds[sAudioCtx.vabLoadSlot]);
					sCdFileCtx.unk_ffd15 = 0;
					sAudioCtx.vabIds[sAudioCtx.vabLoadSlot] = (short)res;
					CURRENT_REQUEST.status = ReqStatus_Error;
					return TRUE;
				}

				SoundVs_SsVabTransCompleted(1);
				CURRENT_REQUEST.status = 16;
				if (sCdFileCtx.unk_ffd15 != 12) return TRUE;
				CdDrive_CdControlB(CdCommand_Pause, NULL, NULL);
				CdDrive_CdIntToPos(sCdFileCtx.reqSector, &sCdFileCtx.unk_ffd40);
				sCdFileCtx.loadDest = sCdReadBuffer + 0x2800; //801fa800
				sCdFileCtx.unk_ffd15 = 10;
				sCdFileCtx.unk_ffd4c = 5;
				sCdFileCtx.unk_ffdd8 = sApplState.unk_feb60;
				CdDrive_CdControl(CdCommand_Setloc, &sCdFileCtx.unk_ffd40, &sCdFileCtx.driveResponse);
				CdDrive_CdControl(CdCommand_ReadN, NULL, NULL);
				return TRUE;
			}
		}

		if (sApplState.unk_feb60 <= (sCdFileCtx.unk_ffdd8 + 300)) return TRUE;
		CdDrive_CdControl(CdCommand_Pause, NULL, NULL);
		sAudioCtx.unk_100040 = sAudioCtx.unk_100020[sAudioCtx.vabLoadSlot] + sAudioCtx.unk_100030[sAudioCtx.vabLoadSlot];
		SoundVs_SsVabClose(sAudioCtx.vabIds[sAudioCtx.vabLoadSlot]);
		sCdFileCtx.unk_ffd15 = 0;
		sAudioCtx.vabIds[sAudioCtx.vabLoadSlot] = -1;
		CURRENT_REQUEST.status = ReqStatus_Error;
		return TRUE;
	}

	SoundVs_SsVabTransCompleted(1);
	sAudioCtx.unk_100040 = sAudioCtx.unk_100020[sAudioCtx.vabLoadSlot] + sAudioCtx.unk_100030[sAudioCtx.vabLoadSlot];
	SoundVs_SsVabClose(sAudioCtx.vabIds[sAudioCtx.vabLoadSlot]);
	sAudioCtx.vabIds[sAudioCtx.vabLoadSlot] = -1;
	CURRENT_REQUEST.status = ReqStatus_Error;
	CdDrive_CdControl(CdCommand_Pause, NULL, NULL);
	sCdFileCtx.unk_ffd15 = 0;
	return TRUE;
}

//This is one of the big ones.
// Seriously, this thing is a monstrosity.
//func_8003D208
void func_8003D208() {

	//Actual locals
	CdlFILE fileInfo;
	char stringBuffer[0x40];

	//Handy locals
	char* cptr1;
	char* cptr2;
	int res;
	int i;
	CdlFILE* cdfile;
	void* params;
	int early_ret = 0;

	//Placeholder locals
	u8 tempu8;
	u16 tempu16;
	u32 tempu32;
	u32 tempu32_2;
	u32 tempu32_3;
	s32 temps32;
	char* tempcptr;
	u8* tempbptr;
	void* tempptr;

	while (TRUE) {
		if (sCdFileCtx.slotsUsed == 0) return;

		if (CURRENT_REQUEST.status != 1) break;
		CURRENT_REQUEST.status = ReqStatus_Idle;

		if (sCdFileCtx.slotIndex < 11) sCdFileCtx.slotIndex++;
		else sCdFileCtx.slotIndex = 0;

		sCdFileCtx.slotsUsed--;
	}

	switch (CURRENT_REQUEST.status) {
	case ReqStatus_Idle:
		early_ret = func_8003D208_case0(&fileInfo, stringBuffer);
		break;
	case 1:
		early_ret = func_8003D208_nextslot();
		break;
	case ReqStatus_Error:
		CURRENT_REQUEST.status = ReqStatus_Idle;
		break;
	default:
		//goto switchD_8003d300_caseD_3;  - this label is just the return at the end.
		return;
	case ReqStatus_PendingData:
		early_ret = func_8003D208_case6();
		break;
	case ReqStatus_PendingData_PC:
		early_ret = func_8003D208_case7(stringBuffer);
		break;
	case 8:
		early_ret = func_8003D208_case8();
		break;
	case 9:
		early_ret = func_8003D208_case9();
		break;
	case 10:
		early_ret = func_8003D208_case10();
		break;
	case 11:
		early_ret = func_8003D208_case11();
		break;
	case 12:
		early_ret = func_8003D208_case12();
		break;
	case 13:
		early_ret = func_8003D208_case13(stringBuffer);
		break;
	case 14:
		early_ret = func_8003D208_case14(stringBuffer);
		break;
	case 15:
		early_ret = func_8003D208_case15();
		break;
	case 16:
		early_ret = func_8003D208_case16();
		break;
	case 17:
		early_ret = func_8003D208_case16();
		break;
	}

	if (early_ret) return;
	sCdFileCtx.slotsUsed--;
}

//func_8003F9F0
void CdFiles_MdecOutCallback() {
	RECT rect;

	u16 temp16;
	u32 temp32;

	if ((sMovieCtx->unk_0018 == 1) && (sCdDriveCtx.unk_10c49c != 0)) {
		CdDrive_StCdInterrupt();
		sCdDriveCtx.unk_10c49c = 0;
	}
	rect = sMovieCtx->unk_0044;
	temp32 = sMovieCtx->unk_0028;
	sMovieCtx->unk_0044.x += sMovieCtx->unk_0044.w;
	sMovieCtx->unk_0028 ^= 0x1;
	if (sMovieCtx->unk_0044.x < (sMovieCtx->unk_0000.x + sMovieCtx->unk_0000.w)){
		//sMovieCtx->unk_0010[sMovieCtx->unk_0028]
		Press_DecDCTout(UINT_FROM_POINTER(&sMovieCtx->unk_0010 + sMovieCtx->unk_0028), sMovieCtx->unk_0038);
	}
	else {
		temp16 = sMovieCtx->unk_0000.y;
		sMovieCtx->unk_001c = 1;
		sMovieCtx->unk_002c = sMovieCtx->unk_0028;
		sMovieCtx->unk_0044 = sMovieCtx->unk_0000;
		if (sMovieCtx->unk_0028 != 0) {
			temp16 += 0xf0;
		}
		sMovieCtx->unk_0044.y = temp16;
	}
	//sMovieCtx->unk_0010[sMovieCtx->unk_0028]
	GpuSys_LoadImage(&rect, UINT_FROM_POINTER(&sMovieCtx->unk_0010 + sMovieCtx->unk_0028));
}

//May be related to LZ decompression?
//8003FB0C
void* CdFiles_DecompressLZ77(void* dst, void* src) {

	int i;
	u32 mask = 0x80; //uvar8
	u32 temp32_1; //uvar9
	u32 temp32_2; //uvar6
	u32 temp32_3; //uvar10
	u32 temp32_4; //uvar7

	byte* src_buffer = src;
	byte* dst_buffer = dst;

	byte* backpos;
	uint32_t params;

	temp32_2 = 0;
	do {
		mask <<= 1;
		while (TRUE) {
			while (TRUE) {
				temp32_1 = mask & 0xffff;
				if (mask & 0x100) {
					temp32_1 = 1;
					temp32_2 = *src_buffer;
					while (*src_buffer++ == 0xff) {
						for (i = 0; i < 8; i++) *dst_buffer++ = *src_buffer++;
						temp32_2 = *src_buffer;
					}
				}

				mask = temp32_2 << 1;
				temp32_2 = mask & 0xffff;
				while (mask & 0x100) {
					mask = temp32_1 << 1;
					temp32_1 = mask & 0xffff;
					*dst_buffer++ = *src_buffer++;
					while (mask & 0x100) {
						temp32_1 = 1;
						temp32_2 = *src_buffer;
						while (*src_buffer == 0xff) {
							for (i = 0; i < 8; i++) *dst_buffer++ = *src_buffer++;
							temp32_2 = *src_buffer;
						}
					}
				}

				temp32_3 = temp32_1 << 1 & 0xffff;
				if (temp32_3 & 0x100) {
					temp32_3 = 1;
					temp32_2 = *src_buffer++;
				}
				mask = temp32_2 << 1;
				temp32_2 = mask & 0xffff;
				if (!(mask & 0x100)) break;

				params = (((uint32_t)src_buffer[0]) << 8) | ((uint32_t)src_buffer[1]);
				backpos = dst_buffer + (params >> 3 | ~0x1fff);

				if ((src_buffer[1] & 0x7) == 0) {
					i = (int)src_buffer[2];
					if (i == 0) return dst_buffer;
					mask = temp32_3 << 1;
					i++;
					if (i != 0) {
						do {
							*dst_buffer++ = *backpos++;
							i--;
						} while (i != 0);
						mask = temp32_3 << 1;
					}
					src_buffer += 3;
				}
				else {
					i = (int)((params & 0x7) + 2);
					mask = temp32_3 << 1;
					if (i != 0) {
						do {
							*dst_buffer++ = *backpos++;
							i--;
						} while (i != 0);
						mask = temp32_3 << 1;
					}
					src_buffer += 2;
				}
			}
			temp32_1 = temp32_3 << 1 & 0xffff;
			if (temp32_1 & 0x100) {
				temp32_1 = 1;
				temp32_2 = *src_buffer++;
			}
			temp32_3 = temp32_2 << 1;

			mask = temp32_1 << 1 & 0xffff;
			if (mask & 0x100) {
				mask = 1;
				temp32_4 = *src_buffer++;
			}

			temp32_2 = temp32_4 << 1 & 0xffff;
			i = ((temp32_3 & 0x100) >> 7 | ((temp32_4 << 1 & 0x100) != 0)) + 2;
			backpos = dst_buffer + ((uint32_t)src_buffer[0] | ~0xff);
			if (i == 0) break;
			do {
				*dst_buffer++ = *backpos++;
				i--;
			} while (i != 0);
			src_buffer++;
			mask <<= 1;
		}
	} while (TRUE);
}

int func_8003FDF8() {
	sCdFileCtx.unk_ffd14 = 0;
	sCdFileCtx.unk_ffd0c = 0;
	sCdFileCtx.driveResponse[0] = 0;
	sCdFileCtx.unk_ffd16 = 0;
	sCdFileCtx.slotsUsed = 0;
	sCdFileCtx.nextSlot = 0;
	sCdFileCtx.slotIndex = 0;
	sCdFileCtx.unk_ffddc = 0;
	sCdFileCtx.unk_ffd0a = 0;
	sCdFileCtx.unk_ffd15 = 0;
	CdDrive_CdReadyCallback(func_8003C8E4);
	sApplState.stateFlags |= 0x400;
	return 1;
}

unktype_4 func_8003FE5C() {
	sCdFileCtx.unk_ffd15 = 0;
	sApplState.stateFlags &= ~0x400;
	func_800DD148(0);
	CdDrive_CdReadyCallback(NULL);
	return 1;
}

int func_8003FEA0(unktype_2 arg0) {
	return (sCdFileCtx.unk_ffddc - arg0 & 0xff) > sCdFileCtx.slotsUsed;
}

//8003FEC4
int CdFiles_AllSlotsEmpty() {
	return sCdFileCtx.slotsUsed == 0;
}

int func_8003FED4(int arg0) {
	return (arg0 & 0xffff) < (12 - sCdFileCtx.slotsUsed);
}

void func_8003FEF0(int arg0, int arg1) {
	//The function that requests from VOICE.STR
	CdFiles_requestCdFile(0x80, 0, arg0, arg1, 0, NULL);
}

int func_8003FF24(unktype_2 arg0) {
	//This is the function that requests a stream from MOVIE.BIN
	sUnk_gp.movieContext = Heap_ArenaAlloc(0x544c);
	sUnk_gp.unk_fea80 = Heap_ArenaAlloc(0x46000);
	sUnk_gp.unk_fea84 = Heap_ArenaAlloc(0x10000);
	sUnk_gp.unk_fea88 = Heap_ArenaAlloc(0x11000);

	if ((sUnk_gp.movieContext == -1) || (sUnk_gp.unk_fea80 == -1) || (sUnk_gp.unk_fea84 == -1) || (sUnk_gp.unk_fea88 == -1)) {
		Heap_FreeMemory(sUnk_gp.unk_fea88, 0x11000);
		Heap_FreeMemory(sUnk_gp.unk_fea84, 0x10000);
		Heap_FreeMemory(sUnk_gp.unk_fea80, 0x46000);
		Heap_FreeMemory(sUnk_gp.movieContext, 0x0544c);
		return 0;
	}
	else {
		return CdFiles_requestCdFile(0xc0, arg0, 0, 0, 0, NULL);
	}
}

//func_80040010
int CdFiles_PCLoadFileData(char* filename, int len, u32 offset, void* dst) {
	void* handle;
	int amt;
	
	handle = PCDebug_PCOpen(filename, 0, 0);
	if (handle != -1) {
		if (len == 0) {
			len = PCDebug_PClSeek(handle, 0, 2);
			PCDebug_PClSeek(handle, 0, 0);
		}
		if (offset != 0) {
			PCDebug_PClSeek(handle, offset, 0);
		}
		for (; len != 0; len -= amt) {
			if (len < 0x800) {
				amt = PCDebug_IPCRead(handle, dst, len);
				if (amt == 0) return -1;
			}
			else {
				amt = PCDebug_IPCRead(handle, dst, 0x800);
				if (amt == 0) return -1;
			}
			(u8*)dst += amt;
		}
		PCDebug_PCClose(handle);
		return 0;
	}
	return -1;
}

//func_80040108
int CdFiles_PCWriteFileData(char* filePath, int len, void* src){

	int fileHandle;
	int res;

	byte* srcpos = src;

	fileHandle = PCDebug_PCCreat(filePath, 0);
	if (fileHandle != -1) {
		while (len != 0) {
			if (len < 0x800) {
				res = PCDebug_IPCWrite(fileHandle, srcpos, len);
				len -= res;
				if (res == 0) break;
				srcpos += res;
			}
			else {
				res = PCDebug_IPCWrite(fileHandle, srcpos, 0x800);
				len -= res;
				if (res == 0) break;
				srcpos += res;
			}
		}
		PCDebug_PCClose(fileHandle);
		return 0;
	}
	return -1;
}

void func_800401AC(){
	if (sCdFileCtx.slotsUsed != 0) {
		CURRENT_REQUEST.status = 0;
		if (sCdFileCtx.slotIndex < 11) sCdFileCtx.slotIndex++;
		else sCdFileCtx.slotIndex = 0;
		sCdFileCtx.slotsUsed--;
	}
}

//func_8004021C
void CdFiles_GetPrefixedGamefileName(int16_t fileIndex, char* dst) {
	char* strpos;
	char* strbuff_pos;

	strpos = CDFILE_PATH_PRE;
	strbuff_pos = dst;
	STRCPY_INCR(strbuff_pos, strpos);

	strpos = sGamefileNameTable[fileIndex];
	STRCPY_INCR(strbuff_pos, strpos);
	*dst = '\0';
}

//800402C4
void CdFiles_GetSuffixedGamefileName(int16_t fileIndex, char* dst) {
	char* strpos = sGamefileNameTable[fileIndex];
	char* strbuff_pos = dst;

	if (*strpos != '\0') {
		STRCPY_INCR(strbuff_pos, strpos);

		strpos = CDFILE_PATH_SUF;
		STRCPY_INCR(strbuff_pos, strpos);
		*dst = '\0';
	}
}

int func_80040348(void) {

	uint32_t* addr = NULL;
	uint32_t* header = NULL;
	uint32_t res;

	int ctr = 2000;

	while (TRUE) {
		res = CdDrive_StGetNext(&addr, &header);
		ctr--;
		if (res == 0) {
			//base = local_20;
			if (sMovieCtx->unk_0080 < header[2]) {
				sMovieCtx->unk_0060 = 1;
			}
		}
		else {
			if (ctr != 0) continue;
			addr = NULL;
		}
		if (addr) {
			sMovieCtx->unk_0044.w ^= 0x01;
			PressVlc_DecDCTvlc2(addr, *(&sMovieCtx + 0x10 + (sMovieCtx->unk_0044.w << 2)), sDctTbl);
			CdDrive_StFreeRing(addr);
			return 0;
		}
		if (ctr == 0) return -1;
		ctr = 2000;
	}
}

void func_80040430(MovieStreamContext* movieCtx) {
	int ctr = 2000;

	while (movieCtx->unk_001c == 0) {
		if (--ctr == 0) {
			movieCtx->unk_001c = 1;
			movieCtx->unk_002c ^= 0x01;
			movieCtx->unk_0044.x = movieCtx->unk_0000.x;
			movieCtx->unk_0044.y = movieCtx->unk_0000.y;
			if (movieCtx->unk_002c != 0) {
				movieCtx->unk_0044.y += 0xf0;
			}
		}
	}
	movieCtx->unk_001c = 0;
}

int func_800404B0(char* filename) {
	int fileHandle;

	fileHandle = PCDebug_PCOpen(filename, 0);
	if (fileHandle != -1) {
		PCDebug_PClSeek(fileHandle, 0, 2);
		PCDebug_PClSeek(fileHandle, 0, 0);
		PCDebug_PCClose(fileHandle);
	}
	return -1;
}



