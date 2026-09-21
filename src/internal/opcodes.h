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
#define MN_LD_r_HL(dest)                                     \
  {                                                          \
    emu->r[dest] = mn_memory_read(emu, MN_GET_PAIR(emu, MN_Regs_H, MN_Regs_L)); \
    emu->cycles += 8;                                        \
  }
#define MN_LD_HL_r(src)                                     \
  {                                                         \
    mn_memory_write(emu, MN_GET_PAIR(emu, MN_Regs_H, MN_Regs_L), emu->r[src]); \
    emu->cycles += 8;                                       \
  }
#define MN_LD_r_BC(dest)                                     \
  {                                                          \
    emu->r[dest] = mn_memory_read(emu, MN_GET_PAIR(emu, MN_Regs_B, MN_Regs_C)); \
    emu->cycles += 8;                                        \
  }
#define MN_LD_r_DE(dest)                                     \
  {                                                          \
    emu->r[dest] = mn_memory_read(emu, MN_GET_PAIR(emu, MN_Regs_D, MN_Regs_E)); \
    emu->cycles += 8;                                        \
  }
#define MN_LD_BC_r(src)                                     \
  {                                                         \
    mn_memory_write(emu, MN_GET_PAIR(emu, MN_Regs_B, MN_Regs_C), emu->r[src]); \
    emu->cycles += 8;                                       \
  }
#define MN_LD_DE_r(src)                                     \
  {                                                         \
    mn_memory_write(emu, MN_GET_PAIR(emu, MN_Regs_D, MN_Regs_E), emu->r[src]); \
    emu->cycles += 8;                                       \
  }

#define MN_ADD_r(src)                   \
  {                                     \
    mn_u8 a = emu->r[MN_Regs_A];                \
    mn_u8 b = emu->r[src];              \
    mn_u16 result = a + b;              \
                                        \
    emu->r[MN_Regs_A] = (mn_u8)result;          \
                                        \
    emu->r[MN_Regs_F] = 0;                      \
    if (emu->r[MN_Regs_A] == 0)                 \
      emu->r[MN_Regs_F] |= (1 << 7);            \
    if ((a & 0x0F) + (b & 0x0F) > 0x0F) \
      emu->r[MN_Regs_F] |= (1 << 5);            \
    if (result > 0xFF)                  \
      emu->r[MN_Regs_F] |= (1 << 4);            \
                                        \
    emu->cycles += 4;                   \
  }
#define MN_ADD_HL()                                     \
  {                                                     \
    mn_u8 a = emu->r[MN_Regs_A];                                \
    mn_u8 b = mn_memory_read(emu, MN_GET_PAIR(emu, MN_Regs_H, MN_Regs_L)); \
    mn_u16 result = a + b;                              \
                                                        \
    emu->r[MN_Regs_A] = (mn_u8)result;                          \
                                                        \
    emu->r[MN_Regs_F] = 0;                                      \
    if (emu->r[MN_Regs_A] == 0)                                 \
      emu->r[MN_Regs_F] |= (1 << 7);                            \
    if ((a & 0x0F) + (b & 0x0F) > 0x0F)                 \
      emu->r[MN_Regs_F] |= (1 << 5);                            \
    if (result > 0xFF)                                  \
      emu->r[MN_Regs_F] |= (1 << 4);                            \
                                                        \
    emu->cycles += 8;                                   \
  }
#define MN_ADD_n()                            \
  {                                           \
    mn_u8 a = emu->r[MN_Regs_A];                      \
    mn_u8 b = mn_memory_read(emu, emu->PC++); \
    mn_u16 result = a + b;                    \
                                              \
    emu->r[MN_Regs_A] = (mn_u8)result;                \
                                              \
    emu->r[MN_Regs_F] = 0;                            \
    if (emu->r[MN_Regs_A] == 0)                       \
      emu->r[MN_Regs_F] |= (1 << 7);                  \
    if ((a & 0x0F) + (b & 0x0F) > 0x0F)       \
      emu->r[MN_Regs_F] |= (1 << 5);                  \
    if (result > 0xFF)                        \
      emu->r[MN_Regs_F] |= (1 << 4);                  \
                                              \
    emu->cycles += 8;                         \
  }

