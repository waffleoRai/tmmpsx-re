#include "global.h"

void func_8003A788(s16 arg0, int arg1) {

	int i, j;
	s8 cbuff[8];
	byte* cpos;

	s8 temp8_0;
	u16 temp16_0;
	s32 temp32_0;
	u32 temp32_1;

	for (i = 0; i < 5; i++) cbuff[i] = 0;

	while (arg0 > 9999) {
		cbuff[0]++;
		arg0 -= 10000;
	}

	while (arg0 > 999) {
		cbuff[1]++;
		arg0 -= 1000;
	}

	while (arg0 > 99) {
		cbuff[2]++;
		arg0 -= 100;
	}

	while (arg0 > 9) {
		cbuff[3]++;
		arg0 -= 10;
	}

	cbuff[4] = arg0;
	for (j = 0; j < 4; j++) {
		if (cbuff[j] != 0) break;
	}

	if ((arg1 & 0xff) > (5 - j)) {
		temp32_0 = (arg1 & 0xff) + (j - 5) & 0xff;
	}
	else temp32_0 = 0;
	temp16_0 = sUnk_ffb80.unk_ffb7a + 0x1000;
	*(byte*)sUnk_ffb80.unk_ffb70 = (byte)temp16_0;
	sUnk_ffb80.unk_ffb70 = (u32)sUnk_ffb80.unk_ffb70 + 1;
	if (sUnk_ffb80.unk_ffb70 == &sHeap.unk_111400) {
		sUnk_ffb80.unk_ffb70 = &sHeap.unk_111000;
	}
	*(byte*)sUnk_ffb80.unk_ffb70 = (byte)(temp16_0 >> 0xff);
	sUnk_ffb80.unk_ffb70 = (u32)sUnk_ffb80.unk_ffb70 + 1;
	if (sUnk_ffb80.unk_ffb70 == &sHeap.unk_111400) {
		sUnk_ffb80.unk_ffb70 = &sHeap.unk_111000;
	}

	sUnk_ffb80.unk_ffb78 += 2;
	*((u16*)(&sHeap.unk_111400 + sUnk_ffb80.unk_ffb7a)) = sUnk_ffb80.unk_ffb7c;
	sUnk_ffb80.unk_ffb7a += 2;
	if (sUnk_ffb80.unk_ffb7a >= 0x200) {
		sUnk_ffb80.unk_ffb7a = 0;
	}

	cpos = &sHeap.unk_111600[sUnk_ffb80.unk_ffb7c];
	for (i = 0; i < temp32_0; i++) {
		*cpos = 0;
		cpos++;
	}

	for (; j < 5; j++) {
		*cpos = cbuff[j] + 0x36;
		cpos++;
	}
	*cpos = 0xff;

	sUnk_ffb80.unk_ffb7c = (u16)((u32)cpos - 0x15ff);
	if (sUnk_ffb80.unk_ffb7c > 0x800) sUnk_ffb80.unk_ffb7c = 0;
}

void func_8003ADB8() {
	sUnk_ffb80.unk_ffb70 = &sHeap.unk_111000;
	sUnk_ffb80.unk_ffb74 = &sHeap.unk_111000;
	sUnk_ffb80.unk_ffb78 = 0;
	sUnk_ffb80.unk_ffb7a = 0;
	sUnk_ffb80.unk_ffb7c = 0;
	return;
}

u8 func_8003AE90() {
	u8 oldval = *((u8*)sUnk_ffb80.unk_ffb74);
	((u8*)sUnk_ffb80.unk_ffb74)++;
	if (sUnk_ffb80.unk_ffb74 == &sHeap.unk_111400) {
		sUnk_ffb80.unk_ffb74 = &sHeap.unk_111000;
	}
	sUnk_ffb80.unk_ffb78--;
	return oldval;
}

