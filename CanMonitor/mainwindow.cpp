#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>

#include "QtSql/QSqlDatabase"
#include "QSqlQuery"

//QSqlDatabase db;

enum State{ NO, START, RUN};
State DevStatus;

#define PERIOD_TIMER    1000

int handlerStatus = 0;
int periodCan = 0;
int countMsg[101];
int countMsgTr[101];

QString st;

QString temp = "0";

bool newSt = false;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    labelV = new QLabel;
    console = new Console;
    tablecaninfo = new TableCanInfo;
    downLoaderDB = new DownLoaderDB;
    spinBox = new QSpinBox;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    myTimer = new QTimer;

    QThread *thread_New = new QThread;//Создаем поток для порта платы
    Port *PortNew = new Port();//Создаем обьект по классу
    PortNew->moveToThread(thread_New);//помешаем класс  в поток
    PortNew->thisPort.moveToThread(thread_New);//Помещаем сам порт в поток
    PortNew->moveToThread(thread_New);

    connect(myTimer, SIGNAL(timeout()), this, SLOT(timer_overflow()));

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    foreach (const QSerialPortInfo &info, QSerialPortInfo:: availablePorts()) {
        ui->comboBoxPort->addItem(info.portName());
    }
    ui->comboBoxSpeed->addItem(QLatin1String("9600"), QSerialPort::Baud9600);
    ui->comboBoxSpeed->addItem(QLatin1String("19200"), QSerialPort::Baud19200);
    ui->comboBoxSpeed->addItem(QLatin1String("38400"), QSerialPort::Baud38400);
    ui->comboBoxSpeed->addItem(QLatin1String("57600"), QSerialPort::Baud57600);
    ui->comboBoxSpeed->addItem(QLatin1String("115200"), QSerialPort::Baud115200);

    ui->comboBoxSpeedCan->addItem(QLatin1String("250 kBps"),"5");
    ui->comboBoxSpeedCan->addItem(QLatin1String("500 kBps"), "6");
    ui->comboBoxSpeedCan->addItem(QLatin1String("1 MBps"), "8");
    /*ui->comboBoxSpeedCan->addItem(QLatin1String("10 kBps"), 0);
    ui->comboBoxSpeedCan->addItem(QLatin1String("10 kBps"), 0);
    ui->comboBoxSpeedCan->addItem(QLatin1String("10 kBps"), 0);
    ui->comboBoxSpeedCan->addItem(QLatin1String("10 kBps"), 0);
    ui->comboBoxSpeedCan->addItem(QLatin1String("10 kBps"), 0);*/

    ui->lineEditMask->setText("FFFFFF");
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    connect(PortNew, SIGNAL(error_(QString)), console, SLOT(PrintConsole(QString)));
    connect(thread_New, SIGNAL(started()), PortNew, SLOT(process_Port()));
    connect(PortNew, SIGNAL(finished_Port()), thread_New, SLOT(quit()));
    connect(thread_New, SIGNAL(finished()), PortNew, SLOT(deleteLater()));
    connect(PortNew, SIGNAL(finished_Port()), thread_New, SLOT(deleteLater()));
    connect(this, SIGNAL(saveSettings(QString,int,int,int,int,int)), PortNew, SLOT(Write_Settings_Port(QString,int,int,int,int,int)));

    connect(ui->btConnect, SIGNAL(clicked(bool)), this, SLOT(Save_Settings()));
    connect(ui->btConnect, SIGNAL(clicked(bool)), PortNew, SLOT(ConnectPort()));
    connect(ui->btDisconnect, SIGNAL(clicked(bool)), PortNew, SLOT(DisconnectPort()));

    connect(ui->btStart, SIGNAL(clicked(bool)), this, SLOT(startCanScan()));
    connect(ui->btSendAllCan, SIGNAL(clicked(bool)), this, SLOT(writeMsgCan()));
    connect(ui->btStopAllCan, SIGNAL(clicked(bool)), this, SLOT(stopTimer()));
    connect(ui->btConsole, SIGNAL(clicked(bool)), console, SLOT(show()));
    connect(ui->btDB, SIGNAL(clicked(bool)), tablecaninfo, SLOT(show()));
    connect(ui->btDB, SIGNAL(clicked(bool)), tablecaninfo, SLOT(openDB()));
    connect(ui->btDownLoader, SIGNAL(clicked(bool)), downLoaderDB, SLOT(show()));
    connect(ui->btAddDB, SIGNAL(clicked(bool)), this, SLOT(addDB()));

    connect(PortNew, SIGNAL(outPortConsole(QString)), console, SLOT(PrintConsole(QString)));
    connect(PortNew, SIGNAL(outPort(QString)), this, SLOT(handlerState(QString)));
    connect(this, SIGNAL(writeData(QByteArray)), PortNew, SLOT(WriteToPort(QByteArray)));
    connect(console, SIGNAL(writeData(QByteArray)), PortNew, SLOT(WriteToPort(QByteArray)));
    connect(PortNew, SIGNAL(infoPort(QString)),this, SLOT(openPort(QString)));
    //ui->textBrowser->textCursor().insertText("p[ort");

    //connect(this, SIGNAL(isPortStatus(bool), PortNew, SLOT(isPortState()));

    connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(spinBoxH(int)));
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setShowGrid(true);

    ui->tableWidget->horizontalHeader()->resizeSection(0, 70);
    ui->tableWidget->horizontalHeader()->resizeSection(1, 70);
    ui->tableWidget->horizontalHeader()->resizeSection(2, 70);
    ui->tableWidget->horizontalHeader()->resizeSection(3, 200);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    //ui->tableWidget->setCellWidget(1, 5);

     //QTableWidgetItem *tab_Item = new QTableWidgetItem();
     //tab_Item->setText("Count");
     QStringList it = {"Count", "Period", "ID", "Data", "Comment"};
     ui->tableWidget->setHorizontalHeaderLabels(it);

     ui->tableWidgetTrn->setColumnCount(6);
     ui->tableWidgetTrn->setShowGrid(true);

     ui->tableWidgetTrn->horizontalHeader()->resizeSection(0, 70);
     ui->tableWidgetTrn->horizontalHeader()->resizeSection(1, 70);
     ui->tableWidgetTrn->horizontalHeader()->resizeSection(2, 70);
     ui->tableWidgetTrn->horizontalHeader()->resizeSection(3, 70);
     ui->tableWidgetTrn->horizontalHeader()->resizeSection(4, 200);
     ui->tableWidgetTrn->horizontalHeader()->setStretchLastSection(true);

     QStringList it2 = {"Count", "Period", "ID","DLC", "Data", "Comment"};
     ui->tableWidgetTrn->setHorizontalHeaderLabels(it2);

    thread_New->start();

    DevStatus = START;

    int Row = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(100);
    ui->tableWidgetTrn->setRowCount(100);
    }

