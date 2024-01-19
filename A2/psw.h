#pragma once
#ifndef PSW_H
#define PSW_H

#include "def.c"

typedef struct PSWbit {
    unsigned C : 1;
    unsigned Z : 1;
    unsigned N : 1;
    unsigned SLP : 1;
    unsigned V : 1;
    unsigned currPriority : 3;
    unsigned flt : 1;
    unsigned : 4;
    unsigned prevPriority : 3;
}PSWbit;

typedef union PSW {
    unsigned short word;
    PSWbit bit;
}PSW;

//  Create an instance of the PSW register to store all PSW values
PSW* pswptr;

extern void update_psw(unsigned short src, unsigned short dst, unsigned short res, unsigned short wb);

extern void update_pswNZ(unsigned short res, unsigned short wb);

extern void pswinit();

#endif PSW_H
