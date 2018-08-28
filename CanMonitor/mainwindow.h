#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "port.h"
//#include <QLabel>
#include <QTimer>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QString>
#include <QLabel>
#include <QSpinBox>
#include "console.h"
#include "tablecaninfo.h"
#include "caninfo.h"
#include "downloaderdb.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QTimer *myTimer;
    QLabel *labelV;
    Console *console;
    QSpinBox *spinBox;
    TableCanInfo *tablecaninfo;
    DownLoaderDB *downLoaderDB;
    CanInfo *caninfo;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void saveSettings(QString name, int baudrate, int DataBits, int Parity
                      , int StopBits, int FlowControl);
    void error(QString str);

    bool isPortStatus();
    void writeData(QByteArray);



private slots:
    void timer_overflow();

void PrintConsole(QString str);

void Save_Settings();
void writeMsg();

void startCanScan();

void handlerState(QString msg);
void handlerStart(QString msg);
void handlerCan(QString msg);

void writeMsgCan();
void stopTimer();

void openPort(QString info);

void  spinBoxH(int n);

void addDB();

private:
    Ui::MainWindow *ui;
};



#endif // MAINWINDOW_H
