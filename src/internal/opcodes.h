#ifndef OPCODES_H
#define OPCODES_H
#include "internal/emulator.h"
#include "internal/memory.h"

// feel free to change the names, im js typing shi

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
    emu->r[dest] = mn_memory_read(emu, get_pair(emu, H, L)); \
    emu->cycles += 8;                                        \
  }
#define MN_LD_HL_r(src)                                     \
  {                                                         \
    mn_memory_write(emu, get_pair(emu, H, L), emu->r[src]); \
    emu->cycles += 8;                                       \
  }
#define MN_LD_r_BC(dest)                                     \
  {                                                          \
    emu->r[dest] = mn_memory_read(emu, get_pair(emu, B, C)); \
    emu->cycles += 8;                                        \
  }
#define MN_LD_r_DE(dest)                                     \
  {                                                          \
    emu->r[dest] = mn_memory_read(emu, get_pair(emu, D, E)); \
    emu->cycles += 8;                                        \
  }
#define MN_LD_BC_r(src)                                     \
  {                                                         \
    mn_memory_write(emu, get_pair(emu, B, C), emu->r[src]); \
    emu->cycles += 8;                                       \
  }
#define MN_LD_DE_r(src)                                     \
  {                                                         \
    mn_memory_write(emu, get_pair(emu, D, E), emu->r[src]); \
    emu->cycles += 8;                                       \
  }

#define MN_ADD_r(src)                   \
  {                                     \
    mn_u8 a = emu->r[A];                \
    mn_u8 b = emu->r[src];              \
    mn_u16 result = a + b;              \
                                        \
    emu->r[A] = (mn_u8)result;          \
                                        \
    emu->r[F] &= 0x0F;                  \
    if (emu->r[A] == 0)                 \
      emu->r[F] |= (1 << 7);            \
    if ((a & 0x0F) + (b & 0x0F) > 0x0F) \
      emu->r[F] |= (1 << 5);            \
    if (result > 0xFF)                  \
      emu->r[F] |= (1 << 4);            \
                                        \
    emu->cycles += 4;                   \
  }
#define MN_ADD_HL()                                     \
  {                                                     \
    mn_u8 a = emu->r[A];                                \
    mn_u8 b = mn_memory_read(emu, get_pair(emu, H, L)); \
    mn_u16 result = a + b;                              \
                                                        \
    emu->r[A] = (mn_u8)result;                          \
                                                        \
    emu->r[F] &= 0x0F;                                  \
    if (emu->r[A] == 0)                                 \
      emu->r[F] |= (1 << 7);                            \
    if ((a & 0x0F) + (b & 0x0F) > 0x0F)                 \
      emu->r[F] |= (1 << 5);                            \
    if (result > 0xFF)                                  \
      emu->r[F] |= (1 << 4);                            \
                                                        \
    emu->cycles += 8;                                   \
  }
#define MN_ADD_n()                            \
  {                                           \
    mn_u8 a = emu->r[A];                      \
    mn_u8 b = mn_memory_read(emu, emu->PC++); \
    mn_u16 result = a + b;                    \
                                              \
    emu->r[A] = (mn_u8)result;                \
                                              \
    emu->r[F] &= 0x0F;                        \
    if (emu->r[A] == 0)                       \
      emu->r[F] |= (1 << 7);                  \
    if ((a & 0x0F) + (b & 0x0F) > 0x0F)       \
      emu->r[F] |= (1 << 5);                  \
    if (result > 0xFF)                        \
      emu->r[F] |= (1 << 4);                  \
                                              \
    emu->cycles += 8;                         \
  }

#define MN_ADC_r(src) \
  {                   \
  }
#define MN_ADC_HL() \
  {                 \
  }
#define MN_ADC_n() \
  {                \
  }
#define MN_SUB_r(src)         \
  {                           \
    mn_u8 a = emu->r[A];      \
    mn_u8 b = emu->r[src];    \
    mn_u16 result = a - b;    \
    emu->r[A] = result;       \
    /* sm bs badme karunga */ \
    emu->cycles += 4;         \
  }
