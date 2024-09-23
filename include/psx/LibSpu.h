#ifndef PSXSDK_LIBSPU_H
#define PSXSDK_LIBSPU_H

#include "psx/PSXTypes.h"

#define SPU_TRANSFER_WAIT 1
#define SPU_TRANSFER_PEEK 0
#define SPU_TRANSFER_GLANCE SPU_TRANSFER_PEEK

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    /*0x00*/ int16_t left;
    /*0x02*/ int16_t right;
} SpuVolume; //Size 0x4

typedef struct {
    /*0x00*/ uint32_t	voice;
    /*0x04*/ uint32_t	mask;
    /*0x08*/ SpuVolume	volume;
    /*0x0c*/ SpuVolume	volmode;
    /*0x10*/ SpuVolume	volumex;
    /*0x14*/ uint16_t	pitch;
    /*0x16*/ uint16_t	note;
    /*0x18*/ uint16_t	sample_note;
    /*0x1a*/ int16_t	envx;
	/*0x1c*/ uint32_t	addr;
    /*0x20*/ uint32_t	loop_addr;
    /*0x24*/ int32_t	a_mode;
    /*0x28*/ int32_t	s_mode;
    /*0x2c*/ int32_t	r_mode;
    /*0x30*/ uint16_t	ar;
    /*0x32*/ uint16_t	dr;
    /*0x34*/ uint16_t	sr;
    /*0x36*/ uint16_t	rr;
    /*0x38*/ uint16_t	sl;
    /*0x3a*/ uint16_t	adsr1;
    /*0x3c*/ uint16_t	adsr2;
} SpuVoiceAttr; //Size 0x3e

typedef struct {
    /*0x00*/ SpuVolume	volume;
    /*0x04*/ int32_t	reverb;
    /*0x08*/ int32_t	mix;
} SpuExtAttr; //Size 0xc

typedef struct {
    /*0x00*/ uint32_t	mask;
    /*0x04*/ SpuVolume	mvol;
    /*0x08*/ SpuVolume	mvolmode;
    /*0x0c*/ SpuVolume	mvolx;
    /*0x10*/ SpuExtAttr	cd;
    /*0x1c*/ SpuExtAttr	ext;
} SpuCommonAttr; //Size 0x28

extern void Spu_SpuInit(void);
extern void Spu_SpuStart(void);
extern void Spu_SpuQuit(void);
extern int32_t Spu_SpuSetNoiseClock(int32_t n_clock);
extern uint32_t Spu_SpuSetNoiseVoice(int32_t on_off, uint32_t voice_bit);
extern uint32_t Spu_SpuGetNoiseVoice(void);
extern uint32_t Spu_SpuSetReverbVoice(int32_t on_off, uint32_t voice_bit);
extern uint32_t Spu_SpuGetReverbVoice(void);
extern int32_t Spu_SpuClearReverbWorkArea(int32_t mode);
extern int32_t Spu_SpuSetTransferMode(int32_t mode);
extern uint32_t Spu_SpuSetTransferStartAddr(uint32_t addr);
extern uint32_t Spu_SpuWritePartly(uint8_t* addr, uint32_t size);
extern int32_t Spu_SpuIsTransferCompleted(int32_t flag);
extern void Spu_SpuSetVoiceAttr(SpuVoiceAttr* arg);
extern void Spu_SpuSetKey(int32_t on_off, uint32_t voice_bit);
extern void Spu_SpuSetCommonAttr(SpuCommonAttr* attr);
extern int32_t Spu_SpuInitMalloc(int32_t num, char* top);
extern int32_t Spu_SpuMalloc(int32_t size);
extern void Spu_SpuFree(uint32_t addr);
extern void Spu_SpuGetVoiceEnvelope(int vNum, int16_t* envx);
extern void Spu_SpuSetCommonCDMix(int32_t cd_mix);
extern void Spu_SpuSetCommonCDVolume(int16_t cd_left, int16_t cd_right);

extern int32_t SoundUt_SpuSetReverb(int32_t on_off);

#ifdef __cplusplus
}
#endif

#endif