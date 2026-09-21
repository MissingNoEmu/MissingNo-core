#include "internal/memory.h"
#include "internal/emulator.h"
#include "internal/mbc.h"

mn_u8 mn_memory_read(MN_Emu* emu, mn_u16 loc) {
    MN_Mem_Data* mem = &emu->mem;

    if (loc < 0x4000)
        return emu->ROM[loc];
    
    else if (loc < 0xA000)
        return mem->VRAM[loc - 0x8000];

    else if (loc < 0xC000)
        return mem->cart_mem[mem->RAM_bank + loc - 0xA000];

    else if (loc < 0xE000)
        return mem->internal_mem[loc - 0xC000];

    else if (loc < 0xFE00)
        return mem->internal_mem[loc - 0xE000];

    else if (loc < 0xFEA0)
        return mem->SAB[loc - 0xFE00];

    else if (loc < 0xFF00)
        return 0xFF;

    else if (loc < 0xFF4C)
        return mem->IO[loc - 0xFF00];

    else if (loc < 0xFF80)
        return 0xFF;

    else if (loc < 0xFFFF)
        return mem->sec_internal_mem[loc - 0xFF80];

    else
        return mem->interr_en;
}

void mn_memory_write(MN_Emu* emu, mn_u16 loc, mn_u8 val) {
    MN_Mem_Data* mem = &emu->mem;

    if (loc < 0x8000)
        mn_mbc_write(emu, loc, val);
    
    else if (loc < 0x8000)
        emu->ROM[mem->ROM_bank + loc - 0x4000] = val;

    else if (loc < 0xA000)
        mem->VRAM[loc - 0x8000] = val;

    else if (loc < 0xC000)
        mem->cart_mem[mem->RAM_bank + loc - 0xA000] = val;

    else if (loc < 0xE000)
        mem->internal_mem[loc - 0xC000] = val;

    else if (loc < 0xFE00)
        mem->internal_mem[loc - 0xE000] = val;

    else if (loc < 0xFEA0)
        mem->SAB[loc - 0xFE00] = val;

    else if (loc < 0xFF00)
        return;

    else if (loc < 0xFF4C)
        mem->IO[loc - 0xFF00] = val;

    else if (loc < 0xFF80)
        return;

    else if (loc < 0xFFFF)
        mem->sec_internal_mem[loc - 0xFF80] = val;

    else
        mem->interr_en = val;
}