#define MN_SUB_HL()                                     \
  {                                                     \
    mn_u8 a = emu->r[A];                                \
    mn_u8 b = mn_memory_read(emu, get_pair(emu, H, L)); \
    mn_u16 result = a - b;                              \
    emu->r[A] = result;                                 \
    /* sm bs badme karunga */                           \
    emu->cycles += 8;                                   \
  }
#define MN_SUB_n()                            \
  {                                           \
    mn_u8 a = emu->r[A];                      \
    mn_u8 b = mn_memory_read(emu, emu->PC++); \
    mn_u16 result = a - b;                    \
    emu->r[A] = result;                       \
    /* sm bs badme karunga */                 \
    emu->cycles += 8;                         \
  }
#define MN_SBC_r(src) \
  {                   \
  }
#define MN_SBC_HL() \
  {                 \
  }
#define MN_SBC_n() \
  {                \
  }

#define MN_AND_r(src)         \
  {                           \
    emu->r[A] &= emu->r[src]; \
    emu->r[F] = 0;            \
    if (emu->r[A] == 0)       \
      emu->r[F] |= (1 << 7);  \
    emu->r[F] |= (1 << 5);    \
    emu->cycles += 4;         \
  }
#define MN_AND_HL()                                        \
  {                                                        \
    emu->r[A] &= mn_memory_read(emu, get_pair(emu, H, L)); \
    emu->r[F] = 0;                                         \
    if (emu->r[A] == 0)                                    \
      emu->r[F] |= (1 << 7);                               \
    emu->r[F] |= (1 << 5);                                 \
    emu->cycles += 8;                                      \
  }
#define MN_AND_n()                               \
  {                                              \
    emu->r[A] &= mn_memory_read(emu, emu->PC++); \
    emu->r[F] = 0;                               \
    if (emu->r[A] == 0)                          \
      emu->r[F] |= (1 << 7);                     \
    emu->r[F] |= (1 << 5);                       \
    emu->cycles += 8;                            \
  }

#define MN_OR_r(src)          \
  {                           \
    emu->r[A] |= emu->r[src]; \
    emu->r[F] = 0;            \
    if (emu->r[A] == 0)       \
      emu->r[F] |= (1 << 7);  \
    emu->cycles += 4;         \
  }
#define MN_OR_HL()                                         \
  {                                                        \
    emu->r[A] |= mn_memory_read(emu, get_pair(emu, H, L)); \
    emu->r[F] = 0;                                         \
    if (emu->r[A] == 0)                                    \
      emu->r[F] |= (1 << 7);                               \
    emu->cycles += 8;                                      \
  }
#define MN_OR_n()                                \
  {                                              \
    emu->r[A] |= mn_memory_read(emu, emu->PC++); \
    emu->r[F] = 0;                               \
    if (emu->r[A] == 0)                          \
      emu->r[F] |= (1 << 7);                     \
    emu->cycles += 8;                            \
  }
#define MN_XOR_r(src)          \
  {                           \
    emu->r[A] ^= emu->r[src]; \
    emu->r[F] = 0;            \
    if (emu->r[A] == 0)       \
      emu->r[F] |= (1 << 7);  \
    emu->cycles += 4;         \
  }
#define MN_XOR_HL()                                         \
  {                                                        \
    emu->r[A] ^= mn_memory_read(emu, get_pair(emu, H, L)); \
    emu->r[F] = 0;                                         \
    if (emu->r[A] == 0)                                    \
      emu->r[F] |= (1 << 7);                               \
    emu->cycles += 8;                                      \
  }
#define MN_XOR_n()                                \
  {                                              \
    emu->r[A] ^= mn_memory_read(emu, emu->PC++); \
    emu->r[F] = 0;                               \
    if (emu->r[A] == 0)                          \
      emu->r[F] |= (1 << 7);                     \
    emu->cycles += 8;                            \
  }
#endif