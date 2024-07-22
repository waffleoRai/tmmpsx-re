
#include <stdlib.h>
#include "./include/waffleoRai_extractMyu_LzNative.h"
#include "./include/lzmu.h"

#ifdef __cplusplus
extern "C" {
#endif

	jbyteArray Java_waffleoRai_1extractMyu_LzNative_lzCompressMatchC(JNIEnv* env, jclass myclass, jbyteArray data, jint mode) {
		jsize bufferLen;
		jbyte* outbuffer;
		jbyte* dataRaw;
		size_t outsize;
		jbyteArray jout;

		bufferLen = (*env)->GetArrayLength(env, data);
		if (bufferLen <= 0) return NULL;
		dataRaw = (*env)->GetByteArrayElements(env, data, NULL);
		if (!dataRaw) return NULL;

		outbuffer = (jbyte*)malloc(bufferLen);
		if (!outbuffer) return NULL;

		outsize = lzmu_compress_match(outbuffer, dataRaw, (size_t)bufferLen, (int)mode);
		(*env)->ReleaseByteArrayElements(env, data, dataRaw, 0);

		jout = (*env)->NewByteArray(env, (jsize)outsize);
		if (jout) {
			(*env)->SetByteArrayRegion(env, jout, 0, (jsize)outsize, outbuffer);
		}

		free(outbuffer);
		return jout;
	}

	jbyteArray Java_waffleoRai_1extractMyu_LzNative_lzCompressMatchCForceLit(JNIEnv* env, jclass myclass, jbyteArray data, jbyteArray litTable) {
		jsize bufferLen, tblLen;
		jbyte* outbuffer;
		jbyte* dataRaw;
		jbyte* tblRaw = NULL;
		jbyte* tblDatPos = NULL;
		OfsForceLitTable* ofsTable = NULL;
		size_t outsize, entries, amt, i;
		jbyteArray jout;
		int j;

		bufferLen = (*env)->GetArrayLength(env, data);
		if (bufferLen <= 0) return NULL;
		dataRaw = (*env)->GetByteArrayElements(env, data, NULL);
		if (!dataRaw) return NULL;

		tblLen = (*env)->GetArrayLength(env, litTable);
		if (tblLen > 0) {
			tblRaw = (*env)->GetByteArrayElements(env, litTable, NULL);
			if (!tblRaw) return NULL;
			entries = (size_t)tblLen >> 3;
			amt = entries * sizeof(OfsForceLitTable);
			ofsTable = (OfsForceLitTable*)malloc(amt);
			if (ofsTable) {
				memset(ofsTable, 0, amt);
				tblDatPos = tblRaw;
				for (i = 0; i < entries; i++) {
					for (j = 3; j >= 0; j--) {
						ofsTable[i].offset <<= 8;
						ofsTable[i].offset |= ((uint32_t)tblDatPos[j]) & 0xff;
					}
					tblDatPos += 4;
					for (j = 3; j >= 0; j--) {
						ofsTable[i].len <<= 8;
						ofsTable[i].len |= ((uint32_t)tblDatPos[j]) & 0xff;
					}
					tblDatPos += 4;
				}
				//Try direct copy.
				//memcpy(ofsTable, tblRaw, amt);
			}
		}

		outbuffer = (jbyte*)malloc(bufferLen);
		if (!outbuffer) {
			if (ofsTable) free(ofsTable);
			return NULL;
		}

		outsize = lzmu_compress_forceNoThru(outbuffer, dataRaw, (size_t)bufferLen, ofsTable);
		(*env)->ReleaseByteArrayElements(env, data, dataRaw, 0);
		if(tblRaw) (*env)->ReleaseByteArrayElements(env, litTable, tblRaw, 0);

		jout = (*env)->NewByteArray(env, (jsize)outsize);
		if (jout) {
			(*env)->SetByteArrayRegion(env, jout, 0, (jsize)outsize, outbuffer);
		}

		free(outbuffer);
		if (ofsTable) free(ofsTable);
		return jout;
	}

#ifdef __cplusplus
}
#endif