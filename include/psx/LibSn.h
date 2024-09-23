#ifndef PSXSDK_DEBUG_H
#define PSXSDK_DEBUG_H

#include "psx/PSXTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int32_t Sn_PCopen(char* filename, int mode, int perms);
extern int Sn_PCclose(int handle);
extern int32_t Sn_PClseek(int handle, uint32_t offset, int seekmode);
extern int Sn_PCcreat(char* filename, uint32_t attr);
extern int Sn_PCread(int handle, void* buff, int32_t len);
extern int Sn_PCwrite(int handle, void* buff, int32_t len);

#ifdef __cplusplus
}
#endif

#endif