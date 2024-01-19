#include <stdlib.h>
#include "psw.h"
#include "mem.h"

unsigned carry[2][2][2] = { 0, 0, 1, 0, 1, 0, 1, 1 };
unsigned overflow[2][2][2] = { 0, 1, 0, 0, 0, 0, 1, 0 };

void update_psw(unsigned short src, unsigned short dst, unsigned short res,
    unsigned short wb)
{
    /*
     - Update 4 PSW bits (V, N, Z, C)
     - Using src, dst, and res values and whether word or byte
     - For ADD -> CMP
    */
    unsigned short mss, msd, msr; /* Most significant src, dst, and res bits */

    if (wb == 0)
    {
        mss = B15(src);
        msd = B15(dst);
        msr = B15(res);
    }
    else /* Byte */
    {
        mss = B7(src);
        msd = B7(dst);
        msr = B7(res);
        res &= 0x00FF;	/* Mask high byte for 'z' check */
    }

    /* Carry */
    pswptr->bit.C = carry[mss][msd][msr];
    /* Zero */
    pswptr->bit.Z = (res == 0);
    /* Negative */
    pswptr->bit.N = (msr == 1);
    /* oVerflow */
    pswptr->bit.V = overflow[mss][msd][msr];
}

void update_pswNZ(unsigned short res, unsigned short wb) {
    
    /*
     - Update 2 PSW bits (N, Z only)
     - Using only res values and whether word or byte
     - For XOR -> BIS
    */

    unsigned short msr; /* Most significant res bits */

    if (wb == 0)
    {
        msr = B15(res);
    }
    else /* Byte */
    {
        msr = B7(res);
        res &= 0x00FF;	/* Mask high byte for 'z' check */
    }

    /* Zero */
    pswptr->bit.Z = (res == 0);
    /* Negative */
    pswptr->bit.N = (msr == 1);

}

void pswinit() {
    pswptr = (PSW*)malloc(sizeof(PSW));
    if (pswptr == NULL) {
        printf("Memory allocation failed.\n");
    }
    else {
        // Zero out the PSW register
        memset(pswptr, 0, sizeof(PSW));
    }
}
