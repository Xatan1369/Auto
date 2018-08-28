#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QByteArray>
#include <QString>
#include <QMetaEnum>
#include <unistd.h>
#include <errno.h>

QByteArray dataHex;
QString str1;
QString str2;

#define FA1                          (1)
#define FA2                          (2)
#define FA3                          (3)
#define FA4                          (4)

#define FA7                          (5)
#define FA8                          (6)
#define FB1                          (7)
#define FB2                          (8)
#define FB3                          (9)
#define FC1                          (10)
#define FC2                          (11)
#define FC3                          (12)
#define FD1                          (13)
#define FD2                          (14)
#define FD3                          (15)

#define FS4                          (16)
#define FS5                          (17)
#define FADC1                        (18)
#define FADC2                        (19)

#define FV1                          (20)
#define FV2                          (21)
#define FV3                          (22)
#define FV4                          (23)
#define FV5                          (24)
#define FV6                          (25)
#define FV7                          (26)
#define FV8                          (27)
#define FV9                          (28)
#define FV10                         (29)
#define FV11                         (30)
#define FV12                         (31)
#define FV13                         (32)
#define FV14                         (33)
#define FV15                         (34)
#define BT                            (35)
#define b0_                         (36)
#define F1                          (37)
#define CZ                          (38)
#define FZ                          (39)
#define b2_                         (40)
#define CI                          (41)



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->comboBox_Formula->addItem("Новая формула",0);
    ui->comboBox_Formula->addItem("FA1", 1);
    ui->comboBox_Formula->addItem("FA2", 2);
    ui->comboBox_Formula->addItem("FA3", 3);
    ui->comboBox_Formula->addItem("FA4", 4);
    ui->comboBox_Formula->addItem("FA7");
    ui->comboBox_Formula->addItem("FA8");
    ui->comboBox_Formula->addItem("FB1");
    ui->comboBox_Formula->addItem("FB2");
    ui->comboBox_Formula->addItem("FB3");
    ui->comboBox_Formula->addItem("FC1");
    ui->comboBox_Formula->addItem("FC2");
    ui->comboBox_Formula->addItem("FC3");
    ui->comboBox_Formula->addItem("FD1");
    ui->comboBox_Formula->addItem("FD2");
    ui->comboBox_Formula->addItem("FD3");
    ui->comboBox_Formula->addItem("FS4");
    ui->comboBox_Formula->addItem("FS5");
    ui->comboBox_Formula->addItem("FADC1");
    ui->comboBox_Formula->addItem("FADC2");
    ui->comboBox_Formula->addItem("FV1");
    ui->comboBox_Formula->addItem("FV2");
    ui->comboBox_Formula->addItem("FV3");
    ui->comboBox_Formula->addItem("FV4");
    ui->comboBox_Formula->addItem("FV5");
    ui->comboBox_Formula->addItem("FV6");
    ui->comboBox_Formula->addItem("FV7");
    ui->comboBox_Formula->addItem("FV8");
    ui->comboBox_Formula->addItem("FV9");
    ui->comboBox_Formula->addItem("FV10");
    ui->comboBox_Formula->addItem("FV11");
    ui->comboBox_Formula->addItem("FV12");
    ui->comboBox_Formula->addItem("FV13");
    ui->comboBox_Formula->addItem("FV14");
    ui->comboBox_Formula->addItem("FV15");
    ui->comboBox_Formula->addItem("BT");
    ui->comboBox_Formula->addItem("b0_");
    ui->comboBox_Formula->addItem("F1");
    ui->comboBox_Formula->addItem("CZ");
    ui->comboBox_Formula->addItem("FZ");
    ui->comboBox_Formula->addItem("b2_");
    ui->comboBox_Formula->addItem("CI");


    ui->spinBox_startByte->setValue(5);

    str1 = "80 f1 10 4f 61 01 02 b3 ae 5a bc 00 00 14 00 8f 0f 00 04 8f 24 3f 01 95 56 47 2d 70 03 18 7e 80 00 10 00 11 00 0d 00 df 00 00 00 00 00 00 00 00 00 00 00 00 02 3f 58 6b 02 95 61 03 e1 02 eb 7b 16 00 1c 01 11 80 00 52 7e 0e fb 05 80 00 00 00 00 00 3d";
    str2 = "80 f1 10 4f 61 01 02 b3 ae 5a b9 00 00 15 00 93 09 00 04 89 24 3f 01 92 56 46 2d 88 03 1e 7e 80 00 10 00 11 00 0d 01 12 00 00 00 00 00 00 00 00 00 00 00 00 02 3f 58 6b 01 b2 61 03 e6 ff 6d 7b 16 00 1c 01 11 80 00 54 7e 0e fb 05 80 00 00 00 00 00 3d";

}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::on_btEnter_clicked()
{

    dataHex.clear();
    ui->comboBox_Byte->clear();
    QByteArray strByte = ui->lineEdit->text().toLocal8Bit();
    strByte.replace(" ","");
    dataHex = QByteArray::fromHex(strByte);
    int minI = ui->spinBox_startByte->value();
    int maxI = dataHex.size();
    //ui->label_result->setText(QString::number(minI));
    for(int i = minI; i < maxI; ++i){
        ui->comboBox_Byte->addItem(QString::number(i-minI), i-minI);
    }

}

