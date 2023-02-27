#include "AudioLoad.h"

void func_80041958(int);
void func_80042770(int);

uint32_t* func_8004051C() {

	uint32_t* hdr;
	uint32_t* addr;
	int res;

	int ctr = 2000;
	while (ctr-- != 0) {
		res = CdDrive_StGetNext(&addr, &hdr);
		if (res == 0) {
			if (hdr[2] < *(sUnk_gp.unk_fea7c + 0x40)) return addr; //Checked addr is PROBABLY movieCtx->unk_0040
			*(sUnk_gp.unk_fea7c + 0x30) = 1; //Store target is PROBABLY movieCtx->unk_0030
			return addr;
		}
	}
	return NULL;
}

int func_800405A4(void) {

	int i;
	int32_t* ptr;

	sAudioCtx.unk_10001c = 0;
	sAudioCtx.unk_0fff10 = &sAudioCtx.unk_0fff18;
	sAudioCtx.unk_0fff14 = &sAudioCtx.unk_0fff18;
	sAudioCtx.unk_10006d = 0x80;
	sAudioCtx.seqFileIndex = 0xff;
	sAudioCtx.unk_100069 = 0xff;
	sAudioCtx.seqVolume.left = 0;
	sAudioCtx.seqVolume.right = 0;
	sAudioCtx.unk_10006c = 0;
	sAudioCtx.unk_100076 = 0;
	sAudioCtx.unk_100074 = -1;
	sAudioCtx.unk_10001e = 0;
	sAudioCtx.unk_10006e = 0;
	sAudioCtx.unk_10001d = 0;
	sAudioCtx.unk_100040 = sUnkBuffer_1e8000;
	for (i = 0; i < 6; i++) {
		sAudioCtx.seqIds[i] = -1;
	}
	for (i = 0; i < 4; i++) {
		sAudioCtx.vabIds[i] = -1;
		sAudioCtx.unk_100082[i] = -1;
		sAudioCtx.unk_100030[i] = 0;
		sAudioCtx.unk_100020[i] = sAudioCtx.unk_100040;
	}

	//Set all words from fff14 - 100014 to ~0
	ptr = &sAudioCtx.unk_100014;
	for (i = 0; i < 0x40; i++) {
		*ptr = -1;
	}

	for (i = 0; i < 4; i++) {
		sAudioCtx.unk_100078[i] = -1;
	}

	sAudioCtx.unk_10008a = 0;
	SoundSs_SsInit();
	SoundSs_SsSetTableSize(&sGlobalCtx.seqAttrTable, 6, 1);
	SoundVs_SsSetReservedVoice(24);
	SoundUt_SsUtSetReverbType(SPU_REV_MODE_STUDIO_C);
	SoundUt_SsUtSetReverbDepth(0, 0);
	SoundUt_SsUtSetReverbFeedback(0);
	SoundUt_SsUtSetReverbDelay(0);
	func_800CBC18(4);
	SoundUt_SsUtReverbOn();
	SoundSs_SsSetTickMode(SS_NOTICK);
	SoundSs_SsStart2();
	SoundSs_SsSetMVol(127, 127);
	SoundVs_SsSetStereo();
	Spu_SpuSetTransferMode(SPU_TRANSFER_BY_DMA);
	SoundSs_SsSetSerialAttr(SS_SERIAL_A, SS_MIX, SS_SOFF);
	SoundSs_SsSetSerialAttr(SS_SERIAL_A, SS_REV, SS_SOFF);
	SoundSs_SsSetSerialVol(SS_SERIAL_A, 127, 127);
	SoundSs_SsSetSerialAttr(SS_SERIAL_B, SS_MIX, SS_SOFF);
	SoundSs_SsSetSerialAttr(SS_SERIAL_B, SS_REV, SS_SOFF);
	SoundSs_SsSetSerialVol(SS_SERIAL_B, 0, 0);

	sCdFileCtx.unk_ffdc0.val0 = 0x80;
	sCdFileCtx.unk_ffdc0.val1 = 0x00;
	sCdFileCtx.unk_ffdc0.val2 = 0x80;
	sCdFileCtx.unk_ffdc0.val3 = 0x00;

	CdDrive_CdMix(&sCdFileCtx.unk_ffdc0);
	return 1;
}

