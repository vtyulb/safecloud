#include "mainwindow.h"
#include <QApplication>
#include <QSslConfiguration>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
