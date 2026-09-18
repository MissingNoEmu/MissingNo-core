#include "emulator.h"
#include "memory.h"

static mn_u16 pair_get(const MN_Emu *emu, int high)
{
  return ((mn_u16)emu->r[high] << 8) | emu->r[high + 1];
}

static void pair_set(MN_Emu *emu, int high, mn_u16 value)
{
  emu->r[high] = (mn_u8)(value >> 8);
  emu->r[high + 1] = (mn_u8)value;
}

static mn_u8 fetch8(MN_Emu *emu)
{
  return read_memory(emu, emu->PC++);
}

static mn_u16 fetch16(MN_Emu *emu)
{
  mn_u16 value = fetch8(emu);
  return value | ((mn_u16)fetch8(emu) << 8);
}

static void set_flags(MN_Emu *emu, mn_u8 flags)
{
  emu->r[F] = flags & 0xF0;
}

static mn_u8 reg_read(MN_Emu *emu, int index)
{
  return index == 6 ? read_memory(emu, pair_get(emu, H)) : emu->r[index * 2];
}

static void reg_write(MN_Emu *emu, int index, mn_u8 value)
{
  if (index == 6)
    write_memory(emu, pair_get(emu, H), value);
  else
    emu->r[index * 2] = value;
}

static mn_u8 inc8(MN_Emu *emu, mn_u8 value)
{
  mn_u8 result = value + 1;
  set_flags(emu, (emu->r[F] & FLAG_C) | (result == 0 ? FLAG_Z : 0) |
                     ((value & 0x0F) == 0x0F ? FLAG_H : 0));
  return result;
}

static mn_u8 dec8(MN_Emu *emu, mn_u8 value)
{
  mn_u8 result = value - 1;
  set_flags(emu, (emu->r[F] & FLAG_C) | FLAG_N | (result == 0 ? FLAG_Z : 0) |
                     ((value & 0x0F) == 0 ? FLAG_H : 0));
  return result;
}

static void alu_a(MN_Emu *emu, int operation, mn_u8 value)
{
  mn_u8 a = emu->r[A];
  mn_u16 result;
  mn_u8 flags = 0;
  mn_u8 carry = (emu->r[F] & FLAG_C) != 0;

  switch (operation)
  {
  case 0:
    result = a + value;
    emu->r[A] = (mn_u8)result;
    break;
  case 1:
    result = a + value + carry;
    emu->r[A] = (mn_u8)result;
    break;
  case 2:
    result = a - value;
    emu->r[A] = (mn_u8)result;
    flags = FLAG_N;
    break;
  case 3:
    result = a - value - carry;
    emu->r[A] = (mn_u8)result;
    flags = FLAG_N;
    break;
  case 4:
    emu->r[A] = a & value;
    flags = FLAG_H;
    result = emu->r[A];
    break;
  case 5:
    emu->r[A] = a ^ value;
    result = emu->r[A];
    break;
  case 6:
    emu->r[A] = a | value;
    result = emu->r[A];
    break;
  default:
    result = a - value;
    flags = FLAG_N;
    break;
  }
  if ((result & 0xFF) == 0)
    flags |= FLAG_Z;
  if (operation <= 3 && ((a ^ value ^ result) & 0x10))
    flags |= FLAG_H;
  if (operation <= 3 && (operation == 0 || operation == 1 ? result > 0xFF : value + (operation == 3 ? carry : 0) > a))
    flags |= FLAG_C;
  set_flags(emu, flags);
}

static void add_hl(MN_Emu *emu, mn_u16 value)
{
  mn_u16 hl = pair_get(emu, H);
  mn_size result = (mn_size)hl + value;
  set_flags(emu, (emu->r[F] & FLAG_Z) |
                     (((hl ^ value ^ result) & 0x1000) ? FLAG_H : 0) |
                     (result > 0xFFFF ? FLAG_C : 0));
  pair_set(emu, H, (mn_u16)result);
}

