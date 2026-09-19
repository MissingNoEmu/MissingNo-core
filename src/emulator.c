#include "internal/emulator.h"
#include "internal/memory.h"

mn_u16 get_pair(MN_Emu *emu, mn_u8 h, mn_u8 l)
{
  return (emu->r[h] << 8) | emu->r[l];
}

void execute(MN_Emu *emu, mn_u8 opcode)
{
  switch (opcode)
  {
    // 8 bit loads
    {
      // LD nn, n
      case 0x06:
        emu->r[B] = mn_memory_read(emu, emu->PC++);
        emu->cycles += 8;
        break;
      case 0x0E:
        emu->r[C] = mn_memory_read(emu, emu->PC++);
        emu->cycles += 8;
        break;
      case 0x16:
        emu->r[D] = mn_memory_read(emu, emu->PC++);
        emu->cycles += 8;
        break;
      case 0x1E:
        emu->r[E] = mn_memory_read(emu, emu->PC++);
        emu->cycles += 8;
        break;
      case 0x26:
        emu->r[H] = mn_memory_read(emu, emu->PC++);
        emu->cycles += 8;
        break;
      case 0x2E:
        emu->r[L] = mn_memory_read(emu, emu->PC++);
        emu->cycles += 8;
        break;

      // LD r1, r2
      case 0x7F:
        // they js makin random bs
        emu->cycles += 4;
        break;
      case 0x78:
        emu->r[A] = emu->r[B];
        emu->cycles += 4;
        break;
      case 0x79:
        emu->r[A] = emu->r[C];
        emu->cycles += 4;
        break;
      case 0x7A:
        emu->r[A] = emu->r[D];
        emu->cycles += 4;
        break;
      case 0x7B:
        emu->r[A] = emu->r[E];
        emu->cycles += 4;
        break;
      case 0x7C:
        emu->r[A] = emu->r[H];
        emu->cycles += 4;
        break;
      case 0x7D:
        emu->r[A] = emu->r[L];
        emu->cycles += 4;
        break;
      case 0x7E:
        emu->r[A] = mn_memory_read(emu, get_pair(emu, H, L));
        emu->cycles += 8;
        break;
      case 0x40:
        emu->cycles += 4;
        break;
      case 0x41:
        emu->r[B] = emu->r[C];
        emu->cycles += 4;
        break;
      case 0x42:
        emu->r[B] = emu->r[D];
        emu->cycles += 4;
        break;
      case 0x43:
        emu->r[B] = emu->r[E];
        emu->cycles += 4;
        break;
      case 0x44:
        emu->r[B] = emu->r[H];
        emu->cycles += 4;
        break;
      case 0x45:
        emu->r[B] = emu->r[L];
        emu->cycles += 4;
        break;
      case 0x46:
        emu->r[B] = mn_memory_read(emu, get_pair(emu, H, L));
        emu->cycles += 8;
        break;
      case 0x48:
        emu->r[C] = emu->r[B];
        emu->cycles += 4;
        break;
      case 0x49:
        emu->cycles += 4;
        break;
      case 0x4A:
        emu->r[C] = emu->r[D];
        emu->cycles += 4;
        break;
      case 0x4B:
        emu->r[C] = emu->r[E];
        emu->cycles += 4;
        break;
      case 0x4C:
        emu->r[C] = emu->r[H];
        emu->cycles += 4;
        break;
      case 0x4D:
        emu->r[C] = emu->r[L];
        emu->cycles += 4;
        break;
      case 0x4E:
        emu->r[C] = mn_memory_read(emu, get_pair(emu, H, L));
        emu->cycles += 8;
        break;
      case 0x50:
        emu->r[D] = emu->r[B];
        emu->cycles += 4;
        break;
      case 0x51:
        emu->r[D] = emu->r[C];
        emu->cycles += 4;
        break;
      case 0x52:
        emu->cycles += 4;
        break;
      case 0x53:
        emu->r[D] = emu->r[E];
        emu->cycles += 4;
        break;
      case 0x54:
        emu->r[D] = emu->r[H];
        emu->cycles += 4;
        break;
      case 0x55:
        emu->r[D] = emu->r[L];
        emu->cycles += 4;
        break;
      case 0x56:
        emu->r[D] = mn_memory_read(emu, get_pair(emu, H, L));
        emu->cycles += 8;
        break;
      case 0x58:
        emu->r[E] = emu->r[B];
        emu->cycles += 4;
        break;
      case 0x59:
        emu->r[E] = emu->r[C];
        emu->cycles += 4;
        break;
      case 0x5A:
        emu->r[E] = emu->r[D];
        emu->cycles += 4;
        break;
      case 0x5B:
        emu->cycles += 4;
        break;
      case 0x5C:
        emu->r[E] = emu->r[H];
        emu->cycles += 4;
        break;
      case 0x5D:
        emu->r[E] = emu->r[L];
        emu->cycles += 4;
        break;
      case 0x5E:
        emu->r[E] = mn_memory_read(emu, get_pair(emu, H, L));
        emu->cycles += 8;
        break;
      case 0x60:
        emu->r[H] = emu->r[B];
        emu->cycles += 4;
        break;
      case 0x61:
        emu->r[H] = emu->r[C];
        emu->cycles += 4;
        break;
      case 0x62:
        emu->r[H] = emu->r[D];
        emu->cycles += 4;
        break;
      case 0x63:
        emu->r[H] = emu->r[E];
        emu->cycles += 4;
        break;
      case 0x64:
        emu->cycles += 4;
        break;
      case 0x65:
        emu->r[H] = emu->r[L];
        emu->cycles += 4;
        break;
      case 0x66:
        emu->r[H] = mn_memory_read(emu, get_pair(emu, H, L));
        emu->cycles += 8;
        break;
      case 0x68:
        emu->r[L] = emu->r[B];
        emu->cycles += 4;
        break;
      case 0x69:
        emu->r[L] = emu->r[C];
        emu->cycles += 4;
        break;
      case 0x6A:
        emu->r[L] = emu->r[D];
        emu->cycles += 4;
        break;
      case 0x6B:
        emu->r[L] = emu->r[E];
        emu->cycles += 4;
        break;
      case 0x6C:
        emu->r[L] = emu->r[H];
        emu->cycles += 4;
        break;
      case 0x6D:
        emu->cycles += 4;
        break;
      case 0x6E:
        emu->r[L] = mn_memory_read(emu, get_pair(emu, H, L));
        emu->cycles += 8;
        break;
      // write
      case 0x70:
        mn_memory_write(emu, get_pair(emu, H, L), emu->r[B]);
        emu->cycles += 8;
        break;
      case 0x71:
        mn_memory_write(emu, get_pair(emu, H, L), emu->r[C]);
        emu->cycles += 8;
        break;
      case 0x72:
        mn_memory_write(emu, get_pair(emu, H, L), emu->r[D]);
        emu->cycles += 8;
        break;
      case 0x73:
        mn_memory_write(emu, get_pair(emu, H, L), emu->r[E]);
        emu->cycles += 8;
        break;
      case 0x74:
        mn_memory_write(emu, get_pair(emu, H, L), emu->r[H]);
        emu->cycles += 8;
        break;
      case 0x75:
        mn_memory_write(emu, get_pair(emu, H, L), emu->r[L]);
        emu->cycles += 8;
        break;
      case 0x36:
        mn_memory_write(emu, get_pair(emu, H, L), mn_memory_read(emu, emu->PC++));
        emu->cycles += 12;
        break;

      // LD A, n
      case 0x0A:
        emu->r[A] = mn_memory_read(emu, get_pair(emu, B, C));
        emu->cycles += 8;
        break;
      case 0x1A:
        emu->r[A] = mn_memory_read(emu, get_pair(emu, D, E));
        emu->cycles += 8;
        break;
      case 0xFA:
        // DDyansh pls help here
        break;
      case 0x3E:
        emu->r[A] = mn_memory_read(emu, emu->PC++);
        emu->cycles += 8;
        break;

      // LD (nn), A
      case 0x47:
        emu->r[B] = emu->r[A];
        emu->cycles += 4;
        break;
      case 0x4F:
        emu->r[C] = emu->r[A];
        emu->cycles += 4;
        break;
      case 0x57:
        emu->r[D] = emu->r[A];
        emu->cycles += 4;
        break;
      case 0x5F:
        emu->r[E] = emu->r[A];
        emu->cycles += 4;
        break;
      case 0x67:
        emu->r[H] = emu->r[A];
        emu->cycles += 4;
        break;
      case 0x6F:
        emu->r[L] = emu->r[A];
        emu->cycles += 4;
        break;
      case 0x02:
        mn_memory_write(emu, get_pair(emu, B, C), emu->r[A]);
        emu->cycles += 8;
        break;
      case 0x12:
        mn_memory_write(emu, get_pair(emu, D, E), emu->r[A]);
        emu->cycles += 8;
        break;
      case 0x77:
        mn_memory_write(emu, get_pair(emu, H, L), emu->r[A]);
        emu->cycles += 8;
        break;
      case 0xEA:
        // (nn) walo ka aap dekhlo dots
        emu->cycles += 16;
        break;

      // LD A,(C)
      case 0xF2:
        emu->r[A] = mn_memory_read(emu, 0xFF00 + emu->r[C]);
        emu->cycles += 8;
        break;

      // LD (C),A
      case 0xE2:
        mn_memory_write(emu, 0xFF00 + emu->r[C], emu->r[A]);
        emu->cycles += 8;
        break;

      // LDD A,(HL) | LD A,(HLD) | LD A,(HL-)
      case 0x3A:
        // incomplete ALU
        break;
      // LDD (HL),A | LD (HLD),A | LD (HL-),A
      case 0x32:
        // incomplete
        break;
      // LDI A,(HL) | LD A,(HLI) | LD A,(HL+)
      case 0x2A:
        // incomplete
        break;
      // LDI (HL),A | LD (HL),A | LD (HL+),A
      case 0x22:
        // incomplete
        break;

      // LDH (n),A
      case 0xE0:
        mn_memory_write(emu, 0xFF00 + mn_memory_read(emu, emu->PC++), emu->r[A]);
        emu->cycles += 12;
        break;

      // LDH A,(n)
      case 0xF0:
        emu->r[A] = mn_memory_read(emu, 0xFF00 + mn_memory_read(emu, emu->PC++));
        emu->cycles += 12;
        break;
      }
    
    // 16 bit loads
    {

    }
  }
}