void MainWindow::on_pushButton_clicked()
{
    ui->lineEdit->setText(str1);
}


void MainWindow::on_btCalculate_clicked()
{
    int num0 = ui->spinBox_startByte->value();
    int num1 = ui->comboBox_Byte->currentIndex();
    int startByte = num0 + num1;
    int formula = ui->comboBox_Formula->currentIndex();
    ui->label_result->setText(QString::number(handlerParametrsGeneral(startByte, formula, dataHex)));

    //QString msg = ui->comboBox_Formula->currentText();
   // ui->label_result->setText(QString::number(formulas(startByte, msg, dataHex)));
    QByteArray b;
    if(ui->spinBox->value() == 1){
        b.resize(1);
        b[0] = dataHex[startByte];
        ui->label_HEX->setText(ByteToStr(b));
    }
    else if(ui->spinBox->value() == 2){
        b.resize(2);
        b[0] = dataHex[startByte];
        b[1] = dataHex[startByte+1];
        ui->label_HEX->setText(ByteToStr(b));
    }

}

float MainWindow::handlerParametrsGeneral(int startByte, int formuls, QByteArray data){
    int E;
    switch (formuls) {
    case 0: E = (data[startByte] & 0xFF);
        return E / 2 -64;
    case FA1: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF) * 256;
        return E;
    case FA2: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF) * 256;
        return E / 10;
    case FA3: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF) * 256;
        return E / 100;
    case FA4: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF) * 256;
        return E / 1000;
    case FA7: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF) * 256;
        return (float) E / 10;
    case FA8: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF) * 256;
        return (float) E / 100;
    case FB1: E = (data[startByte] & 0xFF);
        return E - 40;
    case FC1:
        return (data[startByte] & 0xFF);
    case FC2: E = (data[startByte] & 0xFF);
        return (float) E / 10;
    case FD1: E = (data[startByte] & 0xFF);
        return E * 1.25 / 256;
    case FS4: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF) * 256;
        return (float) E * 0.00001;
    case FS5: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF) * 256;
        return (float) E * 0.1333;
    case FADC1: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF) * 256;
        return (float) E * 5 / 1024;
    case FADC2: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF) * 256;
        return (float) E * 1.25 / 256;
    case FV1: E = data[startByte] & 0xFF;
        return (float)  14.7 * (E + 128)/ 256;
    case FV2: E = data[startByte] & 0xFF;
        return E * 40;
    case FV3: E = data[startByte] & 0xFF;
        return E * 10;
    case FV4: E = data[startByte] & 0xFF;
        return (float)  (E + 128)/256;
    case FV5: E = data[startByte]/2;
        return (float)  E;
    case FV6: E = data[startByte] & 0xFF;
        return (float)  5.2 + (E) * 0.05;
    case FV7: E = data[startByte] & 0xFF;
        return E * 10;
    case FV8: E = data[startByte] & 0xFF;
        return (float)  1.25 * (E / 256);
    case FV9: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF);
        return (float) E / 125;
    case FV10: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF);
        return (float) E / 6;
    case FV11: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF);
        return (float) E / 50;
    case FV12: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF);
        return (float) E / 128;
    case FV13: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF);
        return (float) E / 10;
    case FV14: E = data[startByte] & 0xFF;
        return (float)  E * 5 / 256;
    case FV15: E = data[startByte] & 0xFF;
        return (float)  0.287 * E * 5 / 256;
    case BT: E = (data[startByte] & 0xFF);
        return (E - 65) / 1.32;
    case b0_: E = (data[startByte] & 0xFF);
        return E / 13.55;
    case F1: E = (data[startByte] & 0xFF);
        return E * 100 / 255;
    case CZ: E = (data[startByte] & 0xFF);
        return E / 256 * 180 - 90;
    case FZ: E = (data[startByte] & 0xFF);
        return E / 2 -64;
    case b2_: E = (data[startByte] & 0xFF);
        return E * 0.75;
    case CI: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF)*256*2;
        return (float) E / 121.5;
    default:
        break;
    }
    return -1;
}

