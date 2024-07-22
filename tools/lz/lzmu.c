#include "./include/lzmu.h"

#define BACK_WIN_SIZE 0x2000

#define MIN_RUN_SIZE 2
#define MAX_RUN_SHORT 5
#define MIN_RUN_SIZE_LONG 3
#define MAX_RUN_SIZE_LONG 256
#define MAX_RUN_SIZE_LONG_2BYTE 9
#define MAX_BACKSET_SHORT 256
#define MAX_BACKSET_LONG 0x2000

#define BACK_WIN_SIZE_LIM 0x1fff

#define MATCH_CACHE_SIZE 512

#define FLUSH_CTRL_AND_DATA if (ctx->ctrlMask == 0) {flushControlByte(ctx); flushDataBuffer(ctx);}
#define FLUSH_CTRL_NOT_DATA if (ctx->ctrlMask == 0) {flushControlByte(ctx); ctx->flushFlag = 1;}

#define FRONT_WIN_PLUS(ctx, amt) ((ctx.frontWinPos + amt) > ctx.readEnd)?(ctx.readEnd):(ctx.frontWinPos + amt)

#define GET_BACK_WIN(src, pos) (((pos) - (src)) > BACK_WIN_SIZE)?((pos) - BACK_WIN_SIZE):(src)
#define GET_BACK_WIN_LIM(src, pos) (((pos) - (src)) > BACK_WIN_SIZE_LIM)?((pos) - BACK_WIN_SIZE_LIM):(src)

