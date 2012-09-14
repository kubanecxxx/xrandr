#include "xmlparser.h"
#include "QXmlStreamReader"
#include "QDir"
#include "QtXml/qdom.h"
#include "QDebug"
#include "qdebug.h"

void xmlparser::parse(QTextStream * inputstream, menuWholeTypedef * menu)
{
    QDomDocument doc;
    QString hoj = inputstream->readAll();

    doc.setContent(hoj);

    QDomElement docElement = doc.documentElement();
    QDomElement hlmenu;
    QDomNode node,subnode;
    QString neco;

    //vyblije věci z hlavniho menu
    hlmenu = docElement.firstChildElement("HlavniMenu");
    node = hlmenu.firstChildElement("prikaz");

    menuTypedef temp;

    while (!node.isNull())
    {
        temp.systemCommmand = node.toElement().text();
        temp.polozka = node.attributes().namedItem("nazev").nodeValue();
        temp.Icon = fullpath(node.attributes().namedItem("ikona").nodeValue());
        menu->primo.append(temp);

        node = node.nextSiblingElement("prikaz");
    }

    hlmenu = docElement.firstChildElement("Submenu");
    node = hlmenu.firstChildElement("menu");

    menuItemTypedef temp2;

    while (!node.isNull())
    {
        temp2.nazevSubmenu = node.attributes().namedItem("nazev").nodeValue();
        temp2.Icon = fullpath(node.attributes().namedItem("ikona").nodeValue());
        temp2.seznam.clear();

        subnode = node.firstChildElement("prikaz");
        while(!subnode.isNull())
        {
            temp.Icon = fullpath(subnode.attributes().namedItem("ikona").nodeValue());
            temp.polozka = subnode.attributes().namedItem("nazev").nodeValue();
            temp.systemCommmand = subnode.toElement().text();
            temp2.seznam.append(temp);
            subnode = subnode.nextSiblingElement("prikaz");
        }
        menu->submenu.append(temp2);
        node = node.nextSiblingElement("menu");
    }
}

void xmlparser::create(menuWholeTypedef &menu, QString &outstream)
{

    QDomDocument cod;

    QDomElement xml = cod.createElement("xml");

    //hlavní menu
    QDomElement Hlavni = cod.createElement("HlavniMenu");

    for (int i  = 0 ; i < menu.primo.length(); i++)
    {
        menuTypedef & temp = menu.primo[i];

        QDomElement prikaz = cod.createElement("prikaz");
        QDomNode node = cod.createTextNode(temp.systemCommmand);

        prikaz.appendChild(node);
        prikaz.setAttribute("nazev", (temp.polozka));
        prikaz.setAttribute("ikona", temp.Icon);

        Hlavni.appendChild(prikaz);
    }

    xml.appendChild(Hlavni);

    QDomElement Sub = cod.createElement("Submenu");
    //submenu
    for (int i = 0 ; i < menu.submenu.length(); i++)
    {
        menuItemTypedef & submenu = menu.submenu[i];
        QDomElement menu = cod.createElement("menu");
        menu.setAttribute("nazev",(submenu.nazevSubmenu));
        menu.setAttribute("ikona", submenu.Icon);

        for (int j = 0 ; j < submenu.seznam.length() ; j ++)
        {
            menuTypedef & temp = submenu.seznam[j];
            QDomElement prikaz = cod.createElement("prikaz");
            prikaz.setAttribute("nazev" , temp.polozka);
            prikaz.setAttribute("ikona" , temp.Icon);
            QDomNode  node = cod.createTextNode(temp.systemCommmand);
            prikaz.appendChild(node);

            menu.appendChild(prikaz);
        }

        Sub.appendChild(menu);
    }

    xml.appendChild(Sub);

    //end
    cod.appendChild(xml);
    outstream = cod.toString();


    qDebug() << "\n \n";
    qDebug() << outstream;
}

QString xmlparser::fullpath(QString data)
{
    if (data.startsWith(tr("~")))
    {
        data.remove(0,1);
        data.insert(0,QDir::homePath());

        return data;
    }
    return data;
}

