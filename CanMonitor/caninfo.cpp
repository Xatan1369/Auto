#include "caninfo.h"
#include "ui_caninfo.h"

CanInfo::CanInfo(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CanInfo)
{
    ui->setupUi(this);
}

CanInfo::~CanInfo()
{
    delete ui;
}