#define GET_BACK_OFFSET(match) ((uint32_t)(match->matchPos - match->backrefPos))

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct {
		uint8_t* matchPos;
		uint8_t* backrefPos;
		uint32_t runlen;
	} RunMatch;

	typedef struct {
		uint8_t dataBuffer[0x40];
		uint8_t* dataBufferPos;

		uint8_t* readPos;
		uint8_t* writePos;
		uint8_t* readEnd;
		uint8_t* backWinPos;
		uint8_t* frontWinPos;

		RunMatch* useMatch;

		uint8_t ctrlByte;
		uint8_t ctrlMask;
		uint8_t flushFlag;
	} CompressionContext;

	typedef struct {
		RunMatch cache[MATCH_CACHE_SIZE];
		int backPos;
		int frontPos;
		int itr;
		int used;
	} MatchCache;

	inline void flushDataBuffer(CompressionContext* ctx) {
		size_t cpyamt = (ctx->dataBufferPos - ctx->dataBuffer);

		memcpy(ctx->writePos, ctx->dataBuffer, cpyamt);
		ctx->writePos += cpyamt;
		ctx->dataBufferPos = ctx->dataBuffer;
	}

	inline void flushControlByte(CompressionContext* ctx) {
		*ctx->writePos++ = ctx->ctrlByte;
		ctx->ctrlMask = 0x80;
		ctx->ctrlByte = 0;
	}

	inline void writeControlBitLiteral(CompressionContext* ctx) {
		ctx->ctrlByte |= ctx->ctrlMask;
		ctx->ctrlMask >>= 1;
		FLUSH_CTRL_NOT_DATA;
	}

	inline void writeControlBitsLongMatch(CompressionContext* ctx) {
		ctx->ctrlMask >>= 1; //First 0
		FLUSH_CTRL_AND_DATA;

		//Second 1
		ctx->ctrlByte |= ctx->ctrlMask;
		ctx->ctrlMask >>= 1;
		FLUSH_CTRL_NOT_DATA;
	}

	inline void writeControlBitsShortMatch(CompressionContext* ctx, uint8_t runLen) {
		ctx->ctrlMask >>= 1; //First 0
		FLUSH_CTRL_AND_DATA;

		ctx->ctrlMask >>= 1; //Second 0
		FLUSH_CTRL_AND_DATA;

		runLen -= 2;
		if (runLen & 0x2) ctx->ctrlByte |= ctx->ctrlMask;
		ctx->ctrlMask >>= 1;
		FLUSH_CTRL_AND_DATA;

		if (runLen & 0x1) ctx->ctrlByte |= ctx->ctrlMask;
		ctx->ctrlMask >>= 1;
		FLUSH_CTRL_NOT_DATA;
	}

	inline int matchValid(RunMatch* match) {
		if (match->runlen < MIN_RUN_SIZE) return 0;
		if (match->runlen < MIN_RUN_SIZE_LONG) {
			if ((match->matchPos - match->backrefPos) > MAX_BACKSET_SHORT) return 0;
		}
		return 1;
	}

	void initMatchData(RunMatch* match) {
		match->matchPos = NULL;
		match->backrefPos = NULL;
		match->runlen = 0;
	}

	void findMatch(RunMatch* dst, uint8_t* readPos, uint8_t* backWindowLimit, uint8_t* frontWindowLimit, int backStart) {
		uint8_t* bpos, * fitr, * bitr;
		uint32_t run = 0;
		uint64_t backset;

		initMatchData(dst);
		if (!readPos) return;
		bpos = readPos - backStart;
		while (bpos >= backWindowLimit) {
			fitr = readPos; bitr = bpos;
			run = 0;
			while ((fitr < frontWindowLimit) && (*fitr++ == *bitr++)) {
				run++;
			}

			if ((run >= MIN_RUN_SIZE) && (run > dst->runlen)) {
				//Make sure it is an encodable run.
				backset = (readPos - bpos);
				if (backset <= MAX_BACKSET_SHORT || run >= MIN_RUN_SIZE_LONG) {
					dst->runlen = run;
					dst->backrefPos = bpos;
				}
			}
			bpos--;
		}
		dst->matchPos = readPos;
	}

	void findMatchNoThrough(RunMatch* dst, uint8_t* readPos, uint8_t* backWindowLimit, uint8_t* frontWindowLimit) {
		uint8_t* bpos, * fitr, * bitr;
		uint32_t run = 0;
		uint64_t backset;

		initMatchData(dst);
		if (!readPos) return;
		//bpos = readPos - 2;
		bpos = backWindowLimit;
		//while (bpos >= backWindowLimit) {
		while (bpos <= (readPos - 2)) {
			fitr = readPos; bitr = bpos;
			run = 0;
			while ((bitr < readPos) && (fitr < frontWindowLimit) && (*fitr++ == *bitr++)) {
				run++;
			}

			if ((run >= MIN_RUN_SIZE) && (run >= dst->runlen)) {
				//Make sure it is an encodable run.
				backset = (readPos - bpos);
				if (backset <= MAX_BACKSET_SHORT || run >= MIN_RUN_SIZE_LONG) {
					dst->runlen = run;
					dst->backrefPos = bpos;
				}
			}
			if (run >= MAX_RUN_SIZE_LONG) break;
			//bpos--;
			bpos++;
		}
		dst->matchPos = readPos;
	}

	void checkCacheForMatch(uint8_t* readPos, uint8_t* dataEnd, MatchCache* cache, RunMatch* result) {
		RunMatch* cachedMatch;
		uint8_t* fpos;
		uint8_t* bpos;
		uint32_t run, backDist;

		result->runlen = 0;
		result->matchPos = readPos;
		result->backrefPos = NULL;
		cache->itr = cache->backPos;
		while (cache->itr != cache->frontPos) {
			cachedMatch = &cache->cache[cache->itr];
			backDist = (int)((readPos - cachedMatch->matchPos));
			if ((readPos - cachedMatch->backrefPos) > MAX_BACKSET_LONG) break;

			run = 0;
			fpos = readPos;
			bpos = cachedMatch->matchPos;
			while (*fpos++ == *bpos++) {
				run++;
				if (fpos >= dataEnd) break;
				if (bpos >= fpos) break; //No thru
				if (run >= MAX_RUN_SIZE_LONG) break;
			}

			if (run >= result->runlen) {
				if (run >= MIN_RUN_SIZE_LONG) {
					result->runlen = run;
					result->backrefPos = cachedMatch->matchPos;
					if (run >= MAX_RUN_SIZE_LONG) return; //Already as good as it gets.
				}
				else if (run >= MIN_RUN_SIZE) {
					//Backset cannot be more than 256
					if (backDist <= MAX_BACKSET_SHORT) {
						result->runlen = run;
						result->backrefPos = cachedMatch->matchPos;
					}
				}
			}

			//cache->itr--;
			//if (cache->itr <= 0) cache->itr = MATCH_CACHE_SIZE;
			cache->itr++;
			if (cache->itr >= MATCH_CACHE_SIZE) cache->itr = 0;
		}
	}

	inline void cacheMatch(MatchCache* cache, RunMatch* match) {
		memcpy(&cache->cache[cache->frontPos++], match, sizeof(RunMatch));
		if (cache->frontPos >= MATCH_CACHE_SIZE) cache->frontPos = 0;

		if (cache->used >= MATCH_CACHE_SIZE) {
			//Also increment back...
			cache->backPos++; 
			if (cache->backPos >= MATCH_CACHE_SIZE) cache->backPos = 0;
		}
		else cache->used++;
	}

	inline void initMatchCache(MatchCache* cache) {
		memset(cache, 0, sizeof(MatchCache));
	}

	int scoreRun(RunMatch* match) {
		size_t backamt;
		int litsize;
		int trgsize;

		if (match->runlen < MIN_RUN_SIZE) return 9;

		backamt = match->matchPos - match->backrefPos;
		litsize = match->runlen * 9;

		if (match->runlen <= MAX_RUN_SHORT && backamt <= MAX_BACKSET_SHORT) {
			//Short
			trgsize = 12; //1 byte + 4 bits
		}
		else {
			if (match->runlen > MAX_RUN_SIZE_LONG_2BYTE) {
				trgsize = 26; //3 bytes + 2 bits
			}
			else trgsize = 18; //2 bytes + 2 bits
		}

		return trgsize - litsize;
	}

	void initCtx(CompressionContext* ctx, void* dst, void* src, size_t srcSize) {
		ctx->ctrlByte = 0x00;
		ctx->ctrlMask = 0x80;
		ctx->dataBufferPos = ctx->dataBuffer;
		ctx->readPos = (uint8_t*)src;
		ctx->writePos = (uint8_t*)dst;
		ctx->readEnd = ctx->readPos + srcSize;
		ctx->backWinPos = ctx->readPos;
		ctx->frontWinPos = ctx->readPos + MAX_RUN_SIZE_LONG;
		if (ctx->frontWinPos > ctx->readEnd) ctx->frontWinPos = ctx->readEnd;
		ctx->useMatch = NULL;
		ctx->flushFlag = 0;
	}

	void finishWrite(CompressionContext* ctx, void* dst) {
		int ctrlBitsRem;
		int i, temp;
		uint8_t tempMask;
		size_t sizeval;

		if (ctx->ctrlMask != 0x80) {

			//Count remaining bits
			ctrlBitsRem = 0; //ctrl_bits_rem
			tempMask = ctx->ctrlMask;
			while (tempMask != 0) {
				ctrlBitsRem++;
				tempMask >>= 1;
			}

			if (ctrlBitsRem >= 2) {
				//Write another imaginary 01 command
				ctx->ctrlMask >>= 1;
				ctx->ctrlByte |= ctx->ctrlMask;
				ctx->ctrlMask >>= 1;

				for (i = 0; i < 3; i++) *ctx->dataBufferPos++ = 0x00;
				*ctx->writePos++ = ctx->ctrlByte;
				flushDataBuffer(ctx);
			}
			else {
				*ctx->writePos++ = ctx->ctrlByte;
				flushDataBuffer(ctx);
				if (ctrlBitsRem == 1) {
					*ctx->writePos++ = 0x80;
					for (i = 0; i < 3; i++) *ctx->writePos++ = 0x00;
				}
			}
		}
		else {
			//Last command and data blob have been dumped already.
			//Just hallucinate a dummy long command.
			*ctx->writePos++ = 0x40;
			for (i = 0; i < 3; i++) *ctx->writePos++ = 0x00;
		}

		//Pad to 4
		sizeval = ctx->writePos - (uint8_t*)dst;
		temp = (4 - (uint32_t)(sizeval & 0x3)) & 0x3;
		for (i = 0; i < temp; i++) *ctx->writePos++ = 0x00;
	}

	int encodeMatch(CompressionContext* ctx, RunMatch* match) {
		int count = 0;
		int i;
		size_t sizeval;
		int32_t backoff;

		if (!match || match->runlen < MIN_RUN_SIZE) {
			//Literal
			writeControlBitLiteral(ctx);
			*ctx->dataBufferPos++ = *ctx->readPos;
			count++;
		}
		else {
			//Any literals
			sizeval = match->matchPos - ctx->readPos;
			for (i = 0; i < sizeval; i++) {
				writeControlBitLiteral(ctx);
				*ctx->dataBufferPos++ = *(ctx->readPos + i);
				count++;

				if (ctx->flushFlag) {
					flushDataBuffer(ctx);
					ctx->flushFlag = 0;
				}
			}

			//Backref
			backoff = (uint32_t)(match->matchPos - match->backrefPos);
			if ((match->runlen <= MAX_RUN_SHORT) && (backoff <= MAX_BACKSET_SHORT)) {
				writeControlBitsShortMatch(ctx, match->runlen);
				*ctx->dataBufferPos++ = (uint8_t)(-backoff);
			}
			else {
				writeControlBitsLongMatch(ctx);
				ctx->dataBufferPos[0] = (uint8_t)((-backoff >> 5) & 0xff);
				ctx->dataBufferPos[1] = (uint8_t)((-backoff << 3) & 0xff);
				if (match->runlen > MAX_RUN_SIZE_LONG_2BYTE) {
					//3 bytes
					ctx->dataBufferPos[2] = (uint8_t)(match->runlen - 1);
					ctx->dataBufferPos += 3;
				}
				else {
					//2 bytes
					ctx->dataBufferPos[1] |= (uint8_t)((match->runlen - 2) & 0x7);
					ctx->dataBufferPos += 2;
				}
			}

			count += match->runlen;
		}

		return count;
	}

