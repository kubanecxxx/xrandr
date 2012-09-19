#include "QTimer"
#include "QMenu"
#include "QAction"

#include "mainwindow.h"

#include "QFile"
#include "QDir"
#include "QTextStream"
#include "xmlparser.h"
#include "QStyle"
#include "QDebug"
#include "QDateTime"
#include "QProcess"
#include "QDragEnterEvent"
#include "QDropEvent"
#include "QUrl"
#include "QCloseEvent"
#include "QMessageBox"
#include "QToolTip"
#include "QApplication"
#include "QClipboard"
#include "QStyle"
#include "QInputDialog"

#include <QLocalSocket>

void MainWindow::connected()
{
    soc = server->nextPendingConnection();
    connect(soc,SIGNAL(readyRead()),this,SLOT(readyRead()));
}

void MainWindow::readyRead()
{
    QString str = soc->readAll();

    qDebug() << str;

    QString file = str;
    QFile fn(file);

    if(!fn.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    QTextStream in(&fn);
    QString name = in.readLine();
    QString zbytek =(in.readAll());

    fn.close();

    //tray->showMessage(name,zbytek,QSystemTrayIcon::NoIcon,10000);
    pop->notify(name,zbytek);
}

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent,flags)
{
    menu.submenu.clear();
    menu.primo.clear();
    qmenu = NULL;

    QString fil = QString("/tmp/") + QString(SERVER);
    QFile file(fil);

    if(file.exists())
        file.remove();

    server = new QLocalServer;
    server->listen(SERVER);

    connect(server,SIGNAL(newConnection()),this,SLOT(connected()));

    readConf();
    readXml();
    //vyblije hotovy menu ktery se potom jenom přiřadi do tray
    menu2qmenu();

    createTray();

    setAcceptDrops(true);

    setMinimumWidth(300);
    setMinimumHeight(150);

    setWindowTitle(trUtf8("Křida soft"));

    combo = new QComboBox(this);
    combo->setGeometry(10,20,150,25);

    QStringList list;
    list.push_back("Main");
    for (int i = 0 ; i < menu.submenu.length() ; i++)
        list.push_back(menu.submenu.at(i).nazevSubmenu);

    list.push_back("Noj");

    combo->addItems(list);
    combo->insertSeparator(combo->count() - 1);
    combo->setTabOrder(NULL,NULL);

    connect(combo,SIGNAL(activated(int)),this,SLOT(ComboChanged(int)));


    edit = new QTextEdit(this);
    edit->setGeometry(10,60,150,25);
    edit->setTabChangesFocus(true);

    connect (edit,SIGNAL(textChanged()),this,SLOT(EditChanged()));

    QLabel * naame;
    QLabel * priikaz;

    naame = new QLabel(this);
    naame->setText(QString::fromUtf8("Náme:"));
    naame->setGeometry(10,40,150,25);

    priikaz = new QLabel(this);
    priikaz->setText(QString::fromUtf8("Přikaz:"));
    priikaz->setGeometry(10,80,150,25);

    editPrikaz = new QTextEdit(this);
    editPrikaz->setGeometry(10,100,150,25);
    editPrikaz->setTabChangesFocus(true);

    connect (editPrikaz,SIGNAL(textChanged()),this,SLOT(EditChanged()));

    button = new QPushButton(this);
    button->setGeometry(200,15,80,120);
    button->setText(QString::fromUtf8("Přidat"));
    button->setDisabled(true);


    connect(button,SIGNAL(clicked()),this,SLOT(buttonClicked()));


    MenuMazani = new QMenu(this);

    MenuMazani->addAction(style()->standardIcon(QStyle::SP_DialogSaveButton),"Copy");
    MenuMazani->addSeparator();
    MenuMazani->addAction(style()->standardIcon(QStyle::SP_DialogCloseButton),tr("Smazat"));

    connect(MenuMazani,SIGNAL(triggered(QAction*)),this,SLOT(MenuMazatTrigger(QAction*)));

    QDir dir;
    QString home = dir.homePath();
    dir.mkdir(home + "/" + ADRESAR);
    dir.mkdir(PDF);


    pop = new popup(this);
    connect(pop,SIGNAL(wantShow(bool)), this,SLOT(setShown(bool)));

}