float MainWindow::formulas(int startByte, int formuls, QByteArray data){
    float E;
    switch (formuls) {
    case 0: E = (data[startByte+1] & 0xFF);// FT
        return (float) E - 40;
    case 1: E = (data[startByte+1] & 0xFF);// FK
        return (float) (E - 128)*100/128;
    case 2: E = (data[startByte+1] & 0xFF);// FP
        return (float) E*3;
    case 3: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF) * 256;// FR
        return (float) E / 4;
    case 4: E = (data[startByte+1] & 0xFF);// FZ
        return (float) E/2-64;
    case 5: E = (data[startByte+1] & 0xFF);// FD
        return (float) E*0.005;
    case 6: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF) * 256;// FF
        return (float) E / 100;
    case 7: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF) * 256;// FL
        return (float) E * 0.0000305;
    case 8: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF) * 256;// FV
        return (float) E * 0.000122;
    case 9: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF) * 256;// FA
        return (float) E * 0.00390625 - 128;


    case 10: E = (data[startByte+1] & 0xFF);// F1
        return (float) E * 100 / 255;
    case 11: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF) * 256;// F2
        return (float) E * 0.079;
    case 12: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF) * 256;// F3
        return (float) E * 10;
    case 13: E = (data[startByte+1] & 0xFF);// F6
        return (float) E * 0.78125 - 100;
    case 14: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF) * 256;// F7
        return (float) E / 4 - 8.192;
    case 15: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF) * 256;// F4
        return (float) E / 10 - 40;
    case 16: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF) * 256;// F8
        return (float) E / 1000;
    case 17: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF) * 256;// F9
        return (float) E * 100 / 255;
    case 18: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF) * 256;// FO
        return (float) E;
    case 19: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF) * 256;// DA
        return (float) E * 0.009765625;
    case 20: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF) * 256;// DV
        return (float) E * 0.006103516;
    case 21: E = (data[startByte] & 0xFF);// E4
        return (float) E * 32.60 / 255;
    case 22: E = (data[startByte] & 0xFF);// F0
        return (float) E * 1.02 / 255 + 0.5;


    case 23: E = (data[startByte] & 0xFF);// X8
        return (float) E - 80;
    case 24: E = (data[startByte] & 0xFF);// X5
        return (float) (E - 65) / 1.32;
    case 25: E = (data[startByte] & 0xFF);// X0
        return (float) E / 14.4;
    case 26: E = (data[startByte] & 0xFF);// XD
        return (float) E * 0.005 - 0.1;
    case 27: E = (data[startByte] & 0xFF);// X1
        return (float) E / 1000;
    case 28: E = (data[startByte] & 0xFF);// XZ
        return (float) E / 2;
    case 29: E = (data[startByte] & 0xFF);// XI
        return  E / 4000;
    case 30: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF) * 256;// X2
        return (float) E * 100 / 65535;
    case 31: E = (data[startByte] & 0xFF);// X3
        return (float) -2;
    case 32: E = (data[startByte] & 0xFF);// X4
        return (float) -2;
    case 33: E = (data[startByte] & 0xFF);// X6
        return (float) E;
    case 34: E = (data[startByte] & 0xFF);// XA
        return (float) E * 0.5625;
    case 35: E = (data[startByte] & 0xFF);// XB
        return (float) E * 209.1 / 255;
    case 36: E = (data[startByte] & 0xFF);// XO
        return (float) E -38 / 200;
    case 37: E = (data[startByte] & 0xFF);// X7
        return (float) E / 45;
    case 38: E = (data[startByte] & 0xFF);// X9
        return (float) E * 17.9 / 255;
    case 39: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF) * 256;// XF
        return (float) E / (256 * 40);
    case 40: E = (data[startByte] & 0xFF);// XC
        return (float) E / 16.38;
    case 41: E = (data[startByte] & 0xFF);// XK
        return (float) E * 0.98 / 32768;
    case 42: E = (data[startByte] & 0xFF);// XL
        return (float) E * 0.233;
    case 43: E = (data[startByte] & 0xFF);// XE
        return (float) E * 5 - 50;
    case 44: E = (data[startByte] & 0xFF);// XU
        return (float) E - 64;


    case 45: E = (data[startByte] & 0xFF);// XU
        return (float) E - 50;
    case 46: E = (data[startByte] & 0xFF);// XU
        return (float) E * 2;
    case 47: E = (data[startByte] & 0xFF);// XU
        return (float) E * 0.08;
    case 48: E = (data[startByte] & 0xFF);// XU
        return (float) 112 - E;
    case 49: E = (data[startByte] & 0xFF);// XU
        return (float) E * 0.02;
    case 50: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF) * 256;
        return (float) E * 12.5;
    case 51: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF) * 256;
        return (float) E * 0.005;
    case 52: E = (data[startByte] & 0xFF);// XU
        return (float) E * 0.01;
    case 53: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF) * 256;
        return (float) E * 0.01;
    case 54: E = (data[startByte] & 0xFF);// XU
        return (float) E * 12.5;
    case 55: E = (data[startByte] & 0xFF);// XU
        return (float) E * 0.5;
    case 56: E = (data[startByte] & 0xFF);// XU
        return (float) E * 0.04;
    case 57: E = (data[startByte] & 0xFF);// XU
        return (float) E * 0.390625;
    case 58: E = (data[startByte] & 0xFF);// XU
        return (float) E * 0.35;
    case 59: E = (data[startByte] & 0xFF);// XU
        return (float) E * 256;
    case 60: E = (data[startByte] & 0xFF);// XU
        return E * 0.5 - 64;
    case 61: E = (data[startByte] & 0xFF);// XU
        return (float) E - 128;
    case 62: E = (data[startByte] & 0xFF);// XU
        return (float) E * 10;
    case 63: E = (data[startByte] & 0xFF);// XU
        return (float) E * 0.075 - 108;
    case 64: E = (data[startByte] & 0xFF);// XU
        return (float) E * 0.78125;
    case 65: E = (data[startByte] & 0xFF);// XU
        return (float) E * 32 / 255;
    case 66: E = (data[startByte] & 0xFF);// XU
        return (float)E * 20 / 255;
    case 67: E = (data[startByte] & 0xFF);// XU
        return (float) E * 12.8;


    case 68: E = (data[startByte] & 0xFF);// XU
        return (float) E / 10;
    case 69: E = (data[startByte] & 0xFF);// XU
        return (float) E * 94 / 255 + 10.4;
    case 70: E = (data[startByte] & 0xFF);// XU
        return (float) E * 19.53;
    case 71: E = (data[startByte] & 0xFF);// XU
        return (float) E * 192 / 256 - 40;
    case 72: E = (data[startByte] & 0xFF);// XU
        return (float) E / 256 * 180 - 90;
    case 73: E = (data[startByte] & 0xFF);// XU
        return (float) E * 20;
    case 74: E = (data[startByte+1] & 0xFF)/125 + (data[startByte] & 0xFF)*2;// XU
        return (float) E;
    case 75: E = (data[startByte] & 0xFF);// XU
        return (float) E / 256 * 1128;
    case 76: E = (data[startByte] & 0xFF);// XU
        return (float) E * 0.1;
    case 77: E = (data[startByte] & 0xFF);// XU
        return -2;
    case 78: E = (data[startByte] & 0xFF);// XU
        return (float) E / 256 * 95 + 10;


    case 80: E = (data[startByte] & 0xFF);// XU
        return (float) E / 256;
    case 81: E = (data[startByte] & 0xFF);// XU
        return (float) E * 40;
    case 82: E = (data[startByte] & 0xFF);// XU
        return (float) (E + 128) / 256;
    case 83: E = (data[startByte] & 0xFF);// XU
        return (float) 5.2 * E * 0.05;
    case 84: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF) * 256;
        return (float) E / 125;
    case 85: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF) * 256;
        return (float) E / 10;
    case 86: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF) * 256;
        return (float) E / 6;
    case 87: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF) * 256;
        return (float) E / 50;
    case 88: E = (data[startByte+1] & 0xFF) + (data[startByte] & 0xFF) * 256;
        return (float) E / 128;
    case 89: E = (data[startByte] & 0xFF);// XU
        return (float) E * 5 / 256;
    case 90: E = (data[startByte] & 0xFF);// XU
        return (float) E * 5 * 0.287 / 256;
    case 91: E = (data[startByte] & 0xFF);// XU
        return (float) (E + 128) * 14.7 / 256;
    case 92: E = (data[startByte] & 0xFF);// XU
        return (float) E * 1.25 / 256;
    case 93: E = (data[startByte] & 0xFF);// b0_
        return E / 13.55;
    case 94: E = (data[startByte] & 0xFF);// b1_
        return E * 1.25;
    case 95: E = (data[startByte] & 0xFF);// b2_
        return E * 0.75;
    default:
        break;
    }
    return -1;
}

