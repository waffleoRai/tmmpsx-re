#!/bin/bash

# Usage message ----------------------------

usage(){
cat << EOF
-----------------------------------------------------------------------
PURPOSE: Auto-converts PSX compressed audio data (in .vag or .aifc containers) in a directory to a more common format.

ARGUMENTS:
	-i	PATH	[Optional]	Path to input directory. Defaults to wd
	-f	STRING	[Optional]	Target format. Valid options below. Defaults to flac.
	-j	PATH	[Optional]	MyuPackager JAR path. Defaults to wd/tools/myu_packager/myupack.jar
	-k	FLAG	[Optional]	Keep original files.
	-r	FLAG	[Optional]	Scan for input files recursively.
	
FORMAT STRINGS:
	wav		RIFF wave uncompressed 16-bit PCM
	aiff	AIFF uncompressed 16-bit PCM
	flac	FLAC lossless compressed 16-bit PCM
	ogg		Ogg-Vorbis (lossy)
	mp3		MP3 (lossy)

-----------------------------------------------------------------------
EOF
}

MYUPACK_JAR_PATH="./tools/myu_packager/myupack.jar"

# Gather args ----------------------------

INPUT_DIR=""
RECURSIVE=0
KEEP_OG=0
FORMAT="flac"

while getopts "i:f:j:kr" OPTION; do
	case $OPTION in
		i)
			INPUT_DIR=$OPTARG;;
		f)
			FORMAT=$OPTARG;;
		j)
			MYUPACK_JAR_PATH=$OPTARG;;
		k)
			KEEP_OG=1;;
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
echo -e "Running unpack_audio v${VERSION} (${UPDATE_DATE})!"

if [ -z "${INPUT_DIR}" ]; then
	INPUT_DIR=$(pwd)
	echo -e "Input directory was not provided. Set to wd: ${INPUT_DIR}"
fi

if [ ! -d ${INPUT_DIR} ]; then
	echo -e "Input directory provided does not exist!"
	usage
	exit 1
fi

if [ "${FORMAT}" == "wav" ]; then
	echo -e "Target format: WAV (Uncompressed)"
elif [ "${FORMAT}" == "aiff" ]; then
	echo -e "Target format: AIFF (Uncompressed)"
elif [ "${FORMAT}" == "flac" ]; then
	echo -e "Target format: FLAC (Lossless)"
elif [ "${FORMAT}" == "ogg" ]; then
	echo -e "Target format: Ogg-Vorbis (Lossy)"
elif [ "${FORMAT}" == "mp3" ]; then
	echo -e "Target format: MP3 (Lossy)"
else
	echo -e "Format \"${FORMAT}\" not recognized! Exiting."
	usage
	exit 1
fi

# Get input file list ----------------------------
tempFileListPath="${INPUT_DIR}/psxaudiofilelist.txt"
addParam=""
if [ $RECURSIVE -eq 0 ]; then
	addParam=" -maxdepth 1"
fi

find "${INPUT_DIR}"${addParam} -type f -iregex '.*.vag$' > "${tempFileListPath}"
find "${INPUT_DIR}"${addParam} -type f -iregex '.*.VAG$' >> "${tempFileListPath}"
find "${INPUT_DIR}"${addParam} -type f -iregex '.*.aifc$' >> "${tempFileListPath}"

while read -r InputFile; do
	echo -e "Now processing ${InputFile} ..."
	
	if [ "${FORMAT}" == "aiff" ]; then
		PCMFile="${InputFile%.*}.aiff"
	else
		PCMFile="${InputFile%.*}.wav"
	fi
	
	java -jar ${MYUPACK_JAR_PATH} pcmme --input "${InputFile}" --output "${PCMFile}"
	
	#If compressed in a different codec, do compression and delete intermediate wav
	if [ "${FORMAT}" == "flac" ]; then
		OutFile="${InputFile%.*}.flac"
		flac --best --verify -f -o "${OutFile}" "${PCMFile}"
		rm "${PCMFile}"
	elif [ "${FORMAT}" == "ogg" ]; then
		OutFile="${InputFile%.*}.ogg"
		oggenc -q 7 -o "${OutFile}" "${PCMFile}"
		rm "${PCMFile}"
	elif [ "${FORMAT}" == "mp3" ]; then
		OutFile="${InputFile%.*}.mp3"
		lame --preset extreme "${PCMFile}" "${OutFile}" < /dev/null
		rm "${PCMFile}"
	fi
	
	#Delete original if requested
	if [ $KEEP_OG -eq 0 ]; then
		rm "${InputFile}"
	fi
	
done < "${tempFileListPath}"

rm "${tempFileListPath}"
