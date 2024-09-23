#!/bin/bash

ovrw=0

while [[ $# -gt 0 ]]; do
#echo -e "Arg: $1"
  case $1 in
    -R|--rodata)
      RODATA_PATH="$2"
      shift
      shift
      ;;
	-T|--text)
      TEXT_PATH="$2"
      shift
      shift
      ;;
	-D|--data)
      DATA_PATH="$2"
      shift
      shift
      ;;
	-B|--bss)
      BSS_PATH="$2"
      shift
      shift
      ;;
	-d|--sdata)
      SDATA_PATH="$2"
      shift
      shift
      ;;
	-b|--sbss)
      SBSS_PATH="$2"
      shift
      shift
      ;;
	-o|--output)
      OUT_FILE="$2"
      shift
      shift
      ;;
	-O|--overwrite)
      ovrw=1
      shift
      ;;
    -*|--*)
      echo "Unknown option $1"
      exit 1
      ;;
  esac
done

if [ -z "${OUT_FILE}" ]; then
	echo "Please provide an output target"
    exit 1
fi

if [ -s "${OUT_FILE}" ]; then
	if [ $ovrw -eq 1 ]; then
		#Delete
		rm "${OUT_FILE}"
	else
		#Print message and exit
		echo "Output file ${OUT_FILE} already exists! Please specify -O to overwrite. Now exiting..."
		exit 1
	fi
fi

echo -e ".include \"macro.inc\"\n" > "${OUT_FILE}"

if [ -n "${RODATA_PATH}" ]; then
	if [ -s "${RODATA_PATH}" ]; then
		grep -v "^.include \"macro.inc\"" "${RODATA_PATH}" >> "${OUT_FILE}"
	fi
fi

if [ -n "${TEXT_PATH}" ]; then
	if [ -s "${TEXT_PATH}" ]; then
		grep -v "^.include \"macro.inc\"" "${TEXT_PATH}" >> "${OUT_FILE}"
	fi
fi

if [ -n "${DATA_PATH}" ]; then
	if [ -s "${DATA_PATH}" ]; then
		grep -v "^.include \"macro.inc\"" "${DATA_PATH}" >> "${OUT_FILE}"
	fi
fi

if [ -n "${SDATA_PATH}" ]; then
	if [ -s "${SDATA_PATH}" ]; then
		grep -v "^.include \"macro.inc\"" "${SDATA_PATH}" >> "${OUT_FILE}"
	fi
fi

if [ -n "${SBSS_PATH}" ]; then
	if [ -s "${SBSS_PATH}" ]; then
		grep -v "^.include \"macro.inc\"" "${SBSS_PATH}" >> "${OUT_FILE}"
	fi
fi

if [ -n "${BSS_PATH}" ]; then
	if [ -s "${BSS_PATH}" ]; then
		grep -v "^.include \"macro.inc\"" "${BSS_PATH}" >> "${OUT_FILE}"
	fi
fi