void MainWindow::MenuMazatTrigger(QAction * action)
{
    if (action->text() == "Copy")
    {
        QApplication::clipboard()->setText(tempstring.command);
        tray->showMessage(trUtf8("Zkopčeno"),tempstring.command, QSystemTrayIcon::Information, 3000);
    }
    else if (action->text() == "Smazat")
    {
        Smazat(tempstring.text,tempstring.menu);
    }
}

void MainWindow::popni(QString text, QString menu_l, QString command , QPoint pint)
{
    MenuMazani->popup((pint));

    tempstring.text = text;
    tempstring.menu = menu_l;
    tempstring.command = command;
}

void menuKrida::mousePressEvent(QMouseEvent *evt)
{
    if (evt->button() == Qt::RightButton)
    {
        MainWindow * par = (MainWindow *) parent();
        menuKrida * menu;

        for (int i = 1 ; i < par->seznamMenu.length(); i++)
        {
            if (!par->seznamMenu.at(i)->isHidden())
            {
                menu = par->seznamMenu[i];
                break;
            }

            if (i ==  par->seznamMenu.length() - 1)
            {
                menu = par->seznamMenu[0];
            }
        }

        if (menu == NULL)
            return;

        QAction * act  = menu->actionAt(evt->pos());

         if (act != NULL)
         {
             QString text = act->text();
             QString textmenu = menu->title();
             QString prikaz = act->toolTip();

             if (text != "Konec" && text != trUtf8("Občerstvit") )
             {
                  emit popnout(text,textmenu,prikaz, evt->globalPos());
                  qDebug() << text  + "  " + textmenu;
             }
         }
    }
    else
    {
        QMenu::mousePressEvent(evt);
    }
}

