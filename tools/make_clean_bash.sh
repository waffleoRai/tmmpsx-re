#!/bin/bash

ASMDIR=asm
CODEBUILD_DIR=build
CDDIR=cd
ASSETDIR=assets
ASSETSPECDIR=assets/arcspec
REF_DIR=expected

SPLAT_YAML=splat.yaml
BUILD_ISO=build.iso
MODULE_BUILD_SH=myubuild.sh
MODULE_BUILD_LD=./myubuild.ld

ASSET_GEN_DIRS=$(find ${ASSETDIR}/* -type d -not -path "${ASSETSPECDIR}*")
ASSET_GEN_FILES=$(find ${ASSETDIR}/* -maxdepth 1 -type f -not -path "${ASSETSPECDIR}*")

rm -f splatlink.ld
rm -f undefined_funcs_auto.txt
rm -f undefined_syms_auto.txt
rm -f ${BUILD_ISO}
rm -f ${SPLAT_YAML}
rm -f ${MODULE_BUILD_SH}
rm -f ${MODULE_BUILD_LD}
rm -rf ${CDDIR}
rm -rf ${CODEBUILD_DIR}
rm -rf ${ASSET_GEN_FILES}
rm -rf ${ASSET_GEN_DIRS}
rm -rf ${ASMDIR}
rm -rf ${REF_DIR}