#define MN_ADC_r(src)                       \
  {                                         \
    mn_u8 a = emu->r[MN_Regs_A];                    \
    mn_u8 b = emu->r[src];                  \
    mn_u8 c = (emu->r[MN_Regs_F] >> 4) & 1;         \
    mn_u16 result = a + b + c;              \
                                            \
    emu->r[MN_Regs_A] = (mn_u8)result;              \
                                            \
    emu->r[MN_Regs_F] = 0;                          \
    if (emu->r[MN_Regs_A] == 0)                     \
      emu->r[MN_Regs_F] |= (1 << 7);                \
    if ((a & 0x0F) + (b & 0x0F) + c > 0x0F) \
      emu->r[MN_Regs_F] |= (1 << 5);                \
    if (result > 0xFF)                      \
      emu->r[MN_Regs_F] |= (1 << 4);                \
                                            \
    emu->cycles += 4;                       \
  }
#define MN_ADC_HL()                                     \
  {                                                     \
    mn_u8 a = emu->r[MN_Regs_A];                                \
    mn_u8 b = mn_memory_read(emu, MN_GET_PAIR(emu, MN_Regs_H, MN_Regs_L)); \
    mn_u8 c = (emu->r[MN_Regs_F] >> 4) & 1;                     \
    mn_u16 result = a + b + c;                          \
                                                        \
    emu->r[MN_Regs_A] = (mn_u8)result;                          \
                                                        \
    emu->r[MN_Regs_F] = 0;                                      \
    if (emu->r[MN_Regs_A] == 0)                                 \
      emu->r[MN_Regs_F] |= (1 << 7);                            \
    if ((a & 0x0F) + (b & 0x0F) + c > 0x0F)             \
      emu->r[MN_Regs_F] |= (1 << 5);                            \
    if (result > 0xFF)                                  \
      emu->r[MN_Regs_F] |= (1 << 4);                            \
                                                        \
    emu->cycles += 8;                                   \
  }
#define MN_ADC_n()                            \
  {                                           \
    mn_u8 a = emu->r[MN_Regs_A];                      \
    mn_u8 b = mn_memory_read(emu, emu->PC++); \
    mn_u8 c = (emu->r[MN_Regs_F] >> 4) & 1;           \
    mn_u16 result = a + b + c;                \
                                              \
    emu->r[MN_Regs_A] = (mn_u8)result;                \
                                              \
    emu->r[MN_Regs_F] = 0;                            \
    if (emu->r[MN_Regs_A] == 0)                       \
      emu->r[MN_Regs_F] |= (1 << 7);                  \
    if ((a & 0x0F) + (b & 0x0F) + c > 0x0F)   \
      emu->r[MN_Regs_F] |= (1 << 5);                  \
    if (result > 0xFF)                        \
      emu->r[MN_Regs_F] |= (1 << 4);                  \
                                              \
    emu->cycles += 8;                         \
  }
#define MN_SUB_r(src)            \
  {                              \
    mn_u8 a = emu->r[MN_Regs_A];         \
    mn_u8 b = emu->r[src];       \
    mn_u16 result = a - b;       \
    emu->r[MN_Regs_A] = result;          \
    emu->r[MN_Regs_F] = 0;               \
    if (result == 0)             \
      emu->r[MN_Regs_F] |= (1 << 7);     \
    emu->r[MN_Regs_F] |= (1 << 6);       \
    if (a < b)                   \
      emu->r[MN_Regs_F] |= (1 << 4);     \
    if ((a & 0x0F) < (b & 0x0F)) \
      emu->r[MN_Regs_F] |= (1 << 5);     \
    emu->cycles += 4;            \
  }
#define MN_SUB_HL()                                     \
  {                                                     \
    mn_u8 a = emu->r[MN_Regs_A];                                \
    mn_u8 b = mn_memory_read(emu, MN_GET_PAIR(emu, MN_Regs_H, MN_Regs_L)); \
    mn_u16 result = a - b;                              \
    emu->r[MN_Regs_A] = result;                                 \
    emu->r[MN_Regs_F] = 0;                                      \
    if (result == 0)                                    \
      emu->r[MN_Regs_F] |= (1 << 7);                            \
    emu->r[MN_Regs_F] |= (1 << 6);                              \
    if (a < b)                                          \
      emu->r[MN_Regs_F] |= (1 << 4);                            \
    if ((a & 0x0F) < (b & 0x0F))                        \
      emu->r[MN_Regs_F] |= (1 << 5);                            \
    emu->cycles += 8;                                   \
  }