#define MY_MATCH matches[0]
#define NEXT_MATCH matches[1]
#define OTHER_MATCH matches[2]

	size_t lzmu_compress_lookahead(void* dst, void* src, size_t srcLen) {
		CompressionContext ctx;
		uint32_t i32;
		int score1, score2;
		int skipFlag = 0;
		uint8_t* bwinTemp;
		uint8_t* fwinTemp;
		RunMatch matches[3];
		//uint64_t dbgPos = 0LL;

		if (!dst || !src) return 0;

		initCtx(&ctx, dst, src, srcLen);

		//Write decompressed size (Little-Endian)
		/*i32 = (uint32_t)srcLen;
		for (i = 0; i < 4; i++) {
			*ctx.writePos++ = (uint8_t)(srcLen & 0xFF);
			srcLen >>= 8;
		}*/

		while (ctx.readPos < ctx.readEnd) {
			//dbgPos = ctx.readPos - src;
			ctx.useMatch = NULL;
			i32 = 0; //Increment count
			findMatch(&MY_MATCH, ctx.readPos, ctx.backWinPos, ctx.frontWinPos, 1);

			if (!skipFlag && (MY_MATCH.runlen >= MIN_RUN_SIZE) && ((ctx.readPos + 1) < ctx.readEnd)) {

				//Check position + 1
				fwinTemp = FRONT_WIN_PLUS(ctx, 1);
				bwinTemp = GET_BACK_WIN((uint8_t*)src, ctx.readPos + 1);
				findMatch(&NEXT_MATCH, ctx.readPos + 1, bwinTemp, fwinTemp, 1);
				if (NEXT_MATCH.runlen >= MIN_RUN_SIZE) {
					score1 = scoreRun(&MY_MATCH);
					score2 = scoreRun(&NEXT_MATCH);
					if (MY_MATCH.runlen <= NEXT_MATCH.runlen) {
						fwinTemp = FRONT_WIN_PLUS(ctx, MY_MATCH.runlen);
						bwinTemp = GET_BACK_WIN((uint8_t*)src, ctx.readPos + MY_MATCH.runlen);
						findMatch(&OTHER_MATCH, ctx.readPos + MY_MATCH.runlen, bwinTemp, fwinTemp, 1);
						if (OTHER_MATCH.runlen >= MIN_RUN_SIZE) {
							score1 += scoreRun(&OTHER_MATCH);
						}

						if (score2 < score1) ctx.useMatch = &NEXT_MATCH;
						else {
							ctx.useMatch = &MY_MATCH;
							skipFlag = 1;
						}
					}
					else {
						if(score2 < score1) ctx.useMatch = &NEXT_MATCH;
						else ctx.useMatch = &MY_MATCH;
					}
				}
				else {
					ctx.useMatch = &MY_MATCH;
				}
			}
			else {
				ctx.useMatch = &MY_MATCH;
				skipFlag = 0;
			}

			//Encode
			i32 = encodeMatch(&ctx, ctx.useMatch);
			if (ctx.useMatch && (ctx.useMatch->runlen == MAX_RUN_SIZE_LONG)) {
				skipFlag = 0;
			}

			//Flush data buffer, if requested
			if (ctx.flushFlag) {
				flushDataBuffer(&ctx);
				ctx.flushFlag = 0;
			}

			//Increment position (also if runlen is 256, this seems to reset the skip flag)
			ctx.readPos += i32;
			ctx.backWinPos = GET_BACK_WIN((uint8_t*)src, ctx.readPos);
			ctx.frontWinPos = FRONT_WIN_PLUS(ctx, i32);
		}

		//Finishup
		finishWrite(&ctx, dst);
	
		//Save outputsize
		return ctx.writePos - (uint8_t*)dst;
	}

	size_t lzmu_compress_lookaheadFast(void* dst, void* src, size_t srcLen) {
		CompressionContext ctx;
		uint32_t i32;
		int score1, score2;
		int skipFlag = 0;
		uint8_t* bwinTemp;
		uint8_t* fwinTemp;
		RunMatch matches[3];
		//uint64_t dbgPos = 0LL;

		if (!dst || !src) return 0;

		initCtx(&ctx, dst, src, srcLen);
		while (ctx.readPos < ctx.readEnd) {
			//dbgPos = ctx.readPos - src;
			ctx.useMatch = NULL;
			findMatch(&MY_MATCH, ctx.readPos, ctx.backWinPos, ctx.frontWinPos, 1);

			if ((MY_MATCH.runlen >= MIN_RUN_SIZE) && ((ctx.readPos + 1) < ctx.readEnd)) {
				//Check position + 1
				fwinTemp = FRONT_WIN_PLUS(ctx, 1);
				bwinTemp = GET_BACK_WIN((uint8_t*)src, ctx.readPos + 1);
				findMatch(&NEXT_MATCH, ctx.readPos + 1, bwinTemp, fwinTemp, 1);

				if (NEXT_MATCH.runlen >= MIN_RUN_SIZE) {
					score1 = scoreRun(&MY_MATCH);
					score2 = scoreRun(&NEXT_MATCH);
					if (score2 < score1) ctx.useMatch = &NEXT_MATCH;
					else ctx.useMatch = &MY_MATCH;
				}
				else {
					ctx.useMatch = &MY_MATCH;
				}
			}
			else {
				ctx.useMatch = &MY_MATCH;
				skipFlag = 0;
			}

			i32 = encodeMatch(&ctx, ctx.useMatch);
			if (ctx.useMatch && (ctx.useMatch->runlen == MAX_RUN_SIZE_LONG)) {
				skipFlag = 0;
			}

			//Flush data buffer, if requested
			if (ctx.flushFlag) {
				flushDataBuffer(&ctx);
				ctx.flushFlag = 0;
			}

			//Increment position (also if runlen is 256, this seems to reset the skip flag)
			ctx.readPos += i32;
			ctx.backWinPos = GET_BACK_WIN((uint8_t*)src, ctx.readPos);
			ctx.frontWinPos = FRONT_WIN_PLUS(ctx, i32);
		}

		finishWrite(&ctx, dst);

		//Save outputsize
		return ctx.writePos - (uint8_t*)dst;
	}

	size_t lzmu_compress_greedy(void* dst, void* src, size_t srcLen) {
		CompressionContext ctx;
		RunMatch bestMatch;
		int incr;

		if (!dst || !src) return 0;

		initCtx(&ctx, dst, src, srcLen);
		while (ctx.readPos < ctx.readEnd) {
			findMatch(&bestMatch, ctx.readPos, ctx.backWinPos, ctx.frontWinPos, 1);

			//Encode
			incr = encodeMatch(&ctx, &bestMatch);

			//Flush data buffer, if requested
			if (ctx.flushFlag) {
				flushDataBuffer(&ctx);
				ctx.flushFlag = 0;
			}

			//Increment position
			ctx.readPos += incr;
			ctx.backWinPos = GET_BACK_WIN((uint8_t*)src, ctx.readPos);
			ctx.frontWinPos = FRONT_WIN_PLUS(ctx, incr);
		}

		finishWrite(&ctx, dst);

		//Save outputsize
		return ctx.writePos - (uint8_t*)dst;
	}

	size_t lzmu_compress_greedy_nothru(void* dst, void* src, size_t srcLen) {
		CompressionContext ctx;
		RunMatch bestMatch;
		int incr;

		if (!dst || !src) return 0;

		initCtx(&ctx, dst, src, srcLen);
		while (ctx.readPos < ctx.readEnd) {
			findMatchNoThrough(&bestMatch, ctx.readPos, ctx.backWinPos, ctx.frontWinPos);

			//Encode
			incr = encodeMatch(&ctx, &bestMatch);

			//Flush data buffer, if requested
			if (ctx.flushFlag) {
				flushDataBuffer(&ctx);
				ctx.flushFlag = 0;
			}

			//Increment position
			ctx.readPos += incr;
			ctx.backWinPos = GET_BACK_WIN((uint8_t*)src, ctx.readPos);
			ctx.frontWinPos = FRONT_WIN_PLUS(ctx, incr);
		}

		finishWrite(&ctx, dst);

		//Save outputsize
		return ctx.writePos - (uint8_t*)dst;
	}

	size_t lzmu_compress_nothru(void* dst, void* src, size_t srcLen) {
		CompressionContext ctx;
		uint32_t i32;
		int lookahead = 1;
		int checkMatch = 1;
		int literalRun = 0;
		int literalTotal = 0;
		int copiedTotal = 0;
		int events = 0;
		uint32_t i;
		RunMatch matches[3];
		MatchCache matchCache;

		uint64_t dbgPos = 0LL, dbgWpos = 0LL;
		RunMatch* current = NULL;
		RunMatch* onDeck = NULL;

		int byteCounts[256];

		if (!dst || !src) return 0;

		initCtx(&ctx, dst, src, srcLen);
		initMatchCache(&matchCache);
		memset(byteCounts, 0, 256 * sizeof(int));

		while (ctx.readPos < ctx.readEnd) {
			dbgPos = ctx.readPos - (uint8_t*)src;
			dbgWpos = ctx.writePos - (uint8_t*)dst;
			copiedTotal = (int)dbgPos - literalTotal;
			if (dbgPos >= 0x6e1) {
				i32 = 0;
			}

			i32 = 0; //Increment count
			ctx.useMatch = NULL;

			if (checkMatch) {
				/*checkCacheForMatch(ctx.readPos, ctx.readEnd, &matchCache, &MY_MATCH);

				if (MY_MATCH.runlen < MIN_RUN_SIZE) {
					findMatchNoThrough(&MY_MATCH, ctx.readPos, ctx.backWinPos, ctx.frontWinPos);
				}*/
				findMatchNoThrough(&MY_MATCH, ctx.readPos, ctx.backWinPos, ctx.frontWinPos);
			}
			else {
				initMatchData(&MY_MATCH);
				MY_MATCH.matchPos = ctx.readPos;
			}
			ctx.useMatch = &MY_MATCH;
			checkMatch = 1;

			if ((literalRun > 0) && (MY_MATCH.runlen == 2)) {
				//TODO
				//Determine whether or not to break literal run (it really shouldn't matter???)
			}

			if (ctx.useMatch->runlen < MIN_RUN_SIZE) {
				literalRun++;
				literalTotal++;
			}
			else literalRun = 0;

			//Encode
			i32 = encodeMatch(&ctx, ctx.useMatch);
			if (ctx.useMatch && (ctx.useMatch->runlen == MAX_RUN_SIZE_LONG)) {
				onDeck = NULL;
			}

			if (ctx.useMatch->runlen >= MIN_RUN_SIZE) {
				//Cache
				cacheMatch(&matchCache, &MY_MATCH);
			}

			//Flush data buffer, if requested
			if (ctx.flushFlag) {
				flushDataBuffer(&ctx);
				ctx.flushFlag = 0;
			}

			//Record bytes (DEBUG)
			for (i = 0; i < i32; i++) {
				byteCounts[ctx.readPos[i]]++;
			}

			//Increment position
			ctx.readPos += i32;
			ctx.backWinPos = GET_BACK_WIN((uint8_t*)src, ctx.readPos);
			ctx.frontWinPos = FRONT_WIN_PLUS(ctx, i32);
			current = NULL;
			events++;
		}

		//Finishup
		finishWrite(&ctx, dst);

		//Save outputsize
		return ctx.writePos - (uint8_t*)dst;
	}

	size_t lzmu_compress_forceNoThru(void* dst, void* src, size_t srcLen, OfsForceLitTable* holdOfsTable) {
		CompressionContext ctx;
		uint32_t i32, i;
		int checkMatch = 1;
		int ofsTablePos = 0;
		RunMatch matches[3];
		MatchCache matchCache;

		uint64_t dbgPos = 0LL, dbgWpos = 0LL;
		RunMatch* current = NULL;
		RunMatch* onDeck = NULL;

		if (!dst || !src) return 0;

		initCtx(&ctx, dst, src, srcLen);
		initMatchCache(&matchCache);

		while (ctx.readPos < ctx.readEnd) {
			dbgPos = ctx.readPos - (uint8_t*)src;
			dbgWpos = ctx.writePos - (uint8_t*)dst;
			//copiedTotal = dbgPos - literalTotal;
			/*if (dbgPos >= 0x6e1) {
				i32 = 0;
			}*/

			i32 = 0; //Increment count
			ctx.useMatch = NULL;

			if (holdOfsTable[ofsTablePos].offset == (uint32_t)dbgPos) {
				//Do a literal run and continue.
				MY_MATCH.runlen = 0;
				MY_MATCH.backrefPos = NULL;
				for (i = 0; i < holdOfsTable[ofsTablePos].len; i++) {
					MY_MATCH.matchPos = ctx.readPos;
					encodeMatch(&ctx, &MY_MATCH);

					if (ctx.flushFlag) {
						flushDataBuffer(&ctx);
						ctx.flushFlag = 0;
					}

					ctx.readPos++;
				}
				ctx.backWinPos = GET_BACK_WIN_LIM((uint8_t*)src, ctx.readPos);
				ctx.frontWinPos = FRONT_WIN_PLUS(ctx, holdOfsTable[ofsTablePos].len);
				current = NULL;
				//events += holdOfsTable[ofsTablePos].len;
				//literalRun += holdOfsTable[ofsTablePos].len;
				//literalTotal += holdOfsTable[ofsTablePos].len;
				ofsTablePos++;
				continue;
			}

			if (checkMatch) {
				/*checkCacheForMatch(ctx.readPos, ctx.readEnd, &matchCache, &MY_MATCH);

				if (MY_MATCH.runlen < MIN_RUN_SIZE) {
					findMatchNoThrough(&MY_MATCH, ctx.readPos, ctx.backWinPos, ctx.frontWinPos);
				}*/
				findMatchNoThrough(&MY_MATCH, ctx.readPos, ctx.backWinPos, ctx.frontWinPos);
			}
			else {
				initMatchData(&MY_MATCH);
				MY_MATCH.matchPos = ctx.readPos;
			}
			ctx.useMatch = &MY_MATCH;
			checkMatch = 1;

			/*if (ctx.useMatch->runlen < MIN_RUN_SIZE) {
				literalRun++;
				literalTotal++;
			}
			else literalRun = 0;*/

			//Encode
			i32 = encodeMatch(&ctx, ctx.useMatch);
			if (ctx.useMatch && (ctx.useMatch->runlen == MAX_RUN_SIZE_LONG)) {
				onDeck = NULL;
			}

			if (ctx.useMatch->runlen >= MIN_RUN_SIZE) {
				//Cache
				cacheMatch(&matchCache, &MY_MATCH);
			}

			//Flush data buffer, if requested
			if (ctx.flushFlag) {
				flushDataBuffer(&ctx);
				ctx.flushFlag = 0;
			}

			//Increment position
			ctx.readPos += i32;
			ctx.backWinPos = GET_BACK_WIN_LIM((uint8_t*)src, ctx.readPos);
			ctx.frontWinPos = FRONT_WIN_PLUS(ctx, i32);
			current = NULL;
			//events++;
		}

		//Finishup
		finishWrite(&ctx, dst);

		//Save outputsize
		return ctx.writePos - (uint8_t*)dst;
	}

	size_t lzmu_compress_match(void* dst, void* src, size_t srcLen, int strat){
		switch (strat) {
		case COMPR_STRAT_GREEDY:
			return lzmu_compress_greedy(dst, src, srcLen);
		case COMPR_STRAT_QUICK_LOOKAHEAD:
			return lzmu_compress_lookaheadFast(dst, src, srcLen);
		case COMPR_STRAT_STD_LOOKAHEAD:
			return lzmu_compress_lookahead(dst, src, srcLen);
		case COMPR_STRAT_NO_THRU:
			return lzmu_compress_nothru(dst, src, srcLen);
		}

		return 0L;
	}


#ifdef __cplusplus
}
#endif