#ifndef MYUC_DATATABLES_H
#define MYUC_DATATABLES_H

#include "psx/PSXTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct __attribute__((packed)) VoiceEntry {
		/*0x00*/	uint8_t id;
		/*0x01*/	uint8_t channel;
		/*0x02*/	uint32_t startSec : 18;
		/*0x04*/	uint32_t secCount : 14;
	} VoiceEntry; //Size = 0x6
	
	extern uint8_t _udat5_dat_800ed5b4[];
	extern uint8_t _udat5_dat_800f0100[];
	extern uint8_t _udat5_dat_800f9000[];
	
	extern uint8_t _udat6_dat_800fa754[];
	extern uint8_t _udat6_dat_800fa764[];
	extern uint8_t _udat6_dat_800fa774[];
	extern uint8_t _udat6_dat_800fa784[];
	extern uint8_t _udat6_dat_800fa794[];

#ifdef __cplusplus
}
#endif

#endif