void MainWindow::Save_Settings(){
    saveSettings(ui->comboBoxPort->currentText(), ui->comboBoxSpeed->currentText().toInt(), QSerialPort::Data8,
                 QSerialPort::NoParity, QSerialPort::OneStop, QSerialPort::NoFlowControl);
}

void MainWindow::openPort(QString info){
    //info = "open";
    ui->labelPort->setText(info);
}

void MainWindow::PrintConsole(QString str){
    QString str2;
    str2 = "state";
    QByteArray byteMsg = "raeuigr";

    /*if(isPortStatus)
        ui->plainTextEdit->textCursor().insertText(str2 + "\r");*/
    //ui->plainTextEdit->textCursor().insertText(str +" - " + QString::number(str.size()));
    //ui->plainTextEdit->moveCursor(QTextCursor::End);

    //ui->plainTextEdit->textCursor().insertText(QString::number(str.size()));
    //ui->plainTextEdit->moveCursor(QTextCursor::End);


}

void MainWindow::writeMsg(){
    QString msg;
    //msg = ui->->text() + '\r';
    writeData(msg.toLocal8Bit());
}

void MainWindow::startCanScan(){
    DevStatus = START;
    handlerStart(0);
}

void MainWindow::handlerState(QString msg){
    if(msg.contains('\r') & DevStatus == START){
        handlerStart(msg);
        //ui->labelV->setText(str);
        //++handlerStatus;
    }else if(DevStatus == RUN){
        handlerCan(msg);
    }
}

