#include "linklabel.h"
#include <iostream>
#include "mywebpage.h"
#include <qjson/parser.h>
using namespace std;
LinkLabel::LinkLabel(QVBoxLayout *parent, int index, QString id, Store *db,
                     QNetworkCookieJar *cookieJar, int crawl_follower, int crawl_following) :
    QWidget(0)
{
    this->index     =   index;
    this->id        =   id;
    this->par       =   parent;
    this->db        =   db;
    this->m_page.networkAccessManager()->setCookieJar(cookieJar);

    this->crawl_follower = crawl_follower==1;
    this->crawl_following = crawl_following==1;
    urlLabel.setText(QString(id.toAscii().data()));
    con.addWidget(&urlLabel);
    con.addWidget(&statusLabel);
    par->addLayout(&con);
    m_page.connect(  &m_page, SIGNAL(loadFinished(bool)), this, SLOT(saveIt(bool)));
    timeOut.connect(&timeOut, SIGNAL(timeout()), this, SLOT(connectionTimedOut()));
}

void LinkLabel::connectionTimedOut() {
    statusLabel.setText("Loading Failed!");
    emit accountFinished(index, configs::LOAD_FAILED);
}

void LinkLabel::startLoad() {
    offset=0;
    if(crawl_follower)
    {
        m_page.mainFrame()->load(QString("http://api.twitter.com/1/friends/ids.json?cursor=-1&user_id=%1").arg(id));
        this->twtStatus =   configs::FOLLOWER;
        statusLabel.setText(QString("Loading Followers from 0 to 19"));
    }
    else if(crawl_following)
    {
        m_page.mainFrame()->load(QString("http://api.twitter.com/1/followers/ids.json?cursor=-1&user_id=%1").arg(id));
        this->twtStatus=configs::FOLLOWING;
        statusLabel.setText(QString("Loading Following from 0 to 19"));
    }else
    {
        emit accountFinished(index, configs::LOAD_SUCCESS);
    }
    //this->timeOut.start(configs::TIME_OUT);
}

void LinkLabel::setId(QString id) {
    this->id   =   id;
    urlLabel.setText(id);
}

void LinkLabel::saveIt(bool okay) {
    QWebElement doc, rNode;
    QWebElementCollection nodes;
    int fcount, i;
    bool flag=false;
    if(1) {
        switch(twtStatus) {
        case configs::FOLLOWER:
        {
            QJson::Parser parser;
            bool ok;
            QString response = m_page.mainFrame()->toPlainText();
            QVariantMap result = parser.parse(response.toAscii().data(), &ok).toMap();
            if(!ok)
            {
                qWarning("Error parsing Json response:");
                qWarning(response.toAscii().data());
                qWarning(id.toAscii().data());

                emit accountFinished(index, configs::LOAD_SUCCESS);
                return;
            }

            if(result.contains("error"))
            {
                qWarning("Server returned Error Message:");
                qWarning(result["error"].toString().toAscii().data());
                emit accountFinished(index, configs::LOAD_SUCCESS);
                return;
            }

            QFile f(QString("data/%1.follower").arg(this->id));
            f.open(QIODevice::WriteOnly);
            QTextStream ts(&f);
            cursor = result["next_cursor_str"].toString();

            int count=0;
            foreach (QVariant uids, result["ids"].toList()) {
                ts << uids.toString()<<endl;
                db->addId(uids.toString());
                count++;
            }
            cout<<"Query returned "<<count<<" follower ids" << endl;

            if(cursor.compare(QString("0"))!=0) {
                offset+=20;
                statusLabel.setText(QString("Loading Followers."));
                m_page.mainFrame()->load(QString("http://api.twitter.com/1/friends/ids.json?cursor=%2&user_id=%1").arg(id, cursor));
            }else if(crawl_following){
                offset=0;
                statusLabel.setText(QString("Loading Following"));
                m_page.mainFrame()->load(QString("http://api.twitter.com/1/followers/ids.json?cursor=-%2&user_id=%1").arg(id, cursor));
                twtStatus=configs::FOLLOWING;
            }else
            {
                emit accountFinished(index, configs::LOAD_SUCCESS);
            }
            ts.flush();
            f.close();
            break;
        }
        case configs::FOLLOWING:
        {
            QJson::Parser parser;
            bool ok;
            QString response = m_page.mainFrame()->toPlainText();
            QVariantMap result = parser.parse(response.toAscii().data(), &ok).toMap();
            if(!ok)
            {
                qWarning("Error parsing Json response:");
                qWarning(response.toAscii().data());
                qWarning(id.toAscii().data());

                emit accountFinished(index, configs::LOAD_SUCCESS);
                return;
            }

            if(result.contains("error"))
            {
                qWarning("Server returned Error Message:");
                qWarning(result["error"].toString().toAscii().data());
                emit accountFinished(index, configs::LOAD_SUCCESS);
                return;
            }

            QFile f(QString("data/%1.following").arg(this->id));
            f.open(QIODevice::WriteOnly);
            QTextStream ts(&f);
            cursor = result["next_cursor_str"].toString();
            int count = 0;
            foreach (QVariant uids, result["ids"].toList()) {
                ts << uids.toString()<<endl;
                db->addId(uids.toString());
                count++;
            }
            cout<<"Query returned "<<count<<" following ids"<<endl;
            if(cursor.compare(QString("0"))!=0) {
                offset=0;
                statusLabel.setText(QString("Loading Following"));
                m_page.mainFrame()->load(QString("http://api.twitter.com/1/followers/ids.json?cursor=-%2&user_id=%1").arg(id, cursor));
                twtStatus=configs::FOLLOWING;
            }else
            {
                emit accountFinished(index, configs::LOAD_SUCCESS);
            }
            ts.flush();
            f.close();
            break;
        }
        case configs::TWEET:
            break;
        }
    }
}

LinkLabel::~LinkLabel() {
}

void LinkLabel::setStatus(QString &status) {
    this->statusLabel.setText(status);
}
