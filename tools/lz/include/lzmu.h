#ifndef LZMU_COMPRESSION_H
#define LZMU_COMPRESSION_H

//Might move this to a jni header instead...
#ifdef _WIN32
#    ifdef LZMU_BUILD
#        define LZMU_DLL_API __declspec(dllexport)
#    else
#        define LZMU_DLL_API __declspec(dllimport)
#    endif
#else
#   define LZMU_DLL_API
#endif

#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define COMPR_STRAT_GREEDY 1 //Greedy
#define COMPR_STRAT_NO_THRU 2 //field
#define COMPR_STRAT_QUICK_LOOKAHEAD 3
#define COMPR_STRAT_STD_LOOKAHEAD 4 //Lookahead 1 (sprites)

	typedef struct {
		uint32_t offset;
		uint32_t len;
	} OfsForceLitTable;

LZMU_DLL_API size_t lzmu_compress_forceNoThru(void* dst, void* src, size_t srcLen, OfsForceLitTable* holdOfsTable);
LZMU_DLL_API size_t lzmu_compress_match(void* dst, void* src, size_t srcLen, int strat);

#ifdef __cplusplus
}
#endif

#endif