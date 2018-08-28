#ifndef DOWNLOADERDB_H
#define DOWNLOADERDB_H

#include <QFrame>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QUrl>

#include "tablecaninfo.h"
//#include <QFile>

namespace Ui {
class DownLoaderDB;
}

class DownLoaderDB : public QFrame
{
    Q_OBJECT

    QNetworkAccessManager *manager;
    TableCanInfo *tableCanInfo;

public:
    explicit DownLoaderDB(QWidget *parent = 0);
    ~DownLoaderDB();

signals:
    void onReady();
    void onDataBase();

public slots:
    void getData();     // Метод инициализации запроса на получение данных
    void onResult(QNetworkReply *reply);    // Слот обработки ответа о полученных данных

    void loaderDB();

private slots:
    //void readFile();

private:
    Ui::DownLoaderDB *ui;
};

#endif // DOWNLOADERDB_H
