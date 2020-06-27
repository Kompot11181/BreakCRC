#include "mainwindow.h"
#include "ui_mainwindow.h"

quint32 CalcCRC(QString str, quint32 poly, quint32 init = 0, quint32 crcxor = 0, bool refin = true, bool refout = false, bool dir = true);

bool compareCRCarray(quint32 * arrptr1, quint32 * arrptr2, int len, quint32 &mask, quint32 &diff);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    maintitle = windowTitle();
    // set dynamic widgets of cPackCRC in MainWindow
    QWidget * tempContainer = new QWidget();    // требуется дополнительная обёртка, иначе
    tempContainer->setLayout(ui->vLayout);
    ui->scrollArea->setWidget(tempContainer);   // иначе слой в scrollArea не добавляется
    // set ProgressBar on StatusBar
    progressbar = new QProgressBar;
    progressbar->setMaximum(255);
    progressbar->setFixedSize(150,12);
    statusBar()->addPermanentWidget(progressbar);
    // set TaskbarProgress on StatusBar
//    buttontask = new QWinTaskbarButton(this);
//    buttontask->setWindow(windowHandle());
//    buttontask->setOverlayIcon(style()->standardIcon(QStyle::SP_DirHomeIcon));

//    progresstask = buttontask->progress();
//    progresstask->setRange(0, 256);
//    progresstask->setValue(0);
    // add one cPackCRC widget into window
    MainWindow::on_pbAdd_clicked();
    ui->spinBoxPoly->setRange    (0x80000000, 0x7FFFFFFF);
    ui->spinBoxInitCRC->setRange (0x80000000, 0x7FFFFFFF);
    ui->spinBoxFinalXOR->setRange(0x80000000, 0x7FFFFFFF);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbAdd_clicked()
{
    //    cPackCRC * wPackArr;
    //    if(pack.length() == 0)
    //        wPackArr = new cPackCRC;
    //     else
    //        wPackArr = new cPackCRC(pack.length(), pack.at(pack.length()-1)->pack, pack.at(pack.length()-1)->crc);
    //    ui->vLayout->addWidget(wPackArr);
    //    pack.append(wPackArr);

    if(pack.length() == 0)
        pack.append(new cPackCRC);
    else
        pack.append(new cPackCRC(static_cast<quint32>(pack.length()), pack.last()->getPack(), pack.last()->getCRC()));
    ui->vLayout->addWidget(pack.last());
}

void MainWindow::on_pbMinus_clicked()
{
    if(pack.length() > 1)
    {
        delete pack.last();
        pack.pop_back();
    }
}

void MainWindow::on_spinBoxInitCRC_valueChanged(int arg1)
{
    ui->spinBoxInitCRC->setToolTip("0b" + QString::number(arg1, 2));
    ui->leCRC->setText(""); // очисить CRC
}

void MainWindow::on_spinBoxPoly_valueChanged(int arg1)
{
    ui->spinBoxPoly->setToolTip("0b" + QString::number(arg1, 2));
    ui->leCRC->setText(""); // очисить CRC
}

void MainWindow::on_spinBoxFinalXOR_valueChanged(int arg1)
{
    ui->spinBoxFinalXOR->setToolTip("0b" + QString::number(arg1, 2));
    ui->leCRC->setText(""); // очисить CRC
}

void MainWindow::on_lePack_returnPressed()
{
    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
    QByteArray s = codec->fromUnicode(ui->lePack->text());
    QByteArray hex = QByteArray::fromHex(s);
    QString str = hex.toHex().toUpper();
    for (int i = str.length()-2; i > 1; i-=2)
        str.insert(i, ' ');
    ui->lePack->setText(str);
    on_pbCalculate_clicked();
    //    ui->leCRC->setText(""); // очисить CRC
}

