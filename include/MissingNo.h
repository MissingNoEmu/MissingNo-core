#ifndef MISSING_NO_H
#define MISSING_NO_H

#include "commons.h"

typedef struct MN_Emu MissingNoCore;
typedef MN_Inputs MissingNoInputs;

typedef enum {
    MN_OK,
    MN_NINTENDO_GRAPHIC,
    MN_HEADER_CHECKSUM,
    MN_GLOBAL_CHECKSUM
} MissingNo_Verification;

mn_size missingno_cartROM_size(const mn_u8* buffer);
mn_size missingno_cartRAM_size(const mn_u8* buffer);
mn_size missingno_core_size(void);
mn_bool missingno_init(MissingNoCore* ctx, const mn_u8* buffer);
MissingNo_Verification missingno_verify(MissingNoCore* ctx);
void missingno_game_title(MissingNoCore* ctx, char* write_back_buffer);
mn_bool missingno_tick(MissingNoCore* ctx, const MissingNoInputs* inputs);

#endif
