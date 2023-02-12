#ifndef PSX_PCDBG_H
#define PSX_PCDBG_H

#include "psxtypes.h"

int PCDebug_PCOpen(char* filename, int mode);
int PCDebug_PCClose(int handle);
void* PCDebug_PClSeek(int handle, uint32_t offset, int seekmode);
int PCDebug_PCCreat(char* filename, uint32_t attr);
int PCDebug_IPCRead(uint32_t handle, void* dest, int len); //func_800E1CDC
int PCDebug_PCRead(int handle, int32_t len, void* dst);
int PCDebug_IPCWrite(uint32_t handle, void* src, int len);
int PCDebug_PCWrite(int handle, int32_t len, void* dst);

#endif
