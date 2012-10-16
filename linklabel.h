#ifndef LINKLABEL_H
#define LINKLABEL_H
#include <QLabel>
#include <QWebView>
#include <QWebPage>
#include <QWebFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWebElement>
#include <QTimer>
#include <QRegExp>
#include "config.h"
#include "store.h"
#include "mywebpage.h"
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QTimer>
#include <QPaintEngine>
#include <QPaintDevice>
#include <QLayout>
#include <QImage>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNodeList>
#include <QNetworkCookieJar>
#include <QNetworkAccessManager>

class LinkLabel : public QWidget
{
    Q_OBJECT

public:
    explicit LinkLabel(QVBoxLayout *parent, int index, QString url, Store *db, QNetworkCookieJar *cookieJar, int crawl_follower, int crawl_following);
    ~LinkLabel();
    void    startLoad();
    void    setId(QString id);
    void    setStatus(QString &status);

private:
    int         index;
    int         lcount;     // Link Count
    int         offset;
    bool        crawl_follower;
    bool        crawl_following;
    QLabel      urlLabel;
    QLabel      statusLabel;
    MyWebPage    m_page;
    QString     id;
    QVBoxLayout *par;
    QHBoxLayout con;
    QHBoxLayout viewLayout;
    Store       *db;
    QTimer      timeOut;
    configs::twitterStatus twtStatus;
    QString cursor;

signals:
    void accountFinished(int, configs::LoadStatus);
    void twitterFinished(int, configs::twitterStatus);

protected slots:
    void saveIt(bool);
    void connectionTimedOut();

};

#endif // LINKLABEL_H
