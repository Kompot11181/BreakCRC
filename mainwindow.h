#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
//#include <QtWinExtras>         // для отображения прогресса на панели задач
#include <QStandardItemModel>
#include <QtGui>
#include <QtWidgets>
#include "packcrc.h"
#include "crc8.h"
#include "crc8saej1850.h"

namespace Ui {
class MainWindow;
}

QT_FORWARD_DECLARE_CLASS(QWinTaskbarButton)
QT_FORWARD_DECLARE_CLASS(QWinTaskbarProgress)

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pbAdd_clicked();

    void on_pbMinus_clicked();

    void on_lePack_returnPressed();

    void on_pbCalculate_clicked();

    void on_spinBoxPoly_valueChanged(int arg1);

    void on_spinBoxInitCRC_valueChanged(int arg1);

    void on_spinBoxFinalXOR_valueChanged(int arg1);

    void on_pbShowTable_clicked();

    void on_pbSearch_clicked();

private:
    Ui::MainWindow *ui;

    QString maintitle;
// массив данных для подбора CRC
    QVector<cPackCRC *> pack;

// ProgressBar на StatusBar`е. Ставится програмно
    QProgressBar * progressbar;
// кнопка с progressbar`ом на панели задач в Винде (Taskbar)
    QWinTaskbarButton *buttontask = nullptr;
    QWinTaskbarProgress *progresstask = nullptr;
};

#endif // MAINWINDOW_H
