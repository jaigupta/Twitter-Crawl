#ifndef MYWEBPAGE_H
#define MYWEBPAGE_H

#include <QWebPage>
#include <QString>
class MyWebPage : public QWebPage
{
    Q_OBJECT
public:
    explicit MyWebPage(QWidget *parent = 0);
    QString userAgentForUrl(const QUrl &url ) const;
signals:

public slots:

};

#endif // MYWEBPAGE_H