void MainWindow::on_pbCalculate_clicked()
{
    //    qDebug() << ui->lePack->text() << ui->spinBoxPoly->value();
    if (ui->lePack->text().length())
    {
        QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
        QByteArray s = codec->fromUnicode(ui->lePack->text());
        QByteArray hex = QByteArray::fromHex(s);
        crc8SAEJ1850 crc;
        crc.polinom = static_cast<quint8>(ui->spinBoxPoly->value());
        crc.start = static_cast<quint8>(ui->spinBoxInitCRC->value());
        crc.xorOut = static_cast<quint8>(ui->spinBoxFinalXOR->value());
        crc.CRCInit();
        ui->leCRC->setText(QString::number(crc.CalcCRC(hex), 16).toUpper());
//        ui->leCRC->setText(QString::number(
//                               CalcCRC(ui->lePack->text(),
//                                       static_cast<quint32>(ui->spinBoxPoly->value()),
//                                       static_cast<quint32>(ui->spinBoxInitCRC->value()),
//                                       static_cast<quint32>(ui->spinBoxFinalXOR->value()),
//                                       ui->checkBoxRefIn->checkState(),
//                                       ui->checkBoxRefOut->checkState(),
//                                       ui->checkBoxDirect->checkState()),
//                               16).toUpper());
    }
}

quint32 CalcCRC(QString str, quint32 poly, quint32 init, quint32 crcxor, bool refin, bool refout, bool dir)
{

    if (str.length() == 0) return 0;
    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
    QByteArray s = codec->fromUnicode(str);
    QByteArray hex = QByteArray::fromHex(s);


    uint8_t buf[hex.length()];

    qDebug() << hex;

    for (int i = 0; i<hex.length();++i) {
        buf[i] = (uint8_t)hex[i];
    }

    ccrc8 crc;
    crc.setData(hex.data(), hex.length());
    crc.setPolynom(poly);
    crc.setCrcInit(init);
    crc.setCrcXor(crcxor);
    crc.setReflectIn(refin);
    crc.setReflectOut(refout);
    crc.setDirection(dir);
    int ErrCode = crc.calculate();
    if(ErrCode != static_cast<int>(ERR_CODE::ERR_NO_ERROR))
    {
        //        throw crc.getCrcErrorMsg(ErrCode);
        return 0;
    };
    return crc.getCRC();
}


void MainWindow::on_pbShowTable_clicked()
{
    QStandardItemModel *tablem = new QStandardItemModel;
    ccrc8 crc;
    QString str = ui->lePack->text();
    if (str.length() == 0) return;
    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
    QByteArray s = codec->fromUnicode(str);
    QByteArray hex = QByteArray::fromHex(s);
    crc.setData(hex.data(), hex.length());
    crc.setPolynom(static_cast<quint32>(ui->spinBoxPoly->value()));
    crc.setCrcInit(static_cast<quint32>(ui->spinBoxInitCRC->value()));
    crc.setCrcXor(static_cast<quint32>(ui->spinBoxFinalXOR->value()));
    crc.setReflectIn(ui->checkBoxRefIn->checkState());
    crc.setReflectOut(ui->checkBoxRefOut->checkState());
    crc.setDirection(ui->checkBoxDirect->checkState());
    int ErrCode = crc.calculate();
    if(ErrCode != static_cast<int>(ERR_CODE::ERR_NO_ERROR))
    {
        ui->statusBar->showMessage("Can not calculate CRC. Please check your settings", 1500);
        return;
    };
    for(int i=0; i < 16; ++i)
        for(int u=0; u < 16; ++u)
            tablem->setItem(i, u, new QStandardItem("0x" + QString::number(crc.getCrcTable(i*16 + u),16).toUpper()));
    tablem->setHorizontalHeaderLabels({"0x00", "0x01", "0x02", "0x03", "0x04", "0x05", "0x06", "0x07",
                                       "0x08", "0x09", "0x0A", "0x0B", "0x0C", "0x0D", "0x0E", "0x0F"});
    tablem->setVerticalHeaderLabels({"0x00", "0x10", "0x20", "0x30", "0x40", "0x50", "0x60", "0x70",
                                     "0x80", "0x90", "0xA0", "0xB0", "0xC0", "0xD0", "0xE0", "0xF0"});

    QWidget *t = new QWidget;
    QHBoxLayout *hL = new QHBoxLayout();
    t->setLayout(hL);
    QTableView *tablev = new QTableView(t);
    t->resize(620,400);
    tablev->setModel(tablem);
    tablev->resizeColumnsToContents();
    tablev->resizeRowsToContents();
    tablev->resize(680,480);
    t->setWindowTitle("Polynom = 0x" + QString::number(ui->spinBoxPoly->value(), 16) + "; " + (ui->checkBoxRefIn->checkState() ? "(reflected)" : "(not reflectred)"));
    t->show();
}

