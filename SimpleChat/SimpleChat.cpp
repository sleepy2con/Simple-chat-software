#include "SimpleChat.h"
#include "LoginWidget.h"
#include <QFileSystemWatcher>
#include <QFile>
#include <QDebug> 
SimpleChat::SimpleChat(QWidget *parent)
    : QMainWindow(parent)
    , m_pLoginWidget(new LoginWidget(0))
    , m_fileWatcher(new QFileSystemWatcher())
{
    m_ui.setupUi(this);
    this->hide();
    m_pLoginWidget->show();
    

    QString strQssPath = "://qss/style.qss";


    // 监视Qss文件当发生变动时自动重新加载QSS
    m_fileWatcher->addPath(strQssPath);
    autoLoadQssFile(strQssPath);
    connect(m_fileWatcher, &QFileSystemWatcher::fileChanged, this,
        [=]() {
            autoLoadQssFile(strQssPath);
        });
}

void SimpleChat::autoLoadQssFile(const QString& strQssPath)
{
    QFile* qssFile = new QFile(strQssPath);
    if (!qssFile->open(QFile::ReadOnly))
    {
        qDebug() << "file open error";
        return;
    }
    qApp->setStyleSheet(qssFile->readAll());
    qssFile->close();
}
