#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setStyle("Windows");
    MainWindow w;
    w.show();
    return app.exec();
}
