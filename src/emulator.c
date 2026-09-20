#include "internal/emulator.h"
#include "internal/memory.h"
#include "internal/opcodes.h"

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
      MN_LD_nn_n(B);
      break;
    case 0x0E:
      MN_LD_nn_n(C);
      break;
    case 0x16:
      MN_LD_nn_n(D);
      break;
    case 0x1E:
      MN_LD_nn_n(E);
      break;
    case 0x26:
      MN_LD_nn_n(H);
      break;
    case 0x2E:
      MN_LD_nn_n(L);
      break;

    // LD r1, r2
    case 0x7F:
      // they js makin random bs
      MN_LD_r1_r2(A, A);
      break;
    case 0x78:
      MN_LD_r1_r2(A, B);
      break;
    case 0x79:
      MN_LD_r1_r2(A, C);
      break;
    case 0x7A:
      MN_LD_r1_r2(A, D);
      break;
    case 0x7B:
      MN_LD_r1_r2(A, E);
      break;
    case 0x7C:
      MN_LD_r1_r2(A, H);
      break;
    case 0x7D:
      MN_LD_r1_r2(A, L);
      break;
    case 0x7E:
      MN_LD_r_HL(A);
      break;
    case 0x40:
      MN_LD_r1_r2(B, B);
      break;
    case 0x41:
      MN_LD_r1_r2(B, C);
      break;
    case 0x42:
      MN_LD_r1_r2(B, D);
      break;
    case 0x43:
      MN_LD_r1_r2(B, E);
      break;
    case 0x44:
      MN_LD_r1_r2(B, H);
      break;
    case 0x45:
      MN_LD_r1_r2(B, L);
      break;
    case 0x46:
      MN_LD_r_HL(B);
      break;
    case 0x48:
      MN_LD_r1_r2(C, B);
      break;
    case 0x49:
      MN_LD_r1_r2(C, C);
      break;
    case 0x4A:
      MN_LD_r1_r2(C, D);
      break;
    case 0x4B:
      MN_LD_r1_r2(C, E);
      break;
    case 0x4C:
      MN_LD_r1_r2(C, H);
      break;
    case 0x4D:
      MN_LD_r1_r2(C, L);
      break;
    case 0x4E:
      MN_LD_r_HL(C);
      break;
    case 0x50:
      MN_LD_r1_r2(D, B);
      break;
    case 0x51:
      MN_LD_r1_r2(D, C);
      break;
    case 0x52:
      MN_LD_r1_r2(D, D);
      break;
    case 0x53:
      MN_LD_r1_r2(D, E);
      break;
    case 0x54:
      MN_LD_r1_r2(D, H);
      break;
    case 0x55:
      MN_LD_r1_r2(D, L);
      break;
    case 0x56:
      MN_LD_r_HL(D);
      break;
    case 0x58:
      MN_LD_r1_r2(E, B);
      break;
    case 0x59:
      MN_LD_r1_r2(E, C);
      break;
    case 0x5A:
      MN_LD_r1_r2(E, D);
      break;
    case 0x5B:
      MN_LD_r1_r2(E, E);
      break;
    case 0x5C:
      MN_LD_r1_r2(E, H);
      break;
    case 0x5D:
      MN_LD_r1_r2(E, L);
      break;
    case 0x5E:
      MN_LD_r_HL(E);
      break;
    case 0x60:
      MN_LD_r1_r2(H, B);
      break;
    case 0x61:
      MN_LD_r1_r2(H, C);
      break;
    case 0x62:
      MN_LD_r1_r2(H, D);
      break;
    case 0x63:
      MN_LD_r1_r2(H, E);
      break;
    case 0x64:
      MN_LD_r1_r2(H, H);
      break;
    case 0x65:
      MN_LD_r1_r2(H, L);
      break;
    case 0x66:
      MN_LD_r_HL(H);
      break;
    case 0x68:
      MN_LD_r1_r2(L, B);
      break;
    case 0x69:
      MN_LD_r1_r2(L, C);
      break;
    case 0x6A:
      MN_LD_r1_r2(L, D);
      break;
    case 0x6B:
      MN_LD_r1_r2(L, E);
      break;
    case 0x6C:
      MN_LD_r1_r2(L, H);
      break;
    case 0x6D:
      MN_LD_r1_r2(L, L);
      break;
    case 0x6E:
      MN_LD_r_HL(L);
      break;
    // write
    case 0x70:
      MN_LD_HL_r(B);
      break;
    case 0x71:
      MN_LD_HL_r(C);
      break;
    case 0x72:
      MN_LD_HL_r(D);
      break;
    case 0x73:
      MN_LD_HL_r(E);
      break;
    case 0x74:
      MN_LD_HL_r(H);
      break;
    case 0x75:
      MN_LD_HL_r(L);
      break;
    case 0x36:
      mn_memory_write(emu, get_pair(emu, H, L), mn_memory_read(emu, emu->PC++));
      emu->cycles += 12;
      break;

    // LD A, n
    case 0x0A:
      MN_LD_r_BC(A);
      break;
    case 0x1A:
      MN_LD_r_DE(A);
      break;
    case 0xFA:
      // DDyansh pls help here
      break;

    // LD (nn), A
    case 0x47:
      MN_LD_r1_r2(B, A);
      break;
    case 0x4F:
      MN_LD_r1_r2(C, A);
      break;
    case 0x57:
      MN_LD_r1_r2(D, A);
      break;
    case 0x5F:
      MN_LD_r1_r2(E, A);
      break;
    case 0x67:
      MN_LD_r1_r2(H, A);
      break;
    case 0x6F:
      MN_LD_r1_r2(L, A);
      break;
    case 0x02:
      MN_LD_BC_r(A);
      break;
    case 0x12:
      MN_LD_DE_r(A);
      break;
    case 0x77:
      MN_LD_HL_r(A);
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

    // 8 bit ALU
    {
    // ADD n
    case 0x87:
      MN_ADD_r(A);
      break;
    case 0x80:
      MN_ADD_r(B);
      break;
    case 0x81:
      MN_ADD_r(C);
      break;
    case 0x82:
      MN_ADD_r(D);
      break;
    case 0x83:
      MN_ADD_r(E);
      break;
    case 0x84:
      MN_ADD_r(H);
      break;
    case 0x85:
      MN_ADD_r(L);
      break;
    case 0x86:
      MN_ADD_HL();
      break;
    case 0xC6:
      MN_ADD_n();
      break;
    // ADC A, n
    case 0x8F:
      MN_ADC_r(A);
      break;
    case 0x88:
      MN_ADC_r(B);
      break;
    case 0x89:
      MN_ADC_r(C);
      break;
    case 0x8A:
      MN_ADC_r(D);
      break;
    case 0x8B:
      MN_ADC_r(E);
      break;
    case 0x8C:
      MN_ADC_r(H);
      break;
    case 0x8D:
      MN_ADC_r(L);
      break;
    case 0x8E:
      MN_ADC_HL();
      break;
    case 0xCE:
      MN_ADC_n();
      break;

    // SUB n
    case 0x97:
      MN_SUB_r(A);
      break;
    case 0x90:
      MN_SUB_r(B);
      break;
    case 0x91:
      MN_SUB_r(C);
      break;
    case 0x92:
      MN_SUB_r(D);
      break;
    case 0x93:
      MN_SUB_r(E);
      break;
    case 0x94:
      MN_SUB_r(H);
      break;
    case 0x95:
      MN_SUB_r(L);
      break;
    case 0x96:
      MN_SUB_HL();
      break;
    case 0xD6:
      MN_SUB_n();
      break;

    // SBC
    case 0x9F:
      MN_SBC_r(A);
      break;
    case 0x98:
      MN_SBC_r(B);
      break;
    case 0x9A:
      MN_SBC_r(D);
      break;
    case 0x9B:
      MN_SBC_r(E);
      break;
    case 0x9C:
      MN_SBC_r(H);
      break;
    case 0x9D:
      MN_SBC_r(L);
      break;
    case 0x9E:
      MN_SBC_HL();
      break;
    // WHAT EH FUCK EVEN IS SBC # ?? opcode bruh sonion ring

    // AND n
    case 0xA7:
      MN_AND_r(A);
      break;
    case 0xA0:
      MN_AND_r(B);
      break;
    case 0xA1:
      MN_AND_r(C);
      break;
    case 0xA2:
      MN_AND_r(D);
      break;
    case 0xA3:
      MN_AND_r(E);
      break;
    case 0xA4:
      MN_AND_r(H);
      break;
    case 0xA5:
      MN_AND_r(L);
      break;
    case 0xA6:
      MN_AND_HL();
      break;
    case 0xE6:
      MN_AND_n();
      break;

    // OR n
    case 0xB7:
      MN_OR_r(A);
      break;
    case 0xB0:
      MN_OR_r(B);
      break;
    case 0xB1:
      MN_OR_r(C);
      break;
    case 0xB2:
      MN_OR_r(D);
      break;
    case 0xB3:
      MN_OR_r(E);
      break;
    case 0xB4:
      MN_OR_r(H);
      break;
    case 0xB5:
      MN_OR_r(L);
      break;
    case 0xB6:
      MN_OR_HL();
      break;
    case 0xF6:
      MN_OR_n();
      break;

    // XOR n
    case 0xAF:
      MN_XOR_r(A);
      break;
    case 0xA8:
      MN_XOR_r(B);
      break;
    case 0xA9:
      MN_XOR_r(C);
      break;
    case 0xAA:
      MN_XOR_r(D);
      break;
    case 0xAB:
      MN_XOR_r(E);
      break;
    case 0xAC:
      MN_XOR_r(H);
      break;
    case 0xAD:
      MN_XOR_r(L);
      break;
    case 0xAE:
      MN_XOR_HL();
      break;
    case 0xEE:
      MN_XOR_n();
      break;

    // CP n
    }

    // 16 bit ALU
    {
    }
  }
}