void MainWindow::on_pbSearch_clicked()
{
    bool excelent = true;   // флаг, успешного совпадения по всем посылкам
    // на всякий случай, если вдруг пользователь на подтвердил воод,
    // пересчитываем все поыслки
    foreach(cPackCRC *pck, pack){
        pck->packedited();
    }
//    progresstask->setValue(0); progresstask->show();
    progressbar->setValue(0); progressbar->show();
    quint8 poly = 0;
    quint8 start = 0;
    quint8 xorout = 0;
    crc8SAEJ1850 crc;
    // начальное значение полиномадля перебора
    int poly_count = ui->spinBoxPoly->value();
    // перебор полинома
    for ( ; poly_count < 256; poly_count++) {
//        progresstask->setValue(static_cast<int>(poly));
        progressbar->setValue(static_cast<int>(poly_count));
        poly = static_cast<quint8>(poly_count);
        crc.polinom = poly;
        crc.CRCInit();
        // перебор по маске
        for (int xor_count = 0; xor_count < 256; xor_count++) {
            xorout = static_cast<quint8>(xor_count);
            crc.xorOut = xorout;
            // перебор стартового значения
            for (int st_count = 0; st_count < 256; st_count++) {
                start = static_cast<quint8>(st_count);
                crc.start = start;
                // перебор всех заданных значений
                excelent = true;
                foreach(cPackCRC *pck, pack){
                    if(crc.CalcCRC(pck->getPack()) != pck->getCRC()) {
                        excelent = false;
                        break;
                    }
                }
                if(excelent) break;
            }
            if(excelent) break;
        }
        if(excelent) break;
    }

    if(excelent)
    {
        QString str = "Polynom = 0x" + QString::number(poly, 16).toUpper()
                + ";\n Start Byte = 0x" + QString::number(start, 16).toUpper()
                + ";\n Xor Byte = 0x" + QString::number(xorout, 16).toUpper()
                //+ "\n with flags: " + ((flagRefIn > 0)? "/ RefIn " : "/ ")
                //+ ((flagRefOut > 0)? "/ RefOut " : "/ ")
                //+ ((flagDir > 0)? "/ Dir /" : "/ /")
                //+ "\nFinal mask: 0x" +  QString::number(mask, 16).toUpper()
                //+ "\nDifferense: 0x" +  QString::number(diff, 16).toUpper()
                //+ "\n(spend time - " + QString::number(time.elapsed()) + " ms)";
                ;
        setWindowTitle(maintitle);
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Success!", str + "\n\nWould you like to copy this settings?");
        if (reply == QMessageBox::Yes)
        {
            ui->spinBoxPoly->setValue(static_cast<int>(poly));
            ui->spinBoxInitCRC->setValue(static_cast<int>(start));
            ui->spinBoxFinalXOR->setValue(static_cast<int>(xorout));
//            ui->checkBoxRefIn->setCheckState((flagRefIn > 0)?(Qt::CheckState::Checked):(Qt::CheckState::Unchecked));
//            ui->checkBoxRefOut->setCheckState((flagRefOut > 0)?(Qt::CheckState::Checked):(Qt::CheckState::Unchecked));
//            ui->checkBoxDirect->setCheckState((flagDir > 0)?(Qt::CheckState::Checked):(Qt::CheckState::Unchecked));
        }
    } else {
        QMessageBox::information(this, "Calculation", "CRC not found. Try another data settings.");
    };
//    progresstask->hide();
    progressbar->hide();
}
