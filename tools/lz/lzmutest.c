
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lzmu.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FILECOUNT 191
#define BUFF_SIZE 0x60000

#define FIELD_MODE 1
#define OFS_TABLE_SIZE 0xf000 //In bytes

int main(int argc, char** argv) {
	//TODO
	//TODO Note: This doesn't take buffer garbage into account. Will need to be added to input files.
	const char* inDir = "D:\\usr\\bghos\\code\\test\\field\\bb";
	const char* outDir = "D:\\usr\\bghos\\code\\test\\field\\cc";
	const char* ofsDir = "D:\\usr\\bghos\\code\\test\\field\\_compare\\lit";
	char fnBufferIn[48];
	char fnBufferOut[48];
	char fnOfsTbl[64];
	uint8_t* readBuff = NULL;
	uint8_t* writeBuff = NULL;
	OfsForceLitTable* ofsBuff = NULL;
	FILE* myfile = NULL;
	int i, j;
	size_t decsize, encsize;
	uint32_t temp;

	readBuff = (uint8_t*)malloc(BUFF_SIZE);
	if (!readBuff) return 1;

	writeBuff = (uint8_t*)malloc(BUFF_SIZE);
	if (!writeBuff) {
		free(readBuff);
		return 1;
	}

	if (FIELD_MODE){
		ofsBuff = (OfsForceLitTable*)malloc(OFS_TABLE_SIZE);
		if (!ofsBuff) {
			free(readBuff);
			free(writeBuff);
			return 1;
		}
	}

	for (i = 0; i < FILECOUNT; i++) {
		//Read offset table, if applicable
		if (FIELD_MODE) {
			sprintf(fnOfsTbl, "%s\\%03d_holdlit.bin\0", ofsDir, i);
			memset(ofsBuff, 0, OFS_TABLE_SIZE);
			myfile = fopen(fnOfsTbl, "rb");
			fseek(myfile, 0L, SEEK_END);
			decsize = ftell(myfile);
			fseek(myfile, 0L, SEEK_SET);
			fread(ofsBuff, 1, decsize, myfile);
			fclose(myfile);
		}

		sprintf(fnBufferIn, "%s\\%03d.bin\0", inDir, i);
		myfile = fopen(fnBufferIn, "rb");

		if (!myfile) continue;
		printf("Working on %s...\n", fnBufferIn);

		//Get file size
		fseek(myfile, 0L, SEEK_END);
		decsize = ftell(myfile);
		fseek(myfile, 0L, SEEK_SET);

		//Read.
		fread(readBuff, 1, decsize, myfile);
		fclose(myfile);

		if (FIELD_MODE) {
			encsize = lzmu_compress_forceNoThru(writeBuff, readBuff, decsize, ofsBuff);
		}
		else {
			encsize = lzmu_compress_match(writeBuff, readBuff, decsize, COMPR_STRAT_STD_LOOKAHEAD);
		}
		if (encsize < 1) {
			printf("%s compression failed!\n", fnBufferIn);
			continue;
		}

		//Write to output
		sprintf(fnBufferOut, "%s\\%03d.bin.lz\0", outDir, i);
		myfile = fopen(fnBufferOut, "wb");
		
		//Size of dec file...
		temp = (uint32_t)decsize;
		for (j = 0; j < 4; j++) {
			fputc((char)(temp & 0xFF), myfile);
			temp >>= 8;
		}

		//Data
		fwrite(writeBuff, 1, encsize, myfile);

		fclose(myfile);
	}

	if (ofsBuff) free(ofsBuff);
	free(readBuff);
	free(writeBuff);
	return 0;
}

#ifdef __cplusplus
}
#endif