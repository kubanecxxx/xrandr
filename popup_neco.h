#ifndef POPUP_NECO_H
#define POPUP_NECO_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class Popup_neco;
}

class Popup_neco : public QWidget
{
    Q_OBJECT
    
public:
    explicit Popup_neco(QWidget *parent  , int pos_);
    ~Popup_neco();
    int GetPos(void)
    {
        return pos;
    }

    void SetText(QString & header, QString & text);



    
private:
    Ui::Popup_neco *ui;
    QTimer * timer;
    int pos;


signals:
    void wantDestroy(Popup_neco * object);

protected:
    void mousePressEvent(QMouseEvent * evt);

private slots:
    void clicked(void);
    //void timeout(void);
};

#endif // POPUP_NECO_H
