#ifndef PACKCRC_H
#define PACKCRC_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QTextCodec>

class cPackCRC : public QWidget
{
    Q_OBJECT
private:
     QVBoxLayout * verLayout;
     QHBoxLayout * horLayout;
     QLabel * lPackArr;
     QLineEdit * lePackArr;
     QSpacerItem * hSpacerArr;
     QLabel * lCRCArr;
     QSpinBox * spinbCRCArr;

     QByteArray  pack;
     quint32     crc;
     quint32     number;
public:
    explicit cPackCRC(QWidget *parent = 0);
    cPackCRC(const quint32 num) : cPackCRC() {setNum(num);}
    cPackCRC(const quint32 num, const QByteArray pck, const quint32 inp_crc) : cPackCRC()
        {setNum(num); setPack(pck); setCRC(inp_crc);}
    cPackCRC(const cPackCRC &pck) : cPackCRC()
        {setNum(pck.number); setPack(pck.pack); setCRC(pck.crc);}
    ~cPackCRC()
        {

        }
    void setPack(const QByteArray &pck);
    void setNum(const quint32 num);
    void setCRC(const quint32 inp_crc);
    QByteArray getPack() const {return pack;}
    quint32 getNumber() const {return number;}
    quint32 getCRC() const {return crc;}

public slots:
     void   packedited();
     void   setCRC(int inp_crc);
};

#endif // PACKCRC_H
