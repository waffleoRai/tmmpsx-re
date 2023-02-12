#include "psxcd_internal.h"

int CdDrive_cacheFile(int index);
int CdDrive_namesEqual(char* str1, char* str2);
int CdDrive_newMedia();
int func_800D9A04(u32 sectors, int relSec, void* dst);

//func_800D9108 CdSearchFile
CdlFILE* CdDrive_SearchFile(CdlFILE* dst, char* filepath) {

	int res;
	int loop_break = FALSE;
	char* cptr1;
	char* cptr2;
	char* message;

	char strBuffer[32];

	int pathLevelOkay;
	int i;
	int pathLevel;
	int dirIndex;

	if (sCdDriveData.unk_fd218[0] != sCdDriveData.unk_fd300) {
		if (!CdDrive_newMedia()) return NULL;
		sCdDriveData.unk_fd218[0] = sCdDriveData.unk_fd300;
	}

	if (*filepath != '\\') return NULL;

	strBuffer[0] = '\0';
	cptr1 = filepath;
	cptr2 = strBuffer;
	pathLevel = 0;
	dirIndex = 1;
	do {
		if (*cptr1 != '\\') {
			do {
				pathLevelOkay = pathLevel < 8;
				if (*cptr1 == '\0') {
					loop_break = TRUE;
					break;
				}
				*cptr2++ = *cptr1++;
			} while (*cptr1 != '\\');
		}
		if (loop_break) break;
		pathLevelOkay = pathLevel < 8;
		if (*cptr1++ == '\0') break;
		*cptr2 = '\0';
		dirIndex = func_800D96C4(dirIndex, strBuffer);
		if (dirIndex == -1) {
			strBuffer[0] = '\0';
			break;
		}
	} while (++pathLevel < 8);
	pathLevelOkay = pathLevel < 8;

	//@ 800D9214
	if (pathLevelOkay) {
		if (strBuffer[0] == '\0') {
			if (sCdDriveData.verbosity < 1) return NULL;
			message = "%s: dir was not found\n";
		}
		else {
			*cptr2 = '\0';
			res = CdDrive_cacheFile(dirIndex);
			if (res == 0) {
				if (sCdDriveData.verbosity > 0) {
					printf("CdSearchFile: disc error\n");
					return NULL;
				}
				return NULL;
			}
			if (sCdDriveData.verbosity > 1) {
				printf("CdSearchFile: searching %s...\n", strBuffer);
			}
			i = 0;
			do {
				if (sCdDriveCtx.fileCache[i].name[0] == '\0') break;
				if (CdDrive_namesEqual(&sCdDriveCtx.fileCache[i].name, strBuffer)) {
					if (sCdDriveData.verbosity > 1) {
						printf("%s:  found\n", strBuffer);
					}
					*dst = sCdDriveCtx.fileCache[i];
					return &sCdDriveCtx.fileCache[i];
				}
			} while (++i < 64);
			filepath = strBuffer;
			if (sCdDriveData.verbosity < 1) return NULL;
			message = "%s: not found\n";
		}
		printf(message, filepath);
	}
	else {
		if (sCdDriveData.verbosity > 0) {
			printf("%s: path level (%d) error\n", filepath, pathLevel);
			return NULL;
		}
	}

	return NULL;
}

//Strings equal?
//CdDrive_namesEqual
//func_800D93E0
int CdDrive_namesEqual(char* str1, char* str2) {
	return strncmp(str1, str2, 12) == 0;
}

