#include <QtGui/QApplication>
#include <QWebView>
#include <QNetworkRequest>
#include <QNetworkProxy>
#include <QWebPage>
#include <QWebFrame>
#include <QErrorMessage>
#include "mainwindow.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{

    QDir::current().mkdir("data");
    QApplication a(argc, argv);
    MainWindow *w;
    QStringList args = a.arguments();
    cout<<args.count()<<endl;
    if(args.count()>1)
    {
        if(args[1].compare(QString("nogui"))==0)
        {
         w =   new MainWindow(false,0);
        }
    }else {
        w = new MainWindow(true,0);
        w->show();
    }

    a.exec();
    return 0;
}
