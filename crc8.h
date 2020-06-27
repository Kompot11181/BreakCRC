#ifndef CRC8_H
#define CRC8_H
#include <QtCore>

enum class ERR_CODE
{
    ERR_NO_ERROR = 0,
    ERR_WRONG_ORDER,
    ERR_WRONG_POLY,
    ERR_WRONG_CRCINIT,
    ERR_WRONG_CRCXOR,
    ERR_EMPTY_DATA,
    ERR_UNKNOWN,
    ERR_MAX_ERROR
};

static const QString ErrorMsg[static_cast<int>(ERR_CODE::ERR_MAX_ERROR)] = {
    "",
    "Invalid order",
    "Invalid polynom",
    "Invalid CRC init byte",
    "Invalid CRC Xor byte",
    "Unknown CRC error"
};

class ccrc8
{
private:
    static const int lengthOfTable = 256;   // длина теблицы CRC всегда 256 зачений
    unsigned int order = 8;          // основание; для CRC8 естественно всегда 8
    bool direct = true;              // true - прямой алгоритм чтения последовательности
    bool refin = true;               // true - отобразить входящие данные перед расчётом
    bool refout = true;              // true - отобразить полученный CRC перед маской XOR
    quint32 polynom;                 // полином для расчёта таблицы CRC
    quint32 crcinit;                 // начальное значение CRC
    quint32 crcxor;                  // маска XOR для наложения после расчёта
    quint32 crc;                     // значение CRC конечное
    const char * data;               // указатель на данные для расчёта контрольной суммы
    unsigned long length;            // длина данных в байтах
    quint32 crcmask;                 // маска AND (длина CRC)
    quint32 crchighbit;              // маска AND старшего бита (длина CRC)
    quint32 crcinit_direct;          // значение CRC для прямого алгоритма
    quint32 crcinit_nondirect;       // зеркальное значение CRC
    quint32 crctab[lengthOfTable];   // таблица CRC
    bool isTableChange = true;       // флаг необходимости пересчёта таблицы CRC
    bool isSettingChange = true;     // флаг необходимости пересчёта вспомогательных параметров
public:
    ccrc8();
    void setOrder(const int ord)
    {
        order = static_cast<quint32>(ord);
        crcmask = (((static_cast<quint32>(1)<<(order-1))-1)<<1)|1;
        crchighbit = static_cast<quint32>(1)<<(order-1);
        isTableChange = true;
    }
    void setPolynom(const quint32 poly) {if(polynom != poly){polynom = poly; isTableChange = true;}}
    void setReflectIn(const bool ref = true) {if(refin != ref){refin = ref; isTableChange = true;}}
    void setReflectOut(const bool ref = true) {if(refout != ref){refout = ref; isSettingChange = true;}}
    void setDirection(const bool dir = true) {if(direct != dir){direct = dir; isSettingChange = true;}}
    void setCrcInit(const quint32 init) {if(crcinit != init){crcinit = init; isSettingChange = true;}}
    void setCrcXor(const quint32 xorc) {crcxor = xorc;}
    void setData(const char * dta, int len) {data = dta; length = len;}
    bool getDirection() const {return direct;}
    bool getReflectIn() const {return refin;}
    bool getReflectOut() const {return refout;}
    quint32 getOrder() const {return order;}
    quint32 getPolynom() const {return polynom;}
    quint32 getCrcInit() const {return crcinit;}
    quint32 getCrcXor() const {return crcxor;}
    quint32 getCRC() const {return crc;}
    quint32 getCrcMask() const {return crcmask;}
    quint32 getCrcHighBit() const {return crchighbit;}
    const quint32 *getCrcTable() const {return crctab;}
    quint32 getCrcTable(int cnt) const {return crctab[cnt];}

    int getCrcError();                                      // проверка корректности введённых значений
    static const QString getCrcErrorMsg(int err);

    quint32 reflect (quint32 crc_inp, int bitnum);
    void generate_crc_table();
    quint32 crctablefast (const char* p, unsigned long len);
    quint32 crctable (const char* p, unsigned long len);
    quint32 crcbitbybit(const char* p, unsigned long len);
    quint32 crcbitbybitfast(const char* p, unsigned long len);
    int calculate();
};

#endif // CRC8_H
