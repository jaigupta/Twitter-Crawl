#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "linklabel.h"

MainWindow::MainWindow(bool gui, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    loadSettings();
    cookieFile.setFileName(mysettings["cookie_file"]);
    running = new bool[mysettings["crawler_num"].toInt()];
    label = new LinkLabel*[mysettings["crawler_num"].toInt()];
    ui->setupUi(this);
    ui->crawlVertical->setAlignment(Qt::AlignTop);
    this->pageCount =   0;
    this->gui = gui;
    view    =   new QWebView();
    view->setPage(new MyWebPage(view));
    validUrl.setPattern("^[a-zA-Z0-9]+://");
    ui->webVertical->addWidget(view);
    QNetworkProxy proxy(QNetworkProxy::HttpProxy, mysettings["proxy"], mysettings["port"].toInt());
    QNetworkProxy::setApplicationProxy(proxy);
    view->show();
    if(cookieFile.exists())
    {
        MyCookieJar *c = new MyCookieJar(this);
        c->loadCookies(cookieFile);
        view->page()->networkAccessManager()->setCookieJar(c);
    }else if(!gui)
    {
        cout<<"Error: Could not load cookies file"<<endl;
        exit(0);
    }
    QNetworkCookieJar *cookie    =   view->page()->networkAccessManager()->cookieJar();
    view->load(QUrl("https://mobile.twitter.com"));
    connect(view->page(), SIGNAL(loadFinished(bool)), this, SLOT(changeAddress(bool)));
    connect(this->ui->actionSave_Settings, SIGNAL(triggered()), this, SLOT(saveSettings()));
    connect(this->ui->actionReload_Settings, SIGNAL(triggered()), this, SLOT(loadSettings()));
    connect(this, SIGNAL(destroyed()), this, SLOT(saveSettings()));
    for(int i=0; i<mysettings["crawler_num"].toInt();i++) {
        this->label[i]  =   new LinkLabel(ui->crawlVertical, i, QString(""), &db, cookie, isettings["crawl_follower"], isettings["crawl_following"]);
        connect(label[i], SIGNAL(accountFinished(int, configs::LoadStatus)), this, SLOT(getNextID(int, configs::LoadStatus)));
        running[i]=false;
    }
    saveSettings();
    if(!gui)
    {
        on_onThisPage_triggered();
    }
}

void MainWindow::changeAddress(bool) {
    ui->addressBar->setText(view->url().toString());
}

MainWindow::~MainWindow()
{
}

void MainWindow::getNextID(int index, configs::LoadStatus status) {
    running[index]  =   false;
    QString id;
    bool flag=false;
    for(int i=0;i<mysettings["crawler_num"].toInt();i++) {
        if(!running[i]) {
            id =   db.getId();
            if(id!=NULL) {
                cout<<"Starting "<<id.toAscii().data()<<endl;
                flag=true;
                label[i]->setId(id);
                label[i]->startLoad();
                running[i]  =   true;
            }
        }
    }
    if(!flag) {
        for(int i=0;i<mysettings["crawler_num"].toInt();i++)
            if(running[i])
                flag=true;
        if(!flag){
            QString done = QString("done");
            for(int i=0;i<mysettings["crawler_num"].toInt();i++) {
                label[i]->setId("");
                label[i]->setStatus(done);
            }
        }
    }
}

void MainWindow::on_onThisPage_triggered() {
    QFile f(mysettings["accounts_file"]);
    if(!f.open(QIODevice::ReadOnly))
    {
        cout<<"Error: Unable to read accounts_file:"<<mysettings["accounts_file"].toAscii().data()<<endl;
        exit(0);
    }
    QString s;
    while((s=f.readLine())!=NULL)
        db.addId(s);
    for(int i=0; i<mysettings["crawler_num"].toInt(); i++)
        getNextID(i,configs::LOAD_SUCCESS);
    ui->webTab->setEnabled(false);
    ui->crawlTab->setEnabled(true);
    ui->tabs->setCurrentIndex(1);
}

void MainWindow::on_goBtn_clicked() {
    QString address =   ui->addressBar->text();
    int index=0;
    qDebug(address.toAscii().data());
    if(address.indexOf("://")==-1) {
        if(address.startsWith("ftp.")){
            address="ftp://"+address;     //we need to crawl only http or https pages. still handling this request
        } else {
            address="http://"+address;
        }
    }
    qDebug(address.toAscii().data());
    view->load(QUrl(address));
}

void MainWindow::on_addressBar_returnPressed() {
    on_goBtn_clicked();
}

void MainWindow::loadSettings()
{
    QFile f("settings.txt");
    QString s;
    if(f.exists() && f.open(QIODevice::ReadOnly))
    {
        while((s = f.readLine())!=NULL)
        {
            QStringList val = s.split(' ');
            mysettings[val[0]] = val[1];
        }
    }
    // Loading proxy
    if(!mysettings.contains("proxy"))           mysettings["proxy"] = "144.16.192.247";
    if(!mysettings.contains("port"))            mysettings["port"]  = "8080";
    if(!mysettings.contains("cookie_file"))     mysettings["cookie_file"] = "coookie.txt";
    if(!mysettings.contains("crawler_num"))     mysettings["crawler_num"] = "15";
    if(!mysettings.contains("accounts_file"))   mysettings["accounts_file"] = "accounts.txt";
    if(!mysettings.contains("crawl_follower"))  mysettings["crawl_follower"] = "true";
    if(!mysettings.contains("crawl_following")) mysettings["crawl_following"] = "true";
    isettings["crawl_follower"] = (mysettings["crawl_follower"].compare("true")==0)?1:0;
    isettings["crawl_following"] = (mysettings["crawl_following"].compare("true")==0)?1:0;
}

void MainWindow::saveSettings()
{
    QFile f("settings.txt");
    if(f.open(QIODevice::WriteOnly))
    {
        QTextStream ts(&f);
        ts << QString("proxy %1 --proxy in format same as 144.16.192.245. Will be taken as http proxy \n").arg(mysettings["proxy"]);
        ts << QString("port %1 --port to be used for proxy connection\n").arg(mysettings["port"]);
        ts << QString("cookie_file %1 --path to load and store cookies\n").arg(mysettings["cookie_file"]);
        ts << QString("crawler_num %1 --number of crawlers\n").arg(mysettings["crawler_num"]);
        ts << QString("accounts_file %1 --list of start nodes of the crawler\n").arg(mysettings["accounts_file"]);
        ts << QString("crawl_follower %1 --true/false based on if follower is to be crawled\n").arg(mysettings["crawl_follower"]);
        ts << QString("crawl_following %1 --true/false based on if following is to be crawled\n").arg(mysettings["crawl_following"]);
        ts.flush();
        f.close();
    }
    MyCookieJar *cookie = (MyCookieJar*)view->page()->networkAccessManager()->cookieJar();
    cookie->saveCookies(cookieFile);
}
