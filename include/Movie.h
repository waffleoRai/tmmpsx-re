#ifndef MYUC_MOVIECMN_H
#define MYUC_MOVIECMN_H

#include "psx/LibCd.h"
#include "Common.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct MovieContext {
		/*0x0000*/	URECT unk0;
		/*0x0008*/	void* unk8[2];
		/*0x0010*/	uint32_t* unk10[2];
		/*0x0018*/	u32 unk18; //StSetStream color mode
		/*0x001C*/	u32 unk1C;
		/*0x0020*/	u32 unk20;
		/*0x0024*/	u32 unk24;
		/*0x0028*/	u32 unk28;
		/*0x002C*/	u32 unk2C;
		/*0x0030*/	u32 unk30;
		/*0x0034*/	u32 unk34;
		/*0x0038*/	u32 unk38;
		/*0x003C*/	CdlLOC unk3c;
		/*0x0040*/	u32 unk40;
		/*0x0044*/	URECT unk44;
		/*0x004C*/	u8 unk4c[0x2a00];
		/*0x2A4C*/	u8 unk2a4c[0x2a00];
	} MovieContext;  //Probably MOVIE related. Size = 0x544C

	typedef struct UnkStr_fea80 {
		/*0x00000*/	u8 unk0[0x23000];
		/*0x23000*/	u8 unk23000[0x23000];
	} UnkStr_fea80; //Size = 0x46000
	
	//   .data 0x800e1f20
	extern URECT gMovieScreenParam[];

	//   .data 0x800e1f60
	extern uint8_t _cdfile_dat_800e1f60[];

#ifdef __cplusplus
}
#endif

#endif
