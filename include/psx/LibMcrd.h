#ifndef PSXSDK_LIBMCRD_H
#define PSXSDK_LIBMCRD_H

#include "psx/LibCard.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void Mcrd_MemCardInit(int32_t val);
extern void Mcrd_MemCardEnd(void);
extern void Mcrd_MemCardStart(void);
extern void Mcrd_MemCardStop(void);
extern int32_t Mcrd_MemCardExist(int32_t chan);
extern int32_t Mcrd_MemCardAccept(int32_t chan);
extern int32_t Mcrd_MemCardOpen(int32_t chan, char* file, int32_t flag);
extern void Mcrd_MemCardClose(void);
extern int32_t Mcrd_MemCardReadData(uint32_t* adrs, int32_t ofs, int32_t bytes);
extern int32_t Mcrd_MemCardReadFile(int32_t chan, char* file, uint32_t* adrs, int32_t ofs, int32_t bytes);
extern int32_t Mcrd_MemCardWriteData(uint32_t* adrs, int32_t ofs, int32_t bytes);
extern int32_t Mcrd_MemCardWriteFile(int32_t chan, char* file, uint32_t* adrs, int32_t ofs ,int32_t bytes);
extern int32_t Mcrd_MemCardCreateFile(int32_t chan, char* file, int32_t blocks);
extern int32_t Mcrd_MemCardFormat(int32_t chan);
extern int32_t Mcrd_MemCardUnformat(int32_t chan);
extern int32_t Mcrd_MemCardSync(int32_t mode, int32_t* cmds, int32_t* rslt);
extern MemCB Mcrd_MemCardCallback(MemCB func);
extern int32_t Mcrd_MemCardGetDirentry(int32_t chan, char* name, struct DIRENTRY* dir, int32_t* files, int32_t ofs, int32_t max);

#ifdef __cplusplus
}
#endif


#endif