static mn_u8 cb_rotate(MN_Emu *emu, int operation, mn_u8 value)
{
  mn_u8 carry = emu->r[F] & FLAG_C;
  mn_u8 bit;
  mn_u8 result;

  switch (operation)
  {
  case 0:
    bit = value >> 7;
    result = (value << 1) | bit;
    break;
  case 1:
    bit = value & 1;
    result = (value >> 1) | (bit << 7);
    break;
  case 2:
    bit = value >> 7;
    result = (value << 1) | (carry ? 1 : 0);
    break;
  case 3:
    bit = value & 1;
    result = (value >> 1) | (carry ? 0x80 : 0);
    break;
  case 4:
    bit = value >> 7;
    result = value << 1;
    break;
  case 5:
    bit = value & 1;
    result = (value >> 1) | (value & 0x80);
    break;
  case 6:
    bit = value >> 7;
    result = (value << 1) | 1;
    break;
  default:
    bit = value & 1;
    result = value >> 1;
    break;
  }
  set_flags(emu, (result == 0 ? FLAG_Z : 0) | (bit ? FLAG_C : 0));
  return result;
}

static mn_u8 cb_step(MN_Emu *emu)
{
  mn_u8 opcode = fetch8(emu);
  int group = opcode >> 6;
  int index = opcode & 7;
  int bit = (opcode >> 3) & 7;
  mn_u8 value = reg_read(emu, index);

  if (group == 0)
  {
    // Draw when the CB opcode targets (HL) in VRAM or OAM.
    reg_write(emu, index, cb_rotate(emu, bit, value));
    return index == 6 ? 16 : 8;
  }
  if (group == 1)
  {
    set_flags(emu, (emu->r[F] & FLAG_C) | FLAG_H |
                       (!(value & (1 << bit)) ? FLAG_Z : 0));
    return index == 6 ? 12 : 8;
  }
  if (group == 2)
    value &= (mn_u8) ~(1 << bit);
  else
    value |= (mn_u8)(1 << bit);
  // Draw when a RES or SET opcode targets (HL) in VRAM or OAM.
  reg_write(emu, index, value);
  return index == 6 ? 16 : 8;
}

static void push16(MN_Emu *emu, mn_u16 value)
{
  write_memory(emu, --emu->SP, (mn_u8)(value >> 8));
  write_memory(emu, --emu->SP, (mn_u8)value);
}

static mn_u16 pop16(MN_Emu *emu)
{
  mn_u16 value = read_memory(emu, emu->SP++);
  return value | ((mn_u16)read_memory(emu, emu->SP++) << 8);
}

static mn_u8 condition_taken(const MN_Emu *emu, int condition)
{
  if (condition == 0)
    return !(emu->r[F] & FLAG_Z);
  if (condition == 1)
    return !!(emu->r[F] & FLAG_Z);
  if (condition == 2)
    return !(emu->r[F] & FLAG_C);
  return !!(emu->r[F] & FLAG_C);
}

static mn_u8 rotate_a(MN_Emu *emu, int operation)
{
  mn_u8 value = emu->r[A];
  mn_u8 carry;

  if (operation == 0 || operation == 2)
  {
    carry = value >> 7;
    value = (value << 1) | (operation == 2 ? ((emu->r[F] & FLAG_C) ? 1 : 0) : carry);
  }
  else
  {
    carry = value & 1;
    value = (value >> 1) | (operation == 3 ? ((emu->r[F] & FLAG_C) ? 0x80 : 0) : (carry << 7));
  }
  emu->r[A] = value;
  set_flags(emu, carry ? FLAG_C : 0);
  return 4;
}

