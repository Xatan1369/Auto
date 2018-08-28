#include "console.h"
#include "ui_console.h"

Console::Console(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Console)
{
    ui->setupUi(this);

    connect(ui->btSend, SIGNAL(clicked(bool)), this, SLOT(writeMsg()));
}

void Console::PrintConsole(QString str){
    ui->textEdit->textCursor().insertText(str.toLocal8Bit());
    ui->textEdit->moveCursor(QTextCursor::End);
}

void Console::writeMsg(){
    QString msg;
    QString cr = "0D";
    msg = ui->lineEditMsg->text() + '\r';
    writeData(msg.toLatin1());
    PrintConsole(">>" + msg);
}

Console::~Console()
{
    delete ui;
}
