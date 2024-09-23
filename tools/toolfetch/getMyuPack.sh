#!/bin/bash

target=$1
echo -e "Get MyuPackager: Target = ${target}"

MYUPKG_JAR_URL="https://github.com/waffleoRai/myupackager/jar/myupack.jar"
if [ ! -s "${target}" ]; then
	echo -e "JAR not found. Downloading from GitHub..."
	wget -O ${target} ${MYUPKG_JAR_URL}
	
	if [ -s "${target}" ]; then
		echo -e "Download succeeded!"
		chmod 755 "${target}"
	else
		echo -e "ERROR: Download failed!"
	fi
else
	echo -e "JAR already found!"
fi