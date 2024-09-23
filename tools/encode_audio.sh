#!/bin/bash

# Usage message ----------------------------

usage(){
cat << EOF
-----------------------------------------------------------------------
PURPOSE: Scans input directory for compatible audio files (.wav, .aiff, .ogg, .flac, .mp3) and 

ARGUMENTS:
	-i	PATH	[Optional]	Path to input directory. Defaults to wd
	-j	PATH	[Optional]	MyuPackager JAR path. Defaults to wd/tools/myu_packager/myupack.jar
	-s	FLAG	[Optional]	Output for streaming (outputs to .aifc instead of .vag with samples arranged for an XA stream)
	-c	FLAG	[Optional]	Remove (clean up) original files.
	-r	FLAG	[Optional]	Scan for input files recursively.
	
NOTES:
	- Input files with more than one channel will be split into one file for each channel if the output is VAG.

-----------------------------------------------------------------------
EOF
}

MYUPACK_JAR_PATH="./tools/myu_packager/myupack.jar"

# Gather args ----------------------------

INPUT_DIR=""
RECURSIVE=0
KEEP_OG=1
STREAM_FMT=0

while getopts "i:o:T:a:n:f:kc" OPTION; do
	case $OPTION in
		i)
			INPUT_DIR=$OPTARG;;
		j)
			MYUPACK_JAR_PATH=$OPTARG;;
		s)
			STREAM_FMT=1;;
		c)
			KEEP_OG=0;;
		r)
			RECURSIVE=1;;
		\?)
			echo -e "Invalid argument found!"
			usage
			exit 1
	esac
done

# Check args ----------------------------

VERSION=1.1.0
UPDATE_DATE="Sep 8, 2024"
echo -e "Running encode_audio v${VERSION} (${UPDATE_DATE})!"

if [ -z "${INPUT_DIR}" ]; then
	INPUT_DIR=$(pwd)
	echo -e "Input directory was not provided. Set to wd: ${INPUT_DIR}"
fi

if [ ! -d ${INPUT_DIR} ]; then
	echo -e "Input directory provided does not exist!"
	usage
	exit 1
fi

# Get input file list ----------------------------
tempFileListPath="${INPUT_DIR}/psxaudiofilelist.txt"
addParam=""
if [ $RECURSIVE -eq 0 ]; then
	addParam=" -maxdepth 1"
fi

find "${INPUT_DIR}"${addParam} -type f -iregex '.*.wav$' > "${tempFileListPath}"
find "${INPUT_DIR}"${addParam} -type f -iregex '.*.WAV$' >> "${tempFileListPath}"
find "${INPUT_DIR}"${addParam} -type f -iregex '.*.wave$' >> "${tempFileListPath}"
find "${INPUT_DIR}"${addParam} -type f -iregex '.*.WAVE$' >> "${tempFileListPath}"
find "${INPUT_DIR}"${addParam} -type f -iregex '.*.aif$' >> "${tempFileListPath}"
find "${INPUT_DIR}"${addParam} -type f -iregex '.*.aiff$' >> "${tempFileListPath}"
find "${INPUT_DIR}"${addParam} -type f -iregex '.*.AIF$' >> "${tempFileListPath}"
find "${INPUT_DIR}"${addParam} -type f -iregex '.*.AIFF$' >> "${tempFileListPath}"
find "${INPUT_DIR}"${addParam} -type f -iregex '.*.flac$' >> "${tempFileListPath}"
find "${INPUT_DIR}"${addParam} -type f -iregex '.*.FLAC$' >> "${tempFileListPath}"
find "${INPUT_DIR}"${addParam} -type f -iregex '.*.ogg$' > "${tempFileListPath}"
find "${INPUT_DIR}"${addParam} -type f -iregex '.*.OGG$' >> "${tempFileListPath}"
find "${INPUT_DIR}"${addParam} -type f -iregex '.*.mp3$' >> "${tempFileListPath}"
find "${INPUT_DIR}"${addParam} -type f -iregex '.*.MP3$' >> "${tempFileListPath}"

while read -r InputFile; do
	echo -e "Now processing ${InputFile} ..."
	#https://stackoverflow.com/questions/2264428/how-to-convert-a-string-to-lower-case-in-bash
	InputExt="${InputFile##*.}"
	InputExt=$(echo "${InputExt}" | awk '{print tolower($0)}')
	
	#If PCM intermediate is needed, generate that.
	if [[ "${FORMAT}" == "wav" ]] || [[ "${FORMAT}" == "wave" ]]; then
		echo -e "Format assumed from file extension: WAV (PCM)"
		PCMFile="${InputFile}"
	elif [[ "${FORMAT}" == "aiff" ]] || [[ "${FORMAT}" == "aif" ]]; then
		echo -e "Format assumed from file extension: AIFF (PCM)"
		PCMFile="${InputFile}"
	elif [ "${FORMAT}" == "flac" ]; then
		echo -e "Format assumed from file extension: FLAC"
		PCMFile="${InputFile%.*}.wav"
		flac -d -o "${PCMFile}" "${InputFile}"
	elif [ "${FORMAT}" == "ogg" ]; then
		echo -e "Format assumed from file extension: Ogg Vorbis"
		PCMFile="${InputFile%.*}.wav"
		oggdec -b 16 -o "${PCMFile}" "${InputFile}"
	elif [ "${FORMAT}" == "mp3" ]; then
		echo -e "Format assumed from file extension: MP3"
		PCMFile="${InputFile%.*}.wav"
		lame --decode --mp3input "${InputFile}" "${PCMFile}" < /dev/null
	fi
	
	if [ $STREAM_FMT -eq 1 ]; then
		ADPCMFile="${InputFile%.*}.aifc"
	else
		ADPCMFile="${InputFile%.*}.vag"
	fi
	
	java -jar ${MYUPACK_JAR_PATH} spuenc --input "${PCMFile}" --output "${ADPCMFile}"
	
	#Remove intermediate, if applicable
	if [ "${FORMAT}" == "flac" ]; then
		rm "${PCMFile}"
	elif [ "${FORMAT}" == "ogg" ]; then
		rm "${PCMFile}"
	elif [ "${FORMAT}" == "mp3" ]; then
		rm "${PCMFile}"
	fi
	
	#Delete original if requested
	if [ $KEEP_OG -eq 0 ]; then
		rm "${InputFile}"
	fi
	
done < "${tempFileListPath}"

rm "${tempFileListPath}"
