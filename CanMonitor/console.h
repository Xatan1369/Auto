#ifndef CONSOLE_H
#define CONSOLE_H

#include <QFrame>

namespace Ui {
class Console;
}

class Console : public QFrame
{
    Q_OBJECT

public:
    explicit Console(QWidget *parent = 0);
    ~Console();

private:
    Ui::Console *ui;

signals:
    void writeData(QByteArray msg);

public slots:
    void PrintConsole(QString str);

private slots:
    void writeMsg();

};


#endif // CONSOLE_H
