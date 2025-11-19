#include "mainwindow.h"
#include <QApplication>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("English Dictionary Pro+");
    a.setApplicationVersion("2025");

    QDir().mkpath("Data");
    QDir().mkpath("users");

    MainWindow w;
    w.showMaximized();
    return a.exec();
}