#define MN_SUB_n()                            \
  {                                           \
    mn_u8 a = emu->r[MN_Regs_A];                      \
    mn_u8 b = mn_memory_read(emu, emu->PC++); \
    mn_u16 result = a - b;                    \
    emu->r[MN_Regs_A] = result;                       \
    emu->r[MN_Regs_F] = 0;                            \
    if (result == 0)                          \
      emu->r[MN_Regs_F] |= (1 << 7);                  \
    emu->r[MN_Regs_F] |= (1 << 6);                    \
    if (a < b)                                \
      emu->r[MN_Regs_F] |= (1 << 4);                  \
    if ((a & 0x0F) < (b & 0x0F))              \
      emu->r[MN_Regs_F] |= (1 << 5);                  \
    emu->cycles += 8;                         \
  }
#define MN_SBC_r(src)                \
  {                                  \
    mn_u8 a = emu->r[MN_Regs_A];             \
    mn_u8 b = emu->r[src];           \
    mn_u8 c = (emu->r[MN_Regs_F] >> 4) & 1;  \
    mn_u16 result = a - b - c;       \
    emu->r[MN_Regs_A] = result;              \
    emu->r[MN_Regs_F] = 0;                   \
    if (result == 0)                 \
      emu->r[MN_Regs_F] |= (1 << 7);         \
    emu->r[MN_Regs_F] |= (1 << 6);           \
    if (a < b + c)                   \
      emu->r[MN_Regs_F] |= (1 << 4);         \
    if ((a & 0x0F) < (b & 0x0F) + c) \
      emu->r[MN_Regs_F] |= (1 << 5);         \
    emu->cycles += 4;                \
  }
#define MN_SBC_HL()                                     \
  {                                                     \
    mn_u8 a = emu->r[MN_Regs_A];                                \
    mn_u8 b = mn_memory_read(emu, MN_GET_PAIR(emu, MN_Regs_H, MN_Regs_L)); \
    mn_u8 c = (emu->r[MN_Regs_F] >> 4) & 1;                     \
    mn_u16 result = a - b - c;                          \
    emu->r[MN_Regs_A] = result;                                 \
    emu->r[MN_Regs_F] = 0;                                      \
    if (result == 0)                                    \
      emu->r[MN_Regs_F] |= (1 << 7);                            \
    emu->r[MN_Regs_F] |= (1 << 6);                              \
    if (a < b + c)                                      \
      emu->r[MN_Regs_F] |= (1 << 4);                            \
    if ((a & 0x0F) < (b & 0x0F) + c)                    \
      emu->r[MN_Regs_F] |= (1 << 5);                            \
    emu->cycles += 8;                                   \
  }
#define MN_SBC_n()                            \
  {                                           \
    mn_u8 a = emu->r[MN_Regs_A];                      \
    mn_u8 b = mn_memory_read(emu, emu->PC++); \
    mn_u8 c = (emu->r[MN_Regs_F] >> 4) & 1;           \
    mn_u16 result = a - b - c;                \
    emu->r[MN_Regs_A] = result;                       \
    emu->r[MN_Regs_F] = 0;                            \
    if (result == 0)                          \
      emu->r[MN_Regs_F] |= (1 << 7);                  \
    emu->r[MN_Regs_F] |= (1 << 6);                    \
    if (a < b + c)                            \
      emu->r[MN_Regs_F] |= (1 << 4);                  \
    if ((a & 0x0F) < (b & 0x0F) + c)          \
      emu->r[MN_Regs_F] |= (1 << 5);                  \
    emu->cycles += 8;                         \
  }

