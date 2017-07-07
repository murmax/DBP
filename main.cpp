#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec> // подключение класса кодека текста

int main(int argc, char *argv[])
{
    //Q_INIT_RESOURCE(resources);

    QStringList paths = QCoreApplication::libraryPaths();
    paths.append(".");
    paths.append("imageformats");
    paths.append("platforms");
    paths.append("sqldrivers");
    QCoreApplication::setLibraryPaths(paths);
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8")); //изменения
    MainWindow w;
    w.show();

    return a.exec();
}
