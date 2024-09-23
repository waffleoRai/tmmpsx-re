#ifndef MYUC_RESHEAD_H
#define MYUC_RESHEAD_H

#include "res/ResAnime.h"
#include "res/ResBgm.h"
#include "res/ResEffect.h"
#include "res/ResFace.h"
#include "res/ResField.h"
#include "res/ResScreen.h"
#include "res/ResSce.h"
#include "res/ResSe.h"
#include "res/ResSystem.h"
#include "res/ResUnit.h"
#include "res/ResMovie.h"
#include "res/ResVoice.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum GameFileID {
	/*00*/ NoFile = 0,
	/*01*/ D_Unit,
	/*02*/ D_Screen,
	/*03*/ D_System,
	/*04*/ D_Sce,
	/*05*/ D_Anime,
	/*06*/ D_Effect,
	/*07*/ D_Face,
	/*08*/ D_Field,
	/*09*/ Movie,
	/*10*/ D_Bgm,
	/*11*/ D_Se,
	/*12*/ VoiceStr,
	/*13*/ _GameFileCount
} GameFileID;

#define CD_FILE_NAMES sCdFilesEmptyString, "\\D_UNIT.BIN", "\\D_SCREEN.BIN", "\\D_SYSTEM.BIN", \
				"\\D_SCE.BIN", "\\D_ANIME.BIN", "\\D_EFFECT.BIN", "\\D_FACE.BIN", \
				"\\D_FIELD.BIN", "\\MOVIE.BIN", "\\D_BGM.BIN", "\\D_SE.BIN", "\\VOICE.STR"


#ifdef __cplusplus
}
#endif

#endif

