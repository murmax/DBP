#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec> // подключение класса кодека текста

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8")); //изменения
    MainWindow w;
    w.show();

    return a.exec();
}
