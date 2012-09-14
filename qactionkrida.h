#ifndef QACTIONKRIDA_H
#define QACTIONKRIDA_H

#include <QAction>

class QActionKrida : public QAction
{
    Q_OBJECT
public:
    explicit QActionKrida(QObject *parent = 0);
    void setCommand (const QString & command){commandGlob = command; setToolTip(commandGlob);}

signals:
    void triggered(QString & command);
    void hovered(QActionKrida & action);
    
protected slots:
    void tempTriger (void);
    void tempHover(void);

private:
    QString commandGlob;
    
};

#endif // QACTIONKRIDA_H
