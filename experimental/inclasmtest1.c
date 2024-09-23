__asm__( ".section .rodata\n" "    .include \""  "asm/data"  "/" "\"CdFiles.rodata\"" ".s\"\n" ".section .text" ) ;

static char D_cdfile_sdata_fe888 = '\0';

static unsigned char D_cdfile_sdata_fe88c[] = {
	0x69, 0x3A, 0x5C, 0x63, 0x64 };
	
static char* gFileNames[] = {&D_cdfile_sdata_fe888, "\\D_UNIT.BIN", "\\D_SCREEN.BIN", "\\D_SYSTEM.BIN", "\\D_SCE.BIN", "\\D_ANIME.BIN", "\\D_EFFECT.BIN", "\\D_FACE.BIN", "\\D_FIELD.BIN", "\\MOVIE.BIN", "\\D_BGM.BIN", "\\D_SE.BIN", "\\VOICE.STR" };

__asm__( ".section .text\n" "    .set noat\n" "    .set noreorder\n" "    .globl " "\"CdFiles_ClearLoadRequests\"" ".NON_MATCHING\n" "    .type " "\"CdFiles_ClearLoadRequests\"" ".NON_MATCHING, @object\n" "    " "\"CdFiles_ClearLoadRequests\"" ".NON_MATCHING:\n" "    .include \""  "asm/split/CdFiles"  "/" "\"CdFiles_ClearLoadRequests\"" ".s\"\n" "    .set reorder\n" "    .set at\n" ) 