void MainWindow::handlerStart(QString msg){
    qDebug ("HalderStart");
    qDebug(msg.toLocal8Bit());
    QString msg2;
    //st += msg;
    if(msg.contains("V"))
        ui->labelV->setText(msg);
    else if(msg.contains("v"))
        ui->labelv->setText(msg);

    switch (handlerStatus) {
    case 0: qDebug ("HalderStatus case0"); //Подробная версия прошивки устройства
        msg2 = "v";
        writeData(msg2.toLocal8Bit() +'\r');
        ui->labelV->setText(msg);
        ++handlerStatus;
        //ui->plainTextEdit->textCursor().insertText(msg);
        //ui->plainTextEdit->moveCursor(QTextCursor::End);
        break;
    case 1: qDebug ("HalderStatus case1"); //Версия прошивки устройства
        msg2 = "V";
        writeData(msg2.toLocal8Bit() +'\r');
        ++handlerStatus;
        break;
    case 2: qDebug ("HalderStatus case2"); //Маска
        msg2 = "C" + ui->lineEditMask->text();
        writeData(msg2.toLocal8Bit() +'\r');
        ++handlerStatus;
        break;
    case 3: qDebug ("HalderStatus case3"); //Скорость CAN
        msg2 = "S" + ui->comboBoxSpeedCan->currentData().toString();
        writeData(msg2.toLocal8Bit() +'\r');
        ++handlerStatus;
        break;
    case 4: qDebug ("HalderStatus case4"); //Метка времени для приема кадров
        msg2 = "Z0";
        writeData(msg2.toLocal8Bit() +'\r');
        ++handlerStatus;
        break;
    case 5: qDebug ("HalderStatus case5"); //Вкл рабочий режим
        msg2 = "O";
        writeData(msg2.toLocal8Bit() +'\r');
        ++handlerStatus;
        ui->comboBoxSpeedCan->setEnabled(false);
        ui->lineEditMask->setEnabled(false);
        ui->btStart->setEnabled(false);
        DevStatus = RUN;
        break;
    default:
        break;
    }
    //QString str = "handler";
    //ui->plainTextEdit->textCursor().insertText(dataPort);
}

