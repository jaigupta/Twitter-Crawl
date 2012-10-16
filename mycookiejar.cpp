#include "mycookiejar.h"
#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QDebug>

MyCookieJar::MyCookieJar(QObject *parent) : QNetworkCookieJar(parent) {

}

void MyCookieJar::saveCookies(QFile &cookieFile) {
 cookieFile.open(QFile::WriteOnly);

 //Save cookies, if there are in the allowesCookieDomains-list
 QList<QNetworkCookie> cookies = allCookies();

 for (int i = 0; i < cookies.size(); i++) {

  //Check if cookies may not be saved
  if (cookies.value(i).isSessionCookie()) continue;
  if (cookies.value(i).expirationDate() < QDateTime::currentDateTime()) continue;

   cookieFile.write(cookies.value(i).toRawForm()+"\n");
   qDebug() << "Saved cookie from: " << cookies.value(i).domain();
 }
 cookieFile.flush();
 cookieFile.close();

}

void MyCookieJar::loadCookies(QFile &cookies)
{
    if (cookies.exists()) {
     QList<QNetworkCookie> cookieList;
     cookies.open(QFile::ReadOnly);
     while (cookies.bytesAvailable()) {
      QByteArray arr = cookies.readLine();
      QString cookie = QString::fromUtf8(arr);
      if (cookie.endsWith("\n")) cookie.chop(1);
      cookieList.append(QNetworkCookie::parseCookies(cookie.toUtf8()));
     }
     cookies.close();
     setAllCookies(cookieList);
    }
}
