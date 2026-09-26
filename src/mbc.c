#include "internal/mbc.h"
#include "internal/emulator.h"

static void mn_mbc1_write_rombank_(MN_Emu* emu, mn_u16 loc, mn_u8 val) {
    MN_MBC1_Data* data = &emu->mem.mbc_data.MBC1;

    mn_u8 bank = (data->hi << 5) | (data->lo ? data->lo : 1);
    emu->mem.ROM_bank = bank * 0x4000;
}

static void mn_mbc1_write_rambank_(MN_Emu* emu, mn_u16 loc, mn_u8 val) {
    MN_MBC1_Data* data = &emu->mem.mbc_data.MBC1;
    
    if (data->ram_enabled)
        emu->mem.RAM_bank = data->hi * 0x2000;
}

static void mn_mbc1_write_(MN_Emu* emu, mn_u16 loc, mn_u8 val) {
    MN_MBC1_Data* data = &emu->mem.mbc_data.MBC1;

    if (0x0 <= loc && loc < 0x2000)
        data->ram_enabled = ((val & 0x0F) == 0x0A);
    else if (0x2000 <= loc && loc < 0x4000) {
        data->lo = val & 0x1F;
        mn_mbc1_write_rombank_(emu, loc, val);
    }
    else if (0x4000 <= loc && loc < 0x6000) {
        data->hi = val & 0x03;

        if (data->mode == MN_MBC1_Mode_16_8)
            mn_mbc1_write_rombank_(emu, loc, val);
        else
            mn_mbc1_write_rambank_(emu, loc, val);
    }
    else if (0x6000 <= loc && loc < 0x8000)
        data->mode = val & ((mn_u8)1);
}

void mn_mbc_write(MN_Emu* emu, mn_u16 loc, mn_u8 val) {
    switch (emu->mem.mbc_type) {
    case MN_MBC1:
        mn_mbc1_write_(emu, loc, val);
    case MN_MBC2:
        break;
    case MN_MBC3:
        break;
    case MN_MBC5:
        break;
    case MN_MBCNONE:
        break;
    }
}
