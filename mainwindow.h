#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QErrorMessage>
#include <QWebView>
#include <QNetworkProxy>
#include <QWebPage>
#include <QWebFrame>
#include <QInputDialog>
#include <QStringList>
#include <QThread>
#include "linklabel.h"
#include <QErrorMessage>
#include <QRegExp>
#include <QRegExpValidator>
#include <QFile>
#include <QProcess>
#include "store.h"
#include "config.h"
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include "mycookiejar.h"
#include <iostream>
using namespace std;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(bool gui, QWidget *parent = 0);
    ~MainWindow();

private:
    int             pageCount;
    bool            *running;
    Ui::MainWindow  *ui;
    Store           db;
    QWebView        *view;
    QRegExp         validUrl;
    LinkLabel       **label;
    QFile           cookieFile;
    QMap<QString, QString> mysettings;
    QMap<QString, int> isettings;
    bool gui;

public slots:
    void getNextID(int, configs::LoadStatus);
    void on_onThisPage_triggered();
    void on_addressBar_returnPressed();
    void on_goBtn_clicked();
    void changeAddress(bool);
    void loadSettings();
    void saveSettings();
};

#endif // MAINWINDOW_H
