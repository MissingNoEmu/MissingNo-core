#ifndef MEMORY_H
#define MEMORY_H

#include "MissingNo/MissingNo.h"
#include "emulator.h"

mn_u8 mn_memory_read(MN_Emu* emu, mn_u16 loc);
void mn_memory_write(MN_Emu* emu, mn_u16 loc, mn_u8 val);

#endif
