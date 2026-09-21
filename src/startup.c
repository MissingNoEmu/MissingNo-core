#include "internal/startup.h"

mn_bool mn_startup_graphics_check(const mn_u8* ROM) {
    mn_u8 sig[] = {
        0xCE,
        0xED,
        0x66,
        0x66,
        0xCC,
        0x0D,
        0x00,
        0x0B,
        0x03,
        0x73,
        0x00,
        0x83,
        0x00,
        0x0C,
        0x00,
        0x0D,
        0x00,
        0x08,
        0x11,
        0x1F,
        0x88, 
        0x89,
        0x00,
        0x0E,
        0xDC,
        0xCC,
        0x6E,
        0xE6,
        0xDD,
        0xDD,
        0xD9,
        0x99,
        0xBB,
        0xBB,
        0x67,
        0x63,
        0x6E,
        0x0E,
        0xEC,
        0xCC,
        0xDD,
        0xDC,
        0x99,
        0x9F,
        0xBB,
        0xB9,
        0x33,
        0x3E
    };

    mn_bool ok = MN_TRUE;

    for (int i = 0x104; i <= 0x133; i++)
        if (ROM[i] != sig[i - 0x104])
            ok = MN_FALSE;

    return ok;
}

mn_bool mn_startup_header_checksum(const mn_u8* ROM) {
    mn_u8 x = 0;
    for (int i = 0x0134; i <= 0x014C; i++)
        x = x - ROM[i] - 1;

    return x == ROM[0x14D];
}

mn_bool mn_startup_global_checksum(const mn_u8* ROM) {
    mn_u16 sum = 0;

    for (int i = 0; i <= 0x14D; i++)
        sum += ROM[i];

    for (int i = 0x150; i <= 2 * 1024 * 1024; i++)
        sum += ROM[i];

    mn_u16 tmp = ROM[0x14E];
    tmp <<= 8;
    tmp += ROM[0x14F];

    return sum == tmp;
}

void mn_startup_game_title(const mn_u8* ROM, char* buffer) {
    for (int i = 0x134; i <= 0x142; i++)
        buffer[i - 0x134] = ROM[i];

    buffer[15] = '\0';
}

MN_ROM_Type mn_startup_ROM_type(const mn_u8* ROM) {
    switch (ROM[0x143]) {
        case 0x80:
            return MN_ROM_Type_Dual;
        case 0xC0:
            return MN_ROM_Type_GBC;
        default:
            return MN_ROM_Type_GB;
    }
}

const char* mn_startup_ROM_type_name(MN_ROM_Type t) {
    static const char* names[] = {
        "MN_ROM_Type_Dual mode Game Boy / Game Boy Color",
        "Game Boy Color only",
        "Original Game Boy"
    };

    return names[t];
}

MN_Cartridge_Type mn_startup_cartridge_type(const mn_u8* ROM) {
    switch (ROM[0x147]) {
        case 0x0:
            return ROM_ONLY;
        case 0x1:
            return ROM_MBC1;
        case 0x2:
            return ROM_MBC1_RAM;
        case 0x3:
            return ROM_MBC1_RAM_BATT;
        case 0x5:
            return ROM_MBC2;
        case 0x6:
            return ROM_MBC2_BATTERY;
        case 0x8:
            return ROM_RAM;
        case 0x9:
            return ROM_RAM_BATTERY;
        case 0xB:
            return ROM_MMM01;
        case 0xC:
            return ROM_MMM01_SRAM;
        case 0xD:
            return ROM_MMM01_SRAM_BATT;
        case 0xF:
            return ROM_MBC3_TIMER_BATT;
        case 0x10:
            return ROM_MBC3_TIMER_RAM_BATT;
        case 0x11:
            return ROM_MBC3;
        case 0x12:
            return ROM_MBC3_RAM;
        case 0x13:
            return ROM_MBC3_RAM_BATT;
        case 0x19:
            return ROM_MBC5;
        case 0x1A:
            return ROM_MBC5_RAM;
        case 0x1B:
            return ROM_MBC5_RAM_BATT;
        case 0x1C:
            return ROM_MBC5_RUMBLE;
        case 0x1D:
            return ROM_MBC5_RUMBLE_SRAM;
        case 0x1E:
            return ROM_MBC5_RUMBLE_SRAM_BATT;
        case 0x1F:
            return POCKET_CAMERA;
        case 0xFD:
            return BUNDAI_TAMA5;
        case 0xFE:
            return HUDSON_HUC_3;
        case 0xFF:
            return HUDSON_HUC_1;
        default:
            return CARTRIDGE_TYPE_ERROR;
    }
}

const char* mn_startup_cartridge_type_name(MN_Cartridge_Type t) {
    static const char* names[] = {
        "ROM_ONLY",
        "ROM_MBC1",
        "ROM_MBC1_RAM",
        "ROM_MBC1_RAM_BATT",
        "ROM_MBC2",
        "ROM_MBC2_BATTERY",
        "ROM_RAM",
        "ROM_RAM_BATTERY",
        "ROM_MMM01",
        "ROM_MMM01_SRAM",
        "ROM_MMM01_SRAM_BATT",
        "ROM_MBC3_TIMER_BATT",
        "ROM_MBC3_TIMER_RAM_BATT",
        "ROM_MBC3",
        "ROM_MBC3_RAM",
        "ROM_MBC5",
        "ROM_MBC5_RAM",
        "ROM_MBC3_RAM_BATT",
        "ROM_MBC5_RAM_BATT",
        "ROM_MBC5_RUMBLE",
        "ROM_MBC5_RUMBLE_SRAM",
        "ROM_MBC5_RUMBLE_SRAM_BATT",
        "POCKET_CAMERA",
        "BUNDAI_TAMA5",
        "HUDSON_HUC_3",
        "HUDSON_HUC_1",
        "CARTRIDGE_TYPE_ERROR"
    };

    return names[t];
}

mn_size mn_startup_ROM_size(const mn_u8* ROM) {
    switch (ROM[0x148]) {
        case 0x00: return 32 * 1024;
        case 0x01: return 64 * 1024;
        case 0x02: return 128 * 1024;
        case 0x03: return 256 * 1024;
        case 0x04: return 512 * 1024;
        case 0x05: return 1024 * 1024;
        case 0x06: return 2 * 1024 * 1024;
        case 0x07: return 4 * 1024 * 1024;
        case 0x08: return 8 * 1024 * 1024;

        case 0x52: return (mn_size)(9 * 1024 * 1024) / 8;
        case 0x53: return (mn_size)(10 * 1024 * 1024) / 8;
        case 0x54: return (mn_size)(12 * 1024 * 1024) / 8;

        default: return 0;
    }
}

mn_size mn_startup_RAM_size(const mn_u8* ROM) {
    switch (ROM[0x149]) {
        case 0x00: return 0;
        case 0x01: return 2 * 1024;
        case 0x02: return 8 * 1024;
        case 0x03: return 32 * 1024;
        case 0x04: return 128 * 1024;
        case 0x05: return 64 * 1024;

        default: return 0;
    }
}
