#ifndef TRAY_ICON_H
#define TRAY_ICON_H

#include "QSystemTrayIcon"
#include "QWidget"
#include "QMenu"

class tray_icon : public QWidget
{
public:
    tray_icon(QWidget * parent = 0);
    void run(QString & file );

private:
    QWidget * parent;
    QSystemTrayIcon * tray;
    QMenu * trayIconMenu;
    QAction * minimizeAction;
    QTimer * timer;

protected slots:
    void tray_clicked (QSystemTrayIcon::ActivationReason reason);
    void hide(void);
    void chumaj (void);
};

#endif // TRAY_ICON_H
