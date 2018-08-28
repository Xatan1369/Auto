#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btEnter_clicked();
    float handlerParametrsGeneral(int startByte, int formuls, QByteArray data);
    QString ByteToStr(QByteArray data);

    float formulas(int startByte, int formuls, QByteArray data);

    void on_btCalculate_clicked();

    void on_pushButton_clicked();

    void on_btCalculate2_clicked();



    void on_btAuto_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
