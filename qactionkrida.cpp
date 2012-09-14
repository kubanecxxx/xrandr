#include "qactionkrida.h"

QActionKrida::QActionKrida(QObject *parent) :
    QAction(parent)
{
    connect(this,SIGNAL(triggered()),this,SLOT(tempTriger()));
    connect(this,SIGNAL(hovered()),this, SLOT(tempHover()));
}

void QActionKrida::tempTriger(void)
{
    emit triggered(commandGlob);
}

void QActionKrida::tempHover()
{
    emit hovered(*this);
}

