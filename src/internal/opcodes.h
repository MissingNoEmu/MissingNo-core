#ifndef OPCODES_H
#define OPCODES_H
#include "internal/emulator.h"
#include "internal/memory.h"

// feel free to change the names, im js typing shi

#define MN_GET_PAIR(emu, h, l) (emu->r[h] << 8) | emu->r[l]

#define MN_LD_nn_n(dest)                           \
  {                                                \
    emu->r[dest] = mn_memory_read(emu, emu->PC++); \
    emu->cycles += 8;                              \
  }
#define MN_LD_r1_r2(dest, src)  \
  {                             \
    emu->r[dest] = emu->r[src]; \
    emu->cycles += 4;           \
  }
#define MN_LD_r_HL(dest)                                                        \
  {                                                                             \
    emu->r[dest] = mn_memory_read(emu, MN_GET_PAIR(emu, MN_Regs_H, MN_Regs_L)); \
    emu->cycles += 8;                                                           \
  }
#define MN_LD_HL_r(src)                                                        \
  {                                                                            \
    mn_memory_write(emu, MN_GET_PAIR(emu, MN_Regs_H, MN_Regs_L), emu->r[src]); \
    emu->cycles += 8;                                                          \
  }
#define MN_LD_r_BC(dest)                                                        \
  {                                                                             \
    emu->r[dest] = mn_memory_read(emu, MN_GET_PAIR(emu, MN_Regs_B, MN_Regs_C)); \
    emu->cycles += 8;                                                           \
  }
#define MN_LD_r_DE(dest)                                                        \
  {                                                                             \
    emu->r[dest] = mn_memory_read(emu, MN_GET_PAIR(emu, MN_Regs_D, MN_Regs_E)); \
    emu->cycles += 8;                                                           \
  }
#define MN_LD_BC_r(src)                                                        \
  {                                                                            \
    mn_memory_write(emu, MN_GET_PAIR(emu, MN_Regs_B, MN_Regs_C), emu->r[src]); \
    emu->cycles += 8;                                                          \
  }
#define MN_LD_DE_r(src)                                                        \
  {                                                                            \
    mn_memory_write(emu, MN_GET_PAIR(emu, MN_Regs_D, MN_Regs_E), emu->r[src]); \
    emu->cycles += 8;                                                          \
  }

#define MN_ADD_r(src)                   \
  {                                     \
    mn_u8 a = emu->r[MN_Regs_A];        \
    mn_u8 b = emu->r[src];              \
    mn_u16 result = a + b;              \
                                        \
    emu->r[MN_Regs_A] = (mn_u8)result;  \
                                        \
    emu->r[MN_Regs_F] = 0;              \
    if (emu->r[MN_Regs_A] == 0)         \
      emu->r[MN_Regs_F] |= (1 << 7);    \
    if ((a & 0x0F) + (b & 0x0F) > 0x0F) \
      emu->r[MN_Regs_F] |= (1 << 5);    \
    if (result > 0xFF)                  \
      emu->r[MN_Regs_F] |= (1 << 4);    \
                                        \
    emu->cycles += 4;                   \
  }
#define MN_ADD_HL()                                                        \
  {                                                                        \
    mn_u8 a = emu->r[MN_Regs_A];                                           \
    mn_u8 b = mn_memory_read(emu, MN_GET_PAIR(emu, MN_Regs_H, MN_Regs_L)); \
    mn_u16 result = a + b;                                                 \
                                                                           \
    emu->r[MN_Regs_A] = (mn_u8)result;                                     \
                                                                           \
    emu->r[MN_Regs_F] = 0;                                                 \
    if (emu->r[MN_Regs_A] == 0)                                            \
      emu->r[MN_Regs_F] |= (1 << 7);                                       \
    if ((a & 0x0F) + (b & 0x0F) > 0x0F)                                    \
      emu->r[MN_Regs_F] |= (1 << 5);                                       \
    if (result > 0xFF)                                                     \
      emu->r[MN_Regs_F] |= (1 << 4);                                       \
                                                                           \
    emu->cycles += 8;                                                      \
  }
