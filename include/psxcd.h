#ifndef PSX_LIBCD_H
#define PSX_LIBCD_H

//A lot of this is from LIBCD.h

#include "global.h"

/*----- Macros -----*/

#define CdlMAXFILE	64	/* max number of files in a directory */
#define CdlMAXDIR	128	/* max number of total directories */
#define CdlMAXLEVEL	8	/* max levels of directories */

/*
 * CD-ROM Mode (used int CdlSetmode)
 */
#define CdlModeStream  0x100    /* Normal Streaming                     */
#define CdlModeStream2 0x120    /* SUB HEADER information includes      */
#define CdlModeSpeed	0x80	/* 0: normal speed	1: double speed	*/
#define CdlModeRT	0x40	/* 0: ADPCM off		1: ADPCM on	*/
#define CdlModeSize1	0x20	/* 0: 2048 byte		1: 2340byte	*/
#define CdlModeSize0	0x10	/* 0: -			1: 2328byte	*/
#define CdlModeSF	0x08	/* 0: Channel off	1: Channel on	*/
#define CdlModeRept  	0x04	/* 0: Report off	1: Report on	*/
#define CdlModeAP	0x02	/* 0: AutoPause off	1: AutoPause on */
#define CdlModeDA	0x01	/* 0: CD-DA off		1: CD-DA on	*/

 /*
  * Status Contents
  */
#define CdlStatPlay		0x80	/* playing CD-DA */
#define CdlStatSeek		0x40	/* seeking */
#define CdlStatRead		0x20	/* reading data sectors */
#define CdlStatShellOpen	0x10	/* once shell open */
#define CdlStatSeekError	0x04	/* seek error detected */
#define CdlStatStandby		0x02	/* spindle motor rotating */
#define CdlStatError		0x01	/* command error detected */

/*----- Typedefs -----*/

typedef void (*CdlCB)(uint8_t, uint8_t*);

/*----- Enums -----*/

typedef enum CdDriveIntrStatus {
	/* 00 */ CdIntr_NoIntr,
	/* 01 */ CdIntr_DataReady,
	/* 02 */ CdIntr_Complete,
	/* 03 */ CdIntr_Acknowledge,
	/* 04 */ CdIntr_DataEnd,
	/* 05 */ CdIntr_DiskError
} CdDriveIntrStatus;

typedef enum CdDriveDiskType {
	/* 00 */ CdDiskType_StatNoDisk,
	/* 01 */ CdDiskType_OtherFormat,
	/* 02 */ CdDiskType_CdromFormat
} CdDriveDiskType;

typedef enum CdDriveCommand {
	/* 00 */ CdCommand_Sync,
	/* 01 */ CdCommand_Getstat,
	/* 02 */ CdCommand_Setloc,
	/* 03 */ CdCommand_Play,
	/* 04 */ CdCommand_Forward,
	/* 05 */ CdCommand_Backward,
	/* 06 */ CdCommand_ReadN,
	/* 07 */ CdCommand_MotorOn,
	/* 08 */ CdCommand_Stop,
	/* 09 */ CdCommand_Pause,
	/* 10 */ CdCommand_Init,
	/* 11 */ CdCommand_Mute,
	/* 12 */ CdCommand_Demute,
	/* 13 */ CdCommand_Setfilter,
	/* 14 */ CdCommand_Setmode,
	/* 15 */ CdCommand_Getparam,
	/* 16 */ CdCommand_GetlocL,
	/* 17 */ CdCommand_GetlocP,
	/* 18 */ CdCommand_SetSession,
	/* 19 */ CdCommand_GetTN,
	/* 20 */ CdCommand_GetTD,
	/* 21 */ CdCommand_SeekL,
	/* 22 */ CdCommand_SeekP,
	/* 23 */ CdCommand_SetClock,
	/* 24 */ CdCommand_GetClock,
	/* 25 */ CdCommand_Test,
	/* 26 */ CdCommand_GetID,
	/* 27 */ CdCommand_ReadS,
	/* 28 */ CdCommand_Reset,
	/* 29 */ CdCommand_GetQ,
	/* 30 */ CdCommand_ReadTOC,
} CdDriveCommand;

