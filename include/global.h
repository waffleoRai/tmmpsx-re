#ifndef SLPM87176_GLOBAL_H
#define SLPM87176_GLOBAL_H

#include "psxmcrd.h"

#include "exemisc.h"
#include "arena.h"
#include "gamestate.h"
#include "gfx.h"
#include "movie.h"
#include "FileLoad.h"
#include "RoData.h"
#include "GameSave.h"

#include "DataArc_System.h"

#define sApplState sGlobalCtx.applState

#define sAudioCtx sGlobalCtx.audioCtx
#define sCdFileCtx sGlobalCtx.cdFileCtx
#define sCdDriveCtx sGlobalCtx.cdDriveCtx
#define sDataLoadCtx sGlobalCtx.dataLoadCtx
#define sHeapCtx sGlobalCtx.heapCtx
#define sHeap sGlobalCtx.heap
#define sMemCardCtx sGlobalCtx.memcardCtx

#define sCdDriveData sGlobalData.cdDriveData

#define sUnk_gp sGlobalData.gpData

#define sUnk_ffb80 sGlobalCtx.unk_ffb80
#define sSaveCtx sGlobalCtx.saveCtx
#define sUnk_ff868 sGlobalCtx.unk_ff868
#define sUnk_100588 sGlobalCtx.unk_100588
#define gSpriteCtx sGlobalCtx.spriteCtx
#define sUnk_108ac0 sGlobalCtx.unk_108ac0

#define sUnk_ed5b4 sGlobalData.unk_ed5b4
#define gDrawingCtx sGlobalData.gpData.drawingContext
#define gMovieCtx sGlobalData.gpData.movieContext

typedef union LoadInfo {
	u8* dataBuffer;
	s32 intValue;
} LoadInfo;