#define MN_ADD_n()                            \
  {                                           \
    mn_u8 a = emu->r[MN_Regs_A];              \
    mn_u8 b = mn_memory_read(emu, emu->PC++); \
    mn_u16 result = a + b;                    \
                                              \
    emu->r[MN_Regs_A] = (mn_u8)result;        \
                                              \
    emu->r[MN_Regs_F] = 0;                    \
    if (emu->r[MN_Regs_A] == 0)               \
      emu->r[MN_Regs_F] |= (1 << 7);          \
    if ((a & 0x0F) + (b & 0x0F) > 0x0F)       \
      emu->r[MN_Regs_F] |= (1 << 5);          \
    if (result > 0xFF)                        \
      emu->r[MN_Regs_F] |= (1 << 4);          \
                                              \
    emu->cycles += 8;                         \
  }

#define MN_ADC_r(src)                       \
  {                                         \
    mn_u8 a = emu->r[MN_Regs_A];            \
    mn_u8 b = emu->r[src];                  \
    mn_u8 c = (emu->r[MN_Regs_F] >> 4) & 1; \
    mn_u16 result = a + b + c;              \
                                            \
    emu->r[MN_Regs_A] = (mn_u8)result;      \
                                            \
    emu->r[MN_Regs_F] = 0;                  \
    if (emu->r[MN_Regs_A] == 0)             \
      emu->r[MN_Regs_F] |= (1 << 7);        \
    if ((a & 0x0F) + (b & 0x0F) + c > 0x0F) \
      emu->r[MN_Regs_F] |= (1 << 5);        \
    if (result > 0xFF)                      \
      emu->r[MN_Regs_F] |= (1 << 4);        \
                                            \
    emu->cycles += 4;                       \
  }
#define MN_ADC_HL()                                                        \
  {                                                                        \
    mn_u8 a = emu->r[MN_Regs_A];                                           \
    mn_u8 b = mn_memory_read(emu, MN_GET_PAIR(emu, MN_Regs_H, MN_Regs_L)); \
    mn_u8 c = (emu->r[MN_Regs_F] >> 4) & 1;                                \
    mn_u16 result = a + b + c;                                             \
                                                                           \
    emu->r[MN_Regs_A] = (mn_u8)result;                                     \
                                                                           \
    emu->r[MN_Regs_F] = 0;                                                 \
    if (emu->r[MN_Regs_A] == 0)                                            \
      emu->r[MN_Regs_F] |= (1 << 7);                                       \
    if ((a & 0x0F) + (b & 0x0F) + c > 0x0F)                                \
      emu->r[MN_Regs_F] |= (1 << 5);                                       \
    if (result > 0xFF)                                                     \
      emu->r[MN_Regs_F] |= (1 << 4);                                       \
                                                                           \
    emu->cycles += 8;                                                      \
  }
#define MN_ADC_n()                            \
  {                                           \
    mn_u8 a = emu->r[MN_Regs_A];              \
    mn_u8 b = mn_memory_read(emu, emu->PC++); \
    mn_u8 c = (emu->r[MN_Regs_F] >> 4) & 1;   \
    mn_u16 result = a + b + c;                \
                                              \
    emu->r[MN_Regs_A] = (mn_u8)result;        \
                                              \
    emu->r[MN_Regs_F] = 0;                    \
    if (emu->r[MN_Regs_A] == 0)               \
      emu->r[MN_Regs_F] |= (1 << 7);          \
    if ((a & 0x0F) + (b & 0x0F) + c > 0x0F)   \
      emu->r[MN_Regs_F] |= (1 << 5);          \
    if (result > 0xFF)                        \
      emu->r[MN_Regs_F] |= (1 << 4);          \
                                              \
    emu->cycles += 8;                         \
  }
#define MN_SUB_r(src)                \
  {                                  \
    mn_u8 a = emu->r[MN_Regs_A];     \
    mn_u8 b = emu->r[src];           \
    mn_u16 result = a - b;           \
    emu->r[MN_Regs_A] = result;      \
    emu->r[MN_Regs_F] = 0;           \
    if (result == 0)                 \
      emu->r[MN_Regs_F] |= (1 << 7); \
    emu->r[MN_Regs_F] |= (1 << 6);   \
    if (a < b)                       \
      emu->r[MN_Regs_F] |= (1 << 4); \
    if ((a & 0x0F) < (b & 0x0F))     \
      emu->r[MN_Regs_F] |= (1 << 5); \
    emu->cycles += 4;                \
  }
