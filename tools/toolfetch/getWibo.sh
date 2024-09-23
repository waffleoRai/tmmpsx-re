#!/bin/bash

target=$1
echo -e "Get WiBo: Target = ${target}"

WIBO_URL="https://github.com/decompals/wibo/releases/download/0.6.15/wibo"
if [ ! -s "${target}" ]; then
	echo -e "WiBo binary not found. Downloading from GitHub..."
	wget -O ${target} ${WIBO_URL}
	
	if [ -s "${target}" ]; then
		echo -e "Download succeeded!"
		chmod 755 "${target}"
	else
		echo -e "ERROR: Download failed!"
	fi
else
	echo -e "WiBo already found!"
fi