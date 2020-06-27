#include "packcrc.h"
#include <QDebug>

cPackCRC::cPackCRC(QWidget *parent) : QWidget(parent)
{
//    wPackArr = new QWidget();
//    wPackArr->setMinimumSize(QSize(0, 25));
//    wPackArr->setStatusTip("Example pack #" + QString::number(pack.length()));
    this->setMinimumSize(QSize(0, 25));
    verLayout = new QVBoxLayout(this);
    verLayout->setSpacing(6);
    verLayout->setContentsMargins(0, 0, 0, 0);
    horLayout = new QHBoxLayout();
    horLayout->setSpacing(6);
    lPackArr = new QLabel(this);
    lPackArr->setAlignment(Qt::AlignLeading|Qt::AlignRight|Qt::AlignVCenter);
    lPackArr->setMargin(5);
    lPackArr->setMinimumWidth(70);
    horLayout->addWidget(lPackArr);
    lePackArr = new QLineEdit(this);
    lePackArr->setMinimumWidth(50);
    horLayout->addWidget(lePackArr);
    hSpacerArr = new QSpacerItem(13, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);
    horLayout->addItem(hSpacerArr);
    lCRCArr = new QLabel(this);
    lCRCArr->setMinimumSize(QSize(20, 0));
    lCRCArr->setMaximumSize(QSize(20, 16777215));
    lCRCArr->setText("CRC:");
    horLayout->addWidget(lCRCArr);
    spinbCRCArr = new QSpinBox(this);
    spinbCRCArr->setMinimumSize(QSize(50, 0));
    spinbCRCArr->setMaximumSize(QSize(50, 16777215));
    spinbCRCArr->setWrapping(true);
    spinbCRCArr->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    spinbCRCArr->setMaximum(255);
    spinbCRCArr->setDisplayIntegerBase(16);
    horLayout->addWidget(spinbCRCArr);
    verLayout->addLayout(horLayout);
    setNum(0); setCRC(0); setPack(QByteArray());
    connect(lePackArr, SIGNAL(returnPressed()), this, SLOT(packedited()));
    connect(spinbCRCArr,SIGNAL(valueChanged(int)), this, SLOT(setCRC(int)));
}

void cPackCRC::setPack(const QByteArray &pck)
{
    pack = pck;
    QString str = pck.toHex().toUpper();
    for (int i = str.length()-2; i > 1; i-=2)
        str.insert(i, ' ');
    lePackArr->setText(str);
    lePackArr->setToolTip("Pack #" + QString::number(number) + ": " + str);
}

void cPackCRC::setNum(const quint32 num)
{
    number = num;
    this->setStatusTip("Example pack #" + QString::number(number));
    lPackArr->setText("Pack #" + QString::number(number) + ":");
    lePackArr->setStatusTip("Example data pack #" + QString::number(number));
    spinbCRCArr->setStatusTip("Example CRC of data pack #" + QString::number(number));
}

void cPackCRC::setCRC(const quint32 inp_crc)
{
    crc = inp_crc;
    if (spinbCRCArr->value() != (int)crc)
        spinbCRCArr->setValue(crc);
}

void cPackCRC::setCRC(const int inp_crc)
{
    crc = inp_crc;
    if (spinbCRCArr->value() != (int)crc)
        spinbCRCArr->setValue(crc);
}


void cPackCRC::packedited()
{
    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
    QByteArray str = codec->fromUnicode(lePackArr->text());
    QByteArray hex = QByteArray::fromHex(str);
    setPack(hex);
}
