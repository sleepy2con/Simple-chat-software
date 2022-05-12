#include "SimpleChat.h"
#include "LoginWidget.h"
#include <QFileSystemWatcher>
#include <QFile>
#include <QDebug> 
#include <QMessageBox>

SimpleChat::SimpleChat(QWidget* parent)
	: QMainWindow(parent)
	, m_pLoginWidget(new LoginWidget(0))
	, m_fileWatcher(new QFileSystemWatcher())
	, m_dbManager(new DataBaseManager())
{
	qDebug() << QSqlDatabase::drivers();
	m_ui.setupUi(this);
	this->hide();
	// 初始化数据库
	m_dbManager->openDB();

	m_pLoginWidget->show();
	m_pLoginWidget->setDBPtr(m_dbManager);







	initConnect();
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

void SimpleChat::initConnect()
{
	// 监视Qss文件当发生变动时自动重新加载QSS
	QString strQssPath = "://qss/style.qss";
	m_fileWatcher->addPath(strQssPath);
	autoLoadQssFile(strQssPath);
	connect(m_fileWatcher, &QFileSystemWatcher::fileChanged, this,
		[=]() {
			autoLoadQssFile(strQssPath);
		});

	connect(m_pLoginWidget, &LoginWidget::loginResponse, [=](int iStateNum) {
		ResponseByDifferentStateNum(iStateNum);
		});

}

void SimpleChat::ResponseByDifferentStateNum(int iStateNum)
{
	switch (iStateNum)
	{
	case LoginSuccess:
	{
		m_pLoginWidget->hide();
		this->show();
		break;
	}
	case DBIsNotOpen:
	{
		qDebug()<<tr("数据库未打开");
		break;
	}
	case QueryExecFailed:
	{
		qDebug() << tr("数据库语句执行失败");
		break;
	}
	case QueryCountLessThan0:
	{
		QMessageBox::warning(0, tr("错误"), tr("错误的用户名或密码"));
		break;
	}
	case AddUserSuccess:
	{
		QMessageBox::warning(0, tr("成功"), tr("添加用户成功"));
		break;
	}
	case WannaAddUserButThereHasBeen:
	{
		QMessageBox::warning(0, tr("失败"), tr("已经有改用户了"));
		break;
	}

	default:
		break;
	}

}
