#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QSystemTrayIcon"
#include "xmlparser.h"
#include "QDropEvent"
#include "QComboBox"
#include "QPushButton"
#include "QTextEdit"
#include "QMenu"
#include "QDebug"
#include "qactionkrida.h"
#include <QLabel>
#include <QLocalServer>

#include "popup.h"

#define KONFIGURAK "krida_conf"
#define ADRESAR ".krida"
#define PDF  (QDir::homePath() + "/" + ADRESAR + "/" + "pdf")
#define SERVER "SERVER"
#define SERVER_CELEJ "/tmp/SERVER"

class menuKrida : public QMenu
{
    Q_OBJECT

public:
    menuKrida(QWidget * parent)  : QMenu(parent)
    {
        connect(this,SIGNAL(popnout(QString,QString,QString, QPoint)),parentWidget(),
                SLOT(popni(QString,QString,QString, QPoint)));
    }
private:
    void mousePressEvent(QMouseEvent * evt);
    void mouseDoubleClickEvent(QMouseEvent *)
    {}

signals:
    void popnout(QString text, QString menu_l, QString command, QPoint pint);
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~MainWindow();


    QList<menuKrida *> seznamMenu;

private:
      QTimer * timer;
      QSystemTrayIcon * tray;
      menuKrida * qmenu;
      QString icon;
      QString xml;
      QString logg;
      QString filemanager;
      QComboBox * combo;
      QPushButton * button;
      QLineEdit * edit;
      QString path;
      QLineEdit * editPrikaz;

      QLocalServer * server;
      QLocalSocket * soc;

      popup *  pop;

      QString newFile;
      QString oldFile;
      bool neco;

    QMenu * MenuMazani;

    struct tempstring
    {
        QString text;
        QString menu;
        QString command;
        QString icon;
    } tempstring;

      void createTray(void);
      void menu2qmenu(void);
      menuWholeTypedef menu;
      void addItem(const menuTypedef * data, QMenu * kam);
      void conffileLine(QString &line,const QString & data, QString & kam);
      void readXml (void);
      void readConf(void);
      void saveXml (void);



      void dropEvent(QDropEvent * event);
      void dragEnterEvent(QDragEnterEvent * event);
      void closeEvent(QCloseEvent * event);
      void Smazat(QString & polozka, QString & menu);

protected slots:
      void EditChanged(QString);
      void refreshMenu(void);
      void chumaj (void);
      void tray_clicked (QSystemTrayIcon::ActivationReason);
      void menu_callback(QString & command);
      void buttonClicked(void);
      void MenuHover(QActionKrida & action);
      void MenuMazatTrigger(QAction * action);
      void ComboChanged(int index);

      void connected();
      void readyRead(void);

public slots:
      void popni  (QString text, QString menu_l , QString command, QPoint pint);

protected:

};

#endif // MAINWINDOW_H
