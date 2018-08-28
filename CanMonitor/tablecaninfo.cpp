#include "tablecaninfo.h"
#include "ui_tablecaninfo.h"

#include "QtSql/QSqlDatabase"
#include "QSqlQuery"

QSqlDatabase db;
//QSqlQuery query;

TableCanInfo::TableCanInfo(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::TableCanInfo)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setShowGrid(true);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

    //connect(ui->btConnect, SIGNAL(clicked(bool)), this, SLOT(Save_Settings()));
    connect(ui->comboBoxProtocol, SIGNAL(currentIndexChanged(int)), this, SLOT(protocol(int)));



      //ui->comboBoxSpeed->addItem(QLatin1String("115200"), QSerialPort::Baud115200);
    //openDB();
}

void TableCanInfo::openDB(){
    //Подключаем базу данных

      db = QSqlDatabase::addDatabase("QSQLITE");
      //db.setDatabaseName("database.db3");//bd_test.db
      db.setDatabaseName("CanID");//bd_test.db
      db.open();
       qDebug("OPEN");

      QStringList tableName = db.tables();
      ui->comboBoxProtocol->addItems(tableName);


}

void TableCanInfo::protocol(int ind){
    if(ind == 0)
        qDebug("0");
    if(ind == 1)
        qDebug("1");
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);

    //Подключаем базу данных

      /*db = QSqlDatabase::addDatabase("QSQLITE");
      //db.setDatabaseName("database.db3");//bd_test.db
      db.setDatabaseName("CanID");//bd_test.db
      db.open();*/

      //Осуществляем запрос
      QSqlQuery query;

      //SELECT 1кол, 2кол, 3кол FROM таблица
      QString tableName = ui->comboBoxProtocol->itemText(ind);
      ui->label->setText(tableName);
      query.exec("SELECT ID, Period, Description, Additional FROM " + tableName.toLocal8Bit());
      //query.prepare()
      //query.exec("SELECT id, ed, rd FROM test");
      //query.exec("SELECT _id, name, age FROM People");

      int Row = ui->tableWidget->rowCount();
      //int i = 0;

      //Выводим значения из запроса
      while (query.next())
      {
      QString _id = query.value(0).toString();
      QString _period = query.value(1).toString();
      QString _desc = query.value(2).toString();
      QString _addit = query.value(3).toString();
      //ui->textEdit->insertPlainText(_id+" "+_period+" "+_desc+" "+_addit+"\n");

      Row = ui->tableWidget->rowCount();
      ui->tableWidget->setRowCount(Row+1);
      ui->tableWidget->setItem(Row, 0, new QTableWidgetItem(_id));
      ui->tableWidget->setItem(Row, 1, new QTableWidgetItem(_period));
      ui->tableWidget->setItem(Row, 2, new QTableWidgetItem(_desc));
      ui->tableWidget->setItem(Row, 3, new QTableWidgetItem(_addit));
      }
}

TableCanInfo::~TableCanInfo()
{
    delete ui;
}
