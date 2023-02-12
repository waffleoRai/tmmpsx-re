#include "psxcd_internal.h"

int CdDrive_CdInitInternal();
void CdDrive_EventCallback_d9004(CdDriveIntrStatus intrStat, void* response);
void CdDrive_EventCallback_d902c(CdDriveIntrStatus intrStat, void* response);
void CdDrive_EventCallback_d9054();

//func_800D8F38 CdInit
int CdDrive_CdInit() {
	int i;
	for (i = 0; i < 4; i++) {
		if (CdDrive_CdInitInternal()) {
			sCdDriveData.unk_fd204 = CdDrive_EventCallback_d9054;
			sCdDriveData.unk_fd208 = 0;
			sCdDriveData.syncCallback = CdDrive_EventCallback_d9004;
			sCdDriveData.readyCallback = CdDrive_EventCallback_d902c;
			return TRUE;
		}
	}
	printf("CdInit: Init failed\n");
	return FALSE;
}

//func_800D8FC8
int CdDrive_CdInitInternal() {
	if (CdDrive_CdBiosInit() == 0) {
		return (CdDrive_CdBiosInitVol() == 0);
	}
	return FALSE;
}

//func_800D9004
void CdDrive_EventCallback_d9004(CdDriveIntrStatus intrStat, void* response) {
	Bios_DeliverEvent(0xf0000003, 0x20);
}

//func_800D902C
void CdDrive_EventCallback_d902c(CdDriveIntrStatus intrStat, void* response) {
	Bios_DeliverEvent(0xf0000003, 0x40);
}

//func_800D9054
void CdDrive_EventCallback_d9054() {
	Bios_DeliverEvent(0xf0000003, 0x40);
}

//func_800D9088 CdPosToInt? That would be in SYS.OBJ
int32_t CdDrive_CdPosToInt(CdlLOC* bcdTime) {
	int32_t val = (int32_t)(bcdTime->minute >> 4) * 10;
	val += (int32_t)(bcdTime->minute & 0xf);
	val *= 60;
	val += (int32_t)(bcdTime->second >> 4) * 10;
	val += (int32_t)(bcdTime->second & 0xf);
	val *= 75;
	val += (int32_t)(bcdTime->sector >> 4) * 10;
	val += (int32_t)(bcdTime->sector & 0xf);
	val -= 150;
	return val;
}