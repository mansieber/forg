#include "mainwindow.h"
#include "generalconstants.h"

#include <QApplication>
#include <QSplashScreen>
#include <QTimer>
#include <QtDebug>

void delay(int millis);

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setStyle("Windows");
    QSplashScreen *splash = new QSplashScreen();
    splash->setPixmap(QPixmap(":/images/splashimage.jpg"));
    splash->show();
    splash->showMessage(QString("Version ").append(VERSION), Qt::AlignRight | Qt::AlignBottom, Qt::black);
    qDebug() << splash->message();
    app.processEvents();
    delay(3000);
    MainWindow w;
    w.show();
    splash->finish(&w);
    delete splash;
    return app.exec();
}

/*
 * Function waits millis milliseconds.
 */
inline void delay(int millis)
{
    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(millis);
    loop.exec();
}