//CdDrive_newMedia
//func_800D9400 CD_newmedia
int CdDrive_newMedia() {
	int res, i;
	u8* rec_ptr = &sCdDriveCtx.secdataBuffer;
	int path_table_sec;

	if (func_800D9A04(1, 16, &sCdDriveCtx.secdataBuffer) == 1) {
		//Check if loaded sector is indeed volume descriptor.
		path_table_sec = INT_FROM_POINTER(&sCdDriveCtx.secdataBuffer[0x8c]);
		if (strncmp(&sCdDriveCtx.secdataBuffer[1], "CD001", 5) == 0) {
			//Read the sector containing the first path table
			if (func_800D9A04(1, path_table_sec, &sCdDriveCtx.secdataBuffer)) {
				if (sCdDriveData.verbosity > 1) {
					printf("CD_newmedia: sarching dir..\n");
				}
				for (i = 0; i < 128; i++) {
					//Note: In path table, record[0] is a u8 containing len of dir name
					if (*rec_ptr == '\0') break;
					sCdDriveCtx.pathTable[i].sector = INT_FROM_POINTER(rec_ptr + 2);
					//dst = &sCdUnk->unk_10cad8[i].unk_0c
					sCdDriveCtx.pathTable[i].recordIndex = i + 1;
					sCdDriveCtx.pathTable[i].parentIndex = INT_FROM_POINTER(rec_ptr + 6);
					memcpy(&sCdDriveCtx.pathTable[i].name, rec_ptr + 8, *rec_ptr);
					sCdDriveCtx.pathTable[i].name[*rec_ptr] = '\0';
					rec_ptr += *rec_ptr + (*rec_ptr & 0x1) + 8; //Next record in table.
					if (sCdDriveData.verbosity > 1) {
						printf("\t%08x,%04x,%04x,%s\n", sCdDriveCtx.pathTable[i].sector,
							sCdDriveCtx.pathTable[i].recordIndex, sCdDriveCtx.pathTable[i].parentIndex, &sCdDriveCtx.pathTable[i].name);
					}
					if (rec_ptr >= (&sCdDriveCtx.pathTable + 0x800)) break;
				}
				if (i < 128) {
					sCdDriveCtx.pathTable[i].parentIndex = 0;
				}
				sCdDriveData.cachedDirId = 0;
				if (sCdDriveData.verbosity > 1) {
					printf("CD_newmedia: %d dir entries found\n", i);
				}
				return TRUE;
			}
			else {
				if (sCdDriveData.verbosity > 1) {
					printf("CD_newmedia: Read error (PT:%08x)\n", path_table_sec);
				}
				return FALSE;
			}
		}
		else {
			if (sCdDriveData.verbosity > 1) {
				printf("CD_newmedia: Disc format error in cd_read(PVD)\n");
			}
			return FALSE;
		}
	}
	else {
		if (sCdDriveData.verbosity > 1) {
			printf("CD_newmedia: Read error in cd_read(PVD)\n");
		}
		return FALSE;
	}
}

//func_800D96C4
int func_800D96C4(int parentIdx, char* dirname) {
	int i = 0;
	//mystery struct size is 44 (0x2c)

	for (i = 0; i < 128; i++) {
		if (sCdDriveCtx.pathTable[i].parentIndex == 0) return -1;
		if ((sCdDriveCtx.pathTable[i].parentIndex == parentIdx) && (strcmp(dirname, &sCdDriveCtx.pathTable[i].name) == 0)) return i;
	}

	return -1;
}

/* ----------------------- ???.OBJ ----------------------- */
//CdDrive_cacheFile
//func_800D9768 - CD_cachefile
int CdDrive_cacheFile(int dirId) {
	//This actually caches a directory.

	int res, i;
	u8* ptr;

	if (sCdDriveData.cachedDirId != dirId) {
		res = func_800D9A04(1, sCdDriveCtx.pathTable[dirId - 1].sector, &sCdDriveCtx.secdataBuffer);
		if (res != 1) {
			if (sCdDriveData.verbosity < 1) return -1;
			printf("CD_cachefile: dir not found\n");
			return -1;
		}
		if (sCdDriveData.verbosity > 1) {
			printf("CD_cachefile: searching...\n");
		}

		ptr = &sCdDriveCtx.secdataBuffer;
		for (i = 0; i < 64; i++) {
			if (*ptr == '\0') break;
			CdDrive_CdIntToPos(INT_FROM_POINTER(ptr+2), &sCdDriveCtx.fileCache[i].pos);
			sCdDriveCtx.fileCache[i].size = INT_FROM_POINTER(ptr + 10);
			if (i == 0) {
				sCdDriveCtx.fileCache[i].name[0] = '.';
				sCdDriveCtx.fileCache[i].name[1] = '\0';
			}
			else if (i == 1) {
				sCdDriveCtx.fileCache[i].name[0] = '.';
				sCdDriveCtx.fileCache[i].name[1] = '.';
				sCdDriveCtx.fileCache[i].name[2] = '\0';
			}
			else {
				memcpy(&sCdDriveCtx.fileCache[i].name, ptr + 0x21, *(ptr + 0x20));
				sCdDriveCtx.fileCache[i].name[*(ptr + 0x20)] = '\0';
			}
			if (sCdDriveData.verbosity > 1) {
				printf("\t(%02x:%02x:%02x) %8d %s\n", sCdDriveCtx.fileCache[i].pos.minute, sCdDriveCtx.fileCache[i].pos.second, 
					sCdDriveCtx.fileCache[i].pos.sector, sCdDriveCtx.fileCache[i].size, &sCdDriveCtx.fileCache[i].name);
			}
			ptr += *ptr;
		}

		sCdDriveData.cachedDirId = dirId;
		if (i < 64) sCdDriveCtx.fileCache[i].name[0] = '\0';
		if (sCdDriveData.verbosity > 1) {
			printf("CD_cachefile: %d files found\n", i);
		}
	}
	return 1;
}

//func_800D9A04
int func_800D9A04(u32 sectors, int relSec, void* dst) {
	u8* buffer[8];

	CdDrive_CdIntToPos(relSec, buffer);
	CdDrive_CdControl(CdCommand_Setloc, buffer, NULL);
	CdDrive_CdRead(sectors, (uint32_t*)dst, 0x80);

	return (CdDrive_CdReadSync(0, 0) == 0);
}