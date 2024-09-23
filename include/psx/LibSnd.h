#ifndef PSXSDK_LIBSND_H
#define PSXSDK_LIBSND_H

#include "psx/LibSpu.h"

#ifdef __cplusplus
extern "C" {
#endif

//From LIBSND.H

typedef struct ProgAtr {
	/*0x00*/ uint8_t tones;
	/*0x01*/ uint8_t mvol;
	/*0x02*/ uint8_t prior;
	/*0x03*/ uint8_t mode;
	/*0x04*/ uint8_t mpan;
	/*0x05*/ int8_t reserved0;
	/*0x06*/ int16_t attr;
	/*0x08*/ uint32_t reserved1;
	/*0x0c*/ uint32_t reserved2;
} ProgAtr; //Size: 0x10

typedef struct VagAtr {
	/*0x00*/ uint8_t prior;
	/*0x01*/ uint8_t mode;
	/*0x02*/ uint8_t vol;
	/*0x03*/ uint8_t pan;
	/*0x04*/ uint8_t center;
	/*0x05*/ uint8_t shift;
	/*0x06*/ uint8_t min;
	/*0x07*/ uint8_t max;
	/*0x08*/ uint8_t vibW;
	/*0x09*/ uint8_t vibT;
	/*0x0a*/ uint8_t porW;
	/*0x0b*/ uint8_t porT;
	/*0x0c*/ uint8_t pbmin;
	/*0x0d*/ uint8_t pbmax;
	/*0x0e*/ uint8_t reserved1;
	/*0x0f*/ uint8_t reserved2;
	/*0x10*/ uint16_t adsr1;
	/*0x12*/ uint16_t adsr2;
	/*0x14*/ int16_t prog;
	/*0x16*/ int16_t vag;
	/*0x18*/ int16_t reserved[4];
} VagAtr; //Size: 0x20

typedef struct SndVolume2 {
	int16_t       left;
	int16_t       right;
} SndVolume2;

extern int16_t SoundVs_SsVabOpenHead(uint8_t*, int16_t);
extern int16_t SoundVs_SsVabOpenHeadSticky(uint8_t*, int16_t, uint32_t);
extern int16_t SoundVs_SsVabTransBodyPartly(uint8_t*, uint32_t, int16_t);
extern int16_t SoundVs_SsVabTransCompleted(int16_t);
extern void  SoundVs_SsVabClose(int16_t);

extern void SoundSs_SsInit(void);
extern void SoundSs_SsSetTableSize(uint8_t*, int16_t, int16_t);
extern void SoundSs_SsSetTickMode(int32_t);
extern void SoundSs_SsStart(void);
extern void SoundSs_SsStart2(void);
extern void SoundSs_SsEnd(void);
extern void SoundSs_SsQuit(void);
extern void SoundSs_SsSeqCalledTbyT(void);

extern int16_t SoundSs_SsSeqOpen(uint32_t*, int16_t);
extern void SoundSs_SsSeqPlay(int16_t, int8_t, int16_t);
extern void SoundSs_SsSeqPause(int16_t);
extern void SoundSs_SsSeqReplay(int16_t); 
extern void SoundSs_SsSeqStop(int16_t);   
extern void SoundSs_SsSeqSetVol(int16_t, int16_t, int16_t);

extern void SoundSs_SsSeqSetCrescendo(int16_t, int16_t, int32_t); 
extern void SoundSs_SsSeqSetDecrescendo(int16_t, int16_t, int32_t); 
extern void SoundSs_SsSeqSetAccelerando(int16_t, int32_t, int32_t);
extern void SoundSs_SsSeqClose(int16_t);

extern void SoundSs_SsSepPause(int16_t, int16_t);
extern void SoundSs_SsSepReplay(int16_t, int16_t);
extern void SoundSs_SsSepStop(int16_t, int16_t);
extern void SoundSs_SsSepSetVol(int16_t, int16_t, int16_t, int16_t);
extern void SoundSs_SsSepSetCrescendo(int16_t, int16_t, int16_t, int32_t);
extern void SoundSs_SsSepSetDecrescendo(int16_t, int16_t, int16_t, int32_t);
extern void SoundSs_SsSepSetAccelerando(int16_t, int16_t, int32_t, int32_t);
extern void SoundSs_SsSepClose(int16_t);

extern void SoundSs_SsSetMVol(int16_t, int16_t);
extern void SoundSs_SsSetSerialAttr(int8_t, int8_t, int8_t); 
extern void SoundSs_SsSetSerialVol(int8_t, int16_t, int16_t); 

extern void SoundVs_SsSetMono(void);                         
extern void SoundVs_SsSetStereo(void);
extern int16_t SoundSs_SsIsEos(int16_t, int16_t);
extern void SoundSs_SsPlayBack(int16_t, int16_t, int16_t);
extern int8_t SoundVs_SsSetReservedVoice(int8_t);

extern int16_t SoundUt_SsUtGetProgAtr(int16_t, int16_t, ProgAtr*);
extern int16_t SoundUt_SsUtSetVagAtr(int16_t, int16_t, int16_t, VagAtr*);
extern int16_t SoundUt_SsUtGetVagAtr(int16_t, int16_t, int16_t, VagAtr*);
extern void SoundUt_SsUtReverbOn(void);
extern void SoundUt_SsUtReverbOff(void);
extern int16_t SoundUt_SsUtSetReverbType(int16_t);
extern void SoundUt_SsUtSetReverbDepth(int16_t, int16_t);
extern void SoundUt_SsUtSetReverbFeedback(int16_t);
extern void SoundUt_SsUtSetReverbDelay(int16_t);
extern void SoundUt_SsUtAllKeyOff(int16_t);
extern int16_t SoundVs_SsVabFakeHead(uint8_t*, int16_t, uint32_t);
extern void SoundSs_SsSeqGetVol(int16_t, int16_t, int16_t*, int16_t*);
extern uint16_t SoundVm_SsPitchFromNote(int16_t, int16_t, uint8_t, uint8_t);


#ifdef __cplusplus
}
#endif

#endif