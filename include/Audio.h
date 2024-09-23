#ifndef MYUC_AUDIO_H
#define MYUC_AUDIO_H

#include "psx/LibSnd.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct AudioContext {
		/*0x0000*/ void* unk_0fff10; //0x800fff10
		/*0x0004*/ void* unk_0fff14; //0x800fff14
		/*0x0008*/ u32 unk_0fff18; //0x800fff18
		/*0x000c*/ u8 unk_0fff1c[0xe4]; //0x800fff1c
		/*0x00f0*/ u8 unk_100000[0x10]; //0x80100000
		/*0x0100*/ u32 unk_100010; //0x80100010
		/*0x0104*/ u32 unk_100014; //0x80100014
		/*0x0108*/ u32 unk_100018; //0x80100018 (u16?) Probably part of a struct based on how it is accessed...
		/*0x010c*/ u8 unk_10001c; //0x8010001c
		/*0x010d*/ u8 unk_10001d; //0x8010001d
		/*0x010e*/ u16 unk_10001e; //0x8010001e
		/*0x0110*/ u32 unk_100020[4]; //0x80100020
		/*0x0120*/ u32 unk_100030[4]; //0x80100030
		/*0x0130*/ u8* unk_100040; //0x80100040 
		/*0x0134*/ u32 loadFileOffset; //0x80100044 Used when loading BGM and SE files
		/*0x0138*/ u32 loadFileSize; //0x80100048 Used when loading BGM and SE files
		/*0x013c*/ u32 unk_10004c; //0x8010004c
		/*0x014a*/ s16 seqIds[6]; //0x80100050. May or may not be array?
		/*0x014c*/ s16 vabIds[4]; //0x8010005c VAB IDs?
		/*0x0154*/ u16 unk_100064; //0x80100064
		/*0x0156*/ u16 vabLoadSlot; //0x80100066 (@0x156)
		/*0x0158*/ s8 seqFileIndex; //0x80100068
		/*0x0159*/ u8 unk_100069; //0x80100069
		/*0x015a*/ u8 unk_10006a; //0x8010006a
		/*0x015b*/ u8 unk_10006b; //0x8010006b
		/*0x015c*/ u8 unk_10006c; //0x8010006c
		/*0x015d*/ s8 unk_10006d; //0x8010006d
		/*0x015e*/ u16 unk_10006e; //0x8010006e
		/*0x0160*/ SndVolume2 seqVolume; //0x80100070 
		/*0x0164*/ s16 unk_100074; //0x80100074
		/*0x0166*/ u8 unk_100076; //0x80100076
		/*0x0167*/ u8 unk_100077; //0x80100077
		/*0x0168*/ s16 unk_100078; //0x80100078
		/*0x016a*/ s16 unk_10007a[4]; //0x8010007a
		/*0x0172*/ s16 unk_100082[4]; //0x80100082
		/*0x017a*/ u8 unk_10008a; //0x8010008a
		/*0x017b*/ u8 unk_10008b; //0x8010008b
		/*0x017c*/ u32 unk_10008c; //0x8010008c VOICE.STR sample group
		/*0x0180*/ u32 unk_100090; //0x80100090 VOICE.STR sample id (within group)
		/*0x0184*/ u8 unk_100094; //0x80100094 VOICE channel?
		/*0x0185*/ u8 unk_100095; //0x80100095 VOICE stream volume
		/*0x0186*/ u8 unk_100096[2]; //0x80100096
	} AudioContext;

	extern AudioContext gAudioContext;

#ifdef __cplusplus
}
#endif

#endif
