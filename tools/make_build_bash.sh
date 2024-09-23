#!/bin/bash

LINUX_LIB_PATH="/usr/lib/x86_64-linux-gnu"
MY_DIR=$(pwd)

CODEBUILD_DIR=build
CDDIR=cd
TOOLS_DIR=tools
REF_DIR=expected
INCL_DIR=include
SRC_DIR=src

CHECKSUM_FILE=checksums.csv
CD_BUILD_XML=isobuild.xml
SEC_TABLE=symbols/secs.tsv
EXEBUILD_SPEC_XML=exebuild_spec.xml
MODULE_BUILD_SH=myubuild.sh
MODULE_BUILD_LD=./myubuild.ld
KERNEL_SYMS=./kernel_syms.txt
EXE_BUILD_ELF=${CODEBUILD_DIR}/myubuild.elf
LD_MAP_FILE=${CODEBUILD_DIR}/myubuild.map
EXE_BUILD_TEMP=${CODEBUILD_DIR}/myubuild.psxexe
PSX_EXE_FILE_BACKUP=${REF_DIR}/main.psxexe

MYUPACK_JAR=${TOOLS_DIR}/myu_packager/myupack.jar
MYUPACK="java -Djava.library.path=${MY_DIR}/${TOOLS_DIR}/myu_packager:${LINUX_LIB_PATH}:\${env_var:LD_LIBRARY_PATH} -jar ${MYUPACK_JAR}"

${MYUPACK} isopack --spec "${CD_BUILD_XML}" --checksums "${CHECKSUM_FILE}" --incldir "${INCL_DIR}/res" --cdir "${SRC_DIR}/res" -buildall -match -arconly

${MYUPACK} genbld --xmlspec "${EXEBUILD_SPEC_XML}" --builddir "${CODEBUILD_DIR}" --shout "${MODULE_BUILD_SH}" --ldout "${MODULE_BUILD_LD}" --binout "${EXE_BUILD_ELF}" -gnu
./myubuild.sh
${MYUPACK} chkobj --sectbl "${SEC_TABLE}" --builddir "${CODEBUILD_DIR}/obj" --exefile "${PSX_EXE_FILE_BACKUP}"
mips-linux-gnu-ld -EL -N --no-relax -Map ${LD_MAP_FILE} -T ${MODULE_BUILD_LD} -T ${KERNEL_SYMS} -o ${EXE_BUILD_ELF}
EXE_FILE_NAME=$(grep "^BOOT" ${CDDIR}/SYSTEM.CNF)
EXE_FILE_NAME=${EXE_FILE_NAME#*\\}
EXE_FILE_NAME=${EXE_FILE_NAME%;*}
${MYUPACK} elf2psxexe "--input" "${EXE_BUILD_ELF}" --output "${CDDIR}/${EXE_FILE_NAME}"
