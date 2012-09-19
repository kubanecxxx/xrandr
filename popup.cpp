#include "popup.h"
#include "QFrame"
#include "QTextEdit"
#include "QApplication"
#include "QDesktopWidget"

popup::popup(QWidget *parent) :
    QWidget(parent)
{
    frames.clear();
}

void popup::notify(QString header, QString text)
{
    Popup_neco * temp;

    int max;
    if (frames.count() != 0 )
        max = frames.at(frames.length() - 1)->GetPos();
    else
        max = 0;

    temp = new Popup_neco(0,++max);

    QRect rect = QApplication::desktop()->screenGeometry();

    int pos = rect.width();
    temp->setGeometry(pos - 250, max * 130 - 100,200,100);
    temp->SetText(header,text);
    temp->show();

    connect(temp, SIGNAL(wantDestroy(Popup_neco * )), this, SLOT(wantDestroy(Popup_neco*)));

    frames.push_back(temp);
}


void popup::wantDestroy(Popup_neco *object)
{
    int i = frames.indexOf(object);

    emit wantShow(true);
    frames[i]->close();
    emit wantShow(false);
    delete(frames[i]);
    frames.removeAt(i);
}