void MainWindow::Smazat(QString &polozka, QString &menu_l)
{
    QList<menuTypedef>  *zoznam;
    if (menu_l != "Main")
    {
        for (int i = 0; i< menu.submenu.length(); i++)
        {
            if (menu.submenu.at(i).nazevSubmenu == menu_l)
            {
                zoznam = &(menu.submenu[i].seznam);
                break;
            }
        }
    }
    else
    {
        zoznam = &menu.primo;
    }


    int i;
    for (i = 0 ; i < zoznam->length(); i++)
    {
        if (zoznam->at(i).polozka == polozka)
        {
            break;
        }
    }

    QMessageBox msgbox;
    msgbox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgbox.setDefaultButton(QMessageBox::No);
    msgbox.setWindowTitle(trUtf8("Smazat?"));
    msgbox.setText(trUtf8("Fakt chceš smazat \"") + zoznam->at(i).polozka + "\" z menu \"" + menu_l + "\" ??");
    msgbox.setIcon(QMessageBox::Warning);
    int ret = msgbox.exec();

    qmenu->close();

    if (ret == QMessageBox::Yes)
    {
        qDebug() << zoznam->at(i).polozka << "  " << menu_l;
        tray->showMessage(zoznam->at(i).polozka, "Z \"" + menu_l + trUtf8("\" Smazáno"));

        zoznam->removeAt(i);
        saveXml();
        refreshMenu();
    } else
    {
        setVisible(true);
        setVisible(false);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (isHidden())
    {
        event->accept();
    }
    else
    {
        tempstring.icon.clear();
        event->ignore();
        setHidden(true);
        path.clear();
        edit->clear();
        editPrikaz->clear();
    }
}

void MainWindow::refreshMenu()
{
    menu.submenu.clear();
    menu.primo.clear();
    readXml();

    QStringList list;
    list.push_back("Main");
    for (int i = 0 ; i < menu.submenu.length() ; i++)
        list.push_back(menu.submenu.at(i).nazevSubmenu);

    list.push_back("Noj");
    combo->clear();
    combo->addItems(list);
    combo->insertSeparator(combo->count() - 1);

    seznamMenu.clear();
    menu2qmenu();


    tray->setContextMenu(qmenu);
}

MainWindow::~MainWindow()
{
    delete(server);
}

void MainWindow::dropEvent(QDropEvent *event)
{
    if (!event->mimeData()->hasUrls())
    {
        return;
    }

    QList<QUrl> urlList = event->mimeData()->urls();
    QString file = urlList.at(0).toLocalFile();

    if (file.isEmpty())
        return;

    path = file;
    int i = path.lastIndexOf('/');
    QString temp = path.right(path.length() - i - 1);


    if (file.endsWith(".pdf",Qt::CaseInsensitive))
    {
        editPrikaz->setText("evince");
        QFile source(file);

        QDir dir;
        dir.mkdir(PDF + "/" + combo->currentText());
        QString newfile = PDF + "/" + combo->currentText() + "/" + temp;
        source.copy(newfile);
        path = newfile;
        tempstring.icon = PDF + "/images.jpg";
    }
    else
    {
        editPrikaz->clear();
    }


    temp = temp.left( temp.length() -  4);
    edit->setText(temp);
    edit->selectAll();
    edit->setFocus();
}

void MainWindow::EditChanged()
{
    if (!edit->toPlainText().isEmpty() && !editPrikaz->toPlainText().isEmpty())
        button->setEnabled(true);
    else
        button->setDisabled(true);
}

void MainWindow::buttonClicked()
{
    menuTypedef polozka;

    polozka.Icon = tempstring.icon;
    polozka.polozka = edit->toPlainText();
    polozka.systemCommmand = editPrikaz->toPlainText() +" " + path;


    for (int i = 0 ; i < menu.submenu.size(); i++)
    {
        if  (menu.submenu.at(i).nazevSubmenu == combo->currentText())
        {
            QList<menuTypedef> & vole = menu.submenu[i].seznam;           
            vole.append(polozka);
            break;
        }
        else if (combo->currentText() == "Main")
        {
            menu.primo.append(polozka);
            break;
        }
    }

    tempstring.icon.clear();
    path.clear();
    button->setDisabled(true);
    edit->clear();
    editPrikaz->clear();

    setHidden(true);

    saveXml();
    refreshMenu();

    tray->showMessage(polozka.polozka,QString::fromUtf8("Úspěšně přidáno"));
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}


void MainWindow::menu2qmenu(void)
{
    //vytvořit akcu, connectnout na menu_callback s parametrem system command
    if (qmenu == NULL)
    {
        qmenu = new menuKrida(this);
    }
    qmenu->clear();
    qmenu->setTitle("Main");
    seznamMenu.push_back(qmenu);

    menuKrida * temp;

    for (int i = 0 ; i < menu.primo.count(); i++)
    {
        addItem(&menu.primo.at(i),qmenu);
    }

    qmenu->addSeparator();

    for (int i = 0 ; i < menu.submenu.count() ; i++)
    {
        temp = new menuKrida(this);
        qmenu->addMenu(temp);

        temp->setTitle(menu.submenu.at(i).nazevSubmenu);
        seznamMenu.push_back(temp);

        const QList<menuTypedef> * tempList;
        tempList = &menu.submenu.at(i).seznam;
        temp->setIcon(QIcon(menu.submenu.at(i).Icon));

        for (int j = 0 ; j < (tempList->count()); j++)
        {
            addItem(&tempList->at(j), temp);
        }
    }

    qmenu->addSeparator();

    QAction * tempAction;

    tempAction = new QAction(this);
    tempAction->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    tempAction->setText(trUtf8("Občerstvit"));
    connect(tempAction,SIGNAL(triggered()) , this,SLOT(refreshMenu()));
    qmenu->addAction(tempAction);

    tempAction = new QAction(this);
    tempAction->setText(tr("Konec"));
    tempAction->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
    //tempAction->setIcon(QStyle::standardIcon(QStyle::SP_TitleBarCloseButton,0,this));
    connect(tempAction,SIGNAL(triggered()),this,SLOT(close()));
    qmenu->addAction(tempAction);
}

void MainWindow::addItem(const menuTypedef * data, QMenu * kam)
{
    QActionKrida * tempAction;

    tempAction = new QActionKrida(this);
    tempAction->setText(data->polozka);
    tempAction->setCommand(data->systemCommmand);
    tempAction->setIcon(QIcon(data->Icon));

    connect(tempAction,SIGNAL(triggered(QString&)),this,SLOT(menu_callback(QString&)));
    connect(tempAction, SIGNAL(hovered(QActionKrida&)),this,SLOT(MenuHover(QActionKrida&)));
    //přidat akci do menu
    kam->addAction(tempAction);
}

void MainWindow::MenuHover(QActionKrida & action)
{
    QToolTip::showText(QPoint(100,100),action.toolTip());
}

void MainWindow::chumaj (void)
{

}

void MainWindow::createTray(void)
{
    tray = new QSystemTrayIcon;

    tray->setContextMenu(qmenu);
    tray->setIcon(QIcon(icon));
    tray->show();

    connect(tray,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                        this,SLOT(tray_clicked(QSystemTrayIcon::ActivationReason)));
}


void MainWindow::tray_clicked(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::MiddleClick)
    {

    if (isHidden())
        show();
    else
       hide();
    }
    else if (reason == QSystemTrayIcon::DoubleClick)
    {
        QString comm = filemanager+ " " + PDF + "&";
        system(comm.toAscii() );
    }

#ifdef QT_DEBUG
    else if (reason == QSystemTrayIcon::Trigger)
    {
        pop->notify(QString("head"),QString("zbytrek"));
    }
#endif


}

