#ifndef MEMORY_H
#define MEMORY_H

#include "commons.h"
#include "emulator.h"

mn_u8 read_memory(MN_Emu* emu, mn_u16 loc);
void write_memory(MN_Emu* emu, mn_u16 loc, mn_u8 val);

#endif
