#include "qtextbrowserkrida.h"

QTextBrowserKrida::QTextBrowserKrida(QWidget * parent) : QTextBrowser(parent)
{
}


void QTextBrowserKrida::mousePressEvent(QMouseEvent *ev)
{
    emit clicked();
}

