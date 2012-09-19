#ifndef POPUP_H
#define POPUP_H

#include <QWidget>
#include <QFrame>
#include <QString>
#include "popup_neco.h"

class popup : public QWidget
{
    Q_OBJECT
public:
    explicit popup(QWidget *parent = 0);
    void notify(QString header, QString text);
    
signals:
    void wantShow(bool show);


public slots:
    void wantDestroy(Popup_neco * object);

private:

    typedef struct
    {
        Popup_neco * frame;
        int pos;
    } notify_t;

    QList<Popup_neco *> frames;
};

#endif // POPUP_H
