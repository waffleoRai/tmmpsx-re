#!/bin/bash

SPLAT="python3 -m splat"

ASMDIR=asm
CDDIR=cd
ASSETDIR=assets
ASSETSPECDIR=assets/arcspec
TOOLS_DIR=tools
REF_DIR=expected

CHECKSUM_FILE=checksums.csv
CD_BUILD_XML=isobuild.xml
SPLAT_YAML=splat.yaml
SPLAT_YAML_BASE=splatops.yaml
SEC_TABLE=symbols/secs.tsv
DATA_DUMP_XML=symbols/datadump.xml

MYUPACK_JAR=${TOOLS_DIR}/myu_packager/myupack.jar
MYUPACK="java -Djava.library.path=${MY_DIR}/${TOOLS_DIR}/myu_packager:${LINUX_LIB_PATH}:\${env_var:LD_LIBRARY_PATH} -jar ${MYUPACK_JAR}"
WIBO=${TOOLS_DIR}/wibo

CDIMG=$(find ./ -maxdepth 1 -type f -iregex '.*.iso$' | head -1)
${MYUPACK} isounpack --iso "${CDIMG}" --cdout "${CDDIR}" --assetout "${ASSETDIR}" --arcspec "${ASSETSPECDIR}" --checksums "${CHECKSUM_FILE}" --xmlout "${CD_BUILD_XML}"
	
#Find executable file and break it down
PSX_EXE_FILE=$(find ./${CDDIR} -maxdepth 1 -type f -name '*SLPM_*' | head -1)
PSX_EXE_FILE_BACKUP=${REF_DIR}/main.psxexe

${MYUPACK} splatyaml --exepath "${PSX_EXE_FILE}" --out "${SPLAT_YAML}" --sectbl "${SEC_TABLE}" --ops "${SPLAT_YAML_BASE}"
	
${SPLAT} split "${SPLAT_YAML}"
${MYUPACK} asmsplit --asmdir "${ASMDIR}" --outdir "${ASMDIR}/split"
${MYUPACK} data2c --xml "${DATA_DUMP_XML}"
mkdir ${REF_DIR}
cp ${PSX_EXE_FILE} ${PSX_EXE_FILE_BACKUP}