int FUN_800407cc(int arg0) {
	sAudioCtx.unk_10001c = 0;
	sAudioCtx.unk_0fff10 = &sAudioCtx.unk_0fff18;
	sAudioCtx.unk_0fff14 = &sAudioCtx.unk_0fff18;
	SoundSs_SsSetSerialAttr(SS_SERIAL_A, SS_MIX, SS_SOFF);
	SoundSs_SsSetSerialAttr(SS_SERIAL_A, SS_REV, SS_SOFF);
	SoundSs_SsSetSerialVol(SS_SERIAL_A, 0, 0);
	SoundSs_SsSetSerialAttr(SS_SERIAL_B, SS_MIX, SS_SOFF);
	SoundSs_SsSetSerialAttr(SS_SERIAL_B, SS_REV, SS_SOFF);
	SoundSs_SsSetSerialVol(SS_SERIAL_B, 0, 0);
	SoundVs_SsVabTransCompleted(SS_WAIT_COMPLETED);
	func_80041958(3);
	func_80041958(2);
	if (sAudioCtx.seqIds[1] != -1) {
		sAudioCtx.seqVolume.right = 0;
		sAudioCtx.seqVolume.left = 0;
		SoundSs_SsSeqSetVol(sAudioCtx.seqIds[0], 0, 0);
		SoundUt_SsUtAllKeyOff(0);
		func_80042770(0);
		sAudioCtx.seqIds[0] = sAudioCtx.seqIds[1];
		sAudioCtx.seqIds[1] = -1;
		sAudioCtx.unk_10006a = 0xff;
		sAudioCtx.unk_10006b = 0xff;
	}

	if (sAudioCtx.seqIds[0] != -1) {
		sAudioCtx.seqVolume.right = 0;
		sAudioCtx.seqVolume.left = 0;
		SoundSs_SsSeqSetVol(sAudioCtx.seqIds[0], 0, 0);
		SoundUt_SsUtAllKeyOff(0);
		func_80041958(1);
	}
	sAudioCtx.unk_10006d = 0x80;
	if (arg0 != 0) {
		func_80041958(0);
		SoundSs_SsSetMVol(0, 0);
		SoundUt_SsUtAllKeyOff(0);
		SoundUt_SsUtReverbOff();
		SoundUt_SsUtSetReverbType(SPU_REV_MODE_OFF);
		SoundUt_SsUtSetReverbDepth(0, 0);
		SoundUt_SsUtSetReverbFeedback(0);
		SoundUt_SsUtSetReverbDelay(0);
		EtcVid_VSync(0);
		EtcVid_VSync(0);
		EtcVid_VSync(0);
		SoundSs_SsEnd();
		func_800D09D8();
	}
	sAudioCtx.unk_10006c = 0;
	return 1;
}

void* func_80040994() {
	if (((sAudioCtx.unk_10006d + 0x7e) < 2) && (sAudioCtx.unk_10006e == 0)) {
		if (sAudioCtx.unk_10006d == -0x7e) {
			SoundSs_SsSeqGetVol(sAudioCtx.seqIds[0], 0, &sAudioCtx.seqVolume.left, &sAudioCtx.seqVolume.right);
			sAudioCtx.unk_10006d = -0x7f;
		}
		else {
			sAudioCtx.seqVolume.right = 0;
			sAudioCtx.seqVolume.left = 0;
			SoundSs_SsSeqPause(sAudioCtx.seqIds[0]);
			sAudioCtx.unk_10006d = -0x7c;
		}
	}
	else sAudioCtx.unk_10006e--;

	if (sAudioCtx.unk_10001c != 0) {
		sAudioCtx.unk_100018.asInt = *(u32*)sAudioCtx.unk_0fff14;
		if (sAudioCtx.unk_100018.asBytes.b3 < 0x18) {
			return sUnk_e1f68.unk_e2004[sAudioCtx.unk_100018.asBytes.b3];
		}
	}
	return NULL;
}