//something to do with pad init I think?
void func_8003BBB4() {
	sUnk_gp.unk_fea6c = func_8003BC44(&sUnk_ffb80.unk_ffb80);
	sUnk_gp.unk_fea70 = func_8003BC44(&sUnk_ffb80.unk_ffbd8);
	func_800C6228(&sUnk_ffb80.unk_ffb80.unk_20, &sUnk_ffb80.unk_ffbd8.unk_20); //This func looks to be part of libpad, but I can't get a match.
	sUnk_gp.unk_fea74 = func_8003BD18(&sUnk_ffb80.unk_ffc30);
	sUnk_gp.unk_fea78 = func_8003BD18(&sUnk_ffb80.unk_ffc9c);
	func_8003BDFC(0, 1);
	func_8003BE70(0, 0, 0, 0x3c0, 0x184);
	return;
}

UnkStruct_ffb80_inner* func_8003BC44(UnkStruct_ffb80_inner* arg0) {
	int i;
	u16 val = 0x8000;

	if ((byte)sApplState.stateFlags >> 5 == 0) {
		//Throw exception? "trap(7)"
		return NULL;
	}

	arg0->unk_50 = (u16)(0x24 / ((byte)sApplState.stateFlags >> 5));
	arg0->unk_42 = 0;
	arg0->unk_44 = 0;
	arg0->unk_46 = 0;
	arg0->unk_4c = 0;
	arg0->unk_4e = 0;
	arg0->unk_4a = 10;
	arg0->unk_52 = 0;

	arg0->unk_54 = 0;
	arg0->unk_55 = 0;
	arg0->unk_56 = 0;
	arg0->unk_57 = 0;

	for (i = 0; i < 16; i++) {
		arg0->unk_00[i] = val;
		val >>= 1;
	}

	for (i = 0; i < 4; i++) {
		arg0->unk_20[i] = 8;
	}

	return arg0;
}

UnkStruct_ffc30_inner* func_8003BD18(UnkStruct_ffc30_inner* arg0) {

	int i;
	POLY_FT4* poly;

	arg0->unk_0c = 8;
	arg0->unk_0e[0] = 4;
	arg0->unk_0e[3] = 0xff;
	arg0->unk_00 = 0;
	arg0->unk_02 = 0;
	arg0->unk_04 = 0;
	arg0->unk_06 = 0;
	arg0->unk_08 = 0;
	arg0->unk_0a = 0;
	arg0->unk_0e[1] = 0;
	arg0->unk_0e[2] = 0;

	arg0->unk_64 = 0;
	arg0->unk_66 = 0;
	arg0->unk_68 &= 0xffff0000;

	poly = &arg0->unk_14;
	for (int i = 0; i < 2; i++) {
		Gpu_SetPolyFT4(poly);
		poly->r0 = 0x80;
		poly->g0 = 0x80;
		poly->b0 = 0x80;
		poly++;
	}
	return arg0;
}

void func_8003BDFC(int arg0, u32 arg1) {
	int temp;
	UnkStruct_ffc30_inner* ptr;

	temp = arg0;
	if (arg0 < 0) {
		temp = 0;
		arg0 = 1;
	}
	if (!(arg0 < temp)) {
		ptr = &sUnk_ffb80.unk_ffc30 + temp;
		do {
			ptr->unk_68 &= ~1;
			ptr->unk_68 |= (arg1 & 0x1);
			ptr++;
		} while (++temp <= arg0);
	}
}

void func_8003BE70(int arg0, u16 arg1, u16 arg2, u16 arg3, u16 arg4) {
	int temp;
	UnkStruct_ffc30_inner* ptr;

	temp = arg0;
	if (arg0 < 0) {
		temp = 0;
		arg0 = 1;
	}
	if (!(arg0 < temp)) {
		ptr = &sUnk_ffb80.unk_ffc30 + temp;
		do {
			ptr->unk_04 = arg1;
			ptr->unk_06 = arg2;
			ptr->unk_08 = arg3;
			ptr->unk_0a = arg4;

			if (ptr->unk_00 < arg1) ptr->unk_00 = arg1;
			if (ptr->unk_02 < arg2) ptr->unk_02 = arg2;
			if (ptr->unk_00 > arg3) ptr->unk_00 = arg3;
			if (ptr->unk_02 > arg4) ptr->unk_02 = arg4;

			ptr++;
		} while (++temp <= arg0);

	}
}