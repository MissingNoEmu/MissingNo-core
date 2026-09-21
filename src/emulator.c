#include "internal/emulator.h"
#include "internal/memory.h"
#include "internal/opcodes.h"

void mn_emulator_execute(MN_Emu *emu, mn_u8 opcode)
{
  switch (opcode)
  {
    // 8 bit loads
    {
    // LD nn, n
    case 0x06:
      MN_LD_nn_n(MN_Regs_B);
      break;
    case 0x0E:
      MN_LD_nn_n(MN_Regs_C);
      break;
    case 0x16:
      MN_LD_nn_n(MN_Regs_D);
      break;
    case 0x1E:
      MN_LD_nn_n(MN_Regs_E);
      break;
    case 0x26:
      MN_LD_nn_n(MN_Regs_H);
      break;
    case 0x2E:
      MN_LD_nn_n(MN_Regs_L);
      break;

    // LD r1, r2
    case 0x7F:
      // they js makin random bs
      MN_LD_r1_r2(MN_Regs_A, MN_Regs_A);
      break;
    case 0x78:
      MN_LD_r1_r2(MN_Regs_A, MN_Regs_B);
      break;
    case 0x79:
      MN_LD_r1_r2(MN_Regs_A, MN_Regs_C);
      break;
    case 0x7A:
      MN_LD_r1_r2(MN_Regs_A, MN_Regs_D);
      break;
    case 0x7B:
      MN_LD_r1_r2(MN_Regs_A, MN_Regs_E);
      break;
    case 0x7C:
      MN_LD_r1_r2(MN_Regs_A, MN_Regs_H);
      break;
    case 0x7D:
      MN_LD_r1_r2(MN_Regs_A, MN_Regs_L);
      break;
    case 0x7E:
      MN_LD_r_HL(MN_Regs_A);
      break;
    case 0x40:
      MN_LD_r1_r2(MN_Regs_B, MN_Regs_B);
      break;
    case 0x41:
      MN_LD_r1_r2(MN_Regs_B, MN_Regs_C);
      break;
    case 0x42:
      MN_LD_r1_r2(MN_Regs_B, MN_Regs_D);
      break;
    case 0x43:
      MN_LD_r1_r2(MN_Regs_B, MN_Regs_E);
      break;
    case 0x44:
      MN_LD_r1_r2(MN_Regs_B, MN_Regs_H);
      break;
    case 0x45:
      MN_LD_r1_r2(MN_Regs_B, MN_Regs_L);
      break;
    case 0x46:
      MN_LD_r_HL(MN_Regs_B);
      break;
    case 0x48:
      MN_LD_r1_r2(MN_Regs_C, MN_Regs_B);
      break;
    case 0x49:
      MN_LD_r1_r2(MN_Regs_C, MN_Regs_C);
      break;
    case 0x4A:
      MN_LD_r1_r2(MN_Regs_C, MN_Regs_D);
      break;
    case 0x4B:
      MN_LD_r1_r2(MN_Regs_C, MN_Regs_E);
      break;
    case 0x4C:
      MN_LD_r1_r2(MN_Regs_C, MN_Regs_H);
      break;
    case 0x4D:
      MN_LD_r1_r2(MN_Regs_C, MN_Regs_L);
      break;
    case 0x4E:
      MN_LD_r_HL(MN_Regs_C);
      break;
    case 0x50:
      MN_LD_r1_r2(MN_Regs_D, MN_Regs_B);
      break;
    case 0x51:
      MN_LD_r1_r2(MN_Regs_D, MN_Regs_C);
      break;
    case 0x52:
      MN_LD_r1_r2(MN_Regs_D, MN_Regs_D);
      break;
    case 0x53:
      MN_LD_r1_r2(MN_Regs_D, MN_Regs_E);
      break;
    case 0x54:
      MN_LD_r1_r2(MN_Regs_D, MN_Regs_H);
      break;
    case 0x55:
      MN_LD_r1_r2(MN_Regs_D, MN_Regs_L);
      break;
    case 0x56:
      MN_LD_r_HL(MN_Regs_D);
      break;
    case 0x58:
      MN_LD_r1_r2(MN_Regs_E, MN_Regs_B);
      break;
    case 0x59:
      MN_LD_r1_r2(MN_Regs_E, MN_Regs_C);
      break;
    case 0x5A:
      MN_LD_r1_r2(MN_Regs_E, MN_Regs_D);
      break;
    case 0x5B:
      MN_LD_r1_r2(MN_Regs_E, MN_Regs_E);
      break;
    case 0x5C:
      MN_LD_r1_r2(MN_Regs_E, MN_Regs_H);
      break;
    case 0x5D:
      MN_LD_r1_r2(MN_Regs_E, MN_Regs_L);
      break;
    case 0x5E:
      MN_LD_r_HL(MN_Regs_E);
      break;
    case 0x60:
      MN_LD_r1_r2(MN_Regs_H, MN_Regs_B);
      break;
    case 0x61:
      MN_LD_r1_r2(MN_Regs_H, MN_Regs_C);
      break;
    case 0x62:
      MN_LD_r1_r2(MN_Regs_H, MN_Regs_D);
      break;
    case 0x63:
      MN_LD_r1_r2(MN_Regs_H, MN_Regs_E);
      break;
    case 0x64:
      MN_LD_r1_r2(MN_Regs_H, MN_Regs_H);
      break;
    case 0x65:
      MN_LD_r1_r2(MN_Regs_H, MN_Regs_L);
      break;
    case 0x66:
      MN_LD_r_HL(MN_Regs_H);
      break;
    case 0x68:
      MN_LD_r1_r2(MN_Regs_L, MN_Regs_B);
      break;
    case 0x69:
      MN_LD_r1_r2(MN_Regs_L, MN_Regs_C);
      break;
    case 0x6A:
      MN_LD_r1_r2(MN_Regs_L, MN_Regs_D);
      break;
    case 0x6B:
      MN_LD_r1_r2(MN_Regs_L, MN_Regs_E);
      break;
    case 0x6C:
      MN_LD_r1_r2(MN_Regs_L, MN_Regs_H);
      break;
    case 0x6D:
      MN_LD_r1_r2(MN_Regs_L, MN_Regs_L);
      break;
    case 0x6E:
      MN_LD_r_HL(MN_Regs_L);
      break;
    // write
    case 0x70:
      MN_LD_HL_r(MN_Regs_B);
      break;
    case 0x71:
      MN_LD_HL_r(MN_Regs_C);
      break;
    case 0x72:
      MN_LD_HL_r(MN_Regs_D);
      break;
    case 0x73:
      MN_LD_HL_r(MN_Regs_E);
      break;
    case 0x74:
      MN_LD_HL_r(MN_Regs_H);
      break;
    case 0x75:
      MN_LD_HL_r(MN_Regs_L);
      break;
    case 0x36:
      mn_memory_write(emu, MN_GET_PAIR(emu, MN_Regs_H, MN_Regs_L), mn_memory_read(emu, emu->PC++));
      emu->cycles += 12;
      break;

    // LD MN_Regs_A, n
    case 0x0A:
      MN_LD_r_BC(MN_Regs_A);
      break;
    case 0x1A:
      MN_LD_r_DE(MN_Regs_A);
      break;
    case 0xFA:
      // DDyansh pls help here
      break;

    // LD (nn), MN_Regs_A
    case 0x47:
      MN_LD_r1_r2(MN_Regs_B, MN_Regs_A);
      break;
    case 0x4F:
      MN_LD_r1_r2(MN_Regs_C, MN_Regs_A);
      break;
    case 0x57:
      MN_LD_r1_r2(MN_Regs_D, MN_Regs_A);
      break;
    case 0x5F:
      MN_LD_r1_r2(MN_Regs_E, MN_Regs_A);
      break;
    case 0x67:
      MN_LD_r1_r2(MN_Regs_H, MN_Regs_A);
      break;
    case 0x6F:
      MN_LD_r1_r2(MN_Regs_L, MN_Regs_A);
      break;
    case 0x02:
      MN_LD_BC_r(MN_Regs_A);
      break;
    case 0x12:
      MN_LD_DE_r(MN_Regs_A);
      break;
    case 0x77:
      MN_LD_HL_r(MN_Regs_A);
      break;
    case 0xEA:
      // (nn) walo ka aap dekhlo dots
      emu->cycles += 16;
      break;

    // LD MN_Regs_A,(MN_Regs_C)
    case 0xF2:
      emu->r[MN_Regs_A] = mn_memory_read(emu, 0xFF00 + emu->r[MN_Regs_C]);
      emu->cycles += 8;
      break;

    // LD (MN_Regs_C),MN_Regs_A
    case 0xE2:
      mn_memory_write(emu, 0xFF00 + emu->r[MN_Regs_C], emu->r[MN_Regs_A]);
      emu->cycles += 8;
      break;

    // LDD MN_Regs_A,(HL) | LD MN_Regs_A,(HLD) | LD MN_Regs_A,(HL-)
    case 0x3A:
      // incomplete ALU
      break;
    // LDD (HL),MN_Regs_A | LD (HLD),MN_Regs_A | LD (HL-),MN_Regs_A
    case 0x32:
      // incomplete
      break;
    // LDI MN_Regs_A,(HL) | LD MN_Regs_A,(HLI) | LD MN_Regs_A,(HL+)
    case 0x2A:
      // incomplete
      break;
    // LDI (HL),MN_Regs_A | LD (HL),MN_Regs_A | LD (HL+),MN_Regs_A
    case 0x22:
      // incomplete
      break;

    // LDH (n),MN_Regs_A
    case 0xE0:
      mn_memory_write(emu, 0xFF00 + mn_memory_read(emu, emu->PC++), emu->r[MN_Regs_A]);
      emu->cycles += 12;
      break;

    // LDH MN_Regs_A,(n)
    case 0xF0:
      emu->r[MN_Regs_A] = mn_memory_read(emu, 0xFF00 + mn_memory_read(emu, emu->PC++));
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
      MN_ADD_r(MN_Regs_A);
      break;
    case 0x80:
      MN_ADD_r(MN_Regs_B);
      break;
    case 0x81:
      MN_ADD_r(MN_Regs_C);
      break;
    case 0x82:
      MN_ADD_r(MN_Regs_D);
      break;
    case 0x83:
      MN_ADD_r(MN_Regs_E);
      break;
    case 0x84:
      MN_ADD_r(MN_Regs_H);
      break;
    case 0x85:
      MN_ADD_r(MN_Regs_L);
      break;
    case 0x86:
      MN_ADD_HL();
      break;
    case 0xC6:
      MN_ADD_n();
      break;
    // ADC MN_Regs_A, n
    case 0x8F:
      MN_ADC_r(MN_Regs_A);
      break;
    case 0x88:
      MN_ADC_r(MN_Regs_B);
      break;
    case 0x89:
      MN_ADC_r(MN_Regs_C);
      break;
    case 0x8A:
      MN_ADC_r(MN_Regs_D);
      break;
    case 0x8B:
      MN_ADC_r(MN_Regs_E);
      break;
    case 0x8C:
      MN_ADC_r(MN_Regs_H);
      break;
    case 0x8D:
      MN_ADC_r(MN_Regs_L);
      break;
    case 0x8E:
      MN_ADC_HL();
      break;
    case 0xCE:
      MN_ADC_n();
      break;

    // SUB n
    case 0x97:
      MN_SUB_r(MN_Regs_A);
      break;
    case 0x90:
      MN_SUB_r(MN_Regs_B);
      break;
    case 0x91:
      MN_SUB_r(MN_Regs_C);
      break;
    case 0x92:
      MN_SUB_r(MN_Regs_D);
      break;
    case 0x93:
      MN_SUB_r(MN_Regs_E);
      break;
    case 0x94:
      MN_SUB_r(MN_Regs_H);
      break;
    case 0x95:
      MN_SUB_r(MN_Regs_L);
      break;
    case 0x96:
      MN_SUB_HL();
      break;
    case 0xD6:
      MN_SUB_n();
      break;

    // SBC
    case 0x9F:
      MN_SBC_r(MN_Regs_A);
      break;
    case 0x98:
      MN_SBC_r(MN_Regs_B);
      break;
    case 0x9A:
      MN_SBC_r(MN_Regs_D);
      break;
    case 0x9B:
      MN_SBC_r(MN_Regs_E);
      break;
    case 0x9C:
      MN_SBC_r(MN_Regs_H);
      break;
    case 0x9D:
      MN_SBC_r(MN_Regs_L);
      break;
    case 0x9E:
      MN_SBC_HL();
      break;
    // WHAT EH FUCK EVEN IS SBC # ?? opcode bruh sonion ring

    // AND n
    case 0xA7:
      MN_AND_r(MN_Regs_A);
      break;
    case 0xA0:
      MN_AND_r(MN_Regs_B);
      break;
    case 0xA1:
      MN_AND_r(MN_Regs_C);
      break;
    case 0xA2:
      MN_AND_r(MN_Regs_D);
      break;
    case 0xA3:
      MN_AND_r(MN_Regs_E);
      break;
    case 0xA4:
      MN_AND_r(MN_Regs_H);
      break;
    case 0xA5:
      MN_AND_r(MN_Regs_L);
      break;
    case 0xA6:
      MN_AND_HL();
      break;
    case 0xE6:
      MN_AND_n();
      break;

    // OR n
    case 0xB7:
      MN_OR_r(MN_Regs_A);
      break;
    case 0xB0:
      MN_OR_r(MN_Regs_B);
      break;
    case 0xB1:
      MN_OR_r(MN_Regs_C);
      break;
    case 0xB2:
      MN_OR_r(MN_Regs_D);
      break;
    case 0xB3:
      MN_OR_r(MN_Regs_E);
      break;
    case 0xB4:
      MN_OR_r(MN_Regs_H);
      break;
    case 0xB5:
      MN_OR_r(MN_Regs_L);
      break;
    case 0xB6:
      MN_OR_HL();
      break;
    case 0xF6:
      MN_OR_n();
      break;

    // XOR n
    case 0xAF:
      MN_XOR_r(MN_Regs_A);
      break;
    case 0xA8:
      MN_XOR_r(MN_Regs_B);
      break;
    case 0xA9:
      MN_XOR_r(MN_Regs_C);
      break;
    case 0xAA:
      MN_XOR_r(MN_Regs_D);
      break;
    case 0xAB:
      MN_XOR_r(MN_Regs_E);
      break;
    case 0xAC:
      MN_XOR_r(MN_Regs_H);
      break;
    case 0xAD:
      MN_XOR_r(MN_Regs_L);
      break;
    case 0xAE:
      MN_XOR_HL();
      break;
    case 0xEE:
      MN_XOR_n();
      break;

    // CP n
    case 0xBF:
      MN_CP_r(MN_Regs_A);
      break;
    case 0xB8:
      MN_CP_r(MN_Regs_B);
      break;
    case 0xB9:
      MN_CP_r(MN_Regs_C);
      break;
    case 0xBA:
      MN_CP_r(MN_Regs_D);
      break;
    case 0xBB:
      MN_CP_r(MN_Regs_E);
      break;
    case 0xBC:
      MN_CP_r(MN_Regs_H);
      break;
    case 0xBD:
      MN_CP_r(MN_Regs_L);
      break;
    case 0xBE:
      MN_CP_HL();
      break;
    case 0xFE:
      MN_CP_n();
      break;

    // INC n
    case 0x3C:
      MN_INC_r(MN_Regs_A);
      break;
    case 0x04:
      MN_INC_r(MN_Regs_B);
      break;
    case 0x0C:
      MN_INC_r(MN_Regs_C);
      break;
    case 0x14:
      MN_INC_r(MN_Regs_D);
      break;
    case 0x1C:
      MN_INC_r(MN_Regs_E);
      break;
    case 0x24:
      MN_INC_r(MN_Regs_H);
      break;
    case 0x2C:
      MN_INC_r(MN_Regs_L);
      break;
    case 0x34:
      MN_INC_HL();
      break;

    // DEC n
    case 0x3D:
      MN_DEC_r(MN_Regs_A);
      break;
    case 0x05:
      MN_DEC_r(MN_Regs_B);
      break;
    case 0x0D:
      MN_DEC_r(MN_Regs_C);
      break;
    case 0x15:
      MN_DEC_r(MN_Regs_D);
      break;
    case 0x1D:
      MN_DEC_r(MN_Regs_E);
      break;
    case 0x25:
      MN_DEC_r(MN_Regs_H);
      break;
    case 0x2D:
      MN_DEC_r(MN_Regs_L);
      break;
    case 0x35:
      MN_DEC_HL();
      break;
    }

    // 16 bit ALU
    {
    }
    
    {
    case 0xCB:
      // 16 bit opcode idk kaise karte
      break;

    case 0x27:
      MN_DAA();
      break;
    case 0x2F:
      MN_CPL();
      break;
    case 0x3F:
      MN_CCF();
      break;
    case 0x37:
      MN_SCF();
      break;
    case 0x00:
      MN_NOP();
      break;
    case 0x76:
      MN_HALT();
      break;
    case 0x10:
      // 16 bit opcode idk kaise karte
      break;
    case 0xF3:
      MN_DI();
      break;
    case 0xFB:
      MN_EI();
      break;

    // Moment of inertia about the axis of a disc about its center of mass is given by the formula I = (1/2) * m * r^2, where m is the mass of the disc and r is its radius. This formula assumes that the disc is uniform and thin, and that the axis of rotation passes through its center and is perpendicular to its plane.
    // Rotation love
    case 0x07:
      MN_RLCA();
      break;
    case 0x17:
      MN_RLA();
      break;
    case 0x0F:
      MN_RRCA();
      break;
    case 0x1F:
      MN_RRA();
      break;
    // rest are 16 bit opcodes of 0xCB

    // Bit also 16 bit opcodes of 0xCB

    // Jumps
    case 0xC3:
      MN_JP_nn();
      break;
    case 0xC2:
      MN_JP_NZ_nn();
      break;
    case 0xCA:
      MN_JP_Z_nn();
      break;
    case 0xD2:
      MN_JP_NC_nn();
      break;
    case 0xDA:
      MN_JP_C_nn();
      break;
    case 0xE9:
      MN_JP_HL();
      break;

    case 0x18:
      MN_JR_n();
      break;
    case 0x20:
      MN_JR_NZ_n();
      break;
    case 0x28:
      MN_JR_Z_n();
      break;
    case 0x30:
      MN_JR_NC_n();
      break;
    case 0x38:
      MN_JR_C_n();
      break;

    // callz
    case 0xCD:
      MN_CALL_nn();
      break;

    case 0xC4:
      MN_CALL_NZ_nn();
      break;
    case 0xCC:
      MN_CALL_Z_nn();
    case 0xD4:
      MN_CALL_NC_nn();
      break;
    case 0xDC:
      MN_CALL_C_nn();
      break;

    // RST sm crazy shi
    case 0xC9:
      MN_RET();
      break;
    case 0xC0:
      MN_RET_NZ();
      break;
    case 0xC8:
      MN_RET_Z();
      break;
    case 0xD0:
      MN_RET_NC();
      break;
    case 0xD8:
      MN_RET_C();
      break;
    case 0xD9:
      MN_RETI();
      break;
    }
  }
}