int func_80040A90() {

	int res;

	if (sAudioCtx.unk_100018.asBytes.b0 == 0) {
		//^Lowest byte of 100018.
		sAudioCtx.vabLoadSlot = 1;
		switch (sAudioCtx.unk_10006d) {
		case 0x80:
			func_80041958(3);
			func_80041958(2);
			break;
		case 0x81:
		case 0x82:
		case 0x84:
			func_80041958(3);
			func_80041958(2);
			sAudioCtx.seqVolume.right = 0;
			sAudioCtx.seqVolume.left = 0;
			func_80041958(1);
			sAudioCtx.unk_10006d = 0x80;
			break;
		case 0x83: return 1;
		}

		sAudioCtx.seqFileIndex = sAudioCtx.unk_100018.asBytes.b2; //Third byte of 100018
		if (sAudioCtx.unk_100018.asBytes.b2 > 0x3e) sAudioCtx.seqFileIndex = 0;
		sAudioCtx.seqVolume.left = 0;
		sAudioCtx.seqVolume.right = 0;

		sAudioCtx.loadFileOffset = sGlobalData.fileposTableBgm[sAudioCtx.seqFileIndex];
		sAudioCtx.loadFileSize = sGlobalData.fileposTableBgm[sAudioCtx.seqFileIndex+1] - (sAudioCtx.loadFileOffset - 4); //Size
		if (sAudioCtx.loadFileSize > 0x7ff) {
			//Probably too big to fit in a buffer in one go? (ie. larger than one CD sector)
			sAudioCtx.loadFileOffset += ((u32)sAudioCtx.seqFileIndex << 2); //Account for the relative offset value
			sAudioCtx.unk_10004c = sAudioCtx.loadFileOffset & 0x7ff;
			sAudioCtx.unk_100064 = sCdFileCtx.unk_ffddc;
			res = CdFiles_requestCdFile(0x40, D_Bgm, sAudioCtx.loadFileOffset, -1, sAudioCtx.loadFileSize, NULL);
			if (res != 0) {
				sAudioCtx.unk_10006c = 1;
				*(u32*)sAudioCtx.unk_0fff14 = sAudioCtx.unk_100018.asInt + 1;
				return 1;
			}
			if (sAudioCtx.unk_10001c != 0) {
				(byte*)sAudioCtx.unk_0fff14 += 4; //It's probably actually a uint ptr?
				if (sAudioCtx.unk_0fff14 == &sAudioCtx.unk_100018) {
					sAudioCtx.unk_0fff14 = &sAudioCtx.unk_0fff18;
				}
				sAudioCtx.unk_10001c--;
			}
			return 0;
		}
	}
	else {
		if (sAudioCtx.unk_100018.asBytes.b0 == 1) {
			res = func_8003FEA0(sAudioCtx.unk_100064);
			if (res == 0) return 1;
			sAudioCtx.unk_10006c = 0;
			if (sAudioCtx.unk_10001c != 0) {
				(byte*)sAudioCtx.unk_0fff14 += 4;
				if (sAudioCtx.unk_0fff14 == &sAudioCtx.unk_100018) {
					sAudioCtx.unk_0fff14 = &sAudioCtx.unk_0fff18;
				}
				sAudioCtx.unk_10001c--;
			}
			return 0;
		}
		return 1;
	}
}

unktype_4 func_80040CFC(void) {

	s16 vabid;
	u32 temp32;

	if (sAudioCtx.unk_100018.asBytes.b1 < (*(u32*)(sAudioCtx.unk_100020[1] + 0xc) - 4 >> 2)) {
		sAudioCtx.unk_100069 = sAudioCtx.unk_100018.asBytes.b1;
		temp32 = ((u32)sAudioCtx.unk_100018.asBytes.b1 << 2) + 12;
		vabid = sAudioCtx.vabIds[1];
		if (sAudioCtx.seqFileIndex > -1) {
			vabid = sAudioCtx.vabIds[0];
		}
		sAudioCtx.seqIds[0] = SoundSs_SsSeqOpen(sAudioCtx.unk_100020[1] + *(u32*)(sAudioCtx.unk_100020[1] + temp32) + temp32, vabid);
		SoundSs_SsSeqSetVol(sAudioCtx.seqIds[0], sAudioCtx.seqVolume.left, sAudioCtx.seqVolume.right);
		if (sAudioCtx.unk_100018.asBytes.b2 == 0) {
			SoundUt_SsUtAllKeyOff(0);
		}

		if (sAudioCtx.unk_100018.asBytes.b0 == 0) {
			SoundSs_SsSeqPlay(sAudioCtx.seqIds[0], SSPLAY_PLAY, SSPLAY_INFINITY);
		}
		else {
			SoundSs_SsSeqPlay(sAudioCtx.seqIds[0], SSPLAY_PLAY, sAudioCtx.unk_100018.asBytes.b0);
		}
		sAudioCtx.unk_10006c = 0;
		sAudioCtx.unk_10006d = 0x81;
	}
	if (sAudioCtx.unk_10001c != 0) {
		(byte*)sAudioCtx.unk_0fff14 += 4;
		if (sAudioCtx.unk_0fff14 == &sAudioCtx.unk_100018) {
			sAudioCtx.unk_0fff14 = &sAudioCtx.unk_0fff18;
		}
		sAudioCtx.unk_10001c--;
	}
	return 1;
}