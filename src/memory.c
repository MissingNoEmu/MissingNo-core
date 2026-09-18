#include "memory.h"
#include "emulator.h"

mn_u8 read_memory(MN_Emu* emu, mn_u16 loc) {
    if (loc < 0x4000)
        return emu->ROM[loc];
    
    else if (0x4000 <= loc && loc < 0x8000)
        return emu->ROM[emu->ROM_bank + loc - 0x4000];

    else if (0x8000 <= loc && loc < 0xA000)
        return emu->VRAM[loc - 0x8000];

    else if (0xA000 <= loc && loc < 0xC000)
        return emu->cart_mem[emu->RAM_bank + loc - 0xA000];

    else if (0xC000 <= loc && loc < 0xE000)
        return emu->internal_mem[loc - 0xC000];

    else if (0xE000 <= loc && loc < 0xFE00)
        return emu->internal_mem[loc - 0xE000];

    else if (0xFE00 <= loc && loc < 0xFEA0)
        return emu->SAB[loc - 0xFE00];

    else if (0xFEA0 <= loc && loc < 0xFF00)
        return 0xFF;

    else if (0xFF00 <= loc && loc < 0xFF4C)
        return emu->IO[loc - 0xFF00];

    else if (0xFF4C <= loc && loc < 0xFF80)
        return 0xFF;

    else if (0xFF80 <= loc && loc < 0xFFFF)
        return emu->sec_internal_mem[loc - 0xFF80];

    else
        return emu->interr_en;
}

void write_memory(MN_Emu* emu, mn_u16 loc, mn_u8 val) {
    if (loc < 0x8000)
        return;
    if (0x8000 <= loc && loc < 0xA000)
        emu->VRAM[loc - 0x8000] = val;
    else if (0xA000 <= loc && loc < 0xC000)
        emu->cart_mem[emu->RAM_bank + loc - 0xA000] = val;
    else if (0xC000 <= loc && loc < 0xE000)
        emu->internal_mem[loc - 0xC000] = val;
    else if (0xE000 <= loc && loc < 0xFE00)
        emu->internal_mem[loc - 0xE000] = val;
    else if (0xFE00 <= loc && loc < 0xFEA0)
        emu->SAB[loc - 0xFE00] = val;
    else if (0xFF00 <= loc && loc < 0xFF4C)
        emu->IO[loc - 0xFF00] = val;
    else if (0xFF80 <= loc && loc < 0xFFFF)
        emu->sec_internal_mem[loc - 0xFF80] = val;
    else if (loc == 0xFFFF)
        emu->interr_en = val;
}