#define MN_SUB_HL()                                                        \
  {                                                                        \
    mn_u8 a = emu->r[MN_Regs_A];                                           \
    mn_u8 b = mn_memory_read(emu, MN_GET_PAIR(emu, MN_Regs_H, MN_Regs_L)); \
    mn_u16 result = a - b;                                                 \
    emu->r[MN_Regs_A] = result;                                            \
    emu->r[MN_Regs_F] = 0;                                                 \
    if (result == 0)                                                       \
      emu->r[MN_Regs_F] |= (1 << 7);                                       \
    emu->r[MN_Regs_F] |= (1 << 6);                                         \
    if (a < b)                                                             \
      emu->r[MN_Regs_F] |= (1 << 4);                                       \
    if ((a & 0x0F) < (b & 0x0F))                                           \
      emu->r[MN_Regs_F] |= (1 << 5);                                       \
    emu->cycles += 8;                                                      \
  }
#define MN_SUB_n()                            \
  {                                           \
    mn_u8 a = emu->r[MN_Regs_A];              \
    mn_u8 b = mn_memory_read(emu, emu->PC++); \
    mn_u16 result = a - b;                    \
    emu->r[MN_Regs_A] = result;               \
    emu->r[MN_Regs_F] = 0;                    \
    if (result == 0)                          \
      emu->r[MN_Regs_F] |= (1 << 7);          \
    emu->r[MN_Regs_F] |= (1 << 6);            \
    if (a < b)                                \
      emu->r[MN_Regs_F] |= (1 << 4);          \
    if ((a & 0x0F) < (b & 0x0F))              \
      emu->r[MN_Regs_F] |= (1 << 5);          \
    emu->cycles += 8;                         \
  }
#define MN_SBC_r(src)                       \
  {                                         \
    mn_u8 a = emu->r[MN_Regs_A];            \
    mn_u8 b = emu->r[src];                  \
    mn_u8 c = (emu->r[MN_Regs_F] >> 4) & 1; \
    mn_u16 result = a - b - c;              \
    emu->r[MN_Regs_A] = result;             \
    emu->r[MN_Regs_F] = 0;                  \
    if (result == 0)                        \
      emu->r[MN_Regs_F] |= (1 << 7);        \
    emu->r[MN_Regs_F] |= (1 << 6);          \
    if (a < b + c)                          \
      emu->r[MN_Regs_F] |= (1 << 4);        \
    if ((a & 0x0F) < (b & 0x0F) + c)        \
      emu->r[MN_Regs_F] |= (1 << 5);        \
    emu->cycles += 4;                       \
  }
#define MN_SBC_HL()                                                        \
  {                                                                        \
    mn_u8 a = emu->r[MN_Regs_A];                                           \
    mn_u8 b = mn_memory_read(emu, MN_GET_PAIR(emu, MN_Regs_H, MN_Regs_L)); \
    mn_u8 c = (emu->r[MN_Regs_F] >> 4) & 1;                                \
    mn_u16 result = a - b - c;                                             \
    emu->r[MN_Regs_A] = result;                                            \
    emu->r[MN_Regs_F] = 0;                                                 \
    if (result == 0)                                                       \
      emu->r[MN_Regs_F] |= (1 << 7);                                       \
    emu->r[MN_Regs_F] |= (1 << 6);                                         \
    if (a < b + c)                                                         \
      emu->r[MN_Regs_F] |= (1 << 4);                                       \
    if ((a & 0x0F) < (b & 0x0F) + c)                                       \
      emu->r[MN_Regs_F] |= (1 << 5);                                       \
    emu->cycles += 8;                                                      \
  }
#define MN_SBC_n()                            \
  {                                           \
    mn_u8 a = emu->r[MN_Regs_A];              \
    mn_u8 b = mn_memory_read(emu, emu->PC++); \
    mn_u8 c = (emu->r[MN_Regs_F] >> 4) & 1;   \
    mn_u16 result = a - b - c;                \
    emu->r[MN_Regs_A] = result;               \
    emu->r[MN_Regs_F] = 0;                    \
    if (result == 0)                          \
      emu->r[MN_Regs_F] |= (1 << 7);          \
    emu->r[MN_Regs_F] |= (1 << 6);            \
    if (a < b + c)                            \
      emu->r[MN_Regs_F] |= (1 << 4);          \
    if ((a & 0x0F) < (b & 0x0F) + c)          \
      emu->r[MN_Regs_F] |= (1 << 5);          \
    emu->cycles += 8;                         \
  }

#define MN_AND_r(src)                 \
  {                                   \
    emu->r[MN_Regs_A] &= emu->r[src]; \
    emu->r[MN_Regs_F] = 0;            \
    if (emu->r[MN_Regs_A] == 0)       \
      emu->r[MN_Regs_F] |= (1 << 7);  \
    emu->r[MN_Regs_F] |= (1 << 5);    \
    emu->cycles += 4;                 \
  }
