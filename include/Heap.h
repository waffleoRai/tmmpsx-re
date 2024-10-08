#ifndef MYUC_HEAP_H
#define MYUC_HEAP_H

/*---------------------------------------
* Header file autogenerated by MyuCTemplatizer
* ---------------------------------------*/

#include "psx/LibC.h"
#include "psx/LibApi.h"
#include "GlobalBuffer.h"

#ifdef __cplusplus
extern "C" {
#endif

	void* Heap_Malloc(size_t size);
	void Heap_IncrAsNumber_ff722_715d4(uint16_t baseFlagId, uint16_t count);
	void Heap_DecrAsNumber_ff722_71728(uint16_t baseFlagId, uint16_t count);
	void Heap_prng_seed_71874(void);
	void Heap_Reset(void);
	void Heap_FreeMemory(void* addr, size_t size);
	void Heap_ClearAllFlags_ff722_71CD0(void);
	void Heap_SetFlag_ff722_71cf8(uint16_t flagId);
	void Heap_ClearFlag_ff722_71d40(uint16_t flagId);
	void Heap_XorFlag_ff722_71d88(uint16_t flagId);
	BOOL Heap_CheckFlag_ff722_71dd0(uint16_t flagId);
	void Heap_SetFlags_ff722_71e1c(uint16_t baseFlagId, uint16_t count, uint16_t mask);
	uint16_t Heap_CheckFlags_ff722_71edc(uint16_t baseFlagId, uint16_t count);
	void Heap_prng_71f64(void);
	uint16_t Heap_prng_71fbc(int arg0);

#ifdef __cplusplus
}
#endif

#endif
