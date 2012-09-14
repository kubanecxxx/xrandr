#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QtCore>
#include <QDebug>

#include "qlocale.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    MainWindow w;

    QTranslator translator ;
    translator.load("/usr/share/qt/translations/qt_cs");
    a.installTranslator(&translator);

    //w.show();
    
    return a.exec();

}
