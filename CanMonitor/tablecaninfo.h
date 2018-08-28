#ifndef TABLECANINFO_H
#define TABLECANINFO_H

#include <QFrame>
#include "QtSql/QSqlDatabase"
#include "QSqlQuery"

namespace Ui {
class TableCanInfo;
}

class TableCanInfo : public QFrame
{
    Q_OBJECT



public:
    explicit TableCanInfo(QWidget *parent = 0);
    ~TableCanInfo();

private slots:
    void protocol(int ind);


public slots:
    void openDB();

private:
    Ui::TableCanInfo *ui;
};

#endif // TABLECANINFO_H
