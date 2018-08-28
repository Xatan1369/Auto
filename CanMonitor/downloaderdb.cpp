#include "downloaderdb.h"
#include "ui_downloaderdb.h"

QString strConsole;

DownLoaderDB::DownLoaderDB(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::DownLoaderDB)
{
    ui->setupUi(this);

    // Инициализируем менеджер ...
    manager = new QNetworkAccessManager();
    tableCanInfo = new TableCanInfo();
    // ... и подключаем сигнал о завершении получения данных к обработчику полученного ответа

    //connect(manager, &QNetworkAccessManager::finished, this, &Downloader::onResult);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onResult(QNetworkReply*)));
    connect(ui->btDownLoader, SIGNAL(clicked(bool)), this, SLOT(loaderDB()));
    connect(this, SIGNAL(onDataBase()), tableCanInfo, SLOT(openDB()));

    ui->lineEditURL->setText("https://downloader.disk.yandex.ru/disk/d8b78fddcbd7ae095edaee599009d5543f041fc2b80856ebd0a1bbc7788fcfe3/5b7d3701/lCD6mXoT4nljLu7K-ne_sEVZ7I-r2G8B_m-BAind6vmKVw3FQqPGqBloRw4ogZRJhMnkyTcBA3HpKbUIOvdo9g%3D%3D?uid=318834437&filename=CanID&disposition=attachment&hash=&limit=0&content_type=application%2Foctet-stream&fsize=3072&hid=879c73414c0182a9d5f7f68e77e5a311&media_type=data&tknv=v2&etag=9c4da64156ec4946b578129e6a85360f");

}

void DownLoaderDB::loaderDB(){
    qDebug("UI");
    getData();
}

void DownLoaderDB::getData(){
    QUrl url(ui->lineEditURL->text()); // URL, к которому будем получать данные
    QNetworkRequest request;    // Отправляемый запрос
    request.setUrl(url);        // Устанавлвиваем URL в запрос
    manager->get(request);      // Выполняем запрос
}

void DownLoaderDB::onResult(QNetworkReply *reply){
    // Если в процесе получения данных произошла ошибка
        if(reply->error()){
            // Сообщаем об этом и показываем информацию об ошибках
            ui->label->setText("ERROR");
            strConsole += reply->errorString();
            strConsole += '/n';
            ui->textBrowser->setText(strConsole);
            qDebug("ERROR");
            qDebug() << reply->errorString();
        } else {
            // В противном случае создаём объект для работы с файлом
            QFile *file = new QFile("CanID");
            // Создаём файл или открываем его на перезапись ...
            if(file->open(QFile::WriteOnly)){
                file->write(reply->readAll());  // ... и записываем всю информацию со страницы в файл
                file->close();                  // закрываем файл
                //strConsole += "Downloading is completed";
                //ui->textBrowser->setText(strConsole);
                ui->label->setText("Downloading is completed");
                onDataBase();
            qDebug() << "Downloading is completed";
            emit onReady(); // Посылаем сигнал о завершении получения файла
            }
        }
}

DownLoaderDB::~DownLoaderDB()
{
    delete ui;
}
