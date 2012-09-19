#ifndef QTEXTBROWSERKRIDA_H
#define QTEXTBROWSERKRIDA_H

#include <QTextBrowser>

class QTextBrowserKrida :public QTextBrowser
{
    Q_OBJECT
public:
    explicit QTextBrowserKrida(QWidget * parent = 0);

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *ev);
};

#endif // QTEXTBROWSERKRIDA_H
