#ifndef MISSING_NO_H
#define MISSING_NO_H

typedef unsigned char mn_u8;
typedef unsigned short mn_u16;
typedef unsigned long long mn_size;

typedef _Bool mn_bool;
#define MN_FALSE ((_Bool)0)
#define MN_TRUE  ((_Bool)1)

typedef struct {
    mn_bool DPadL;
    mn_bool DPadR;
    mn_bool DPadU;
    mn_bool DPadD;
    mn_bool A;
    mn_bool B;
    mn_bool Select;
    mn_bool Start;
} MN_Inputs;

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