//May also not be single struct. Used for organization.
typedef struct UnkStruct_gp {
	/*0x0000*/ u8 unk_fe874; //0x800fe874
	/*0x0001*/ u8 unk_fe875[3]; //0x800fe875
	/*0x0004*/ u32 unk_fe878; //0x800fe878
	/*0x0008*/ u16 unk_fe87c; //0x800fe87c
	/*0x000a*/ u16 unk_fe87e; //0x800fe87e
	/*0x000c*/ u8 unk_fe880[8]; //0x800fe880
	/*0x0014*/ u8 unk_fe888[4]; //0x800fe888
	/*0x0018*/ char cdPathPrefix[8]; //0x800fe88c "i:\\cd"
	/*0x0020*/ u32 unk_fe894; //0x800fe894
	/*0x0024*/ u8 unk_fe898; //0x800fe898
	/*0x0025*/ u8 unk_fe899[3]; //0x800fe899
	/*0x0028*/ u32 unk_fe89c; //0x800fe89c
	/*0x002c*/ u8 unk_fe8a0; //0x800fe8a0
	/*0x002d*/ u8 unk_fe8a1[3]; //0x800fe8a1
	/*0x0030*/ u32 unk_fe8a4; //0x800fe8a4
	/*0x0034*/ u8 unk_fe8a8; //0x800fe8a8
	/*0x0035*/ u8 unk_fe8a9[3]; //0x800fe8a9
	/*0x0038*/ u32 unk_fe8ac; //0x800fe8ac
	/*0x003c*/ u8 unk_fe8b0; //0x800fe8b0
	/*0x003d*/ u8 unk_fe8b1[3]; //0x800fe8b1
	/*0x0040*/ u8 unk_fe8b4[5]; //0x800fe8b4
	/*0x0045*/ u8 unk_fe8b9; //0x800fe8b9
	/*0x0046*/ u8 unk_fe8ba; //0x800fe8ba
	/*0x0047*/ u8 unk_fe8bb; //0x800fe8bb
	/*0x0048*/ u8 unk_fe8bc[8]; //0x800fe8bc
	/*0x0050*/ u8 unk_fe8c4[4]; //0x800fe8c4
	/*0x0054*/ u8 unk_fe8c8[8]; //0x800fe8c8
	/*0x005c*/ u8 unk_fe8d0[4]; //0x800fe8d0
	/*0x0060*/ u8 unk_fe8d4[4]; //0x800fe8d4
	/*0x0064*/ u8 unk_fe8d8[4]; //0x800fe8d8
	/*0x0068*/ u8 unk_fe8dc[4]; //0x800fe8dc
	/*0x006c*/ u8 unk_fe8e0[8]; //0x800fe8e0
	/*0x0074*/ u32 unk_fe8e8; //0x800fe8e8
	/*0x0078*/ u8 unk_fe8ec[8]; //0x800fe8ec
	/*0x0080*/ u8 unk_fe8f4[4]; //0x800fe8f4
	/*0x0084*/ u8 unk_fe8f8[8]; //0x800fe8f8
	/*0x008c*/ u8 unk_fe900[4]; //0x800fe900
	/*0x0090*/ u8 unk_fe904[4]; //0x800fe904
	/*0x0094*/ u8 unk_fe908[4]; //0x800fe908
	/*0x0098*/ u8 unk_fe90c[8]; //0x800fe90c
	/*0x00a0*/ u8 unk_fe914[4]; //0x800fe914
	/*0x00a4*/ u8 unk_fe918[8]; //0x800fe918
	/*0x00ac*/ u8 unk_fe920[0x14]; //0x800fe920
	/*0x00c0*/ u32 unk_fe934[5]; //0x800fe934
	/*0x00d4*/ u8 byteBitMasks[8]; //0x800fe948
	/*0x00dc*/ u8 unk_fe950[4]; //0x800fe950
	/*0x00c0*/ char unk_fe954[1]; //0x800fe954
	//...
	/*0x01d8*/ LoadInfo loadInfoA; //0x800fea4c
	/*0x01dc*/ LoadInfo loadInfoB; //0x800fea50 This seems to be a pointer. However, it sometimes takes the value of 1? Like for mem card cmd results?
	/*0x01e0*/ LoadInfo loadInfoC; //0x800fea54 Another load buffer
	/*0x01e4*/ u32 unk_fea58; //0x800fea58
	/*0x01e8*/ u32 unk_fea5c; //0x800fea5c
	/*0x01ec*/ u32 unk_fea60; //0x800fea60
	/*0x01f0*/ u32 unk_fea64; //0x800fea64
	/*0x01f4*/ u32 unk_fea68; //0x800fea68
	/*0x01f8*/ UnkStruct_ffb80_inner* unk_fea6c; //0x800fea6c Ptr used in sce loading?
	/*0x01fc*/ UnkStruct_ffb80_inner* unk_fea70; //0x800fea70
	/*0x0200*/ UnkStruct_ffc30_inner* unk_fea74; //0x800fea74
	/*0x0204*/ UnkStruct_ffc30_inner* unk_fea78; //0x800fea78
	/*0x0208*/ MovieStreamContext* movieContext; //0x800fea7c Ptr to movie context?. On the heap.
	/*0x020c*/ u32 unk_fea80; //0x800fea80 Pointer, movie related
	/*0x0210*/ u32 unk_fea84; //0x800fea84 Pointer, movie related
	/*0x0214*/ u32 unk_fea88; //0x800fea88 Pointer, movie related
	/*0x0218*/ u8 unk_fea8c[4]; //0x800fea8c
	/*0x021c*/ u16 unk_fea90; //0x800fea90
	/*0x021e*/ u8 unk_fea92; //0x800fea92
	/*0x021f*/ u8 unk_fea93; //0x800fea93
	/*0x0220*/ u16 unk_fea94; //0x800fea94
	/*0x0222*/ u8 unk_fea96[2]; //0x800fea96
	/*0x0224*/ GraphicsContext* drawingContext; //0x800fea98 //Might be some kind of drawing context?
	/*0x0228*/ u8 unk_fea9c; //0x800fea9c
	/*0x0229*/ u8 unk_fea9d; //0x800fea9d
	/*0x022a*/ u8 unk_fea9e; //0x800fea9e
	/*0x022b*/ u8 unk_fea9f; //0x800fea9f

	/*0x0260*/ u16 unk_fead4; //0x800fead4
	/*0x0262*/ u8 unk_fead6[2]; //0x800fead6
	/*0x0264*/ u16 unk_fead8[2]; //0x800fead8
	/*0x0268*/ u16 unk_feadc; //0x800feadc
	/*0x026a*/ u16 unk_feade; //0x800feade
	/*0x026c*/ u16 unk_feae0[2]; //0x800feae0
	/*0x0270*/ u32 unk_feae4; //0x800feae4

	/*0x02ac*/ u8 unk_feb20; //0x800feb20
	/*0x02ad*/ u8 unk_feb21[3]; //0x800feb21
} UnkStruct_gp;

