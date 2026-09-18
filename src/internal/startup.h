#ifndef STARTUP_H
#define STARTUP_H

#include "MissingNo/MissingNo.h"

typedef enum {
    DUAL,
    GBC,
    GB
} ROM_Type;

typedef enum {
    ROM_ONLY,
    ROM_MBC1,
    ROM_MBC1_RAM,
    ROM_MBC1_RAM_BATT,
    ROM_MBC2,
    ROM_MBC2_BATTERY,
    ROM_RAM,
    ROM_RAM_BATTERY,
    ROM_MMM01,
    ROM_MMM01_SRAM,
    ROM_MMM01_SRAM_BATT,
    ROM_MBC3_TIMER_BATT,
    ROM_MBC3_TIMER_RAM_BATT,
    ROM_MBC3,
    ROM_MBC3_RAM,
    ROM_MBC5,
    ROM_MBC5_RAM,
    ROM_MBC3_RAM_BATT,
    ROM_MBC5_RAM_BATT,
    ROM_MBC5_RUMBLE,
    ROM_MBC5_RUMBLE_SRAM,
    ROM_MBC5_RUMBLE_SRAM_BATT,
    POCKET_CAMERA,
    BUNDAI_TAMA5,
    HUDSON_HUC_3,
    HUDSON_HUC_1,
    CARTRIDGE_TYPE_ERROR
} Cartridge_Type;

typedef enum {
    MBC1,
    MBC2,
    MBC3,
    MBC5
} MBC_Type;

mn_bool mn_startup_graphics_check(const mn_u8* ROM);
mn_bool mn_startup_header_checksum(const mn_u8* ROM);
mn_bool mn_startup_global_checksum(const mn_u8* ROM);
void mn_startup_game_title(const mn_u8* ROM, char* buffer);
ROM_Type mn_startup_ROM_type(const mn_u8* ROM);
const char* mn_startup_ROM_type_name(ROM_Type t);
Cartridge_Type mn_startup_cartridge_type(const mn_u8* ROM);
const char* mn_startup_cartridge_type_name(Cartridge_Type t);
mn_size mn_startup_ROM_size(const mn_u8* ROM);
mn_size mn_startup_RAM_size(const mn_u8* ROM);

#endif
