#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QtCore>
#include <QDebug>
#include <QFile>
#include <QMessageBox>

#include "qlocale.h"
#include "mainwindow.h"

#include <QLocalSocket>

int main(int argc, char *argv[])
{

#if QT_NO_DEBUG
    QLocalSocket socket;
    socket.connectToServer(SERVER);
    if(socket.waitForConnected(1000))
    {
        if (argc == 2)
        {

            socket.write(argv[1]);
            socket.flush();
            socket.waitForBytesWritten();

        }
        socket.disconnectFromServer();

        exit(0);
    }
    else
    {
        if (argc != 1)
            exit(0);

    }
#endif

    QApplication a(argc, argv);
    MainWindow w;





    QTranslator translator ;
    translator.load("/usr/share/qt/translations/qt_cs");
    a.installTranslator(&translator);

    //w.show();
    
    return a.exec();

}
