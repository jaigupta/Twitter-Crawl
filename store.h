#ifndef STORE_H
#define STORE_H
#include <QStringList>
#include <QString>
#include <QProcess>
#include <QFile>
#include "config.h"
#include <QTextStream>

class ChildIndex {
public:
    QStringList desc;
    int depth;
    QString url;
    QList<int> childs;
public:
    ChildIndex(QString url, int depth);
};

class Store
{
public:
    static int pCount;
    int crawlNum;
    QStringList pending;
    QStringList loaded;
    QStringList failed;
    QStringList crawled;
    QList<ChildIndex> links;
    void prepVideo();

public:
    Store();
    int getIndex(QString id);
    void    addId(QString id);
    QString getId();
};

#endif // STORE_H