void MainWindow::handlerCan(QString msg){
    qDebug("Handler Can");
    QString msgID;
    QString msgCanOut;

    QString temp2;

    int sizeMsg;// = temp.toInt();
    //ui->labelV->setText(QString::number(msg.size()));
   // qDebug(msg.toLocal8Bit());

    if(msg.size()>3){
        qDebug("msg.size > 3");
        msgID = msg.at(1);
        msgID += msg.at(2);
        msgID += msg.at(3);
        temp2 = msg.at(4);
        sizeMsg = temp2.toInt();
        /*for(int i = 0; i < sizeMsg*2; i+2){
            //msgCanOut[i] = msg.at(i + 5)  msg.at(i + 6);///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        }*/
    }


    if(msg.size() > 3){
        int Row = ui->tableWidget->rowCount();

        for(int i = 0; i <= Row; i+=1){
            qDebug(QString::number(i).toLocal8Bit());

            if(ui->tableWidget->item(i, 2) != 0){
                qDebug("!= NULL");
                temp = ui->tableWidget->item(i, 2)->text();
                if(temp.at(0) == msgID.at(0) & temp.at(1) == msgID.at(1) & temp.at(2) == msgID.at(2)){
                    ++countMsg[i];
                    for(int iB = 5; iB <= sizeMsg * 2 + 4; iB += 2){
                        msgCanOut.append(msg.at(iB));
                        msgCanOut.append(msg.at(iB + 1));
                        qDebug(msgCanOut.toLocal8Bit());
                    }
                    ui->tableWidget->setItem(i, 4, new QTableWidgetItem(msg));
                    ui->tableWidget->setItem(i, 3, new QTableWidgetItem(msgCanOut));
                    ui->tableWidget->setItem(i, 2, new QTableWidgetItem(msgID));
                    ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(countMsg[i])));
                    ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(sizeMsg)));
                    i = 101;
                }
            }else{
                ++countMsg[i];
                for(int iB = 5; iB <= sizeMsg * 2 + 4; iB += 2){
                    msgCanOut.append(msg.at(iB));
                    msgCanOut.append(msg.at(iB + 1));
                    qDebug(msgCanOut.toLocal8Bit());
                }
                ui->tableWidget->setItem(i, 4, new QTableWidgetItem(msg));
                ui->tableWidget->setItem(i, 3, new QTableWidgetItem(msgCanOut));////////////////
                ui->tableWidget->setItem(i, 2, new QTableWidgetItem(msgID));
                ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(countMsg[i])));
                ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(sizeMsg)));
                i = 101;
                 qDebug("else");
            }


        }


    }





}

