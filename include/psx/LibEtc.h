#ifndef PSXSDK_LIBETC_H
#define PSXSDK_LIBETC_H

#include "psx/PSXTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MODE_NTSC 0
#define MODE_PAL 1 

int EtcVid_VSync(int mode);

int EtcIntr_ResetCallback(void);
int EtcIntr_VSyncCallback(CallbackFunction f);
int EtcIntr_StopCallback(void);
int EtcIntr_RestartCallback(void);
int EtcIntr_CheckCallback(void);

int32_t EtcVid_GetVideoMode(void);
int32_t EtcVid_SetVideoMode(int32_t mode);

#ifdef __cplusplus
}
#endif

#endif