#ifndef SLPM87176_FILELOAD_H
#define SLPM87176_FILELOAD_H

#include "global.h"

#define DATA_LOAD_FLAG_UNK_80 0x80
#define DATA_LOAD_FLAG_UNK_01 0x01

typedef enum DataLoadStatus {
	/*0x00*/ LOADSTAT_00,
	/*0x01*/ LOADSTAT_01,
	/*0x02*/ LOADSTAT_02,
	/*0x03*/ LOADSTAT_03,
	/*0x04*/ LOADSTAT_04,
} DataLoadStatus;

typedef struct DataLoadContext {
	/*0x0000*/ u32 unk_ff4b8[16]; //0x800ff4b8 - everything in here looks like ptrs to heap addresses
	/*0x0040*/ byte* dataReqBase; //0x800ff4f8 - maybe ptr to info about what to load? (looks like it's generally on the heap)
	/*0x0044*/ s16 unk_ff4fc; //0x800ff4fc
	/*0x0046*/ s16 unk_ff4fe; //0x800ff4fe
	/*0x0048*/ u32 dataReqOffset; //0x800ff500 - offset of some kind, often added to 4f8
	/*0x004c*/ u16 reqStatus; //0x800ff504 - seems to be some kind of load status
	/*0x004e*/ u16 unk_ff506; //0x800ff506
	/*0x0050*/ byte* sceDataBuffer; //0x800ff508. seems to generally be set to 801e8000
	/*0x0054*/ u8 unk_ff50c; //0x800ff50c
	/*0x0055*/ s8 unk_ff50d; //0x800ff50d
	/*0x0056*/ u8 unk_ff50e; //0x800ff50e
	/*0x0057*/ u8 unk_ff50f; //0x800ff50f
	/*0x0058*/ u8 unk_ff510; //0x800ff510
	/*0x0059*/ u8 unk_ff511; //0x800ff511
	/*0x005a*/ u8 unk_ff512; //0x800ff512
	/*0x005b*/ u8 unk_ff513; //0x800ff513
	/*0x005c*/ u8 unk_ff514; //0x800ff514
	/*0x005d*/ u8 unk_ff515; //0x800ff515
	/*0x005e*/ u8 unk_ff516; //0x800ff516
	/*0x005f*/ u8 unk_ff517; //0x800ff517
	/*0x0060*/ u8 unk_ff518; //0x800ff518
	/*0x0061*/ u8 unk_ff519; //0x800ff519
	/*0x0062*/ u8 unk_ff51a; //0x800ff51a
	/*0x0063*/ u8 unk_ff51b; //0x800ff51b
	/*0x0064*/ u8 unk_ff51c; //0x800ff51c
	/*0x0065*/ u8 unk_ff51d[3]; //0x800ff51d
} DataLoadContext;

typedef struct UnkStruct_ff534 {
	/*0x0000*/ u8 unk_00;
	/*0x0001*/ u8 unk_01;
	/*0x0002*/ u8 unk_02[0x2];
	/*0x0004*/ u16 unk_04; //538
	/*0x0006*/ u16 unk_06;
	/*0x0008*/ u8 unk_08[0x4]; //53c
	/*0x000c*/ u16 unk_0c; //540
	/*0x000e*/ u16 unk_0e; //542
	/*0x0010*/ u8 unk_10[0xe]; //544
} UnkStruct_ff534; //Size 0x1e - NOT word aligned (if it exists)

