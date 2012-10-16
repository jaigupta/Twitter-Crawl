#include "store.h"
#include <QDir>

int Store::pCount=-1;

int Store::getIndex(QString id) {
    return crawled.indexOf(id.trimmed());
}

Store::Store() {
    crawlNum=0;
}

void Store::addId(QString id) {
    int index;
    id = id.trimmed();
    if((index=crawled.indexOf(id))>=0) {
        return;
    }
    crawled.append(id);
    pending.append(id);
}

QString Store::getId() {
    if(pCount+1>=crawled.length())
        return NULL;
    pCount++;
    return this->crawled[pCount];
}