QString MainWindow::ByteToStr(QByteArray data){
    QByteArray bytes = data;
    QString str = QString::fromUtf8(bytes.toHex());
    return str;
}


void MainWindow::on_btCalculate2_clicked(){
    int num0 = ui->spinBox_startByte->value();
    int num1 = ui->comboBox_Byte->currentIndex();
    int startByte = num0 + num1;
    int formula = ui->comboBox_Formula2->currentIndex();
    ui->label_result->setText(QString::number(formulas(startByte, formula, dataHex)));
    QByteArray b;
    if(ui->spinBox->value() == 1){
        b.resize(1);
        b[0] = dataHex[startByte];
        ui->label_HEX->setText(ByteToStr(b));
    }
    else if(ui->spinBox->value() == 2){
        b.resize(2);
        b[0] = dataHex[startByte];
        b[1] = dataHex[startByte+1];
        ui->label_HEX->setText(ByteToStr(b));
    }
}



void MainWindow::on_btAuto_clicked()
{
    float N = ui->doubleSpinBox->value();
    int num0 = ui->spinBox_startByte->value();
    int num1 = ui->comboBox_Byte->currentIndex();
    int startByte = num0 + num1;
    for(int i = 0; i < 96; ++i){
        if(formulas(startByte, i, dataHex) < N + 1 && formulas(startByte, i, dataHex) > N - 1)
            ui->plainTextEdit->textCursor().insertText(" "+ QString::number(i)+ "-"+QString::number(formulas(startByte, i, dataHex)) + " ");
        if(i == 95)
            ui->plainTextEdit->textCursor().insertText(" Расчет-1 окончен");
    }
    for(int i = 0; i < 42; ++i){
        if(formulas(startByte, i, dataHex) < N + 1 && formulas(startByte, i, dataHex) > N - 1)
            ui->plainTextEdit->textCursor().insertText(" "+ QString::number(i)+ "-"+QString::number(formulas(startByte, i, dataHex)) + " ");
        if(i == 41)
            ui->plainTextEdit->textCursor().insertText(" Расчет-2 окончен");
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->lineEdit->setText(str2);
}
