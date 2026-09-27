#ifndef PPU_H
#define PPU_H

#include "MissingNo/MissingNo.h"
#include "emulator.h"
#include "memory.h"

#define MN_HEIGHT 144
#define MN_WIDTH 160
#define MN_OAM_CYCLES 80
typedef enum
{
  MN_LCDC = 0xFF40,
  MN_STAT = 0xFF41,
  MN_SCY = 0xFF42,
  MN_SCX = 0xFF43,
  MN_LY = 0xFF44,
  MN_LYC = 0xFF45,
  MN_BGP = 0xFF47,
  MN_OBP0 = 0xFF48,
  MN_OBP1 = 0xFF49,
  MN_WY = 0xFF4A,
  MN_WX = 0xFF4B
} MN_GB_PPU_Regs;

typedef enum
{
  MN_VBK = 0xFF4F,
  MN_BCPS = 0xFF68,
  MN_BCPD = 0xFF69,
  MN_OCPS = 0xFF6A,
  MN_OCPD = 0xFF6B
} MN_GBC_PPU_Regs;

typedef enum
{
  MN_HBlank,
  MN_VBlank,
  MN_OAM_scan,
  MN_HDraw
} MN_PPU_mode;

typedef struct
{
    mn_u8 y;
    mn_u8 x;
    mn_u8 tile;
    mn_u8 attributes;
} MN_Sprite;

typedef struct
{
  mn_u8 LCDC;
  mn_u8 STAT;
  mn_u8 SCY;
  mn_u8 SCX;
  mn_u8 LY;
  mn_u8 LYC;
  mn_u8 BGP;
  mn_u8 OBP0;
  mn_u8 OBP1;
  mn_u8 WY;
  mn_u8 WX;

  MN_PPU_mode mode;
  mn_u16 mode_clock;

  MN_Sprite sprites[10];
  mn_u8 sprite_count;
  mn_u8 framebuffer[MN_HEIGHT * MN_WIDTH];
} MN_PPU;

void mn_ppu_tick(MN_Emu *emu);
#endif