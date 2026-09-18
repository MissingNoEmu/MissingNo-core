#ifndef COMMONS_H
#define COMMONS_H

typedef unsigned char mn_u8;
typedef unsigned short mn_u16;
typedef unsigned long mn_size;

typedef _Bool mn_bool;
#define MN_FALSE ((_Bool)0)
#define MN_TRUE  ((_Bool)1)

typedef struct {
    mn_bool DPadL;
    mn_bool DPadR;
    mn_bool DPadU;
    mn_bool DPadD;
    mn_bool A;
    mn_bool B;
    mn_bool Select;
    mn_bool Start;
} MN_Inputs;

#endif