void MainWindow::writeMsgCan(){
    QString msgCan;
    QString id = ui->lineEditID->text();
    int sMsg = ui->spinBox->value();
    QString sizeMsg = QString::number(sMsg);

    //ui->label->setText(sizeMsg);

    msgCan = "t" + ui->lineEditID->text() + sizeMsg.toLocal8Bit() + ui->lineEditH1->text() + ui->lineEditH2->text()
             + ui->lineEditH3->text() + ui->lineEditH4->text() + ui->lineEditH5->text() + ui->lineEditH6->text()
             + ui->lineEditH7->text() + ui->lineEditH8->text() + '\r';
    writeData(msgCan.toLocal8Bit());

    ++countMsgTr[0];

    periodCan = ui->lineEditPeriod->text().toInt();
    if(periodCan > 0){
        myTimer->start(periodCan);
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    ui->tableWidget->setRowCount(10);
    ui->tableWidgetTrn->setItem(0, 0, new QTableWidgetItem(QString::number(countMsgTr[0])));
    ui->tableWidgetTrn->setItem(0, 1, new QTableWidgetItem(QString::number(periodCan)));
    ui->tableWidgetTrn->setItem(0, 2, new QTableWidgetItem(id));
    ui->tableWidgetTrn->setItem(0, 3, new QTableWidgetItem(sizeMsg));
    ui->tableWidgetTrn->setItem(0, 4, new QTableWidgetItem(ui->lineEditH1->text() + ui->lineEditH2->text() + ui->lineEditH3->text()
                                                           + ui->lineEditH4->text() + ui->lineEditH5->text() + ui->lineEditH6->text()
                                                           + ui->lineEditH7->text() + ui->lineEditH8->text()));
}

void MainWindow:: timer_overflow(){
    qDebug ("Timer");
    QString msgCan;
    int sMsg = ui->spinBox->value();
    QString sizeMsg = QString::number(sMsg);
    ++countMsgTr[0];
    ui->tableWidgetTrn->setItem(0, 0, new QTableWidgetItem(QString::number(countMsgTr[0])));
    msgCan = "t" + ui->lineEditID->text() + sizeMsg.toLocal8Bit() + ui->lineEditH1->text() + ui->lineEditH2->text()
             + ui->lineEditH3->text() + ui->lineEditH4->text() + ui->lineEditH5->text() + ui->lineEditH6->text()
             + ui->lineEditH7->text() + ui->lineEditH8->text() + '\r';
    writeData(msgCan.toLocal8Bit());
    //QString str = "timer";
    //ui->plainTextEdit->textCursor().insertText(str);
}
void MainWindow:: stopTimer(){
    myTimer->stop();
}

void MainWindow:: spinBoxH(int n){

    switch (n) {
    case 0:
        ui->lineEditH8->setVisible(false);
        ui->lineEditH7->setVisible(false);
        ui->lineEditH6->setVisible(false);
        ui->lineEditH5->setVisible(false);
        ui->lineEditH4->setVisible(false);
        ui->lineEditH3->setVisible(false);
        ui->lineEditH2->setVisible(false);
        ui->lineEditH1->setVisible(false);
        break;
    case 1:
        ui->lineEditH8->setVisible(false);
        ui->lineEditH7->setVisible(false);
        ui->lineEditH6->setVisible(false);
        ui->lineEditH5->setVisible(false);
        ui->lineEditH4->setVisible(false);
        ui->lineEditH3->setVisible(false);
        ui->lineEditH2->setVisible(false);
        ui->lineEditH1->setVisible(true);
        break;
    case 2:
        ui->lineEditH8->setVisible(false);
        ui->lineEditH7->setVisible(false);
        ui->lineEditH6->setVisible(false);
        ui->lineEditH5->setVisible(false);
        ui->lineEditH4->setVisible(false);
        ui->lineEditH3->setVisible(false);
        ui->lineEditH2->setVisible(true);
        ui->lineEditH1->setVisible(true);
        break;
    case 3:
        ui->lineEditH8->setVisible(false);
        ui->lineEditH7->setVisible(false);
        ui->lineEditH6->setVisible(false);
        ui->lineEditH5->setVisible(false);
        ui->lineEditH4->setVisible(false);
        ui->lineEditH3->setVisible(true);
        ui->lineEditH2->setVisible(true);
        ui->lineEditH1->setVisible(true);
        break;
    case 4:
        ui->lineEditH8->setVisible(false);
        ui->lineEditH7->setVisible(false);
        ui->lineEditH6->setVisible(false);
        ui->lineEditH5->setVisible(false);
        ui->lineEditH4->setVisible(true);
        ui->lineEditH3->setVisible(true);
        ui->lineEditH2->setVisible(true);
        ui->lineEditH1->setVisible(true);
        break;
    case 5:
        ui->lineEditH8->setVisible(false);
        ui->lineEditH7->setVisible(false);
        ui->lineEditH6->setVisible(false);
        ui->lineEditH5->setVisible(true);
        ui->lineEditH4->setVisible(true);
        ui->lineEditH3->setVisible(true);
        ui->lineEditH2->setVisible(true);
        ui->lineEditH1->setVisible(true);
        break;
    case 6:
        ui->lineEditH8->setVisible(false);
        ui->lineEditH7->setVisible(false);
        ui->lineEditH6->setVisible(true);
        ui->lineEditH5->setVisible(true);
        ui->lineEditH4->setVisible(true);
        ui->lineEditH3->setVisible(true);
        ui->lineEditH2->setVisible(true);
        ui->lineEditH1->setVisible(true);
        break;
    case 7:
        ui->lineEditH8->setVisible(false);
        ui->lineEditH7->setVisible(true);
        ui->lineEditH6->setVisible(true);
        ui->lineEditH5->setVisible(true);
        ui->lineEditH4->setVisible(true);
        ui->lineEditH3->setVisible(true);
        ui->lineEditH2->setVisible(true);
        ui->lineEditH1->setVisible(true);
        break;
    case 8:
        ui->lineEditH8->setVisible(true);
        ui->lineEditH7->setVisible(true);
        ui->lineEditH6->setVisible(true);
        ui->lineEditH5->setVisible(true);
        ui->lineEditH4->setVisible(true);
        ui->lineEditH3->setVisible(true);
        ui->lineEditH2->setVisible(true);
        ui->lineEditH1->setVisible(true);
        break;
    default:
        break;
    }
}

void MainWindow::addDB(){

}

MainWindow::~MainWindow()
{
    delete ui;
}
