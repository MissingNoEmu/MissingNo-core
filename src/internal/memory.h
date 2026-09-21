#ifndef MEMORY_H
#define MEMORY_H

#include "MissingNo/MissingNo.h"
#include "startup.h"
#include "mbc.h"

typedef struct MN_Emu MN_Emu;

typedef struct {
    mn_u8 internal_mem[0x2000];
    mn_u8 sec_internal_mem[0x7F];
    mn_u8 cart_mem[0x20000];
    mn_u8 VRAM[0x2000];
    mn_u8 IO[0x7F];
    mn_u8 SAB[0xA0];
    mn_u8 interr_en;

    MN_MBC_Type mbc_type;
    MN_MBC_Data mbc_data;

    mn_u16 ROM_bank;
    mn_u16 RAM_bank;
} MN_Mem_Data;

mn_u8 mn_memory_read(MN_Emu* emu, mn_u16 loc);
void mn_memory_write(MN_Emu* emu, mn_u16 loc, mn_u8 val);

#endif
