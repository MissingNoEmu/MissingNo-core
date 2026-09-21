#ifndef EMULATOR_H
#define EMULATOR_H

#include "MissingNo/MissingNo.h"
#include "memory.h"

typedef enum {
    A, F,
    B, C,
    D, E,
    H, L
} Regs;

typedef struct MN_Emu {
    mn_u8 ROM[0x200000];
    mn_u8 r[8];
    mn_u16 SP;
    mn_u16 PC;
    mn_size cycles;

    MN_Mem_Data mem;
} MN_Emu;


#endif
