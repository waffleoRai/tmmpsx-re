#!/bin/bash

TOOLS_DIR=tools
INCL_DIR=include
SRC_DIR=src

CHECKSUM_FILE=checksums.csv
CD_BUILD_XML=isobuild.xml
BUILD_ISO=build.iso

${TOOLS_DIR}/make_build_bash.sh
${MYUPACK} isopack --spec "${CD_BUILD_XML}" --out "${BUILD_ISO}" --checksums "${CHECKSUM_FILE}" --incldir "${INCL_DIR}/res" --cdir "${SRC_DIR}/res" -match