#define MN_AND_HL()                                                                   \
  {                                                                                   \
    emu->r[MN_Regs_A] &= mn_memory_read(emu, MN_GET_PAIR(emu, MN_Regs_H, MN_Regs_L)); \
    emu->r[MN_Regs_F] = 0;                                                            \
    if (emu->r[MN_Regs_A] == 0)                                                       \
      emu->r[MN_Regs_F] |= (1 << 7);                                                  \
    emu->r[MN_Regs_F] |= (1 << 5);                                                    \
    emu->cycles += 8;                                                                 \
  }
#define MN_AND_n()                                       \
  {                                                      \
    emu->r[MN_Regs_A] &= mn_memory_read(emu, emu->PC++); \
    emu->r[MN_Regs_F] = 0;                               \
    if (emu->r[MN_Regs_A] == 0)                          \
      emu->r[MN_Regs_F] |= (1 << 7);                     \
    emu->r[MN_Regs_F] |= (1 << 5);                       \
    emu->cycles += 8;                                    \
  }

#define MN_OR_r(src)                  \
  {                                   \
    emu->r[MN_Regs_A] |= emu->r[src]; \
    emu->r[MN_Regs_F] = 0;            \
    if (emu->r[MN_Regs_A] == 0)       \
      emu->r[MN_Regs_F] |= (1 << 7);  \
    emu->cycles += 4;                 \
  }
#define MN_OR_HL()                                                                    \
  {                                                                                   \
    emu->r[MN_Regs_A] |= mn_memory_read(emu, MN_GET_PAIR(emu, MN_Regs_H, MN_Regs_L)); \
    emu->r[MN_Regs_F] = 0;                                                            \
    if (emu->r[MN_Regs_A] == 0)                                                       \
      emu->r[MN_Regs_F] |= (1 << 7);                                                  \
    emu->cycles += 8;                                                                 \
  }
#define MN_OR_n()                                        \
  {                                                      \
    emu->r[MN_Regs_A] |= mn_memory_read(emu, emu->PC++); \
    emu->r[MN_Regs_F] = 0;                               \
    if (emu->r[MN_Regs_A] == 0)                          \
      emu->r[MN_Regs_F] |= (1 << 7);                     \
    emu->cycles += 8;                                    \
  }
#define MN_XOR_r(src)                 \
  {                                   \
    emu->r[MN_Regs_A] ^= emu->r[src]; \
    emu->r[MN_Regs_F] = 0;            \
    if (emu->r[MN_Regs_A] == 0)       \
      emu->r[MN_Regs_F] |= (1 << 7);  \
    emu->cycles += 4;                 \
  }
#define MN_XOR_HL()                                                                   \
  {                                                                                   \
    emu->r[MN_Regs_A] ^= mn_memory_read(emu, MN_GET_PAIR(emu, MN_Regs_H, MN_Regs_L)); \
    emu->r[MN_Regs_F] = 0;                                                            \
    if (emu->r[MN_Regs_A] == 0)                                                       \
      emu->r[MN_Regs_F] |= (1 << 7);                                                  \
    emu->cycles += 8;                                                                 \
  }
#define MN_XOR_n()                                       \
  {                                                      \
    emu->r[MN_Regs_A] ^= mn_memory_read(emu, emu->PC++); \
    emu->r[MN_Regs_F] = 0;                               \
    if (emu->r[MN_Regs_A] == 0)                          \
      emu->r[MN_Regs_F] |= (1 << 7);                     \
    emu->cycles += 8;                                    \
  }

#define MN_CP_r(src)                                       \
  {                                                        \
    emu->r[MN_Regs_F] = 0;                                 \
    if (emu->r[MN_Regs_A] == emu->r[src])                  \
      emu->r[MN_Regs_F] |= (1 << 7);                       \
    emu->r[MN_Regs_F] |= (1 << 6);                         \
    if (emu->r[MN_Regs_A] < emu->r[src])                   \
      emu->r[MN_Regs_F] |= (1 << 4);                       \
    if ((emu->r[MN_Regs_A] & 0x0F) < (emu->r[src] & 0x0F)) \
      emu->r[MN_Regs_F] |= (1 << 5);                       \
                                                           \
    emu->cycles += 4;                                      \
  }
