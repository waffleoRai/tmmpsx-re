#ifndef PSXSDK_LIBAPI_H
#define PSXSDK_LIBAPI_H

#include "psx/PSXTypes.h"

//Derived from SDK version 4.6 LIBAPI.H, KERNEL.H

#ifdef __cplusplus
extern "C" {
#endif

struct DIRENTRY {
	char name[20];
	int32_t attr;
	int32_t size;
	struct DIRENTRY *next;
	int32_t head;
	int8_t system[4];
};

extern void PsApi_InitHeap(uint32_t* head, uint32_t size);

extern void PsApi_FlushCache(void);
extern void PsApi_ReturnFromException(void);
extern int  PsApi_EnterCriticalSection(void);
extern void PsApi_ExitCriticalSection(void);

extern void PsApi_96_remove(void);

extern int32_t PsApi_OpenEvent(uint32_t desc, int32_t spec, int32_t mode, int32_t* handler());
extern BOOL PsApi_CloseEvent(int32_t event);
extern void PsApi_DeliverEvent(uint32_t ev1, int32_t ev2);
extern BOOL PsApi_WaitEvent(int32_t event);
extern BOOL PsApi_TestEvent(int32_t event);
extern BOOL PsApi_EnableEvent(uint32_t event);
extern BOOL PsApi_DisableEvent(uint32_t event);

extern int32_t PsApi_open(char* devname, uint32_t flag);
extern int32_t PsApi_close(int32_t fd);
extern uint32_t PsApi_lseek(int32_t fd, uint32_t offset, int32_t flag);
extern int32_t PsApi_read(int32_t fd, void* buf, int32_t n);
extern int32_t PsApi_write(int32_t fd, void* buf, int32_t n);
extern struct DIRENTRY* PsApi_First_firstfile(char* name, struct DIRENTRY* dir);
extern struct DIRENTRY* PsApi_nextfile(struct DIRENTRY* dir);

extern void PsApi_ChangeClearPAD(int32_t val);
extern int32_t PsApi_InitPAD(char* bufA, int32_t lenA, char* bufB, int32_t lenB);
extern int32_t PsApi_StartPAD(void);
extern void PsApi_Patch_EnablePAD(void);
extern void PsApi_Patch_DisablePAD(void);

extern BOOL PsApi_Counter_SetRCnt(uint32_t spec, uint16_t target, int32_t mode);
extern int32_t PsApi_Counter_GetRCnt(uint32_t spec);
extern BOOL PsApi_Counter_ResetRCnt(uint32_t spec);
extern BOOL PsApi_Counter_StartRCnt(uint32_t spec);
extern BOOL PsApi_Counter_StopRCnt(uint32_t spec);

//PsApi_GPU_cw
//PsApi_ResetEntryInt
//PsApi_HookEntryInt
//PsApi_SysEnqIntRP
//PsApi_SysDeqIntRP
//PsApi_ChangeClearRCnt

#ifdef __cplusplus
}
#endif

#endif