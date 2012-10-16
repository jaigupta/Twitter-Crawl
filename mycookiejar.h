#ifndef MYCOOKIEJAR_H
#define MYCOOKIEJAR_H

#include <QNetworkCookieJar>
#include <QFile>

class MyCookieJar : public QNetworkCookieJar
{
    Q_OBJECT
public:
    explicit MyCookieJar(QObject *parent = 0);
    void saveCookies(QFile &cookieFile);
    void loadCookies(QFile &cookies);
signals:

public slots:

};

#endif // MYCOOKIEJAR_H