#define MN_CP_HL()                                                         \
  {                                                                        \
    emu->r[MN_Regs_F] = 0;                                                 \
    mn_u8 b = mn_memory_read(emu, MN_GET_PAIR(emu, MN_Regs_H, MN_Regs_L)); \
    if (emu->r[MN_Regs_A] == b)                                            \
      emu->r[MN_Regs_F] |= (1 << 7);                                       \
    emu->r[MN_Regs_F] |= (1 << 6);                                         \
    if (emu->r[MN_Regs_A] < b)                                             \
      emu->r[MN_Regs_F] |= (1 << 4);                                       \
    if ((emu->r[MN_Regs_A] & 0x0F) < (b & 0x0F))                           \
      emu->r[MN_Regs_F] |= (1 << 5);                                       \
                                                                           \
    emu->cycles += 8;                                                      \
  }
#define MN_CP_n()                                \
  {                                              \
    emu->r[MN_Regs_F] = 0;                       \
    mn_u8 b = mn_memory_read(emu, emu->PC++);    \
    if (emu->r[MN_Regs_A] == b)                  \
      emu->r[MN_Regs_F] |= (1 << 7);             \
    emu->r[MN_Regs_F] |= (1 << 6);               \
    if (emu->r[MN_Regs_A] < b)                   \
      emu->r[MN_Regs_F] |= (1 << 4);             \
    if ((emu->r[MN_Regs_A] & 0x0F) < (b & 0x0F)) \
      emu->r[MN_Regs_F] |= (1 << 5);             \
    emu->cycles += 8;                            \
  }

#define MN_INC_r(src)                \
  {                                  \
    emu->r[src]++;                   \
    emu->r[MN_Regs_F] &= (1 << 4);   \
    if (emu->r[src] == 0)            \
      emu->r[MN_Regs_F] |= (1 << 7); \
    if ((emu->r[src] & 0x0F) == 0)   \
      emu->r[MN_Regs_F] |= (1 << 5); \
    emu->cycles += 4;                \
  }

#define MN_INC_HL()                                                         \
  {                                                                         \
    mn_u8 hl = mn_memory_read(emu, MN_GET_PAIR(emu, MN_Regs_H, MN_Regs_L)); \
    mn_memory_write(emu, MN_GET_PAIR(emu, MN_Regs_H, MN_Regs_L), ++hl);     \
    emu->r[MN_Regs_F] &= (1 << 4);                                          \
    if (hl == 0)                                                            \
      emu->r[MN_Regs_F] |= (1 << 7);                                        \
    if ((hl & 0x0F) == 0)                                                   \
      emu->r[MN_Regs_F] |= (1 << 5);                                        \
    emu->cycles += 12;                                                      \
  }

#define MN_DEC_r(src)                 \
  {                                   \
    emu->r[src]--;                    \
    emu->r[MN_Regs_F] &= (1 << 4);    \
    if (emu->r[src] == 0)             \
      emu->r[MN_Regs_F] |= (1 << 7);  \
    emu->r[MN_Regs_F] |= (1 << 6);    \
    if ((emu->r[src] & 0x0F) == 0x0F) \
      emu->r[MN_Regs_F] |= (1 << 5);  \
    emu->cycles += 4;                 \
  }

#define MN_DEC_HL()                                                         \
  {                                                                         \
    mn_u8 hl = mn_memory_read(emu, MN_GET_PAIR(emu, MN_Regs_H, MN_Regs_L)); \
    mn_memory_write(emu, MN_GET_PAIR(emu, MN_Regs_H, MN_Regs_L), --hl);     \
    emu->r[MN_Regs_F] &= (1 << 4);                                          \
    if (hl == 0)                                                            \
      emu->r[MN_Regs_F] |= (1 << 7);                                        \
    emu->r[MN_Regs_F] |= (1 << 6);                                          \
    if ((hl & 0x0F) == 0x0F)                                                \
      emu->r[MN_Regs_F] |= (1 << 5);                                        \
    emu->cycles += 12;                                                      \
  }

#define MN_SWAP_r(src)                                                   \
  {                                                                      \
    emu->r[src] = (emu->r[src] & 0x0F) << 4 | (emu->r[src] & 0xF0) >> 4; \
    emu->r[MN_Regs_F] = 0;                                               \
    if (emu->r[src] == 0)                                                \
      emu->r[MN_Regs_F] |= (1 << 7);                                     \
    emu->cycles += 8;                                                    \
  }

