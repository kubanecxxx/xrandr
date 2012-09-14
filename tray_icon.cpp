#include "QSystemTrayIcon"
#include "QFile"
#include "tray_icon.h"
#include "QAction"
#include "QTimer"

tray_icon::tray_icon(QWidget * parent_)
{
    parent = parent_;
}

void tray_icon::run(QString &file)
{
    tray = new QSystemTrayIcon(parent);
    QIcon icon = QIcon(file);

    timer = new QTimer;
    timer->setInterval(500);
    connect(timer,SIGNAL(timeout()),this,SLOT(chumaj()));
    timer->setSingleShot(false);
    timer->start(500);


    tray->setIcon(icon);
    tray->show();
    tray->setToolTip(trUtf8("nič"));

    minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(chumaj()));

    trayIconMenu = new QMenu(parent);
    trayIconMenu->addAction(minimizeAction);
  //  trayIconMenu->addAction(maximizeAction);
  //  trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
 //   trayIconMenu->addAction(quitAction);

    tray->setContextMenu(trayIconMenu);

    connect(tray,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                            this , SLOT(tray_clicked(QSystemTrayIcon::ActivationReason)));



}

void tray_icon::hide(void)
{

}


void tray_icon::tray_clicked (QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick)
    {

    }
}

void tray_icon::chumaj (void)
{

}