typedef struct GlobalData { //0x800e1e84 - 0x800fea48

	//UnkStruct_e1eac unktbl_e1eac[29]; //0x800e1eac -  e1f20 (?) : Used when loading movies?
	s16 unktbl_e1f20[32]; //0x800e1f20- e1f60
	u8 unktbl_e1f60[8]; //0x800e1f60- e1f68
	u32* fileposTable[13]; //0x800e1f68 - e1f9c
	u32* fileMemsizeTable[13]; //0x800e1f9c - e1fd0
	char* fileNameTable[13]; //0x800e1fd0 - e2004
	void* unktbl_e2004[24]; //0x800e2004 - e2060 : Array of function/code pointers
	void* unktbl_e2064[32]; //0x800e2064 - e20e4 : Array of function/code pointers
	UnkStruct_e20e4 unktbl_e20e4[60]; //0x800e20e4 - e215c : Used in processing VOICE.STR
	UnkStruct_e215c unktbl_e215c[0xfc6]; //0x800e215c - e8000(?) : Used in processing VOICE.STR (voice ptrs? Pweaze?)

	u32 fileposTableBgm[64]; //0x800e88a8 - e89a8 IT'S FUGGIN HARD CODED??? WHY????
	u32 fileposTableSe[80]; //0x800e8ae8 - e8c28
	u32 fileposTableSystem[112]; //0x800e8c28 - e8de8
	u32 fileSizeTableSystem[112]; //0x800e8de8 - e8fa8
	u32 fileposTableFace[192]; //0x800e8fa8 - e92a8
	u32 fileSizeTableFace[192]; //0x800e92a8 - e95a8
	u32 fileposTableScreen[272]; //0x800e95a8 - e99e8
	u32 fileSizeTableScreen[272]; //0x800e99e8 - e9e28
	u32 fileposTableUnit[512]; //0x800e9e28 - ea628
	u32 fileSizeTableUnit[512]; //0x800ea628 - eae28
	u32 fileposTableSce[256]; //0x800eae28 - eb228
	u32 fileposTableField[192]; //0x800eb228 - eb528
	u32 fileSizeTableField[192]; //0x800eb528 - eb828
	u32 fileposTableAnime[608]; //0x800eb828 - ec1a8
	u32 fileposTableEffect[608]; //0x800ec1a8 - ecb28
	u32 fileSizeTableEffect[608]; //0x800ecb28 - ed4a8

	UnkStruct_ed5b4 unk_ed5b4; //0x800ed5b4

	u8 unktbl_fa754[5][16]; //0x800fa754 - fa7a4

	SceLoadDelegate sceLoadDelegateTable[256]; //0x800fa7b4 - fabb4 : Callback table for sce loading?
	u8 unktbl_fabb4[256]; //0x800fabb4 - facb4 : Used for sce loading?

	CDDriveData cdDriveData; //0x800fd204 - 0x800fd6f4
	//GPU Data appears to be next

	UnkStruct_gp gpData; //800fe874
} GlobalData;

typedef struct GlobalContext { //0x800fea48(?) - 0x801101ec
	
	ApplicationState applState; //0x800feb28 - ff44e
	//ff44e - ff4b8
	DataLoadContext dataLoadCtx; //0x800ff4b8 - ff520
	SaveContext saveCtx; //0x800ff520 - ff868
	UnkStruct_ff868 unk_ff868; //0x800ff868

	UnkStruct_ffb80 unk_ffb80; //0x800ffb80 - 0x800ffd08
	CdFileContext cdFileCtx; //0x800ffd08 - ffebc
	//ffebc - fff10
	AudioContext audioCtx; //0x800fff10 - 0x80100096 (pad to 0x80100098)
	byte seqAttrTable[0x420]; //0x80100098 - 801004b8

	UnkStruct_100588 unk_100588; //0x80100588

	SpriteManager spriteCtx; //0x801012c0
	UnkStruct_108ac0 unk_108ac0; //0x80108ac0 - 
	HeapContext heapCtx; //0x8010a260 - 10a468

	MemCardContext memcardCtx; //0x8010a5a0

	CDDriveContext cdDriveCtx; //0x8010c490

	HeapData heap; //0x801101ec - 801fd004
} GlobalContext;

extern void* globalPointer; // = 0x800fe874?
extern GlobalData sGlobalData;
extern GlobalContext sGlobalCtx;

#endif