#define MN_SWAP_HL()                                                        \
  {                                                                         \
    mn_u8 hl = mn_memory_read(emu, MN_GET_PAIR(emu, MN_Regs_H, MN_Regs_L)); \
    hl = (hl & 0x0F) << 4 | (hl & 0xF0) >> 4;                               \
    emu->r[MN_Regs_F] = 0;                                                  \
    if (hl == 0)                                                            \
      emu->r[MN_Regs_F] |= (1 << 7);                                        \
    emu->cycles += 16;                                                      \
  }

// aap karlo
#define MN_DAA()
#define MN_CPL()
#define MN_CCF()
#define MN_SCF()
#define MN_NOP() emu->cycles += 4;
#define MN_HALT()
#define MN_STOP()
#define MN_DI()
#define MN_EI()

#define MN_RLCA()
#define MN_RLA()
#define MN_RRCA()
#define MN_RRA()
#define MN_RCL()
#define MN_RL()

// 16 bit opcode CB xx
#define MN_RLC_r(src)
#define MN_RLC_HL()
#define MN_RL_r(src)
#define MN_RL_HL()
#define MN_RRC_r(src)
#define MN_RRC_HL()
#define MN_RR_r(src)
#define MN_RR_HL()
#define MN_SLA_r(src)
#define MN_SLA_HL()
#define MN_SRA_r(src)
#define MN_SRA_HL()
#define MN_SRL_r(src)
#define MN_SRL_HL()

#define MN_BIT_b_r(bit, src)          \
  {                                   \
    emu->r[MN_Regs_F] &= (1 << 4);    \
    if (emu->r[src] & (1 << bit))     \
      emu->r[MN_Regs_F] &= ~(1 << 7); \
    else                              \
      emu->r[MN_Regs_F] |= (1 << 7);  \
    emu->r[MN_Regs_F] |= (1 << 5);    \
    emu->cycles += 8;                 \
  }
#define MN_BIT_b_HL(bit)                                                          \
  {                                                                               \
    emu->r[MN_Regs_F] &= (1 << 4);                                                \
    if (mn_memory_read(emu, MN_GET_PAIR(emu, MN_Regs_H, MN_Regs_L)) & (1 << bit)) \
      emu->r[MN_Regs_F] &= ~(1 << 7);                                             \
    else                                                                          \
      emu->r[MN_Regs_F] |= (1 << 7);                                              \
    emu->r[MN_Regs_F] |= (1 << 5);                                                \
    emu->cycles += 16;                                                            \
  }

#define MN_SET_b_r(bit, src)   \
  {                            \
    emu->r[src] |= (1 << bit); \
    emu->cycles += 8;          \
  }
#define MN_SET_b_HL(bit)                                                    \
  {                                                                         \
    mn_u8 hl = mn_memory_read(emu, MN_GET_PAIR(emu, MN_Regs_H, MN_Regs_L)); \
    hl |= (1 << bit);                                                       \
    mn_memory_write(emu, MN_GET_PAIR(emu, MN_Regs_H, MN_Regs_L), hl);       \
    emu->cycles += 16;                                                      \
  }

#define MN_RES_b_r(bit, src)    \
  {                             \
    emu->r[src] &= ~(1 << bit); \
    emu->cycles += 8;           \
  }
#define MN_RES_b_HL(bit)                                                    \
  {                                                                         \
    mn_u8 hl = mn_memory_read(emu, MN_GET_PAIR(emu, MN_Regs_H, MN_Regs_L)); \
    hl &= ~(1 << bit);                                                      \
    mn_memory_write(emu, MN_GET_PAIR(emu, MN_Regs_H, MN_Regs_L), hl);       \
    emu->cycles += 16;                                                      \
  }

// Abh kud kudke batao, alkene alkane aur alkyne ka samband;
#define MN_JP_nn()
#define MN_JP_NZ_nn()
#define MN_JP_Z_nn()
#define MN_JP_NC_nn()
#define MN_JP_C_nn()
#define MN_JP_HL()
#define MN_JR_n()
#define MN_JR_NZ_n()
#define MN_JR_Z_n()
#define MN_JR_NC_n()
#define MN_JR_C_n()

// call me maybe
#define MN_CALL_nn()
#define MN_CALL_NZ_nn()
#define MN_CALL_Z_nn()
#define MN_CALL_NC_nn()
#define MN_CALL_C_nn()

// restarts
#define MN_RST_n()

// returns
#define MN_RET()
#define MN_RET_NZ()
#define MN_RET_Z()
#define MN_RET_NC()
#define MN_RET_C()
#define MN_RETI()

// uwa
#endif