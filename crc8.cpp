#include "crc8.h"

ccrc8::ccrc8()
{
    direct = true;
    refin = false;
    refout = false;
    crcmask = ((((quint32)1<<(order-1))-1)<<1)|1;
    crchighbit = (quint32)1<<(order-1);
    crcinit = 0;    //crcmask;
    crcxor = 0;     //crcmask;
    polynom = 0x31;
    length = 0;
    isTableChange = true;
    isSettingChange = true;
}

int ccrc8::getCrcError()
{
    // check parameters

    if (order < 1 || order > 32) {
        return static_cast<int>(ERR_CODE::ERR_WRONG_ORDER);
    }

    if (polynom != (polynom & crcmask)) {
        return static_cast<int>(ERR_CODE::ERR_WRONG_POLY);
    }

    if (crcinit != (crcinit & crcmask)) {
        return static_cast<int>(ERR_CODE::ERR_WRONG_CRCINIT);
    }

    if (crcxor != (crcxor & crcmask)) {
        return static_cast<int>(ERR_CODE::ERR_WRONG_CRCXOR);
    }
    if (length < 1) {
        return static_cast<int>(ERR_CODE::ERR_EMPTY_DATA);
    }
    if (data == NULL) {
         return static_cast<int>(ERR_CODE::ERR_UNKNOWN);
    }
    return static_cast<int>(ERR_CODE::ERR_NO_ERROR);
}

const QString ccrc8::getCrcErrorMsg(int err)
{
    return ErrorMsg[err];
}

quint32 ccrc8::reflect (quint32 crc_inp, int bitnum) {

    // reflects the lower 'bitnum' bits of 'crc'

    quint32 i, j=1, crcout=0;

    for (i=(quint32)1<<(bitnum-1); i; i>>=1) {
        if (crc_inp & i) crcout|=j;
        j<<= 1;
    }
    return (crcout);
}



void ccrc8::generate_crc_table() {

    // make CRC lookup table used by table algorithms

    int i, j;
    quint32 bit, crcout;

    for (i=0; i<256; i++) {

        crcout=(quint32)i;
        if (refin) crcout=reflect(crcout, 8);
        crcout <<= order-8;

        for (j=0; j<8; j++) {

            bit = crcout & crchighbit;
            crcout <<= 1;
            if (bit) crcout ^= polynom;
        }

        if (refin) crcout = reflect(crcout, order);
        crcout &= crcmask;
        crctab[i]= crcout;
    }
    isTableChange = false;
    isSettingChange = true;
}



quint32 ccrc8::crctablefast (const char* p, unsigned long len) {

    // fast lookup table algorithm without augmented zero bytes, e.g. used in pkzip.
    // only usable with polynom orders of 8, 16, 24 or 32.

    quint32 crcout = crcinit_direct;

    if (refin) crcout = reflect(crcout, order);

    if (!refin)
    {
        while (len--)
        {
            crcout = (crcout << 8) ^ crctab[ ((crcout >> (order-8)) & 0xff) ^ *p++];
        }
    }
    else
    {
        while (len--)
        {
            crcout = (crcout >> 8) ^ crctab[ (crcout & 0xff) ^ *p++];
        }
    }

    if (refout^refin) crcout = reflect(crcout, order);
    crcout ^= crcxor;
    crcout &= crcmask;

    return crcout;
}



quint32 ccrc8::crctable (const char* p, unsigned long len) {

    // normal lookup table algorithm with augmented zero bytes.
    // only usable with polynom orders of 8, 16, 24 or 32.

    quint32 crcout = crcinit_nondirect;

    if (refin) crcout = reflect(crcout, order);

    if (!refin) while (len--) crcout = ((crcout << 8) | *p++) ^ crctab[ (crcout >> (order-8))  & 0xff];
    else while (len--) crcout = ((crcout >> 8) | (*p++ << (order-8))) ^ crctab[crcout & 0xff];

    if (!refin) while (++len < order/8) crcout = (crcout << 8) ^ crctab[ (crcout >> (order-8))  & 0xff];
    else while (++len < order/8) crcout = (crcout >> 8) ^ crctab[crcout & 0xff];

    if (refout^refin) crcout = reflect(crcout, order);
    crcout ^= crcxor;
    crcout &= crcmask;

    return(crcout);
}



quint32 ccrc8::crcbitbybit(const char* p, unsigned long len) {

    // bit by bit algorithm with augmented zero bytes.
    // does not use lookup table, suited for polynom orders between 1...32.

    quint32 i, j, c, bit;
    quint32 crcout = crcinit_nondirect;

    for (i=0; i<len; i++) {

        c = (quint32)*p++;
        if (refin) c = reflect(c, 8);

        for (j=0x80; j; j>>=1) {

            bit = crcout & crchighbit;
            crcout <<= 1;
            if (c & j) crcout |= 1;
            if (bit) crcout ^= polynom;
        }
    }

    for (i=0; i<order; i++) {

        bit = crcout & crchighbit;
        crcout <<= 1;
        if (bit) crcout ^= polynom;
    }

    if (refout) crcout = reflect(crcout, order);
    crcout ^= crcxor;
    crcout &= crcmask;

    return(crcout);
}



quint32 ccrc8::crcbitbybitfast(const char* p, unsigned long len) {

    // fast bit by bit algorithm without augmented zero bytes.
    // does not use lookup table, suited for polynom orders between 1...32.

    quint32 i, j, c, bit;
    quint32 crcout = crcinit_direct;

    for (i=0; i < len; i++) {

        c = (quint32)*p++;
        if (refin) c = reflect(c, 8);

        for (j=0x80; j; j>>=1) {

            bit = crcout & crchighbit;
            crcout <<= 1;
            if (c & j) bit ^= crchighbit;
            if (bit) crcout ^= polynom;
        }
    }

    if (refout) crcout = reflect(crcout, order);
    crcout ^= crcxor;
    crcout &= crcmask;

    return(crcout);
}

int ccrc8::calculate()
{
    if(isTableChange || isSettingChange)
    {
        int err = getCrcError();
        if(err) return err;
    }
    // generate lookup table
    if (isTableChange) generate_crc_table();


    // compute missing initial CRC value
    if (isSettingChange)
    {
        unsigned long bit;
        unsigned int i;
        if (!direct)
        {
            crcinit_nondirect = crcinit;
            crc = crcinit;
            for (i=0; i < order; i++) {

                bit = crc & crchighbit;
                crc<<= 1;
                if (bit) crc^= polynom;
            }
            crc &= crcmask;
            crcinit_direct = crc;
        }
        else
        {
            crcinit_direct = crcinit;
            crc = crcinit;
            for (i=0; i<order; i++) {

                bit = crc & 1;
                if (bit) crc^= polynom;
                crc >>= 1;
                if (bit) crc|= crchighbit;
            }
            crcinit_nondirect = crc;
        }
    }

    crc = crctablefast(data, length);
    return static_cast<int>(ERR_CODE::ERR_NO_ERROR);
}

