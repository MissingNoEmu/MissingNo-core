#ifndef MBC_H
#define MBC_H

#include "MissingNo/MissingNo.h"

typedef struct MN_Emu MN_Emu;

typedef enum {
    MN_MBC1_Mode_16_8,
    MN_MBC1_Mode_32_4
} MN_MBC1_Mode;

typedef struct {
    MN_MBC1_Mode mode;
    mn_u8 lo;
    mn_u8 hi;
    mn_bool ram_enabled;
} MN_MBC1_Data;

typedef struct {

} MN_MBC2_Data;

typedef struct {

} MN_MBC3_Data;

typedef struct {

} MN_MBC5_Data;

typedef union {
    MN_MBC1_Data MBC1;
    MN_MBC2_Data MBC2;
    MN_MBC3_Data MBC3;
    MN_MBC5_Data MBC5;
} MN_MBC_Data;


void mn_mbc_write(MN_Emu* emu, mn_u16 loc, mn_u8 val);

#endif
