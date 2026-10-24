#include "internal/mbc.h"
#include "internal/emulator.h"

static void mn_mbc1_write_rombank_(MN_Emu* emu, mn_u16 loc, mn_u8 val) {
    MN_MBC1_Data* data = &emu->mem.mbc_data.MBC1;

    mn_u8 bank = (data->hi << 5) | data->lo;
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
        if (data->lo == 0)
            data->lo = 1;
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

static void mn_mbc2_write_(MN_Emu* emu, mn_u16 loc, mn_u8 val) {
    MN_MBC2_Data* data = &emu->mem.mbc_data.MBC2;
    mn_u8 A8 = (mn_u8)((loc & 0x0100) >> 8);
    val &= 0x0F;

    if (0x0 <= loc && loc < 0x4000) {
        if (A8) {
            if (!val)
                val++;

            emu->mem.ROM_bank = val * 0x4000;
        }
        else {
            data->ram_enabled = (val == 0x0A);
        }
    }
}

static void mn_mbc3_write_(MN_Emu* emu, mn_u16 loc, mn_u8 val) {
    MN_MBC3_Data* data = &emu->mem.mbc_data.MBC3;

    if (loc < 0x2000)
        data->ram_rtc_enabled = ((val & 0x0F) == 0x0A);
    else if (loc < 0x4000) {
        mn_u8 bank = val & 0x7F;
        if (bank == 0)
            bank = 1;

        data->rom_bank = bank;
        emu->mem.ROM_bank = bank * 0x4000;
    }
    else if (loc < 0x6000) {
        data->ram_rtc_select = val;

        if (data->ram_rtc_enabled && val <= 0x03)
            emu->mem.RAM_bank = val * 0x2000;
    }
    else if (loc < 0x8000) {
        if (data->latch_state == 0x00 && val == 0x01) {
            for (int i = 0; i < 5; ++i)
                data->latched_regs[i] = data->rtc_regs[i];
        }
        data->latch_state = val;
    }
}

static void mn_mbc5_write_rombank_(MN_Emu* emu) {
    MN_MBC5_Data* data = &emu->mem.mbc_data.MBC5;

    mn_u16 bank = ((mn_u16)data->rom_bank_hi << 8) | data->rom_bank_lo;
    emu->mem.ROM_bank = bank * 0x4000;
}

static void mn_mbc5_write_(MN_Emu* emu, mn_u16 loc, mn_u8 val) {
    MN_MBC5_Data* data = &emu->mem.mbc_data.MBC5;

    if (loc < 0x2000)
        data->ram_enabled = ((val & 0x0F) == 0x0A);
    else if (loc < 0x3000) {
        data->rom_bank_lo = val;
        mn_mbc5_write_rombank_(emu);
    }
    else if (loc < 0x4000) {
        data->rom_bank_hi = val & 0x01;
        mn_mbc5_write_rombank_(emu);
    }
    else if (loc < 0x6000) {
        data->ram_bank = val & 0x0F;
        if (data->ram_enabled)
            emu->mem.RAM_bank = data->ram_bank * 0x2000;
    }
}

void mn_mbc_write(MN_Emu* emu, mn_u16 loc, mn_u8 val) {
    switch (emu->mem.mbc_type) {
    case MN_MBC1:
        mn_mbc1_write_(emu, loc, val);
        break;
    case MN_MBC2:
        mn_mbc2_write_(emu, loc, val);
        break;
    case MN_MBC3:
        mn_mbc3_write_(emu, loc, val);
        break;
    case MN_MBC5:
        mn_mbc5_write_(emu, loc, val);
        break;
    case MN_MBCNONE:
        break;
    }
}
