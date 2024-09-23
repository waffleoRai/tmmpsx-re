#!/bin/bash

LZMU_LIB=$1

echo -e "Build liblzmu: Target = ${target}"

JAVA_DIR=$(readlink -f $(which java))
JAVA_DIR=$(dirname "${JAVA_DIR}")
JAVA_DIR=$(dirname "${JAVA_DIR}")

#LZMU_LIB="./myu_packager/liblzmu.so"
if [ ! -s "${LZMU_LIB}" ]; then
	echo -e "SO not found. Attempting to build from source..."
	mkdir "./lz/build"
	gcc -Wall -c "./lz/lzmu.c" -o "./lz/build/lzmu.o" -march=x86-64 -O3 -I "./lz/include"
	gcc -Wall -c "./lz/waffleoRai_extractMyu_LzNative.c" -o "./lz/build/jniIFace.o" -march=x86-64 -O3 -I "./lz/include" -I "${JAVA_DIR}/include" -I "${JAVA_DIR}/include/linux"
	gcc -Wall "./lz/build/jniIFace.o" "./lz/build/lzmu.o" -shared -o "${LZMU_LIB}" 
else
	echo -e "Library file was found! No need to build."
fi