#define MN_AND_r(src)         \
  {                           \
    emu->r[MN_Regs_A] &= emu->r[src]; \
    emu->r[MN_Regs_F] = 0;            \
    if (emu->r[MN_Regs_A] == 0)       \
      emu->r[MN_Regs_F] |= (1 << 7);  \
    emu->r[MN_Regs_F] |= (1 << 5);    \
    emu->cycles += 4;         \
  }
#define MN_AND_HL()                                        \
  {                                                        \
    emu->r[MN_Regs_A] &= mn_memory_read(emu, MN_GET_PAIR(emu, MN_Regs_H, MN_Regs_L)); \
    emu->r[MN_Regs_F] = 0;                                         \
    if (emu->r[MN_Regs_A] == 0)                                    \
      emu->r[MN_Regs_F] |= (1 << 7);                               \
    emu->r[MN_Regs_F] |= (1 << 5);                                 \
    emu->cycles += 8;                                      \
  }
#define MN_AND_n()                               \
  {                                              \
    emu->r[MN_Regs_A] &= mn_memory_read(emu, emu->PC++); \
    emu->r[MN_Regs_F] = 0;                               \
    if (emu->r[MN_Regs_A] == 0)                          \
      emu->r[MN_Regs_F] |= (1 << 7);                     \
    emu->r[MN_Regs_F] |= (1 << 5);                       \
    emu->cycles += 8;                            \
  }

#define MN_OR_r(src)          \
  {                           \
    emu->r[MN_Regs_A] |= emu->r[src]; \
    emu->r[MN_Regs_F] = 0;            \
    if (emu->r[MN_Regs_A] == 0)       \
      emu->r[MN_Regs_F] |= (1 << 7);  \
    emu->cycles += 4;         \
  }
#define MN_OR_HL()                                         \
  {                                                        \
    emu->r[MN_Regs_A] |= mn_memory_read(emu, MN_GET_PAIR(emu, MN_Regs_H, MN_Regs_L)); \
    emu->r[MN_Regs_F] = 0;                                         \
    if (emu->r[MN_Regs_A] == 0)                                    \
      emu->r[MN_Regs_F] |= (1 << 7);                               \
    emu->cycles += 8;                                      \
  }
#define MN_OR_n()                                \
  {                                              \
    emu->r[MN_Regs_A] |= mn_memory_read(emu, emu->PC++); \
    emu->r[MN_Regs_F] = 0;                               \
    if (emu->r[MN_Regs_A] == 0)                          \
      emu->r[MN_Regs_F] |= (1 << 7);                     \
    emu->cycles += 8;                            \
  }
#define MN_XOR_r(src)         \
  {                           \
    emu->r[MN_Regs_A] ^= emu->r[src]; \
    emu->r[MN_Regs_F] = 0;            \
    if (emu->r[MN_Regs_A] == 0)       \
      emu->r[MN_Regs_F] |= (1 << 7);  \
    emu->cycles += 4;         \
  }
#define MN_XOR_HL()                                        \
  {                                                        \
    emu->r[MN_Regs_A] ^= mn_memory_read(emu, MN_GET_PAIR(emu, MN_Regs_H, MN_Regs_L)); \
    emu->r[MN_Regs_F] = 0;                                         \
    if (emu->r[MN_Regs_A] == 0)                                    \
      emu->r[MN_Regs_F] |= (1 << 7);                               \
    emu->cycles += 8;                                      \
  }
#define MN_XOR_n()                               \
  {                                              \
    emu->r[MN_Regs_A] ^= mn_memory_read(emu, emu->PC++); \
    emu->r[MN_Regs_F] = 0;                               \
    if (emu->r[MN_Regs_A] == 0)                          \
      emu->r[MN_Regs_F] |= (1 << 7);                     \
    emu->cycles += 8;                            \
  }

#define MN_CP_r(src)                               \
  {                                                \
    emu->r[MN_Regs_F] = 0;                                 \
    if (emu->r[MN_Regs_A] == emu->r[src])                  \
      emu->r[MN_Regs_F] |= (1 << 7);                       \
    emu->r[MN_Regs_F] |= (1 << 6);                         \
    if (emu->r[MN_Regs_A] < emu->r[src])                   \
      emu->r[MN_Regs_F] |= (1 << 4);                       \
    if ((emu->r[MN_Regs_A] & 0x0F) < (emu->r[src] & 0x0F)) \
      emu->r[MN_Regs_F] |= (1 << 5);                       \
                                                   \
    emu->cycles += 4;                              \
  }