static void daa(MN_Emu *emu)
{
  mn_u8 adjust = 0;
  mn_u8 carry = emu->r[F] & FLAG_C;

  if (!(emu->r[F] & FLAG_N))
  {
    if ((emu->r[F] & FLAG_H) || (emu->r[A] & 0x0F) > 9)
      adjust |= 0x06;
    if (carry || emu->r[A] > 0x99)
    {
      adjust |= 0x60;
      carry = FLAG_C;
    }
    emu->r[A] += adjust;
  }
  else
  {
    if (emu->r[F] & FLAG_H)
      adjust |= 0x06;
    if (carry)
      adjust |= 0x60;
    emu->r[A] -= adjust;
  }
  set_flags(emu, (emu->r[F] & FLAG_N) | carry |
                     (emu->r[A] == 0 ? FLAG_Z : 0));
}

static mn_u16 add_sp_signed(MN_Emu *emu, mn_u8 immediate)
{
  mn_u16 result = emu->SP + (signed char)immediate;
  set_flags(emu, ((emu->SP ^ immediate ^ result) & 0x10 ? FLAG_H : 0) |
                     ((emu->SP ^ immediate ^ result) & 0x100 ? FLAG_C : 0));
  return result;
}

mn_u8 emulator_step(MN_Emu *emu)
{
  if (emu->halted)
    return 4;

  mn_u8 opcode = fetch8(emu);
  mn_u16 address;
  signed char offset;
  int index;

  /* 0xCB: PREFIX CB. */
  if (opcode == 0xCB)
    return cb_step(emu);
  /* 0x76: HALT. */
  if (opcode == 0x76)
  {
    emu->halted = MN_TRUE;
    return 4;
  }

  /* 0x06: LD B,d8. 0x0E: LD C,d8. 0x16: LD D,d8. 0x1E: LD E,d8. */
  /* 0x26: LD H,d8. 0x2E: LD L,d8. 0x36: LD (HL),d8. 0x3E: LD A,d8. */
  // Draw when 0x36 writes through HL to VRAM or OAM.
  if ((opcode & 0xC7) == 0x06)
  {
    index = (opcode >> 3) & 7;
    reg_write(emu, index, fetch8(emu));
    return index == 6 ? 12 : 8;
  }
  /* 0x40: LD B,B. 0x41: LD B,C. 0x42: LD B,D. 0x43: LD B,E. */
  /* 0x44: LD B,H. 0x45: LD B,L. 0x46: LD B,(HL). 0x47: LD B,A. */
  /* 0x48: LD C,B. 0x49: LD C,C. 0x4A: LD C,D. 0x4B: LD C,E. */
  /* 0x4C: LD C,H. 0x4D: LD C,L. 0x4E: LD C,(HL). 0x4F: LD C,A. */
  /* 0x50: LD D,B. 0x51: LD D,C. 0x52: LD D,D. 0x53: LD D,E. */
  /* 0x54: LD D,H. 0x55: LD D,L. 0x56: LD D,(HL). 0x57: LD D,A. */
  /* 0x58: LD E,B. 0x59: LD E,C. 0x5A: LD E,D. 0x5B: LD E,E. */
  /* 0x5C: LD E,H. 0x5D: LD E,L. 0x5E: LD E,(HL). 0x5F: LD E,A. */
  /* 0x60: LD H,B. 0x61: LD H,C. 0x62: LD H,D. 0x63: LD H,E. */
  /* 0x64: LD H,H. 0x65: LD H,L. 0x66: LD H,(HL). 0x67: LD H,A. */
  /* 0x68: LD L,B. 0x69: LD L,C. 0x6A: LD L,D. 0x6B: LD L,E. */
  /* 0x6C: LD L,H. 0x6D: LD L,L. 0x6E: LD L,(HL). 0x6F: LD L,A. */
  /* 0x70: LD (HL),B. 0x71: LD (HL),C. 0x72: LD (HL),D. 0x73: LD (HL),E. */
  /* 0x74: LD (HL),H. 0x75: LD (HL),L. 0x77: LD (HL),A. */
  // Draw when the destination HL address is in VRAM or OAM.
  /* 0x78: LD A,B. 0x79: LD A,C. 0x7A: LD A,D. 0x7B: LD A,E. */
  /* 0x7C: LD A,H. 0x7D: LD A,L. 0x7E: LD A,(HL). 0x7F: LD A,A. */
  if ((opcode & 0xC0) == 0x40)
  {
    reg_write(emu, opcode & 7, reg_read(emu, (opcode >> 3) & 7));
    return (opcode & 7) == 6 || ((opcode >> 3) & 7) == 6 ? 8 : 4;
  }
  /* 0x04: INC B. 0x0C: INC C. 0x14: INC D. 0x1C: INC E. */
  /* 0x24: INC H. 0x2C: INC L. 0x34: INC (HL). 0x3C: INC A. */
  /* 0x05: DEC B. 0x0D: DEC C. 0x15: DEC D. 0x1D: DEC E. */
  /* 0x25: DEC H. 0x2D: DEC L. 0x35: DEC (HL). 0x3D: DEC A. */
  // Draw when 0x34 or 0x35 modifies a VRAM or OAM address through HL.
  if ((opcode & 0xC7) == 0x04 || (opcode & 0xC7) == 0x05)
  {
    index = (opcode >> 3) & 7;
    reg_write(emu, index, (opcode & 1) ? dec8(emu, reg_read(emu, index)) : inc8(emu, reg_read(emu, index)));
    return index == 6 ? 12 : 4;
  }
  /* 0x80: ADD A,B. 0x81: ADD A,C. 0x82: ADD A,D. 0x83: ADD A,E. */
  /* 0x84: ADD A,H. 0x85: ADD A,L. 0x86: ADD A,(HL). 0x87: ADD A,A. */
  /* 0x88: ADC A,B. 0x89: ADC A,C. 0x8A: ADC A,D. 0x8B: ADC A,E. */
  /* 0x8C: ADC A,H. 0x8D: ADC A,L. 0x8E: ADC A,(HL). 0x8F: ADC A,A. */
  /* 0x90: SUB B. 0x91: SUB C. 0x92: SUB D. 0x93: SUB E. */
  /* 0x94: SUB H. 0x95: SUB L. 0x96: SUB (HL). 0x97: SUB A. */
  /* 0x98: SBC A,B. 0x99: SBC A,C. 0x9A: SBC A,D. 0x9B: SBC A,E. */
  /* 0x9C: SBC A,H. 0x9D: SBC A,L. 0x9E: SBC A,(HL). 0x9F: SBC A,A. */
  /* 0xA0: AND B. 0xA1: AND C. 0xA2: AND D. 0xA3: AND E. */
  /* 0xA4: AND H. 0xA5: AND L. 0xA6: AND (HL). 0xA7: AND A. */
  /* 0xA8: XOR B. 0xA9: XOR C. 0xAA: XOR D. 0xAB: XOR E. */
  /* 0xAC: XOR H. 0xAD: XOR L. 0xAE: XOR (HL). 0xAF: XOR A. */
  /* 0xB0: OR B. 0xB1: OR C. 0xB2: OR D. 0xB3: OR E. */
  /* 0xB4: OR H. 0xB5: OR L. 0xB6: OR (HL). 0xB7: OR A. */
  /* 0xB8: CP B. 0xB9: CP C. 0xBA: CP D. 0xBB: CP E. */
  /* 0xBC: CP H. 0xBD: CP L. 0xBE: CP (HL). 0xBF: CP A. */
  if ((opcode & 0xC0) == 0x80)
  {
    alu_a(emu, (opcode >> 3) & 7, reg_read(emu, opcode & 7));
    return (opcode & 7) == 6 ? 8 : 4;
  }
  /* 0xC6: ADD A,d8. 0xCE: ADC A,d8. 0xD6: SUB d8. 0xDE: SBC A,d8. */
  /* 0xE6: AND d8. 0xEE: XOR d8. 0xF6: OR d8. 0xFE: CP d8. */
  if ((opcode & 0xC7) == 0xC6)
  {
    alu_a(emu, (opcode >> 3) & 7, fetch8(emu));
    return 8;
  }

  switch (opcode)
  {
  /* 0x00: NOP. */
  case 0x00:
    return 4;
  /* 0x07: RLCA. */
  case 0x07:
    return rotate_a(emu, 0);
  /* 0x0F: RRCA. */
  case 0x0F:
    return rotate_a(emu, 1);
  /* 0x10: STOP. */
  case 0x10:
    fetch8(emu);
    emu->halted = MN_TRUE;
    return 4;
  /* 0x17: RLA. */
  case 0x17:
    return rotate_a(emu, 2);
  /* 0x1F: RRA. */
  case 0x1F:
    return rotate_a(emu, 3);
  /* 0x27: DAA. */
  case 0x27:
    daa(emu);
    return 4;
  /* 0x01: LD BC,d16. */
  case 0x01:
    pair_set(emu, B, fetch16(emu));
    return 12;
  /* 0x11: LD DE,d16. */
  case 0x11:
    pair_set(emu, D, fetch16(emu));
    return 12;
  /* 0x21: LD HL,d16. */
  case 0x21:
    pair_set(emu, H, fetch16(emu));
    return 12;
  /* 0x31: LD SP,d16. */
  case 0x31:
    emu->SP = fetch16(emu);
    return 12;
  /* 0x02: LD (BC),A. */
  // Draw when BC points to VRAM or OAM.
  case 0x02:
    write_memory(emu, pair_get(emu, B), emu->r[A]);
    return 8;
  /* 0x12: LD (DE),A. */
  // Draw when DE points to VRAM or OAM.
  case 0x12:
    write_memory(emu, pair_get(emu, D), emu->r[A]);
    return 8;
  /* 0x22: LD (HL+),A. */
  // Draw when HL points to VRAM or OAM.
  case 0x22:
    address = pair_get(emu, H);
    write_memory(emu, address, emu->r[A]);
    pair_set(emu, H, address + 1);
    return 8;
  /* 0x32: LD (HL-),A. */
  // Draw when HL points to VRAM or OAM.
  case 0x32:
    address = pair_get(emu, H);
    write_memory(emu, address, emu->r[A]);
    pair_set(emu, H, address - 1);
    return 8;
  /* 0x0A: LD A,(BC). */
  case 0x0A:
    emu->r[A] = read_memory(emu, pair_get(emu, B));
    return 8;
  /* 0x1A: LD A,(DE). */
  case 0x1A:
    emu->r[A] = read_memory(emu, pair_get(emu, D));
    return 8;
  /* 0x2A: LD A,(HL+). */
  case 0x2A:
    address = pair_get(emu, H);
    emu->r[A] = read_memory(emu, address);
    pair_set(emu, H, address + 1);
    return 8;
  /* 0x3A: LD A,(HL-). */
  case 0x3A:
    address = pair_get(emu, H);
    emu->r[A] = read_memory(emu, address);
    pair_set(emu, H, address - 1);
    return 8;
  /* 0x03: INC BC. */
  case 0x03:
    pair_set(emu, B, pair_get(emu, B) + 1);
    return 8;
  /* 0x13: INC DE. */
  case 0x13:
    pair_set(emu, D, pair_get(emu, D) + 1);
    return 8;
  /* 0x23: INC HL. */
  case 0x23:
    pair_set(emu, H, pair_get(emu, H) + 1);
    return 8;
  /* 0x33: INC SP. */
  case 0x33:
    emu->SP++;
    return 8;
  /* 0x0B: DEC BC. */
  case 0x0B:
    pair_set(emu, B, pair_get(emu, B) - 1);
    return 8;
  /* 0x1B: DEC DE. */
  case 0x1B:
    pair_set(emu, D, pair_get(emu, D) - 1);
    return 8;
  /* 0x2B: DEC HL. */
  case 0x2B:
    pair_set(emu, H, pair_get(emu, H) - 1);
    return 8;
  /* 0x3B: DEC SP. */
  case 0x3B:
    emu->SP--;
    return 8;
  /* 0x08: LD (a16),SP. */
  // Draw when a16 points to VRAM or OAM.
  case 0x08:
    address = fetch16(emu);
    write_memory(emu, address, (mn_u8)emu->SP);
    write_memory(emu, address + 1, (mn_u8)(emu->SP >> 8));
    return 20;
  /* 0x09: ADD HL,BC. */
  case 0x09:
    add_hl(emu, pair_get(emu, B));
    return 8;
  /* 0x19: ADD HL,DE. */
  case 0x19:
    add_hl(emu, pair_get(emu, D));
    return 8;
  /* 0x29: ADD HL,HL. */
  case 0x29:
    add_hl(emu, pair_get(emu, H));
    return 8;
  /* 0x39: ADD HL,SP. */
  case 0x39:
    add_hl(emu, emu->SP);
    return 8;
  /* 0xC3: JP a16. */
  case 0xC3:
    emu->PC = fetch16(emu);
    return 16;
  /* 0xC2: JP NZ,a16. */
  case 0xC2:
  /* 0xCA: JP Z,a16. */
  case 0xCA:
  /* 0xD2: JP NC,a16. */
  case 0xD2:
  /* 0xDA: JP C,a16. */
  case 0xDA:
    address = fetch16(emu);
    if (condition_taken(emu, (opcode >> 3) & 3))
    {
      emu->PC = address;
      return 16;
    }
    return 12;
  /* 0xE9: JP (HL). */
  case 0xE9:
    emu->PC = pair_get(emu, H);
    return 4;
  /* 0x18: JR r8. */
  case 0x18:
    offset = (signed char)fetch8(emu);
    emu->PC += offset;
    return 12;
  /* 0x20: JR NZ,r8; 0x28: JR Z,r8; 0x30: JR NC,r8; 0x38: JR C,r8. */
  case 0x20:
  case 0x28:
  case 0x30:
  case 0x38:
    offset = (signed char)fetch8(emu);
    if (condition_taken(emu, (opcode >> 3) & 3))
    {
      emu->PC += offset;
      return 12;
    }
    return 8;
  /* 0xCD: CALL a16. */
  case 0xCD:
    address = fetch16(emu);
    push16(emu, emu->PC);
    emu->PC = address;
    return 24;
  /* 0xC4: CALL NZ,a16. */
  case 0xC4:
  /* 0xCC: CALL Z,a16. */
  case 0xCC:
  /* 0xD4: CALL NC,a16. */
  case 0xD4:
  /* 0xDC: CALL C,a16. */
  case 0xDC:
    address = fetch16(emu);
    if (condition_taken(emu, (opcode >> 3) & 3))
    {
      push16(emu, emu->PC);
      emu->PC = address;
      return 24;
    }
    return 12;
  /* 0xC9: RET. */
  case 0xC9:
    emu->PC = pop16(emu);
    return 16;
  /* 0xC0: RET NZ. */
  case 0xC0:
  /* 0xC8: RET Z. */
  case 0xC8:
  /* 0xD0: RET NC. */
  case 0xD0:
  /* 0xD8: RET C. */
  case 0xD8:
    if (condition_taken(emu, (opcode >> 3) & 3))
    {
      emu->PC = pop16(emu);
      return 20;
    }
    return 8;
  /* 0xD9: RETI. */
  case 0xD9:
    emu->PC = pop16(emu);
    emu->ime = MN_TRUE;
    return 16;
  /* 0xF5: PUSH AF. */
  case 0xF5:
    push16(emu, pair_get(emu, A));
    return 16;
  /* 0xC5: PUSH BC. */
  case 0xC5:
    push16(emu, pair_get(emu, B));
    return 16;
  /* 0xD5: PUSH DE. */
  case 0xD5:
    push16(emu, pair_get(emu, D));
    return 16;
  /* 0xE5: PUSH HL. */
  case 0xE5:
    push16(emu, pair_get(emu, H));
    return 16;
  /* 0xF1: POP AF. */
  case 0xF1:
    pair_set(emu, A, pop16(emu));
    emu->r[F] &= 0xF0;
    return 12;
  /* 0xC1: POP BC. */
  case 0xC1:
    pair_set(emu, B, pop16(emu));
    return 12;
  /* 0xD1: POP DE. */
  case 0xD1:
    pair_set(emu, D, pop16(emu));
    return 12;
  /* 0xE1: POP HL. */
  case 0xE1:
    pair_set(emu, H, pop16(emu));
    return 12;
  /* 0xF9: LD SP,HL. */
  case 0xF9:
    emu->SP = pair_get(emu, H);
    return 8;
  /* 0xE0: LDH (a8),A. */
  case 0xE0:
    write_memory(emu, 0xFF00 | fetch8(emu), emu->r[A]);
    return 12;
  /* 0xE2: LD (C),A. */
  case 0xE2:
    write_memory(emu, 0xFF00 | emu->r[C], emu->r[A]);
    return 8;
  /* 0xEA: LD (a16),A. */
  // Draw when a16 points to VRAM or OAM.
  case 0xEA:
    write_memory(emu, fetch16(emu), emu->r[A]);
    return 16;
  /* 0xF0: LDH A,(a8). */
  case 0xF0:
    emu->r[A] = read_memory(emu, 0xFF00 | fetch8(emu));
    return 12;
  /* 0xF2: LD A,(C). */
  case 0xF2:
    emu->r[A] = read_memory(emu, 0xFF00 | emu->r[C]);
    return 8;
  /* 0xFA: LD A,(a16). */
  case 0xFA:
    emu->r[A] = read_memory(emu, fetch16(emu));
    return 16;
  /* 0xF3: DI. */
  case 0xF3:
    emu->ime = MN_FALSE;
    return 4;
  /* 0xFB: EI. */
  case 0xFB:
    emu->ime = MN_TRUE;
    return 4;
  /* 0xF8: LD HL,SP+r8. */
  case 0xF8:
    pair_set(emu, H, add_sp_signed(emu, fetch8(emu)));
    return 12;
    /* 0xC7: RST 00H. */
  case 0xC7:
    /* 0xCF: RST 08H. */
  case 0xCF:
    /* 0xD7: RST 10H. */
  case 0xD7:
    /* 0xDF: RST 18H. */
  case 0xDF:
    /* 0xE7: RST 20H. */
  case 0xE7:
    /* 0xEF: RST 28H. */
  case 0xEF:
    /* 0xF7: RST 30H. */
  case 0xF7:
    /* 0xFF: RST 38H. */
  case 0xFF:
    push16(emu, emu->PC);
    emu->PC = opcode & 0x38;
    return 16;
  /* 0xE8: ADD SP,r8. */
  case 0xE8:
    emu->SP = add_sp_signed(emu, fetch8(emu));
    return 16;
  /* 0x2F: CPL. */
  case 0x2F:
    emu->r[A] ^= 0xFF;
    emu->r[F] = (emu->r[F] & (FLAG_Z | FLAG_C)) | FLAG_N | FLAG_H;
    return 4;
  /* 0x37: SCF. */
  case 0x37:
    emu->r[F] = (emu->r[F] & FLAG_Z) | FLAG_C;
    return 4;
  /* 0x3F: CCF. */
  case 0x3F:
    emu->r[F] = (emu->r[F] & FLAG_Z) | ((emu->r[F] & FLAG_C) ? 0 : FLAG_C);
    return 4;
  default:
    /* The remaining opcodes are not implemented yet. */
    return 4;
  }
}
