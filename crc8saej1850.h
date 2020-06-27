#ifndef CRC8SAEJ1850_H
#define CRC8SAEJ1850_H

#include <QtCore>

// класс, описывающий алгоритм CRC-8-SAE J1850
// x8 + x4 + x3 + x2 + 1 (0x1D | 0x100)
// Нормальное представление:  0x1D
// Реверсивное представление: 0xB8
// Реверснивное от обратного: 0x8E

class crc8SAEJ1850
{
public:
    crc8SAEJ1850();
    void CRCInit(void);
    quint8 CalcCRC(QByteArray buf);
    quint8 crcTable[256];
    bool isInited;
    quint8 start;
    quint8 polinom;
    quint8 xorOut;
};

#endif // CRC8SAEJ1850_H
