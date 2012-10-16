#include "mywebpage.h"

MyWebPage::MyWebPage(QWidget *parent) :
    QWebPage(parent)
{
}

QString MyWebPage::userAgentForUrl(const QUrl &url ) const
{
    return QString("Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Ubuntu/11.10 Chromium/14.0.835.202 Chrome/14.0.835.202 Safari/535.1");
}
