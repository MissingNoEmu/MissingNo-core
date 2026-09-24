#ifndef EMULATOR_H
#define EMULATOR_H

#include "MissingNo/MissingNo.h"
#include "memory.h"

typedef enum {
    MN_Regs_A, MN_Regs_F,
    MN_Regs_B, MN_Regs_C,
    MN_Regs_D, MN_Regs_E,
    MN_Regs_H, MN_Regs_L
} MN_Regs;

typedef struct MN_Emu {
    mn_u8 ROM[0x200000];
    mn_u8 r[8];
    mn_u16 SP;
    mn_u16 PC;
    mn_size cycles;

    MN_Mem_Data mem;
} MN_Emu;

mn_bool mn_emulator_execution_loop(MN_Emu *emu);

#endif