//I think this is the save context
typedef struct SaveContext {
	/*0x0000*/ u32 unk_ff520; //0x800ff520
	/*0x0004*/ u32 unk_ff524; //0x800ff524
	/*0x0008*/ u32 unk_ff528; //0x800ff528
	/*0x000c*/ u32 unk_ff52c; //0x800ff52c
	/*0x0010*/ u32 unk_ff530; //0x800ff530
	/*0x0010*/ UnkStruct_ff534 unk_ff534[2]; //0x800ff534 No idea if these are structs or how many :)
	//...
	/*0x01e8*/ u16 unk_ff708; //0x800ff708
	/*0x01ea*/ u16 unk_ff70a; //0x800ff70a
	/*0x01ec*/ u16 unk_ff70c; //0x800ff70c
	/*0x01ee*/ u16 unk_ff70e; //0x800ff70e
	/*0x01f0*/ u16 unk_ff710; //0x800ff710
	/*0x01f2*/ u16 unk_ff712; //0x800ff712
	/*0x01f4*/ u16 unk_ff714; //0x800ff714
	/*0x01f6*/ u16 unk_ff716; //0x800ff716
	/*0x01f8*/ u16 unk_ff718; //0x800ff718
	/*0x01fa*/ u8 unk_ff71a; //0x800ff71a
	/*0x01fb*/ u8 unk_ff71b; //0x800ff71b
	/*0x01fc*/ u16 unk_ff71c; //0x800ff71c
	/*0x01fe*/ u16 unk_ff71e; //0x800ff71e
	/*0x0200*/ u16 unk_ff720; //0x800ff720
	/*0x0202*/ u8 unk_ff722[0x1c]; //0x800ff722
	//...
	/*0x0282*/ u16 unk_ff7a2; //0x800ff7a2
	/*0x0284*/ u16 unk_ff7a4; //0x800ff7a4
	/*0x0286*/ u8 unk_ff7a6[0x3a]; //0x800ff7a6
	/*0x02c0*/ u8 unk_ff7e0; //0x800ff7e0
	/*0x02c1*/ u8 unk_ff7e1; //0x800ff7e1
	/*0x02c2*/ u8 unk_ff7e2[2]; //0x800ff7e2
	/*0x02c4*/ u32 unk_ff7e4; //0x800ff7e4 
	/*0x02c8*/ u32 unk_ff7e8; //0x800ff7e8
	/*0x02cc*/ u8 unk_ff7ec[0x38]; //0x800ff7ec
	/*0x0304*/ u16 unk_ff824; //0x800ff824
	/*0x0306*/ u16 unk_ff826; //0x800ff826
	/*0x0308*/ u16 unk_ff828; //0x800ff828
	/*0x030a*/ u8 unk_ff82a[2]; //0x800ff82c
	/*0x030c*/ u32 unk_ff82c; //0x800ff82c May be array?
	/*0x0310*/ u32 unk_ff830; //0x800ff830
	/*0x0314*/ u8 unk_ff834[4]; //0x800ff834
	/*0x0318*/ u8 unk_ff838; //0x800ff838
	/*0x0319*/ u8 unk_ff839; //0x800ff839
	/*0x031a*/ u8 unk_ff83a; //0x800ff83a
	/*0x031b*/ u8 unk_ff83b; //0x800ff83b
	/*0x031c*/ u32 unk_ff83c; //0x800ff83c
	/*0x0320*/ u32 unk_ff840; //0x800ff840
	/*0x0324*/ u32 unk_ff844; //0x800ff844
	/*0x0328*/ u32 unk_ff848; //0x800ff848
	/*0x032c*/ u32 unk_ff84c; //0x800ff84c
	/*0x0330*/ u32 unk_ff850; //0x800ff850
	/*0x0334*/ u32 unk_ff854; //0x800ff854
	/*0x0338*/ u32 unk_ff858; //0x800ff858
	/*0x033c*/ u32 unk_ff85c; //0x800ff85c
	/*0x0340*/ u32 unk_ff860; //0x800ff860
	/*0x0344*/ u32 unk_ff864; //0x800ff864
} SaveContext;

typedef struct UnkStruct_108ac0 {
	/*0x0000*/ u8 unk_108ac0; //0x80108ac0
	/*0x0001*/ u8 unk_108ac1; //0x80108ac1
	/*0x0002*/ u8 unk_108ac2[2]; //0x80108ac2
	/*0x0004*/ byte* unk_108ac4; //0x80108ac4
} UnkStruct_108ac0;

#endif