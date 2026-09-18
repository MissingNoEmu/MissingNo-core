#ifndef EMULATOR_H
#define EMULATOR_H

#include "commons.h"
#include "startup.h"

#define FLAG_Z 0x80
#define FLAG_N 0x40
#define FLAG_H 0x20
#define FLAG_C 0x10

typedef enum
{
    A,
    F,
    B,
    C,
    D,
    E,
    H,
    L
} Regs;

typedef struct MN_Emu
{
    mn_u8 ROM[0x200000];
    mn_u8 r[8];
    mn_u16 SP;
    mn_u16 PC;
    mn_bool ime;
    mn_bool halted;

    mn_u8 internal_mem[0x2000];
    mn_u8 sec_internal_mem[0x7F];
    mn_u8 cart_mem[0x20000];
    mn_u8 VRAM[0x2000];
    mn_u8 IO[0x7F];
    mn_u8 SAB[0xA0];
    mn_u8 interr_en;

    MBC_Type mbc_type;

    mn_u16 ROM_bank;
    mn_u16 RAM_bank;
} MN_Emu;

mn_u8 emulator_step(MN_Emu *emu);

#endif