void MainWindow::menu_callback(QString & command)
{
    QFile file(logg);
    if(!file.open(QFile::Append))
    {
        qDebug() << trUtf8("Nejde otevřít logovací soubor");
    }
    else
    {
        QTextStream stream(&file);
        QDateTime cas = QDateTime::currentDateTime();

        bool uz = false;
        for (int i = 0 ; i < command.length(); i++)
        {

            if (command[i] == '/')
                uz = true;

            if (uz)
            {
                if (command[i] == ' ')
                {
                    command[i] = '\\';
                    command.insert(++i ,' ');
                }
            }

        }

        system((command + '&').toAscii());

        stream << cas.toString("dd.MM.yyyy hh:mm:ss")
               << tr("  ")
               << command
               << endl;

        qDebug() << command;

        file.close();
    }
}


void MainWindow::readXml(void)
{
    menu.primo.clear();
    menu.submenu.clear();

    QFile file(xml);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qErrnoWarning("Neni soubor xml pro menu");
        exit(1);
    }

    QTextStream in(&file);
    xmlparser::parse(&in,&menu);

    file.close();
}

void MainWindow::saveXml()
{

    QFile file(xml);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text ))
    {
        qErrnoWarning("Neni soubor xml pro menu");
        exit(1);
    }


    QString ulozit;
    xmlparser::create(menu,ulozit);

    file.write(ulozit.toUtf8());
    file.close();
}

void MainWindow::readConf(void)
{
    QString home = QDir::homePath();
    home += QString("/") + ADRESAR + "/" + KONFIGURAK;
    QFile file(home);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qErrnoWarning("Neni konfiguracni soubor");
        exit(1);
    }

    QTextStream in(&file);

    while(!in.atEnd())
    {
        QString line = in.readLine();
        //process line

        conffileLine(line,tr("menu"),xml);
        conffileLine(line,tr("icon"),icon);
        conffileLine(line,tr("logg"),logg);
        conffileLine(line,"filemanager",filemanager);

    }
    file.close();
}

void MainWindow::conffileLine(QString &line,const QString & data, QString & kam)
{
    if (line.startsWith(data))
    {
        line.remove (data + tr(" "));
        if (QDir::isRelativePath(line))
        {
            if (line.startsWith(tr("~")))
            {
                line.remove(0,1);
                line.insert(0,QDir::homePath());
            }
            else
            {
                 line.insert(0,QDir::homePath() + "/" + ADRESAR + "/");
            }
        }
        kam = line;
    }
}

void MainWindow::ComboChanged(int index)
{
    if(index == combo->count() - 1)
    {
        QInputDialog dialog;
        dialog.setLabelText(QString::fromUtf8("Napiš méno novyho submenu"));

        int ret = dialog.exec();

        if (ret == QInputDialog::Accepted)
        {
            menuItemTypedef noj;
            noj.nazevSubmenu = dialog.textValue();

            menu.submenu.push_back(noj);

            saveXml();
            refreshMenu();
        }
    }
}
