#include "internal/ppu.h"
#include "internal/emulator.h"

void mn_ppu_init(MN_PPU *ppu)
{
  for (mn_size i = 0; i < MN_WIDTH * MN_HEIGHT; i++)
  {
    ppu->framebuffer[i] = 0x00;
  }
  for (mn_u8 i = 0; i < 10; i++)
  {
    ppu->sprites[i].attributes = 0;
    ppu->sprites[i].tile = 0;
    ppu->sprites[i].x = 0;
    ppu->sprites[i].y = 0;
  }
  
  ppu->mode = MN_HBlank;
  ppu->mode_clock = 0;
  ppu->sprite_count = 0;
}

static inline mn_bool overlaps(MN_Sprite *sprite, mn_u8 LY, mn_bool tall){
  mn_i16 y = (mn_i16)sprite->y - 16;
  mn_i16 height = tall ? 16 : 8;
  return LY >= y && LY < y + height;
}

static inline MN_Sprite oam_read_sprite(MN_Emu *emu, mn_u8 i){
  mn_u16 addr = 0xFE00 + i * 4;
  return (MN_Sprite){mn_memory_read(emu, addr), mn_memory_read(emu, addr + 1), mn_memory_read(emu, addr+2), mn_memory_read(emu, addr + 3)};
}

static void oam_scan(MN_Emu *emu, MN_PPU *ppu){
  ppu->sprite_count = 0;
  for (mn_u8 i = 0; i < 40; i++)
  {
    if(ppu->sprite_count == 10)
      break;
    MN_Sprite sprite = oam_read_sprite(emu, i);
    if(overlaps(&sprite, ppu->LY, (mn_bool)(ppu->LCDC & 0x04))){
      ppu->sprites[ppu->sprite_count++] = sprite;
    }
  }
}

static void draw_scanline(MN_Emu *emu, MN_PPU *ppu){
  mn_u8 color = 0;
  mn_u16 tile_map = (ppu->LCDC & 0x08) ? 0x9C00 : 0x9800;
  for (mn_size x = 0; x < MN_WIDTH; x+=8)
  {
    mn_u8 tile_id = mn_memory_read(emu, tile_map + ppu->LY / 8 * 32 + x / 8);

    mn_u16 tile_addr;

    if(ppu->LCDC & 0x10)
      tile_addr = 0x8000 + tile_id * 16;
    else
      tile_addr = 0x9000 + (mn_i8)tile_id * 16;
    mn_u8 row = ppu->LY % 8;
    mn_u8 low = mn_memory_read(emu, tile_addr + 2 * row);
    mn_u8 high = mn_memory_read(emu, tile_addr + 2 * row + 1);

    for (mn_u8 i = 0; i < 8; i++)
    {
      mn_u8 bit = 7 - i;
      mn_u8 color = ((high >> bit) & 1) << 1 | ((low >> bit) & 1);
      ppu->framebuffer[ppu->LY * MN_WIDTH + x + i] = color;
    }
  }
  if(ppu->LCDC & 0b00000100){
    for (mn_u8 i = 0; i < ppu->sprite_count; i++)
    {
      mn_u8 row = ppu->LY - (mn_i16)ppu->sprites[i].y + 16;
      if (ppu->sprites[i].attributes & 0b01000000)
        row = 15 - row;
      mn_u8 tile = ppu->sprites[i].tile & 0xFE;
      if (row >= 8)
        tile++;
      mn_u8 tile_row = row & 7;
      mn_u16 tile_addr = 0x8000 + tile * 16;
      mn_u8 low = mn_memory_read(emu, tile_addr + 2 * tile_row);
      mn_u8 high = mn_memory_read(emu, tile_addr + 2 * tile_row + 1);
    
      for (mn_u8 s = 0; s < 8; s++)
      {
        mn_i16 px = (mn_i16)ppu->sprites[i].x - 8 + s;
        if (px < 0 || px >= MN_WIDTH)
          continue;
        mn_u8 bit = 7 - s;
        if (ppu->sprites[i].attributes & 0x20)
          bit = s;
        mn_u8 color = ((high >> bit) & 1) << 1 | ((low >> bit) & 1);
        if (color == 0)
          continue;
        if ((ppu->sprites[i].attributes & 0x80) && color != 0)
        {
          mn_u8 bg_color =
            ppu->framebuffer[ppu->LY * MN_WIDTH + px];

          if (bg_color != 0)
            continue;
        }
        ppu->framebuffer[ppu->LY * MN_WIDTH + px] = color;
      }
    }
  }
  else
    for (mn_u8 i = 0; i < ppu->sprite_count; i++)
    {
      mn_u8 row = ppu->LY - (mn_i16)ppu->sprites[i].y + 16;
      if (ppu->sprites[i].attributes & 0b01000000)
        row = 7 - row;
      mn_u16 tile_addr = 0x8000 + ppu->sprites[i].tile * 16;
      mn_u8 low = mn_memory_read(emu, tile_addr + 2 * row);
      mn_u8 high = mn_memory_read(emu, tile_addr + 2 * row + 1);

      for (mn_u8 s = 0; s < 8; s++)
      {
        mn_i16 px = (mn_i16)ppu->sprites[i].x - 8 + s;
        if (px < 0 || px >= MN_WIDTH)
          continue;
        mn_u8 bit = 7 - s;
        if (ppu->sprites[i].attributes & 0x20)
          bit = s;
        mn_u8 color = ((high >> bit) & 1) << 1 | ((low >> bit) & 1);
        if (color == 0)
          continue;
        if ((ppu->sprites[i].attributes & 0x80) && color != 0)
        {
          mn_u8 bg_color =
            ppu->framebuffer[ppu->LY * MN_WIDTH + px];
            
          if (bg_color != 0)
            continue;
        }
        ppu->framebuffer[ppu->LY * MN_WIDTH + px] = color;
      }
    }
}

// NOT CYCLE ACCURATE
void mn_ppu_tick(MN_Emu *emu){
  MN_PPU *ppu = &emu->ppu;
  ppu->mode_clock++;
  switch (ppu->mode)
  {
  case MN_OAM_scan:
    if(ppu->mode_clock >= MN_OAM_CYCLES){
      oam_scan(emu, ppu);
      ppu->mode = MN_HDraw;
      ppu->mode_clock = 0;
    }
    break;
  case MN_HDraw:
    if(ppu->mode_clock >= MN_MIN_DRAW_CYCLES){
      draw_scanline(emu, ppu);
      ppu->mode = MN_HBlank;
      ppu->mode_clock = 0;
    }
    break;
  case MN_HBlank:
    ppu->LY++;
    if (ppu->LY == MN_HEIGHT) {
      ppu->mode = MN_VBlank;
    }
    else
      ppu->mode = MN_OAM_scan;
    ppu->mode_clock = 0;
    break;
  case MN_VBlank:
      if (ppu->mode_clock >= MN_SCANLINE_CYCLES) {
        ppu->mode_clock = 0;
        ppu->LY++;

        if (ppu->LY > 153) {
            ppu->LY = 0;
            ppu->mode = MN_OAM_scan;
        }
    }
    break;
  default:
    break;
  }
}