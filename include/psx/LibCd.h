#ifndef PSXSDK_LIBCD_H
#define PSXSDK_LIBCD_H

#include "psx/PSXTypes.h"

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

#define CdlStatPlay		0x80	/* playing CD-DA */
#define CdlStatSeek		0x40	/* seeking */
#define CdlStatRead		0x20	/* reading data sectors */
#define CdlStatShellOpen	0x10	/* once shell open */
#define CdlStatSeekError	0x04	/* seek error detected */
#define CdlStatStandby		0x02	/* spindle motor rotating */
#define CdlStatError		0x01	/* command error detected */

#define CdlStatNoDisk	0
#define CdlOtherFormat	1
#define CdlCdromFormat	2

#define CdlNoIntr	0x00	/* No interrupt */
#define CdlDataReady	0x01	/* Data Ready */
#define CdlComplete	0x02	/* Command Complete */
#define CdlAcknowledge	0x03	/* Acknowledge (reserved) */
#define CdlDataEnd	0x04	/* End of Data Detected */
#define CdlDiskError	0x05	/* Error Detected */

#ifndef btoi
#define btoi(b)		((b)/16*10 + (b)%16)		/* BCD to u_char */
#endif
#ifndef itob
#define itob(i)		((i)/10*16 + (i)%10)		/* u_char to BCD */
#endif

#define CdSeekL(p)	CdControl(CdlSeekL, (u_char *)p, 0)
#define CdSeekP(p)	CdControl(CdlSeekP, (u_char *)p, 0)
#define CdStandby()	CdControl(CdlStandby,  0, 0)
#define CdPause()	CdControl(CdlPause,    0, 0)
#define CdStop()	CdControl(CdlStop,     0, 0)
#define CdMute()	CdControl(CdlMute,     0, 0)
#define CdDeMute()	CdControl(CdlDemute,   0, 0)
#define CdForward()	CdControl(CdlForward,  0, 0)
#define CdBackward()	CdControl(CdlBackward, 0, 0)

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum CdlCommand {
		/*0x01*/ CdlNop = 1,
		/*0x02*/ CdlSetloc,
		/*0x03*/ CdlPlay,
		/*0x04*/ CdlForward,
		/*0x05*/ CdlBackward,
		/*0x06*/ CdlReadN,
		/*0x07*/ CdlStandby,
		/*0x08*/ CdlStop,
		/*0x09*/ CdlPause,
		/*0x0a*/ Cdl_0a,
		/*0x0b*/ CdlMute,
		/*0x0c*/ CdlDemute,
		/*0x0d*/ CdlSetfilter,
		/*0x0e*/ CdlSetmode,
		/*0x0f*/ CdlGetlocL,
		/*0x10*/ CdlGetlocP,
		/*0x15*/ CdlSeekL = 0x15,
		/*0x16*/ CdlSeekP,
		/*0x1b*/ CdlReadS = 0x1b
} CdlCommand;

typedef struct {
	uint8_t minute;
	uint8_t second;
	uint8_t sector;
	uint8_t track;
} CdlLOC; //Size = 0x4

typedef struct {
	/*0x00*/	CdlLOC pos;
	/*0x04*/	uint32_t size;
	/*0x08*/	char name[16];
} CdlFILE; //Size = 0x18

typedef struct {
	uint8_t	val0;		/* volume for CD(L) -> SPU (L) */
	uint8_t	val1;		/* volume for CD(L) -> SPU (R) */
	uint8_t	val2;		/* volume for CD(R) -> SPU (L) */
	uint8_t	val3;		/* volume for CD(R) -> SPU (R) */
} CdlATV; //Size = 0x4

typedef void (*CdlCB)(uint8_t,uint8_t*);

extern int32_t D_libcd_cdrom_10c49c;

extern void CdDrive_StSetRing(uint32_t* ring_addr, uint32_t ring_size);
extern int CdDrive_CdInit(void);
extern int CdDrive_CdPosToInt(CdlLOC *p);
extern CdlFILE* CdDrive_CdSearchFile(CdlFILE* fp, char* name);
extern int CdDrive_CdRead2(int32_t mode);
extern void CdDrive_StClearRing(void);
extern int CdDrive_StGetBackloc(CdlLOC* loc);
extern void CdDrive_StSetStream(uint32_t mode,uint32_t start_frame,uint32_t end_frame, CallbackFunction func1, CallbackFunction func2);
extern uint32_t CdDrive_StFreeRing(uint32_t* base);
extern uint32_t CdDrive_StGetNext(uint32_t** addr, uint32_t** header);
extern void CdDrive_StSetMask(uint32_t mask, uint32_t start, uint32_t end);
extern void CdDrive_StCdInterrupt(void);
extern CdlLOC* CdDrive_CdIntToPos(int i, CdlLOC* p);
extern int CdDrive_CdSetDebug(int level);
extern int CdDrive_CdSync(int mode, uint8_t* result) ;
extern int CdDrive_CdReady(int mode, uint8_t* result) ;
extern int CdDrive_CdControl(uint8_t com, uint8_t* param, uint8_t* result);
extern int CdDrive_CdControlB(uint8_t com, uint8_t* param, uint8_t* result);
extern int CdDrive_CdControlF(uint8_t com, uint8_t* param);
extern int CdDrive_CdMix(CdlATV* vol);
extern int CdDrive_CdGetSector(void* madr, int size);
extern int CdDrive_CdGetSector2( void* madr, int size );
extern int CdDrive_CdDataSync(int mode);
extern void CdDrive_CdReadBreak(void);
extern int CdDrive_CdRead(int sectors, uint32_t* buf, int mode);
extern int CdDrive_CdReadSync(int mode, uint8_t* result);
extern int CdDrive_CdStatus(void);
extern CdlLOC* CdDrive_CdLastPos(void);
extern int CdDrive_CdMode(void);
extern void CdDrive_CdFlush(void);

extern void (*CdDrive_CdDataCallback(CallbackFunction func));
extern CdlCB CdDrive_CdReadCallback(CdlCB func);
extern CdlCB CdDrive_CdReadyCallback(CdlCB func);
extern CdlCB CdDrive_CdSyncCallback(CdlCB func);

#ifdef __cplusplus
}
#endif

#endif