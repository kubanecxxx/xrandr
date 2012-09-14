#ifndef XMLPARSER_H
#define XMLPARSER_H

#include "QTextStream"

typedef struct menuTypedef
{
    QString polozka;
    QString systemCommmand;
    QString Icon;
}menuTypedef;

typedef struct menuItemTypedef
{
    QList<menuTypedef> seznam;
    QString nazevSubmenu;
    QString Icon;
} menuItemTypedef;

typedef struct  menuWholeTypedef
{
    QList<menuItemTypedef> submenu;
    QList<menuTypedef> primo;
} menuWholeTypedef;

class xmlparser : public QObject
{
public:
    static void create(menuWholeTypedef & menu, QString & outstream);
    static void parse(QTextStream * inputstream, menuWholeTypedef  * men);
private:
    static QString fullpath(QString data);
};

#endif // XMLPARSER_H