/*----- Struct Defs -----*/

typedef struct {
	uint8_t minute;		/* minute (BCD) */
	uint8_t second;		/* second (BCD) */
	uint8_t sector;		/* sector (BCD) */
	uint8_t track;		/* track (void) */
} CdlLOC;

typedef struct {
	uint8_t	file;		/* file ID (always 1) */
	uint8_t	chan;		/* channel ID */
	uint8_t	pad;
} CdlFILTER;

typedef struct {
	uint8_t	val0;		/* volume for CD(L) -> SPU (L) */
	uint8_t	val1;		/* volume for CD(L) -> SPU (R) */
	uint8_t	val2;		/* volume for CD(R) -> SPU (L) */
	uint8_t	val3;		/* volume for CD(R) -> SPU (R) */
} CdlATV;

typedef struct {
	/*0x00*/ CdlLOC	 pos;		/* file location */
	/*0x04*/ uint32_t size;		/* file size */
	/*0x08*/ char	 name[16];	/* file name (body) */
} CdlFILE;

typedef struct {
	uint16_t  id;
	uint16_t  type;
	uint16_t  secCount;
	uint16_t  nSectors;
	uint32_t  frameCount;
	uint32_t  frameSize;

	uint16_t  width;
	uint16_t  height;
	uint32_t  dummy1;
	uint32_t  dummy2;
	CdlLOC	  loc;
} StHEADER;             /* CD-ROM STR structure */


/*----- Stream Lib -----*/

void	CdDrive_StSetRing(uint32_t* ring_addr, uint32_t ring_size);
void	CdDrive_StClearRing(void);
void	CdDrive_StUnSetRing(void);

void	CdDrive_StSetStream(uint32_t mode, uint32_t start_frame, uint32_t end_frame,
	void (*func1)(), void (*func2)());

void	CdDrive_StSetEmulate(uint32_t* addr, uint32_t mode, uint32_t start_frame,
	uint32_t end_frame, void (*func1)(), void (*func2)());

uint32_t	CdDrive_StFreeRing(uint32_t* base);
uint32_t	CdDrive_StGetNext(uint32_t** addr, uint32_t** header);
uint32_t	CdDrive_StGetNextS(uint32_t** addr, uint32_t** header);
uint16_t	CdDrive_StNextStatus(uint32_t** addr, uint32_t** header);
void    CdDrive_StRingStatus(int16_t* free_sectors, int16_t* over_sectors);
void	CdDrive_StSetMask(uint32_t mask, uint32_t start, uint32_t end);
void	CdDrive_StCdInterrupt(void);
int     CdDrive_StGetBackloc(CdlLOC* loc);
int     CdDrive_StSetChannel(uint32_t channel);

/*----- CD Lib -----*/

int CdDrive_CdSetDebug(int level);

int32_t CdDrive_CdPosToInt(CdlLOC* bcdTime);
CdlLOC* CdDrive_CdIntToPos(int i, CdlLOC* p);

int CdDrive_CdInit();
CdlFILE* CdDrive_SearchFile(CdlFILE* dst, char* filepath);

int CdDrive_CdControl(CdDriveCommand com, uint8_t* param, uint8_t* result);
int CdDrive_CdControlB(CdDriveCommand com, uint8_t* param, uint8_t* result);
int CdDrive_CdControlF(CdDriveCommand com, uint8_t* param);

int CdDrive_CdRead(int sectors, uint32_t* buf, int mode);
int CdDrive_CdRead2(long mode);
int CdDrive_CdReadFile(char* file, uint32_t* addr, int nbyte);
int CdDrive_CdReadSync(int mode, uint32_t* result);

int CdDrive_CdMix(CdlATV* vol);

int CdDrive_CdDataSync(int mode);

void(*CdDrive_CdDataCallback(void (*func)()));
CdlCB CdDrive_CdReadCallback(CdlCB func);
CdlCB CdDrive_CdReadyCallback(CdlCB func);

int CdDrive_CdGetSector(void* madr, int size);
int CdDrive_CdGetSector2(void* madr, int size);

/*----- Unidentified -----*/

void func_800DD148(int arg0);


#endif //PSX_LIBCD_H
