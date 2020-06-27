#include "crc8saej1850.h"
// взято с https://gist.github.com/hunternsk/aa561a5dcba9e3faa683f34f60d95559

crc8SAEJ1850::crc8SAEJ1850()
{
// https://ru.wikipedia.org/wiki/Циклический_избыточный_код
// CRC-8-SAE J1850 standard
// for "123456789" (0x31 ... 0x39) check = 0x4B
    start = 0xFF;
    polinom = 0x1D;     // x8 + x4 + x3 + x2 + 1 (0x1D | 0x100)
    xorOut = 0xFF;
//    refin = false;
//    refout = false;

    isInited = false;
}

void crc8SAEJ1850::CRCInit()
{
    quint8 _crc;
    for (int i = 0; i < 256; i++) {
        _crc = static_cast<quint8>(i);
        for (quint8 bit = 0; bit < 8; bit++) {
            _crc = (_crc & 0x80) ? (static_cast<quint8>(_crc << 1) ^ polinom) : (static_cast<quint8>(_crc << 1));
        }
        crcTable[i] = _crc;
        isInited = true;
    }
}

quint8 crc8SAEJ1850::CalcCRC(QByteArray buf) // (uint8_t * buf, uint8_t len)
{
//    const uint8_t * ptr = buf;
    quint8 _crc = start;
    quint8 _tmp = 0;
    if(!isInited) return 0;
//    while(len--) _crc = crcTable[_crc ^ *ptr++];
    for (int i = 0; i < buf.length(); ++i) {
        _tmp = static_cast<quint8>(buf[i]);
        _crc = crcTable[ _crc ^ _tmp ];
    }
    return _crc ^ xorOut;   //return ~_crc;
}
