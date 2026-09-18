#include "MissingNo.h"
#include "emulator.h"
#include "startup.h"

mn_size missingno_cartROM_size(const mn_u8* buffer) {
    return startup_ROM_size(buffer);
}

mn_size missingno_cartRAM_size(const mn_u8* buffer) {
    return startup_RAM_size(buffer);
}

mn_size missingno_core_size(void) {
    return sizeof(MissingNoCore);
}

mn_bool missingno_init(MissingNoCore* ctx, const mn_u8* buffer) {
    for (int i = 0; i < 0x200000; i++)
        ctx->ROM[i] = buffer[i];

    ctx->PC = 0x100;
    return MN_TRUE;
}

MissingNo_Verification missingno_verify(MissingNoCore* ctx) {
    if (!startup_graphics_check(ctx->ROM))
        return MN_NINTENDO_GRAPHIC;

    if (!startup_header_checksum(ctx->ROM))
        return MN_HEADER_CHECKSUM;

    if (!startup_global_checksum(ctx->ROM))
        return MN_GLOBAL_CHECKSUM;

    return MN_OK;
}

void missingno_game_title(MissingNoCore* ctx, char* write_back_buffer) {
    startup_game_title(ctx->ROM, write_back_buffer);
}

mn_bool missingno_tick(MissingNoCore* ctx, const MissingNoInputs* inputs) {

}