#define MN_CP_HL()                                      \
  {                                                     \
    emu->r[MN_Regs_F] = 0;                                      \
    mn_u8 b = mn_memory_read(emu, MN_GET_PAIR(emu, MN_Regs_H, MN_Regs_L)); \
    if (emu->r[MN_Regs_A] == b)                                 \
      emu->r[MN_Regs_F] |= (1 << 7);                            \
    emu->r[MN_Regs_F] |= (1 << 6);                              \
    if (emu->r[MN_Regs_A] < b)                                  \
      emu->r[MN_Regs_F] |= (1 << 4);                            \
    if ((emu->r[MN_Regs_A] & 0x0F) < (b & 0x0F))                \
      emu->r[MN_Regs_F] |= (1 << 5);                            \
                                                        \
    emu->cycles += 8;                                   \
  }
#define MN_CP_n()                             \
  {                                           \
    emu->r[MN_Regs_F] = 0;                            \
    mn_u8 b = mn_memory_read(emu, emu->PC++); \
    if (emu->r[MN_Regs_A] == b)                       \
      emu->r[MN_Regs_F] |= (1 << 7);                  \
    emu->r[MN_Regs_F] |= (1 << 6);                    \
    if (emu->r[MN_Regs_A] < b)                        \
      emu->r[MN_Regs_F] |= (1 << 4);                  \
    if ((emu->r[MN_Regs_A] & 0x0F) < (b & 0x0F))      \
      emu->r[MN_Regs_F] |= (1 << 5);                  \
    emu->cycles += 8;                         \
  }

#define MN_INC_r(src)              \
  {                                \
    emu->r[src]++;                 \
    emu->r[MN_Regs_F] &= (1 << 4);         \
    if (emu->r[src] == 0)          \
      emu->r[MN_Regs_F] |= (1 << 7);       \
    if ((emu->r[src] & 0x0F) == 0) \
      emu->r[MN_Regs_F] |= (1 << 5);       \
    emu->cycles += 4;              \
  }

#define MN_INC_HL()                                      \
  {                                                      \
    mn_u8 hl = mn_memory_read(emu, MN_GET_PAIR(emu, MN_Regs_H, MN_Regs_L)); \
    mn_memory_write(emu, MN_GET_PAIR(emu, MN_Regs_H, MN_Regs_L), ++hl);     \
    emu->r[MN_Regs_F] &= (1 << 4);                               \
    if (hl == 0)                                         \
      emu->r[MN_Regs_F] |= (1 << 7);                             \
    if ((hl & 0x0F) == 0)                                \
      emu->r[MN_Regs_F] |= (1 << 5);                             \
    emu->cycles += 12;                                   \
  }

#define MN_DEC_r(src)                 \
  {                                   \
    emu->r[src]--;                    \
    emu->r[MN_Regs_F] &= (1 << 4);            \
    if (emu->r[src] == 0)             \
      emu->r[MN_Regs_F] |= (1 << 7);          \
    emu->r[MN_Regs_F] |= (1 << 6);            \
    if ((emu->r[src] & 0x0F) == 0x0F) \
      emu->r[MN_Regs_F] |= (1 << 5);          \
    emu->cycles += 4;                 \
  }

#define MN_DEC_HL()                                      \
  {                                                      \
    mn_u8 hl = mn_memory_read(emu, MN_GET_PAIR(emu, MN_Regs_H, MN_Regs_L)); \
    mn_memory_write(emu, MN_GET_PAIR(emu, MN_Regs_H, MN_Regs_L), --hl);     \
    emu->r[MN_Regs_F] &= (1 << 4);                               \
    if (hl == 0)                                         \
      emu->r[MN_Regs_F] |= (1 << 7);                             \
    emu->r[MN_Regs_F] |= (1 << 6);                               \
    if ((hl & 0x0F) == 0x0F)                             \
      emu->r[MN_Regs_F] |= (1 << 5);                             \
    emu->cycles += 12;                                   \
  }

#endif