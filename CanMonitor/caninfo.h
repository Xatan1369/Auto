#ifndef CANINFO_H
#define CANINFO_H

#include <QFrame>

namespace Ui {
class CanInfo;
}

class CanInfo : public QFrame
{
    Q_OBJECT

public:
    explicit CanInfo(QWidget *parent = 0);
    ~CanInfo();

private:
    Ui::CanInfo *ui;
};

#endif // CANINFO_H
